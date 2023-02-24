#include <stdint.h>
#include <pru_cfg.h>
#include <pru_ctrl.h>
// #include "resource_table_empty.h"
// Use rpmsg to control the NeoPixels via /dev/rpmsg_pru30
#include <stdio.h>
#include <stdlib.h>         // atoi
#include <string.h>
#include <pru_intc.h>
#include <rsc_types.h>
#include <pru_rpmsg.h>
#include "resource_table_0.h"

#define CYCLES_PER_SECOND 200000000 /* PRU has 200 MHz clock */
#define PRU_SRAM 0x10000
#define P9_31 (1 << 0) 
#define P9_30 (1 << 2) 
#define P9_29 (1 << 1) 


volatile register uint32_t __R30; /* output register for PRU */
volatile register uint32_t __R31; /* output register for PRU */


/* Host-0 Interrupt sets bit 30 in register __R31 */
#define HOST_INT            ((uint32_t) 1 << 30)

/* The PRU-ICSS system events used for RPMsg are defined in the Linux device tree
 * PRU0 uses system event 16 (To ARM) and 17 (From ARM)
 * PRU1 uses system event 18 (To ARM) and 19 (From ARM)
 */
#define TO_ARM_HOST         16
#define FROM_ARM_HOST       17

/*
* Using the name 'rpmsg-pru' will probe the rpmsg_pru driver found
* at linux-x.y.z/drivers/rpmsg/rpmsg_pru.c
*/
#define CHAN_NAME           "rpmsg-pru"
#define CHAN_DESC           "Channel 30"
#define CHAN_PORT           30
#define CYCLES_PER_SECOND 200000000 /* PRU has 200 MHz clock */

/*
 * Used to make sure the Linux drivers are ready for RPMsg communication
 * Found at linux-x.y.z/include/uapi/linux/virtio_config.h
*/
#define VIRTIO_CONFIG_S_DRIVER_OK   4

char payload[RPMSG_BUF_SIZE];

int main(void) {
  struct pru_rpmsg_transport transport;
    uint16_t src, dst, len;
    volatile uint8_t *status;

    // Allow OCP master port access by the PRU so the PRU can read external memories /
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

    // Clear the status of the PRU-ICSS system event that the ARM will use to 'kick' us /
    CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;

    // Make sure the Linux drivers are ready for RPMsg communication /
    status = &resourceTable.rpmsg_vdev.status;
    while (!(*status & VIRTIO_CONFIG_S_DRIVER_OK));

    // Initialize the RPMsg transport structure /
    pru_rpmsg_init(&transport, &resourceTable.rpmsg_vring0, &resourceTable.rpmsg_vring1, TO_ARM_HOST, FROM_ARM_HOST);

    // Create the RPMsg channel between the PRU and ARM user space using the transport structure. /
    while (pru_rpmsg_channel(RPMSG_NS_CREATE, &transport, CHAN_NAME, CHAN_DESC, CHAN_PORT) != PRU_RPMSG_SUCCESS);
    	while (1) {
        /* Check bit 30 of register R31 to see if the ARM has kicked us */
        if (__R31 & HOST_INT) {
            /* Clear the event status */
            CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;
            /* Receive all available messages, multiple messages can be sent per kick */

            while (pru_rpmsg_receive(&transport, &src, &dst, payload, &len) == PRU_RPMSG_SUCCESS) {
                if (strcmp(payload, "STOP_SENDING") == 0) {
                    // A message was received from Linux to stop sending messages
                    break;
                }

                int k = 0;
                for (k = 0; k < 100000; k++) {
                    int i, j = 0;
                    int max = 490;
                    for (i = 0; i < max; i++) {
                        if (i % 50 == 0 && i != 0) {
                            j++;
                        }
                        payload[i] = 'A' + j;
                        __delay_cycles(1000); 

                    }
                    
                    // Check for messages from Linux before sending a message
                    while (pru_rpmsg_receive(&transport, &src, &dst, payload, &len) == PRU_RPMSG_SUCCESS) {
                        if (strcmp(payload, "STOP_SENDING") == 0) {
                            // A message was received from Linux to stop sending messages
                            break;
                        }
                    }
                    
                    // If a stop message was received, break out of the sending loop
                    if (strcmp(payload, "STOP_SENDING") == 0) {
                        break;
                    }

                    pru_rpmsg_send(&transport, dst, src, payload, strlen(payload) + 1);
                }
            }
        }
      }
    
}


