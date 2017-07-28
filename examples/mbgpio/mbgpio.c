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

typedef struct {
  uint32_t cr;
  uint32_t sr;
  uint32_t ir;
  uint32_t sar;
  uint32_t pr;
  uint32_t cnr;
} MbPwm;

static const MbPwm pwm[] = {
  { .cr = 0x02080000, .sr = 0x02080004, .ir = 0x02080008, .sar = 0x0208000C,
    .pr = 0x02080010, .cnr = 0x02080014 } // PWM1
};

#define PWMCR_PRESCALER(x)      (((x - 1) & 0xFFF) << 4)
#define PWMCR_DOZEEN            (1 << 24)
#define PWMCR_WAITEN            (1 << 23)
#define PWMCR_DBGEN             (1 << 22)
#define PWMCR_CLKSRC_IPG_HIGH   (2 << 16)
#define PWMCR_CLKSRC_IPG        (1 << 16)
#define PWMCR_EN                (1 << 0)


static const uint32_t imxbase = 0x02000000;

static volatile uint32_t *imxmap = NULL;

#define MBREG(reg) (*(imxmap+((reg)-imxbase)/4))

int pwm_imx_get_parms(uint32_t period_ns, uint32_t duty_ns,
		      uint32_t* period_c,
		      uint32_t* duty_c,
		      uint32_t* prescale)
{
  const uint32_t CONFIG_IMX6_PWM_PER_CLK = 24000000U;
  uint64_t c;

  /*
   * we have not yet a clock framework for imx6, so add the clock
   * value here as a define. Replace it when we have the clock
   * framework.
   */
  c = CONFIG_IMX6_PWM_PER_CLK;
  c = c * period_ns;
  c /= 1000000000U;
  *period_c = c;

  *prescale = *period_c / 0x10000 + 1U;

  *period_c /= *prescale;
  c = (uint64_t)*period_c * (uint64_t)duty_ns;
  c /= period_ns;
  *duty_c = c;

  /*
   * according to imx pwm RM, the real period value should be
   * PERIOD value in PWMPR plus 2.
   */
  if (*period_c > 2U) {
    *period_c -= 2U;
  } else {
    *period_c = 0U;
  }

  return 0;
}


int main(int argc, char **argv)
{
  //const uint32_t period_ns = 1000000000U; // [ns]
  //const uint32_t duty_ns   =  300000000U; // [ns]
  const uint32_t period_ns = 100000000U; // [ns]
  const uint32_t duty_ns   =  90000000U; // [ns]
  uint32_t period_cycles;
  uint32_t duty_cycles;
  uint32_t prescale;
  int fd = 0;

  // map 1MB starting at GPIO1_DR
  fd = open("/dev/mem", O_RDWR | O_SYNC);
  if (fd < 0) {
    printf("Unable to open /dev/mem: %s\n", strerror(errno));
    return 1;
  }
  imxmap = (uint32_t*)mmap(0, getpagesize() * 250,
			   PROT_READ|PROT_WRITE, MAP_SHARED, 
			   fd, imxbase);
  if (imxmap == MAP_FAILED) {
    printf("mmap failed: %s\n", strerror(errno));
    return 1;
  }

  // set GPIO1_IO_5 as output
  MBREG(gpio[0].gdir) |= 1<<5;

  // configure PWM
  pwm_imx_get_parms(period_ns, duty_ns, &period_cycles, &duty_cycles,
                          &prescale);
  printf("period_cycles=%lu\n", period_cycles);
  printf("duty_cycles=%lu\n", duty_cycles);
  printf("prescale=%lu\n", prescale);
  printf("PWMCR_PRESCALER=%lu\n", PWMCR_PRESCALER(prescale));
  printf("IOMUXC_SW_MUX_CTL_PAD_USB_H_STROBE=%lu\n", MBREG(0x020E02A8));

  // reset
  /*
  MBREG(pwm[0].cr) |= 0x8;
  do {
    printf("PWMCR=%lu\n", MBREG(pwm[0].cr));
  } while (pwm[0].cr & 0x8);
  */
  printf("PWMCR=%lu\n", MBREG(pwm[0].cr));
  MBREG(pwm[0].cr) &= 0xFFFFFFFE; // disable
  printf("PWMCR=%lu\n", MBREG(pwm[0].cr));
  //MBREG(pwm[0].ir) = 0x7;
  MBREG(pwm[0].sar) = duty_cycles;
  printf("PWMSR=%lu\n", MBREG(pwm[0].sr));
  MBREG(pwm[0].sr) |= MBREG(pwm[0].sr);
  printf("PWMSR=%lu\n", MBREG(pwm[0].sr));
  MBREG(pwm[0].pr) = period_cycles;
  MBREG(pwm[0].cr) |= PWMCR_PRESCALER(prescale) |
                     PWMCR_DOZEEN | PWMCR_WAITEN |
    PWMCR_DBGEN | PWMCR_CLKSRC_IPG_HIGH | PWMCR_EN;
  printf("PWMCR=%lu\n", MBREG(pwm[0].cr));

  // blink
  for (;;) {
    printf("PWMSR=%lu\n", MBREG(pwm[0].sr));
    printf("PWMCNR=%lu\n", MBREG(pwm[0].cnr));
    MBREG(gpio[0].dr) ^= 1<<5;
    usleep(500000);
  }

  // shutdown
  if (munmap((void*)imxmap, getpagesize() * 250) < 0) {
    printf("munmap failed: %s\n", strerror(errno));
    return -1;
  }
  close(fd);
}
