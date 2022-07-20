#include <stdint.h>
#include <pru_cfg.h>
#include <pru_ctrl.h>
#include "resource_table_empty.h"

#define CYCLES_PER_SECOND 200000000 /* PRU has 200 MHz clock */
#define PRU_SRAM 0x10000

// Outputs
#define WRST (1 << 13) // P8_20
#define RRST (1 << 1)  // P8_46
#define WR (1 << 2)    // P8_43
// #define RST (1 << 1)    // P8_44

// Inputs
#define RCK (1 << 4)   // P8_41
#define VSYNC (1 << 0) // P8_45
#define D0 (1 << 5)    // P8_42
#define D1 (1 << 6)    // P8_39
#define D2 (1 << 7)    // P8_40
#define D5 (1 << 8)    // P8_27
#define D3 (1 << 9)    // P8_29
#define D6 (1 << 10)   // P8_28
#define D4 (1 << 11)   // P8_30
#define D7 (1 << 12)   // P8_21


volatile register uint32_t __R30; /* output register for PRU */
volatile register uint32_t __R31; /* input register for PRU */
volatile unsigned int *shared_0 = (unsigned int *) (PRU_SRAM + 0x200);


void init_al422b(void) {
  __R30 |= OE;  // Disable al422b data output via OE pin
  __R30 |= WR;
  __delay_cycles(20000);
  __R30 &= ~(WRST | RRST);
  __delay_cycles(20000);
  __R30 |= WRST | RRST; // Apply /WRST and /RRST 0.1ms after power on
}

int main(void) {
  // init_al422b();
  // while(__R31 & VSYNC);
  // // __R30 |= WRST;  
  // __R30 &= ~WR;
  // while(!(__R31 & VSYNC));
  // __R30 |= WR;

  __R30 |= OE;  // Disable al422b data output via OE pin
  __R30 |= WR;
  //__R30 |= WRST | RRST; // Apply /WRST and /RRST 0.1ms after power on



  unsigned int prev_lvl; 

  int n_it = 20;

  int i = 0;
  for (i = 0; i < n_it; i++) {
    shared_0[i] = 1000000;
  }

  int p = 0;
  int k = 0;
  while(1) {
    __R30 &= ~RRST;
    __delay_cycles(20000);
    __R30 |= RRST;
    __delay_cycles(20000);
    k = 0;
    while(k < n_it) {
      prev_lvl = __R31 & RCK;
      if (prev_lvl == 0 && __R31 & RCK) {
        shared_0[k] = __R31 & (D0 | D1 | D2 | D3 | D4 | D5 | D6 | D7);
        k++;
      }
    }
    shared_0[20] = p;  
    p++;
  }  
}
    // if (prev_lvl == 0 && (__R31 & RCK) != 0) {
    //   //shared_0[k] = __R31;  
    //   shared_0[k] = 110; 
    // }
    
    // 
  



