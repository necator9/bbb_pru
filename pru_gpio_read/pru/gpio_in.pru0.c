#include <stdint.h>
#include <pru_cfg.h>
#include <pru_ctrl.h>
#include "resource_table_empty.h"

#define CYCLES_PER_SECOND 200000000 /* PRU has 200 MHz clock */
#define PRU_SRAM 0x10000
#define P9_30 (1 << 2) 


volatile register uint32_t __R30; /* output register for PRU */
volatile unsigned int *shared_0 = (unsigned int *) (PRU_SRAM + 0x200);
unsigned int BLINKING_ON = 0x1212;
unsigned int BLINKING_OFF = 0x1213;
unsigned int ON_STATUS = 0xdeefdeef;
unsigned int OFF_STATUS = 0xbeebbeeb;

int main(void){
  while(1) {
    shared_0[0] = __R30 & P9_30;
  }
}

