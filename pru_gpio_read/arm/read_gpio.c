/*
 *
 * Fetch gpio value read by pru
 *
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>


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
    
    int n_it = 10;

    unsigned int result[n_it];
    result[0] = 0;
    int k = 1;

    clock_t t_result[n_it];

    
    while (k < n_it) {
        unsigned int level = prusharedMem_32int_ptr[0];
        if (level != result[k - 1]) {
            result[k] = level;
            t_result[k] = clock(); 
            k++;
        }
        // sleep(5);
    }

    printf("%d changes happened\n", k);

    for (int i = 1; i < n_it; i++) {
        // long double period = t_result[i] - t_result[i - 1];
        // long double freq = 1 / period / CLOCKS_PER_SEC * 1000000;
        // printf("lvl - %x; time - %ld; period - %Lf; freq - %Lf\n" , result[i], t_result[i], period, freq);
        double diff = (double) t_result[i] - t_result[i - 1];
        printf("lvl - %x; time - %ld; diff - %f\n" , result[i], t_result[i], diff);
    }

    printf("%ld\n", CLOCKS_PER_SEC);

    if(munmap(pru, PRU_LEN)) {
        printf("munmap failed\n");
    } else {
        printf("munmap succeeded\n");
    }
}

