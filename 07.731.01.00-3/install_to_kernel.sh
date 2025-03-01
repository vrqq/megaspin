#!/bin/sh

## Reference
## https://docs.redhat.com/en/documentation/red_hat_enterprise_linux/7/html/kernel_administration_guide/chap-documentation-kernel_administration_guide-working_with_kernel_modules#sec-Loading-kernel-modules-at-system-runtime

rm megaraid_sas_pdoff.ko.xz
xz -z megaraid_sas_pdoff.ko 
cp -f megaraid_sas_pdoff.ko.xz /lib/modules/$(uname -r)/kernel/drivers/scsi/

## insmod when os startup
## https://access.redhat.com/solutions/230963
echo "megaraid_sas_pdoff" > /etc/modules-load.d/megaraid_sas_pdoff.conf

## set if selinux enabled
chcon -u system_u /lib/modules/$(uname -r)/kernel/drivers/scsi/megaraid_sas_pdoff.ko.xz
chcon -u system_u /etc/modules-load.d/megaraid_sas_pdoff.conf

## call depmod to update kernel cache
## using 'modinfo megaraid_sas_pdoff' to check success
depmod

echo "script done"

## =======================================================
## After install, user can call driverctl to set override
##  driverctl set-override 0000:07:00.0 megaraid_sas_pdoff
##  lspci -v -s 0000:07:00.0
## =======================================================
