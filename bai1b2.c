#include <stdint.h>

#define RCC_APB2ENR  (*(volatile uint32_t*)0x40021018)

#define GPIOA_CRH    (*(volatile uint32_t*)0x40010804)

#define USART1_SR    (*(volatile uint32_t*)0x40013800)
#define USART1_DR    (*(volatile uint32_t*)0x40013804)
#define USART1_BRR   (*(volatile uint32_t*)0x40013808)
#define USART1_CR1   (*(volatile uint32_t*)0x4001380C)

#define MA_LOP  "Lop01"
#define MA_NHOM "- N09"

void UART1_Init(void)
{
    RCC_APB2ENR |= (1 << 2);
    RCC_APB2ENR |= (1 << 14);

    GPIOA_CRH &= ~(0xF << 4);
    GPIOA_CRH |= (0xB << 4);

    GPIOA_CRH &= ~(0xF << 8);
    GPIOA_CRH |= (0x4 << 8);

    USART1_BRR = 0x45;

    USART1_CR1 = (1 << 13) | (1 << 3) | (1 << 2);
}

void UART1_SendChar(char c)
{
    while (!(USART1_SR & (1 << 7)));

    USART1_DR = c;
}

void UART1_SendString(char *str)
{
    while (*str)
    {
        UART1_SendChar(*str);
        str++;
    }
}

char UART1_ReceiveChar(void)
{
    while (!(USART1_SR & (1 << 5)));

    return (char)(USART1_DR & 0xFF);
}

int main(void)
{
    char buffer[100];
    uint32_t index = 0;
    char c;

    UART1_Init();

    UART1_SendString("\r\n");
    UART1_SendString("================================\r\n");
    UART1_SendString("STM32 UART START!\r\n");
    UART1_SendString("Nhap tin nhan va ket thuc bang !\r\n");
    UART1_SendString("================================\r\n");

    while (1)
    {
        c = UART1_ReceiveChar();
        if (c == '!')
        {
            buffer[index] = '\0';
            UART1_SendString("\r\n");
            UART1_SendString(MA_LOP);
            UART1_SendString(MA_NHOM);
            UART1_SendString(": ");
            UART1_SendString(buffer);
            UART1_SendString("\r\n");
            index = 0;
        }
        else
        {
            if (index < 99)
            {
                buffer[index] = c;
                index++;
                UART1_SendChar(c);
            }
        }
    }

    return 0;
}
