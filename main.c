#include <stdint.h>

#define RCC_BASE        0x40021000UL
#define RCC_APB2ENR     *(volatile uint32_t *)(RCC_BASE + 0x18)

#define GPIOA_BASE      0x40010800UL
#define GPIOA_CRL       *(volatile uint32_t *)(GPIOA_BASE + 0x00)
#define GPIOA_ODR       *(volatile uint32_t *)(GPIOA_BASE + 0x0C)

#define SYSTICK_BASE    0xE000E010UL
#define STK_CTRL        *(volatile uint32_t *)(SYSTICK_BASE + 0x00)
#define STK_LOAD        *(volatile uint32_t *)(SYSTICK_BASE + 0x04)
#define STK_VAL         *(volatile uint32_t *)(SYSTICK_BASE + 0x08)

volatile uint32_t ms_count_10hz = 0;
volatile uint32_t ms_count_1hz  = 0;
volatile uint32_t ms_count_01hz = 0;

__attribute__((interrupt)) void SysTick_Handler(void) {
    ms_count_10hz++;
    ms_count_1hz++;
    ms_count_01hz++;

    if (ms_count_10hz >= 50) {
        GPIOA_ODR ^= (1 << 2);
        ms_count_10hz = 0;
    }

    if (ms_count_1hz >= 500) {
        GPIOA_ODR ^= (1 << 1);
        ms_count_1hz = 0;
    }

    if (ms_count_01hz >= 5000) {
        GPIOA_ODR ^= (1 << 0); 
        ms_count_01hz = 0;
    }
}

int main(void) {
    RCC_APB2ENR |= (1 << 2);

    GPIOA_CRL &= ~(0xFFF);
    GPIOA_CRL |=  (0x111);

    STK_LOAD = 8000 - 1;
    STK_VAL  = 0;
    
    STK_CTRL = 0x07; 

    __asm volatile ("cpsie i");

    while (1) {
    }
}
