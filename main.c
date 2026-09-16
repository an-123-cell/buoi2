#include <stdint.h>

/* Địa chỉ thanh ghi */
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

/* Bắt buộc gắn attribute để tránh linker loại bỏ hàm ngắt */
__attribute__((interrupt)) void SysTick_Handler(void) {
    ms_count_10hz++;
    ms_count_1hz++;
    ms_count_01hz++;

    // 10 Hz -> Toggling mỗi 50 ms
    if (ms_count_10hz >= 50) {
        GPIOA_ODR ^= (1 << 2); // PA2
        ms_count_10hz = 0;
    }

    // 1 Hz -> Toggling mỗi 500 ms
    if (ms_count_1hz >= 500) {
        GPIOA_ODR ^= (1 << 1); // PA1
        ms_count_1hz = 0;
    }

    // 0.1 Hz -> Toggling mỗi 5000 ms
    if (ms_count_01hz >= 5000) {
        GPIOA_ODR ^= (1 << 0); // PA0
        ms_count_01hz = 0;
    }
}

int main(void) {
    /* Enable Clock GPIOA */
    RCC_APB2ENR |= (1 << 2);

    /* Cấu hình PA0, PA1, PA2 làm Output General Purpose Push-Pull (Max speed 10MHz) */
    GPIOA_CRL &= ~(0xFFF); // Reset cấu hình PA0, PA1, PA2
    GPIOA_CRL |=  (0x111); // Mode: 01 (Output 10MHz), CNF: 00 (Push-Pull)

    /* Cấu hình SysTick ngắt 1ms */
    // Mặc định sau Reset, STM32F1 dùng HSI = 8MHz.
    // 8,000,000 / 1000 = 8000 tick/ms
    STK_LOAD = 8000 - 1;
    STK_VAL  = 0;
    
    // Enable Counter (bit 0), Enable Interrupt (bit 1), Source = AHB (bit 2) -> 0x07
    STK_CTRL = 0x07; 

    // Bật toàn cục ngắt CPSIE i
    __asm volatile ("cpsie i");

    while (1) {
        // Chờ ngắt
    }
}
