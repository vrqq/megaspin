# megasas笔记

`/dev/megaraid_sas_ioctl` 这个设备在`static int __init megasas_init(void)` 中注册. 无论当前主机有多少张卡, 都仅有一个`/dev/megaraid_sas_ioctl`.

`rval = register_chrdev(0, "megaraid_sas_ioctl", &megasas_mgmt_fops);` 其中
```c
    .open   // 无影响
    .fasync // 调用kernel-header相关
    .unlocked_ioctl 
```

`unregister_chrdev` 在`__exit` 和 `__init` 出现, 在load module时 向kernel注册的驱动和相关接口.

## 硬件交互基础知识
“驱动程序” 是软件, raid card是硬件. 在linux中, 内核/cpu已经将硬件上面的“寄存器”及“接收端口”的操作 映射到了内存地址. 软件想读写硬件, 直接读写某个“地址”即可. 这个地址从linux kernel API可以获得.

底层来看就是: 由cpu执行的软件代码, 把要写的数据改为某种固定格式, 经主板电路, 把数据传入硬件上面的处理器.

## ioctl的操作
入口为 `megasas_mgmt_ioctl()`, 因我们mega_io传入值为`cmd = MEGASAS_IOC_FIRMWARE`, 跳转到 `megasas_mgmt_ioctl_fw(file *, ulong user_addr)`

其中 `ioc->host_no` 不匹配 也会导致`error = -ENODEV;` (-19)

**指令流入**
1. 从ioctl获得的原始数据包为 `struct megasas_iocpacket *ioc` (ioc为kernel space变量)
2. 此处会检查 `if (instance->unload == 1)` 这个变量会在`megasas_shutdown()`等函数内被设置为1
3. 最后转发至 `megasas_mgmt_fw_ioctl(pinst*, out*, in*)` 注意该函数只有一个调用入口

在mega_io中 和 driver中的 `megasas_iocpacket` 尺寸相同, 只是在`union .frame` 中多了些扩展, 我们用到的是
```c
// from user to driver
struct megasas_pthru_frame;

// parse in driver
struct megasas_header;
```
我们构建ioctl数据包时, 需要使用`union megasas_frame`下面的其中之一填充, 然后交给驱动解析.

4. 继续阅读`megasas_mgmt_fw_ioctl()`这个函数, 在 8848 - 8874行, 先调用`megasas_get_cmd()`获取给硬件发指令的一个机会, 然后驱动程序将用户的`ioc`转化为内部的`struct megasas_cmd`格式. 

5. 在我们的需求中 `frame.hdr.cmd = MFI_CMD_PD_SCSI_IO` 并且有 `ioc->sge_count == 0` 和 `ioc->sense_len == 0`, 故直接跳至 8969行 `megasas_issue_blocked_cmd(instance, cmd, 0)`

6. 最后的结果在9031行 `copy_to_user(&user_ioc->frame.hdr.cmd_status` 处获得

7. 第4步的函数正常退出时 进入 ':out'标签, 并调用`megasas_return_cmd()`释放由`megasas_get_cmd()`申请的指针.

**hack思路**
我们可以构建`struct megasas_cmd` 并且调用`megasas_issue_blocked_cmd()`同步执行, 驱动代码中亦有多次对此函数的调用 可供参考.

## 开发前代码准备
在`static inline void megasas_init_ctrl_params()`中 修改`megasas_dbg_lvl = 0b111;` 以打印更多log.
另开一个terminal, 使用`dmesg -w`监听未来的kernel message.


## 获取物理磁盘数量
根据注释, `megasas_get_pd_list()` 给raid卡发指令, 然后填充 `instance->pd_list`.
对于`instance->pd_list[256]` 和 `instance->local_pd_list[256]`的区别我不得而知.
根据log表示, 此处`instance->pd_list_buf->count`得到的数量为0, 看来这里的pd似乎不是我理解的意思.

## 整合构建
考虑在`megasas_shutdown()`的末尾, 通过`megasas_mgmt_fw_ioctl()`发送scsi指令.
在`megasas_mgmt_fw_ioctl()` 的8983行有 `if (instance->unload == 1)` 可以跳过`copy_to_user()`阶段,
故我们尝试直接构建`struct ioc_packet`.
由于无法获取物理磁盘数量, 我们只能对所有磁盘发送指令.
```c
static int
megasas_mgmt_fw_ioctl(struct megasas_instance *instance,
		      struct megasas_iocpacket __user * user_ioc,
		      struct megasas_iocpacket *ioc);
static int megasas_spindown_all_physical_disks(struct megasas_instance *instance)
{
    int pd_count = MEGASAS_MAX_PD; // instance->pd_list_buf->count;
    int host_no  = instance->host->host_no;
    uint8_t scsi_cmd_stop[6]  = {0x1b, 0x01, 0x00, 0x00, 0x00, 0x00};

	dev_info(&instance->pdev->dev, 
		"spinning down %d disks on scsi_host %d", pd_count, host_no);

    if(instance->unload == 0){
		dev_err(&instance->pdev->dev, 
			"Instance not unload");
		return -EFAULT;
	}

    struct megasas_iocpacket ioc;
	struct megasas_pthru_frame *pthru = (struct megasas_pthru_frame*)(ioc.frame.raw);
    memset(&ioc, 0, sizeof(ioc));

    ioc.host_no = host_no;
    pthru->cmd = MFI_CMD_PD_SCSI_IO;
    pthru->cmd_status = 0xFF;
    pthru->scsi_status = 0x0;
    pthru->lun = 0;
    pthru->timeout = 0;
    pthru->flags = MFI_FRAME_DIR_NONE;
    
    pthru->cdb_len = sizeof(scsi_cmd_stop);
    memcpy(pthru->cdb, scsi_cmd_stop, sizeof(scsi_cmd_stop));

    for (int i=0; i<pd_count; i++) {
        pthru->target_id = i;
        int ret = megasas_mgmt_fw_ioctl(instance, 0, &ioc);
        if (ret != 0) {
            dev_err(&instance->pdev->dev, 
				"return %d when spindown disk %d", ret, i);
            return ret;
        }
        dev_info(&instance->pdev->dev, 
			"spin down physical disk %d on scsi_host %d", i, host_no);
    }
    
    return 0;
}
```

随后将此函数加入`megasas_shutdown()`, 在`megasas_shutdown_controller`之后执行.
