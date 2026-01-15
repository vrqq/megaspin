#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_MITIGATION_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x81e6b37f, "dmi_get_system_info" },
	{ 0x1c3be00f, "scsi_dma_map" },
	{ 0xc1514a3b, "free_irq" },
	{ 0xc31db0ce, "is_vmalloc_addr" },
	{ 0x5e2e2018, "pcie_capability_read_word" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xc2e587d1, "reset_devices" },
	{ 0xdf9208c0, "alloc_workqueue" },
	{ 0x99c8e080, "pci_cfg_access_unlock" },
	{ 0xfb51bee9, "dma_pool_create" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0xc4f99846, "pci_request_selected_regions" },
	{ 0xab9b8edb, "pci_release_selected_regions" },
	{ 0x4a3ad70e, "wait_for_completion_timeout" },
	{ 0xc60d0620, "__num_online_cpus" },
	{ 0x4d64f443, "pci_enable_device_mem" },
	{ 0xc165389, "pci_alloc_irq_vectors" },
	{ 0x656e4a6e, "snprintf" },
	{ 0xa6257a2f, "complete" },
	{ 0xc5b6f236, "queue_work_on" },
	{ 0x9291cd3b, "memdup_user" },
	{ 0x10ef9a87, "driver_remove_file" },
	{ 0xf8f818a7, "scsi_remove_host" },
	{ 0x608741b5, "__init_swait_queue_head" },
	{ 0x92540fbf, "finish_wait" },
	{ 0xaf8a36d7, "kill_fasync" },
	{ 0x96848186, "scnprintf" },
	{ 0xbc771b1f, "__pci_register_driver" },
	{ 0xcf2a6966, "up" },
	{ 0xedc03953, "iounmap" },
	{ 0x69acdf38, "memcpy" },
	{ 0x27b0fc9f, "scsi_dma_unmap" },
	{ 0x3b6c41ea, "kstrtouint" },
	{ 0x37a0cba, "kfree" },
	{ 0xfcec0987, "enable_irq" },
	{ 0x7a8006ac, "pcpu_hot" },
	{ 0x8c26d495, "prepare_to_wait_event" },
	{ 0xb3087f55, "timer_delete_sync" },
	{ 0xe2964344, "__wake_up" },
	{ 0x4302d0eb, "free_pages" },
	{ 0x1e85d180, "pci_irq_vector" },
	{ 0x9e4cc874, "__scsi_iterate_devices" },
	{ 0x34db050b, "_raw_spin_lock_irqsave" },
	{ 0xa33adda, "__dynamic_dev_dbg" },
	{ 0xd7d280ad, "irq_poll_complete" },
	{ 0x7c1102b1, "pci_unregister_driver" },
	{ 0x2df2c5f0, "pci_cfg_access_lock" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x678b96ec, "dma_pool_alloc" },
	{ 0x7db560f7, "pci_read_config_dword" },
	{ 0x65487097, "__x86_indirect_thunk_rax" },
	{ 0x92997ed8, "_printk" },
	{ 0x5d6e2bdf, "scsi_device_lookup" },
	{ 0x1000e51, "schedule" },
	{ 0x8ddd8aad, "schedule_timeout" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x3ae3bf84, "queue_delayed_work_on" },
	{ 0x6a5cb5ee, "__get_free_pages" },
	{ 0xe3f059ea, "driver_create_file" },
	{ 0xb64408c2, "scsi_add_device" },
	{ 0x40a9b349, "vzalloc" },
	{ 0x4412517b, "_dev_info" },
	{ 0xc6cbbc89, "capable" },
	{ 0xaedd0cfe, "pci_select_bars" },
	{ 0x2edfc2f9, "scsi_device_put" },
	{ 0xea3c74e, "tasklet_kill" },
	{ 0x77d58702, "pci_find_capability" },
	{ 0xe523ad75, "synchronize_irq" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0x68f31cbd, "__list_add_valid" },
	{ 0x199c4833, "__irq_apply_affinity_hint" },
	{ 0x369d3df3, "_dev_err" },
	{ 0x108e3d50, "scsi_host_put" },
	{ 0x8c8569cb, "kstrtoint" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0xc2a113a, "add_timer" },
	{ 0x2f7754a8, "dma_pool_free" },
	{ 0x3cf85989, "mod_timer" },
	{ 0x2364c85a, "tasklet_init" },
	{ 0xd739e9b1, "noop_llseek" },
	{ 0x8c03d20c, "destroy_workqueue" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0xc6b50287, "blk_mq_map_hw_queues" },
	{ 0xfcf543b4, "sdev_prefix_printk" },
	{ 0xd3b80b2f, "dma_alloc_attrs" },
	{ 0x42fcc6f2, "scsi_scan_host" },
	{ 0x6967b809, "debugfs_remove" },
	{ 0x5db16a6, "pci_read_config_word" },
	{ 0x7ab36464, "fasync_helper" },
	{ 0x9d2ab8ac, "__tasklet_schedule" },
	{ 0xde80cd09, "ioremap" },
	{ 0x53a1e8d9, "_find_next_bit" },
	{ 0xe1537255, "__list_del_entry_valid" },
	{ 0xbcab6ee6, "sscanf" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0xd35cce70, "_raw_spin_unlock_irqrestore" },
	{ 0x9e683f75, "__cpu_possible_mask" },
	{ 0xfb578fc5, "memset" },
	{ 0xca66f6d3, "_dev_warn" },
	{ 0x54ae6fd6, "pci_alloc_irq_vectors_affinity" },
	{ 0x13e648fe, "pci_set_master" },
	{ 0x9166fc03, "__flush_workqueue" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x17de3d5, "nr_cpu_ids" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0xd9a5ea54, "__init_waitqueue_head" },
	{ 0xd01ff75, "blk_mq_unique_tag" },
	{ 0xc4e4d552, "scsi_host_busy" },
	{ 0x15ba50a6, "jiffies" },
	{ 0x7f7f7bb4, "irq_poll_disable" },
	{ 0x4ed56abd, "dma_set_coherent_mask" },
	{ 0x9e15f139, "scsi_print_command" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xb8406cb2, "_dev_notice" },
	{ 0xb5aa7165, "dma_pool_destroy" },
	{ 0x6983fbb5, "dma_free_attrs" },
	{ 0x999e8297, "vfree" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0xf39d6569, "cancel_delayed_work_sync" },
	{ 0x78534f62, "init_timer_key" },
	{ 0x70b2505a, "scmd_printk" },
	{ 0xfbe215e4, "sg_next" },
	{ 0xf978b631, "__register_chrdev" },
	{ 0x6729d3df, "__get_user_4" },
	{ 0x56470118, "__warn_printk" },
	{ 0x6bd0e573, "down_interruptible" },
	{ 0xb0e6a56, "delayed_work_timer_fn" },
	{ 0x554ae3a4, "irq_poll_sched" },
	{ 0xb2fa093e, "blk_mq_map_queues" },
	{ 0xb633f115, "irq_poll_enable" },
	{ 0x2a7200ae, "debugfs_create_file_full" },
	{ 0xf6b4b72c, "pci_disable_device" },
	{ 0x653d28e1, "scsi_done" },
	{ 0xee790a06, "dma_set_mask" },
	{ 0xab3697e4, "irq_poll_init" },
	{ 0xcdb16d0f, "kmalloc_trace" },
	{ 0x27bbf221, "disable_irq_nosync" },
	{ 0xae51a4dd, "pci_irq_get_affinity" },
	{ 0x5ddae6ed, "scsi_remove_device" },
	{ 0xbdaf838f, "scsi_add_host_with_dma" },
	{ 0x754d539c, "strlen" },
	{ 0xb8cf473f, "param_ops_int" },
	{ 0x4e77d378, "_dev_printk" },
	{ 0xe0f213b9, "scsi_host_alloc" },
	{ 0x11844f00, "debugfs_create_dir" },
	{ 0x3dce5d7b, "pci_write_config_word" },
	{ 0xec0ba69d, "pci_free_irq_vectors" },
	{ 0x619cb7dd, "simple_read_from_buffer" },
	{ 0xefbec047, "scsi_change_queue_depth" },
	{ 0x45d246da, "node_to_cpumask_map" },
	{ 0xf9a482f9, "msleep" },
	{ 0xc4f0da12, "ktime_get_with_offset" },
	{ 0xd4f0483a, "blk_queue_rq_timeout" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0xe2c17b5d, "__SCT__might_resched" },
	{ 0x8cb4dcac, "pci_write_config_dword" },
	{ 0xf9636917, "kmalloc_caches" },
	{ 0x2d3385d3, "system_wq" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x33bd423, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("pci:v00001000d00000411sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d00000060sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d0000007Csv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d00000078sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d00000079sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d00000073sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d00000071sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d00000413sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001028d00000015sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d0000005Bsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d0000002Fsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d0000005Dsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d0000005Fsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d000000CEsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d000000CFsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d00000052sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d00000053sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d00000014sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d00000015sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d00000016sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d00000017sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d0000001Bsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d0000001Csv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d000010E1sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d000010E2sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d000010E5sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d000010E6sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d000010E0sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d000010E3sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d000010E4sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001000d000010E7sv*sd*bc*sc*i*");

MODULE_INFO(srcversion, "3DDBBCE5CF620F22228CC6D");
MODULE_INFO(rhelversion, "9.7");
