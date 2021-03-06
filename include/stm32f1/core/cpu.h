/*
 * cpu.h - abstract CPU clock initialization, time and sleeping
 */

#ifndef CPU_H_
#define CPU_H_
#include <stm32f1xx.h>
#include <core_cm3.h>
#include <inttypes.h>

template<uint32_t MCLK_FREQ >
struct cpu_base_t {
  static const uint32_t frequency = MCLK_FREQ;
  static const uint32_t cycles_per_msec = frequency / 1000;
  static const uint32_t cycles_per_usec = frequency / 1000000;

  static void enable_clkout() {
  }

  static void reset(void) {
    NVIC_SystemReset();
  }
};

typedef cpu_base_t<F_CPU > CPU;

extern "C" {
volatile uint32_t tickcount;

uint32_t millis(void) {
  return tickcount;
}

/*
 * vector table systick handler must have c name bindings
 */
void SysTick_Handler(void) {
  ++tickcount;
}

}

/**
 * @brief Delay the given number of microseconds.
 *
 * @param us Number of microseconds to delay.
 */
static inline void delay_us(uint32_t us) {
  us *= 20; /*STM32_DELAY_US_MULT */

  /* fudge for function call overhead  */
  us--;
  asm volatile(
      "   mov r0, %[us]          \n\t"
      "1: subs r0, #1            \n\t"
      "   bhi 1b                 \n\t"
      :
      : [us] "r" (us)
      : "r0"
  );
}

void delayMicroseconds(uint32_t us) {
  delay_us(us);
}

/*
 * delay - delay milliseconds
 */
void __noinline delay(const unsigned msecs) {
  if ( msecs ) {
    unsigned t0 = tickcount;

    while((tickcount - t0) < msecs ) {
      //__WFI();
    }
  }
}

#endif /* CPU_H_ */
