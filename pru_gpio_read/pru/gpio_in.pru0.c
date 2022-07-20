#include <stdint.h>
#include <pru_cfg.h>
#include <pru_ctrl.h>
#include "resource_table_empty.h"

#define CYCLES_PER_SECOND 200000000 /* PRU has 200 MHz clock */
#define PRU_SRAM 0x10000
#define P9_31 (1 << 0) 
#define P9_30 (1 << 2) 
#define P9_29 (1 << 1) 


volatile register uint32_t __R30; /* output register for PRU */
volatile register uint32_t __R31; /* output register for PRU */
volatile unsigned int *shared_0 = (unsigned int *) (PRU_SRAM + 0x200);
unsigned int BLINKING_ON = 0x1212;
unsigned int BLINKING_OFF = 0x1213;
unsigned int ON_STATUS = 0xdeefdeef;
unsigned int OFF_STATUS = 0xbeebbeeb;


// void blink_led() {
//   __R30 |= P9_29;  // Switch on LED
//   __delay_cycles(CYCLES_PER_SECOND / 4);
//   __R30 &= ~P9_29; // Switch off LED
// }

int main(void) {
  CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
  // __R30 &= ~P9_29; // Switch off LED
  unsigned int prev_lvl = 0;
  int n_it = 50;
  shared_0[0] = 0;
  int k = 1;
  unsigned int cycles = 1;
  unsigned int prev_clock = 0;
  unsigned int lvl;
  while(k < n_it) {
      unsigned int lvl = __R31 & P9_30;
      shared_0[k] = __R31;
      k++;
    //   if (lvl != prev_lvl) {
    //       shared_0[k] = lvl;
          // shared_0[k] = cycles - prev_clock;
          // prev_clock = cycles;
    //       prev_lvl = lvl;
    //       k++;
    //   }
      // else {
      //   cycles++;
      // }
  }
  // shared_0[k] = cycles;
  // blink_led();
  // __R30 |= P9_29;
}


// if (__R31 & P9_30) {
    //   __R30 |= P9_29; /* set first bit in register 30 */
    //   //__delay_cycles(CYCLES_PER_SECOND / 4); /* wait 0.5 seconds */
    // }
    // else {
    //   __R30 &= ~P9_29; /* set first bit in register 30 */
    //   shared_0[0] = __R31 & P9_30;

    //  // __delay_cycles(CYCLES_PER_SECOND / 4); /* wait 0.5 seconds */
    // }
    // shared_0[0] = __R31 & P9_30; 
    //shared_0[0] = __R30 & P9_30;
    // shared_0[0] = 0x00000007;
