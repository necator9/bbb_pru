#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>


#define PRU_ADDR        0x4A300000      // Start of PRU memory Page 184 am335x TRM
#define PRU_LEN         0x80000         // Length of PRU memory
#define PRU_SHAREDMEM   0x10000         // Offset to shared memory


unsigned int *prusharedMem_32int_ptr;    // Points to the start of the shared memory
unsigned int *buffer_status;

int save_image_rgb(unsigned int *image, unsigned int x_res, unsigned int y_res, const char *filename) {
	FILE *outf;
	outf = fopen(filename, "wb");
	if (!outf)
		return -ENOENT;

	fprintf(outf, "P6\n%d %d\n255\n", x_res, y_res);

    for (int i = 0; i < x_res * y_res; i++) {
        unsigned int red = image[i] >> 11;
        unsigned int green = image[i] >> 5 & 63;
        unsigned int blue = image[i] & 31;
        fputc(red , outf);
        fputc(green, outf);
        fputc(blue, outf);
    }

	fclose(outf);

	return 0;
}





int main(int argc, char *argv[]) {
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

    prusharedMem_32int_ptr = pru + PRU_SHAREDMEM/4 + 0x10/4; // Points to start of shared memory
    buffer_status = pru + PRU_SHAREDMEM/4;

    const int buf_size = 2000;
    unsigned int x_res = 640;
    unsigned int y_res = 480;
    const int img_res = x_res * y_res;
    const int buffer_empty = 752;
    const int buffer_full = 751;
    unsigned int img[img_res + buf_size];

    int i = 0;
    unsigned int buff_counter = 0;

    while (i < img_res) {
        if (buffer_status[0] == buffer_full) {
            buff_counter++;
            for (int k = 0; k < buf_size; k++) {
                unsigned int res = prusharedMem_32int_ptr[k];
                img[i++] = res >> 16;             // 2 bytes for 1st pixel
                unsigned int mask = (1 << 8) - 1;
                img[i++] = res & mask;          // 2 bytes for 2nd pixel
            }
            buffer_status[0] = buffer_empty;
        }
        // printf("%d    %d    %d    %d\n", i, buffer_status[0], buffer_status[1], buff_counter);
    }

    save_image_rgb(img, x_res, y_res, "test.ppm");

    for (int i = 0; i < img_res; i++) {
        printf("%u\n", img[i]);
    }
    // printf("Data: %d\n", prusharedMem_32int_ptr[0]);
    // printf("Buffer status: %d\n", buffer_status[0]);
}

