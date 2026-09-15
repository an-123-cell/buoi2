#include <stdint.h>

extern uint32_t _estack;
extern int main(void);

void Reset_Handler(void) {
    main();
    while(1);
}

__attribute__ ((section(".isr_vector")))
uint32_t *isr_vector[] = {
    (uint32_t *)&_estack,
    (uint32_t *)Reset_Handler
};
