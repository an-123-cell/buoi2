#include <stdint.h>

extern uint32_t _estack;
extern int main(void);

/* Khai báo hàm xử lý ngắt SysTick và Default */
void Reset_Handler(void);
void SysTick_Handler(void);

void Default_Handler(void) {
    while (1);
}

/* Định nghĩa lại ngắt SysTick dưới dạng weak (để bên main.c có thể override) */
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

/* Bảng Vector ngắt chuẩn Cortex-M3 */
__attribute__ ((section(".isr_vector")))
uint32_t *isr_vector[] = {
    (uint32_t *)&_estack,        // 0: Initial Stack Pointer
    (uint32_t *)Reset_Handler,   // 1: Reset
    (uint32_t *)Default_Handler, // 2: NMI
    (uint32_t *)Default_Handler, // 3: HardFault
    (uint32_t *)Default_Handler, // 4: MemManage
    (uint32_t *)Default_Handler, // 5: BusFault
    (uint32_t *)Default_Handler, // 6: UsageFault
    0, 0, 0, 0,                  // 7-10: Reserved
    (uint32_t *)Default_Handler, // 11: SVCall
    (uint32_t *)Default_Handler, // 12: Debug Monitor
    0,                           // 13: Reserved
    (uint32_t *)Default_Handler, // 14: PendSV
    (uint32_t *)SysTick_Handler  // 15: SysTick Handler (BẮT BUỘC PHẢI CÓ)
};

void Reset_Handler(void) {
    main();
    while (1);
}
