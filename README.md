# Examples of working with BeagleBone Black PRUs

- shared_memory - Access shared memory from ARM and PRU

```
clpru --include_path=/usr/lib/ti/pru-software-support-package/include \
--include_path=/usr/lib/ti/pru-software-support-package/include/am335x \
--include_path=/usr/share/ti/cgt-pru/include hello_world.c \
--output_file hello-pru0.o
```