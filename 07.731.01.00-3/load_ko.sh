#!/bin/sh

rmmod megaraid_sas_pdoff.ko
insmod megaraid_sas_pdoff.ko
driverctl set-override 0000:07:00.0 megaraid_sas_pdoff
lspci -v -s 07:00.0