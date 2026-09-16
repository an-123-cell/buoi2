#include <stdint.h>

extern uint32_t _estack;
extern int main(void);

void Reset_Handler(void);
void SysTick_Handler(void);

void Default_Handler(void) {
    while (1);
}

void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

__attribute__ ((section(".isr_vector")))
uint32_t *isr_vector[] = {
    (uint32_t *)&_estack,
    (uint32_t *)Reset_Handler,
    (uint32_t *)Default_Handler,
    (uint32_t *)Default_Handler,
    (uint32_t *)Default_Handler,
    (uint32_t *)Default_Handler,
    (uint32_t *)Default_Handler,
    0, 0, 0, 0,
    (uint32_t *)Default_Handler,
    (uint32_t *)Default_Handler,
    0,
    (uint32_t *)Default_Handler,
    (uint32_t *)SysTick_Handler
};

void Reset_Handler(void) {
    main();
    while (1);
}
