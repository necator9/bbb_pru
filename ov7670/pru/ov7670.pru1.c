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
#define RCK (1 << 4)   // P8_41
// #define RST (1 << 1)    // P8_44

// Inputs

#define VSYNC (1 << 0) // P8_45
#define D0 (1 << 5)    // P8_42
#define D1 (1 << 6)    // P8_39
#define D2 (1 << 7)    // P8_40
#define D5 (1 << 8)    // P8_27
#define D3 (1 << 9)    // P8_29
#define D6 (1 << 10)   // P8_28
#define D4 (1 << 11)   // P8_30
#define D7 (1 << 12)   // P8_21

#define BUFFER_FULL 751
#define BUFFER_EMPTY 752
#define BUFFER_INIT 0

volatile register uint32_t __R30; /* output register for PRU */
volatile register uint32_t __R31; /* input register for PRU */
volatile unsigned int *sram = (unsigned int *) (PRU_SRAM + 0x10);
volatile unsigned int *buffer_status = (unsigned int *) PRU_SRAM;


void init_al422b(void) {
    __R30 |= WR;	
    __delay_cycles(20000);
    __R30 &= ~(WRST | RRST);
    __delay_cycles(20000);
    __R30 |= WRST | RRST; // Apply /WRST and /RRST 0.1ms after power on
}

int main(void) {
    buffer_status[0] = BUFFER_INIT;
    int buf_size = 2000;
    int i = 0;

    for (i = 0; i < buf_size; i++) {
        sram[i] = 1000000;
    }

    init_al422b();
    while(__R31 & VSYNC);
    __R30 &= ~WR;
    while(!(__R31 & VSYNC));
    __R30 |= WR;

    int k = 0;
    int img_res = 640 * 480 / 2;
    for (i = 0; i < img_res; i++) {
        if ((k > buf_size - 1) || (i > img_res - 1)) {
            buffer_status[0] = BUFFER_FULL;
            while(!(buffer_status[0] == BUFFER_EMPTY));
            k = 0;
        }

        unsigned int pixel_byte;

        // Read 2 bytes per pixel
        __R30 |= RCK;
        __delay_cycles(50);
        pixel_byte = (__R31 & (D0 | D1 | D2 | D3 | D4 | D5 | D6 | D7)) << 24;
        __R30 &= ~RCK;
        __delay_cycles(50);

        __R30 |= RCK;
        __delay_cycles(50);
        pixel_byte |= (__R31 & (D0 | D1 | D2 | D3 | D4 | D5 | D6 | D7)) << 16;
        __R30 &= ~RCK;
        __delay_cycles(50);

        __R30 |= RCK;
        __delay_cycles(50);
        pixel_byte |= (__R31 & (D0 | D1 | D2 | D3 | D4 | D5 | D6 | D7)) << 8;
        __R30 &= ~RCK;
        __delay_cycles(50);

        __R30 |= RCK;
        __delay_cycles(50);
        pixel_byte |= (__R31 & (D0 | D1 | D2 | D3 | D4 | D5 | D6 | D7));
        __R30 &= ~RCK;
        __delay_cycles(50);

        sram[k++] = pixel_byte;
    }
    
    buffer_status[0] = BUFFER_FULL;
    buffer_status[1] = i;
}

