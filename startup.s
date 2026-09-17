.syntax unified
.cpu cortex-m3
.thumb

.global _estack
.global Reset_Handler
.global SysTick_Handler
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

.section .text.Reset_Handler
.thumb_func
.type Reset_Handler, %function

Reset_Handler:
    bl main

loop:
    b loop

.section .text.Default_Handler
.thumb_func
.type Default_Handler, %function

Default_Handler:
    b Default_Handler

.section .text.SysTick_Handler
.thumb_func
.type SysTick_Handler, %function

SysTick_Handler:
    bx lr
