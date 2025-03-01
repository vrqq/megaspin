
// code copied from static int megasas_mgmt_fw_ioctl()
// in file megaraid_sas_base.c
//
static int megaraid_issue_spindown(struct megasas_instance *instance, unsigned int disk_id)
{
    uint8_t cmd_stop[6]  = {0x1b, 0x01, 0x00, 0x00, 0x00, 0x00};
    uint8_t cmd_start[6] = {0x1b, 0x01, 0x00, 0x00, 0x01, 0x00};

	struct megasas_cmd *cmd = megasas_get_cmd(instance);
	if (!cmd) {
		dev_printk(KERN_DEBUG, &instance->pdev->dev, "Failed to get a cmd packet\n");
		return -ENOMEM;
	}

    struct megasas_pthru_frame pthru;
    pthru.cmd = MFI_CMD_PD_SCSI_IO;
    pthru.cmd_status = 0xFF;
    pthru.scsi_status = 0x0;
    pthru.target_id = disk_id;
    pthru.lun = 0;
    pthru.cdb_len = 6;
	memcpy(pthru.cdb, cmd_stop, sizeof(cmd_stop));
    pthru.timeout = 0;
	memcpy(cmd->frame, &pthru, 2 * MEGAMFI_FRAME_SIZE);

	cmd->frame->hdr.context = cpu_to_le32(cmd->index);
	cmd->frame->hdr.pad_0 = 0;
	cmd->frame->hdr.flags &= (~MFI_FRAME_IEEE);

	if (instance->consistent_mask_64bit)
		cmd->frame->hdr.flags |= cpu_to_le16((MFI_FRAME_SGL64 |
				       MFI_FRAME_SENSE64));
	else
		cmd->frame->hdr.flags &= cpu_to_le16(~(MFI_FRAME_SGL64 |
					       MFI_FRAME_SENSE64));

	cmd->sync_cmd = 1; 
	int ret = megasas_issue_blocked_cmd(instance, cmd, 0);
	int error = 0;
	if (ret == DCMD_INIT || ret == DCMD_BUSY) {
		dev_err(&instance->pdev->dev,
			"return -EBUSY from %s %d cmd 0x%x cmd->cmd_status_drv 0x%x\n",
			 __func__, __LINE__, cmd->frame->hdr.cmd,
			 cmd->cmd_status_drv);
		error = -EBUSY;
	}

	cmd->sync_cmd = 0; 
	megasas_return_cmd(instance, cmd);             

    return error;
}