#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

typedef struct {
  uint32_t dr;
  uint32_t gdir;
  uint32_t psr;
} MbGpio;

static const MbGpio gpio[] = {
  { .dr = 0x0209C000, .gdir = 0x0209C004, .psr = 0x0209C008 } // GPIO1
};

static const uint32_t gpiobase = 0x0209C000;

static volatile uint32_t *gpiomap = NULL;

int main(int argc, char **argv)
{
    int fd = 0;

    // map 1MB starting at GPIO1_DR
    fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) {
      printf("Unable to open /dev/mem: %s\n", strerror(errno));
      return 1;
    }
    gpiomap = (uint32_t*)mmap(0, getpagesize() * 250,
			   PROT_READ|PROT_WRITE, MAP_SHARED, 
			   fd, gpiobase);
    if (gpiomap == MAP_FAILED) {
      printf("mmap failed: %s\n", strerror(errno));
      return 1;
    }

    // set GPIO1_IO_5 as output
    *(gpiomap+(gpio[0].gdir-gpiobase)/4) |= 1<<5;
    
    // blink
    for (;;) {
      *(gpiomap+(gpio[0].dr-gpiobase)/4) ^= 1<<5;
      usleep(500000);
    }

    // shutdown
    if (munmap((void*)gpiomap, (getpagesize() * 250)) < 0) {
      printf("munmap failed: %s\n", strerror(errno));
      return -1;
    }
    close(fd);
}
