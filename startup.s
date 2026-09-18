.syntax unified
.cpu cortex-m3
.thumb

.global _estack
.global Reset_Handler
.global SysTick_Handler
.global USART1_IRQHandler

.extern main

.section .isr_vector,"a",%progbits

.word _estack
.word Reset_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word SysTick_Handler

.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word USART1_IRQHandler

.section .text.Reset_Handler
.thumb_func

Reset_Handler:
    bl main

loop:
    b loop

.section .text.Default_Handler
.thumb_func

Default_Handler:
    b Default_Handler

.section .text.SysTick_Handler
.thumb_func

SysTick_Handler:
    bx lr
