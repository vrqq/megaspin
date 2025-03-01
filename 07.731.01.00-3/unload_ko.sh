#!/bin/sh

driverctl unset-override 0000:07:00.0
lspci -v -s 07:00.0