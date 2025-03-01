# megaraid_sas_pdoff Driver  

Many hard drives do not support hot-plugging, making them sensitive to sudden power loss if they are not properly shut down. The **MegaRAID SAS RAID controller** is primarily designed for servers, where hard drives may be connected to **dual RAID controllers** or housed in **external disk enclosures**.  

This driver ensures a proper shutdown by sending the **SCSI disk spin-down command** `{0x1b, 0x01, 0x00, 0x00, 0x00, 0x00}` to all connected disks during the system shutdown process. This helps protect **standard workstation hard drives** from abrupt power loss when using a RAID controller.  

## How to Compile  

For RHEL-based systems, install the necessary dependencies with:  
```sh
dnf install gcc kernel-devel-$(uname -r)
```  
Then, navigate to the source directory:  
```sh
cd 07.731.01.00-3
./compile.sh
```  
This will generate the `.ko` kernel module file.  

---

## Loading the Driver manually

Assuming the compiled driver file is named `megaraid_sas_pdoff.ko`, load it with the following commands:  

```sh
modprobe -r megaraid_sas
insmod megaraid_sas_pdoff.ko
lspci -v -s 07:00.0
```
Here, `07:00.0` is the PCI device number on **this machine**. Use `lspci | grep SAS` to find the correct PCI ID for your system.  

**Note:** On some operating systems, the `megaraid_sas` driver cannot be unloaded using `modprobe`. In such cases, you may need to use `driverctl` to manually override the driver.
```sh
insmod megaraid_sas_pdoff.ko
driverctl set-override 0000:07:00.0 megaraid_sas_pdoff
lspci -v -s 07:00.0
```  

## Install to OS Set perferred at OS startup
**The full step is**
1. cd into the source directory, here is '07.731.01.00-3'.
2. build the kernel module.
3. run 'install_to_kernel.sh' to install.
4. using driverctl to set driver override.
```
cd 07.731.01.00-3
./compile.sh
./install_to_kernel.sh
driverctl set-override 0000:07:00.0 megaraid_sas_pdoff
```

---

## Modifying the Driver  

For customization, refer to the **last section** of [megasas分析笔记](megasas分析笔记.md).  
Copy the relevant function into the source code and **call it inside and at the end of** `megasas_shutdown()`.  
