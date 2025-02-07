// Code copied from smartmontools/os_linux.cpp : class linux_megaraid_device
//
#pragma once

class MegaIO
{
public:
    // open device and create /dev/megaraid_sas_ioctl_node
    bool open();

    // close ioctl fd
    bool close();

    // Issue a SCSI STOP command to m_disknum
    // @param m_disknum: 0, 1, ...
    bool disk_spindown(unsigned int m_hba, unsigned int m_disknum);

    // Issue a SCSI START command to m_disknum
    // @param m_disknum: 0, 1, ...
    bool disk_spinup(unsigned int m_hba, unsigned int m_disknum);

    // Execute a raw SCSI command that is passed through by the megaraid_sas driver.
    bool scsi_cmd(unsigned int m_hba, unsigned int m_disknum, 
        int cdbLen, void *cdb, int dataLen, void *data, int direction
    ) {
        if (using_modern_cmd)
            return megasas_cmd(m_hba, m_disknum, cdbLen, cdb, dataLen, data, direction);
        return megadev_cmd(m_hba, m_disknum, cdbLen, cdb, dataLen, data, direction);
    }

    // opt values for 'direction' in scsi_cmd()
    const static int DXFER_NONE        = 0;
    const static int DXFER_FROM_DEVICE = 1;
    const static int DXFER_TO_DEVICE   = 2;

private:
    int m_fd = 0;
    bool using_modern_cmd = true;

    // Issue passthrough scsi command to PERC5/6 controllers
    bool megasas_cmd(unsigned int m_hba, unsigned int m_disknum, 
        int cdbLen, void *cdb, int dataLen, void *data, int direction);

    // Issue passthrough scsi commands to PERC2/3/4 controllers
    bool megadev_cmd(unsigned int m_hba, unsigned int m_disknum, 
        int cdbLen, void *cdb, int dataLen, void *data, int direction);

};