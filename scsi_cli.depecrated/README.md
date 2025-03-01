# Introduction
This project incorporates a modified driver that bypasses cache flushing and relinquishes control to the RAID card firmware upon 'PCI-DRIVER detach or shutdown'. This guarantees that a spun-down hard disk remains inactive and is not reactivated by the firmware during computer shutdown.

While it’s uncertain whether an abrupt power-off can cause damage to the disk, the sudden stoppage of the Hard Disk is not typically expected, so I have chosen to rectify this.

**Related topic below**
* Hard drive spin up before shutting down https://forums.unraid.net/topic/120224-hard-drive-spin-up-before-shutting-down/
* \[存储\]现在Truenas关机硬盘磁头不能正常归位了？ https://www.chiphell.com/thread-2597204-1-1.html
* LSI turns their back on Green https://blog.insanegenius.com/2012/11/21/lsi-turns-their-back-on-green/
* LSI SAS 2108 MegaRAID Power Save for Active/ Configured Drives https://www.servethehome.com/lsi-sas-2108-megaraid-power-save-active-drives/#comment-22499

## Step to make your own module
**My ENV** : Debian 12 + DELL Perc H740p (SAS3508)

* Install necessary tools
```
apt install build-essential make tar xz-utils linux-headers-`uname -r`
```

* Prepare modified driver from the kernel source code
see : [drvspin/README](drvspin/README)

* Build `megaio` tools
```
cd scsi_cli
./build.sh
```

* Modify `shutdown_script.sh` 
Then run `./shutdown_script.sh` to test result.

* Modify `megaspin.service`, then add a service (OS dependency)
```bash
# root user
sudo -s

# Make megaspin.service to /lib/systemd/system/megaspin.service
ln -s $(pwd)/megaspin.service /lib/systemd/system/megaspin.service

# enable it
systemctl enable megaspin

# check status
sudo systemctl status megaspin
```

See also: https://forums.debian.net/viewtopic.php?t=155854

## Notes
When disks are in the Rebuilding state, the SCSI STOP won’t keep spin-down on them; instead, they may be re-spun up as triggered by the firmware. You can set the disks to offline using `perccli` or `storcli`, and then issue the STOP command.

REL BLOG: Linux系统下 raid卡关机磁盘异响（直接断电） https://blog.vrqq.org/archives/959/
