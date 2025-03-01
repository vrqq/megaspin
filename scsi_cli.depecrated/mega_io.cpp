#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstddef>   // for offsetof()

#include <errno.h>
#include <fcntl.h>
#include <scsi/scsi.h>
#include <scsi/scsi_ioctl.h>
#include <scsi/sg.h>
#include <linux/bsg.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <dirent.h>

// glibc 2.25: The inclusion of <sys/sysmacros.h> by <sys/types.h> is
// deprecated.  A warning is printed if major(), minor() or makedev()
// is used but <sys/sysmacros.h> is not included.
#include <sys/sysmacros.h>

#include "megaraid.h"
#include "mega_io.h"

bool MegaIO::open()
{
    int mjr;

    /* Perform mknod of device ioctl node */
    FILE * fp = fopen("/proc/devices", "r");
    if (fp) {
        char line[128];
        while (fgets(line, sizeof(line), fp) != NULL) {
            int n1 = 0;
            if (sscanf(line, "%d megaraid_sas_ioctl%n", &mjr, &n1) == 1 && n1 == 22) {
                n1=mknod("/dev/megaraid_sas_ioctl_node", S_IFCHR|0600, makedev(mjr, 0));
                std::cout<<"Creating /dev/megaraid_sas_ioctl_node\n";
                if (n1 >= 0 || errno == EEXIST)
                    break;
            }
            else if (sscanf(line, "%d megadev%n", &mjr, &n1) == 1 && n1 == 11) {
                n1=mknod("/dev/megadev0", S_IFCHR|0600, makedev(mjr, 0));
                std::cout<<"Creating /dev/megadev0\n";
                if (n1 >= 0 || errno == EEXIST)
                    break;
            }
        }
        fclose(fp);
    }

    /* Open Device IOCTL node */
    if ((m_fd = ::open("/dev/megaraid_sas_ioctl_node", O_RDWR)) >= 0)
        using_modern_cmd = true;
    else if ((m_fd = ::open("/dev/megadev0", O_RDWR)) >= 0)
        using_modern_cmd = false;
    else {
        std::cerr<<"cannot open /dev/megaraid_sas_ioctl_node or /dev/megadev0"
                 <<errno<<std::endl;
        return false;
    }
    return true;
} //MegaIO::open()

bool MegaIO::close()
{
  return ::close(m_fd);
} //MegaIO::close()

bool MegaIO::disk_spindown(unsigned int m_hba, unsigned int m_disknum)
{    
    std::cout<<"Spin DOWN "<<m_disknum<<" on hba "<<m_hba<<"\n";
    uint8_t cmd_stop[6]  = {0x1b, 0x01, 0x00, 0x00, 0x00, 0x00};
    return scsi_cmd(m_hba, m_disknum, 6, cmd_stop, 0, 0, DXFER_NONE);
}

bool MegaIO::disk_spinup(unsigned int m_hba, unsigned int m_disknum)
{
    std::cout<<"Spin UP "<<m_disknum<<" on hba "<<m_hba<<"\n";
    uint8_t cmd_start[6] = {0x1b, 0x01, 0x00, 0x00, 0x01, 0x00};
    return scsi_cmd(m_hba, m_disknum, 6, cmd_start, 0, 0, DXFER_NONE);
}

bool MegaIO::megasas_cmd(
    unsigned int m_hba, unsigned int m_disknum, 
    int cdbLen, void *cdb, int dataLen, void *data, int dxfer_dir
) {
    struct megasas_pthru_frame    *pthru;
    struct megasas_iocpacket      uio;

    memset(&uio, 0, sizeof(uio));
    pthru = &uio.frame.pthru;
    pthru->cmd = MFI_CMD_PD_SCSI_IO;
    pthru->cmd_status = 0xFF;
    pthru->scsi_status = 0x0;
    pthru->target_id = m_disknum;
    pthru->lun = 0;
    pthru->cdb_len = cdbLen;
    pthru->timeout = 0;
    switch (dxfer_dir) {
        case DXFER_NONE:
        pthru->flags = MFI_FRAME_DIR_NONE;
        break;
        case DXFER_FROM_DEVICE:
        pthru->flags = MFI_FRAME_DIR_READ;
        break;
        case DXFER_TO_DEVICE:
        pthru->flags = MFI_FRAME_DIR_WRITE;
        break;
        default:
        std::cout<<"megasas_cmd: bad dxfer_dir\n";
        return false;
    }

    if (dataLen > 0) {
        pthru->sge_count = 1;
        pthru->data_xfer_len = dataLen;
        pthru->sgl.sge32[0].phys_addr = (intptr_t)data;
        pthru->sgl.sge32[0].length = (uint32_t)dataLen;
    }
    memcpy(pthru->cdb, cdb, cdbLen);

    uio.host_no = m_hba;
    if (dataLen > 0) {
        uio.sge_count = 1;
        uio.sgl_off = offsetof(struct megasas_pthru_frame, sgl);
        uio.sgl[0].iov_base = data;
        uio.sgl[0].iov_len = dataLen;
    }

    errno = 0;
    int rc = ioctl(m_fd, MEGASAS_IOC_FIRMWARE, &uio);
    if (pthru->cmd_status || rc != 0) {
        if (pthru->cmd_status == 12)
            std::cerr<<"megasas_cmd: Device "<< m_disknum <<" does not exist"
                     <<std::endl;
        else
            std::cerr<<"megasas_cmd result: m_disknum="<<m_disknum
                    <<" errno="<<errno
                    <<" cmd_status="<<pthru->cmd_status
                    <<std::endl;
        return false;
    }
    return true;
} //MegaIO::megasas_cmd()

#define MAX_REQ_SENSE_LEN  0x20
#define MEGA_MBOXCMD_PASSTHRU 0x03

bool MegaIO::megadev_cmd(
    unsigned int m_hba, unsigned int m_disknum, 
    int cdbLen, void *cdb, int dataLen, void *data, int
) {
    struct uioctl_t uio;
    int rc;

    /* Don't issue to the controller */
    if (m_disknum == 7)
        return false;

    memset(&uio, 0, sizeof(uio));
    uio.inlen  = dataLen;
    uio.outlen = dataLen;

    memset(data, 0, dataLen);
    uio.ui.fcs.opcode = 0x80;             // M_RD_IOCTL_CMD
    uio.ui.fcs.adapno = MKADAP(m_hba);

    uio.data.pointer = (uint8_t *)data;

    uio.mbox.cmd = MEGA_MBOXCMD_PASSTHRU;
    uio.mbox.xferaddr = (intptr_t)&uio.pthru;

    uio.pthru.ars     = 1;
    uio.pthru.timeout = 2;
    uio.pthru.channel = 0;
    uio.pthru.target  = m_disknum;
    uio.pthru.cdblen  = cdbLen;
    uio.pthru.reqsenselen  = MAX_REQ_SENSE_LEN;
    uio.pthru.dataxferaddr = (intptr_t)data;
    uio.pthru.dataxferlen  = dataLen;
    memcpy(uio.pthru.cdb, cdb, cdbLen);

    rc=ioctl(m_fd, MEGAIOCCMD, &uio);
    if (uio.pthru.scsistatus || rc != 0) {
        std::cerr<<"megasas_cmd result: m_disknum="<<m_disknum
                 <<" errno="<<errno
                 <<" cmd_status="<<uio.pthru.scsistatus
                 <<std::endl;
        return false;
    }
    return true;
} //MegaIO::megadev_cmd()