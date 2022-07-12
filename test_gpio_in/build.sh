#!/bin/bash


clpru --include_path=/usr/lib/ti/pru-software-support-package/include \
--include_path=/usr/lib/ti/pru-software-support-package/include/am335x \
--include_path=/usr/share/ti/cgt-pru/include read_gpio.c \
--output_file read_gpio.o


lnkpru /usr/lib/ti/pru-software-support-package/labs/lab_2/AM335x_PRU.cmd \
read_gpio.o \
-o am335x-pru0-fw