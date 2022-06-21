/*
 *
 * Fetch gpio value read by pru
 *
 */

#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>


#define PRU_ADDR        0x4A300000      // Start of PRU memory Page 184 am335x TRM
#define PRU_LEN         0x80000         // Length of PRU memory
#define PRU_SHAREDMEM   0x10000         // Offset to shared memory


unsigned int BLINKING_ON = 0x1212;
unsigned int BLINKING_OFF = 0x1213;
unsigned int ON_STATUS = 0xdeefdeef;
unsigned int OFF_STATUS = 0xbeebbeeb;

unsigned int *prusharedMem_32int_ptr;    // Points to the start of the shared memory


int main(int argc, char *argv[])
{
    unsigned int    *pru;       // Points to start of PRU memory.
    int fd;

    fd = open ("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1) {
        printf ("ERROR: could not open /dev/mem.\n\n");
        return 1;
    }
    pru = mmap (0, PRU_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, PRU_ADDR);
    if (pru == MAP_FAILED) {
        printf ("ERROR: could not map memory.\n\n");
        return 1;
    }
    close(fd);
    printf ("Using /dev/mem.\n");

    prusharedMem_32int_ptr = pru + PRU_SHAREDMEM/4 + 0x200/4; // Points to start of shared memory
    
    int s = 1000;
    unsigned int result[s];

    for (int i = 0; i < s; i++) {
        result[s] = prusharedMem_32int_ptr[0];
        printf("%x\n", result[s]);
    }

    if(munmap(pru, PRU_LEN)) {
        printf("munmap failed\n");
    } else {
        printf("munmap succeeded\n");
    }
}
