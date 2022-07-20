#include <stdio.h> 
#include <string.h>
#include <math.h>
#include <stdio.h> 
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

#include <sys/types.h>

#include "ov7670-i2c.h"

#define I2CBUS 2
#define CAM_ADDR 0x21


int main(void) {
	/* CAM0 connected ro I2C1
	 * Note: Due to dynamic I2C activation, it ends up as /dev/i2c-2 */
	if (ov7670_i2c_setup(I2CBUS, CAM_ADDR, 73)) {
		printf("ERROR: Could not initialize OV7670 CAM0 via I2C\n");
		/* do not exit on errors - the other cam may still capture */
	}
	else {
		printf("Camera initialized\n");
	}

	return 0;
};