make all
sudo insmod module_kernel.ko
sudo insmod conveyor.ko
sudo insmod processes.ko
sudo lsmod
sudo dmesg | egrep --color=auto -E "Oleg_Kernel|Conveyor_Driver|Processes_Driver"
sudo chmod o+rw /dev/Conveyor_Driver
sudo chmod o+rw /dev/Processes_Driver
exit
