sudo rmmod module_kernel.ko
sudo rmmod conveyor.ko
sudo rmmod processes.ko
sudo lsmod
sudo dmesg | egrep --color=auto -E "Oleg_Kernel|Conveyor_Driver|Processes_Driver"
make clean
