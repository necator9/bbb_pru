#include <stdint.h>
#include <rsc_types.h>  /* provides struct resource_table */

#define P9_31 (1 << 0)
#define P8_15 (1 << 15)

volatile register uint32_t __R30, __R31;

void main(void) {
    while (1) {
        if (__R31 & P8_15) /* if button is pressed */
            __R30 |= P9_31; /* set bit */
        else
            __R30 &= ~P9_31; /* remove bit */
    }
}

/* required by PRU */
#pragma DATA_SECTION(pru_remoteproc_ResourceTable, ".resource_table")
#pragma RETAIN(pru_remoteproc_ResourceTable)
struct my_resource_table {
    struct resource_table base;
    uint32_t offset[1];
} pru_remoteproc_ResourceTable = { 1, 0, 0, 0, 0 };
