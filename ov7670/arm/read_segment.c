#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>


#define PRU_ADDR        0x4A300000      // Start of PRU memory Page 184 am335x TRM
#define PRU_LEN         0x80000         // Length of PRU memory
#define PRU_SHAREDMEM   0x10000         // Offset to shared memory


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

    prusharedMem_32int_ptr = pru + PRU_SHAREDMEM/4 + 0x200/4; // Points to start of shared memory
    
    int n_it = 21;

    for (int i = 0; i < n_it; i++) {
        printf("Data: %d\n", prusharedMem_32int_ptr[i]);
    }
}

