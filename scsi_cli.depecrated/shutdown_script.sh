#/bin/bash

# cd to script path
cd "$(dirname "$0")"

# remove the previous driver 
# (the driver would ensure the data flushed corrently)
modprobe -r megaraid_sas

# load the custom driver
insmod ./drvspin/megaraid_sas_spindown.ko

# send SCSI STOP command to DISK{0, 1, 2} inside raid card
./scsi_cli/megaio spindown 0 1 2
