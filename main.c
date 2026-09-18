#include <stdint.h>

#define RCC_APB2ENR (*(volatile uint32_t*)0x40021018)
#define RCC_APB1ENR (*(volatile uint32_t*)0x4002101C)

#define GPIOA_CRL (*(volatile uint32_t*)0x40010800)
#define GPIOA_CRH (*(volatile uint32_t*)0x40010804)

#define USART1_SR (*(volatile uint32_t*)0x40013800)
#define USART1_DR (*(volatile uint32_t*)0x40013804)
#define USART1_BRR (*(volatile uint32_t*)0x40013808)
#define USART1_CR1 (*(volatile uint32_t*)0x4001380C)

#define TIM2_CR1 (*(volatile uint32_t*)0x40000000)
#define TIM2_EGR (*(volatile uint32_t*)0x40000014)
#define TIM2_CCMR1 (*(volatile uint32_t*)0x40000018)
#define TIM2_CCER (*(volatile uint32_t*)0x40000020)
#define TIM2_PSC (*(volatile uint32_t*)0x40000028)
#define TIM2_ARR (*(volatile uint32_t*)0x4000002C)
#define TIM2_CCR1 (*(volatile uint32_t*)0x40000034)

void USART1_IRQHandler(void)
{
}

void UART1_Init(void)
{
    RCC_APB2ENR |= (1<<2);
    RCC_APB2ENR |= (1<<14);

    GPIOA_CRH &= ~(0xF<<4);
    GPIOA_CRH |= (0xB<<4);

    GPIOA_CRH &= ~(0xF<<8);
    GPIOA_CRH |= (0x4<<8);

    USART1_BRR = 0x45;

    USART1_CR1 = 0;
    USART1_CR1 |= (1<<13);
    USART1_CR1 |= (1<<3);
    USART1_CR1 |= (1<<2);
}

void UART_SendChar(char c)
{
    while(!(USART1_SR & (1<<7)));
    USART1_DR = c;
}

char UART_ReceiveChar(void)
{
    while(!(USART1_SR & (1<<5)));
    return (char)USART1_DR;
}

void UART_SendString(char *s)
{
    while(*s)
        UART_SendChar(*s++);
}

void TIM2_Init(void)
{
    RCC_APB1ENR |= (1<<0);

    GPIOA_CRL &= ~(0xF<<0);
    GPIOA_CRL |= (0xB<<0);

    TIM2_PSC = 7;
    TIM2_ARR = 999;

    TIM2_CCMR1 = 0;
    TIM2_CCMR1 |= (6<<4);
    TIM2_CCMR1 |= (1<<3);

    TIM2_CCER |= (1<<0);

    TIM2_CCR1 = 0;

    TIM2_EGR |= (1<<0);
    TIM2_CR1 |= (1<<0);
}

void PWM_Set(uint8_t percent)
{
    if(percent > 100)
        percent = 100;

    TIM2_CCR1 = ((TIM2_ARR + 1) * percent) / 100;
}

void LED_ON(void)
{
    PWM_Set(50);
}

void LED_OFF(void)
{
    TIM2_CCR1 = 0;
}

int main(void)
{
    char buffer[50];
    uint8_t index=0;
    char c;

    UART1_Init();
    TIM2_Init();

    LED_OFF();

    UART_SendString("\r\nBAI 5 UART PWM READY\r\n");
    UART_SendString("Nhap lenh va ket thuc bang !\r\n");
    UART_SendString("ON!  OFF!  PWM:50!  Status!\r\n");
    UART_SendString("> ");

    while(1)
    {
        c = UART_ReceiveChar();

        if(c == '!')
        {
            buffer[index] = '\0';

            UART_SendString("\r\nBan vua nhap: ");
            UART_SendString(buffer);
            UART_SendString("\r\n");

            if(buffer[0]=='O' && buffer[1]=='N' && buffer[2]=='\0')
            {
                LED_ON();
                UART_SendString("LED ON - PWM 50%\r\n");
            }
            else if(buffer[0]=='O' && buffer[1]=='F' && buffer[2]=='F' && buffer[3]=='\0')
            {
                LED_OFF();
                UART_SendString("LED OFF - PWM 0%\r\n");
            }
            else if(buffer[0]=='P' && buffer[1]=='W' && buffer[2]=='M' && buffer[3]==':' )
            {uint8_t value=0;
                uint8_t i=4;

                while(buffer[i]>='0' && buffer[i]<='9')
                {
                    value = value*10 + (buffer[i]-'0');
                    i++;
                }

                if(value<=100)
                {
                    PWM_Set(value);
                    UART_SendString("PWM = ");

                    if(value>=100)
                        UART_SendChar('1');

                    if(value>=10)
                        UART_SendChar('0'+((value/10)%10));

                    UART_SendChar('0'+(value%10));
                    UART_SendString("%\r\n");
                }
                else
                {
                    UART_SendString("Invalid PWM\r\n");
                }
            }
            else if(buffer[0]=='S' && buffer[1]=='t' && buffer[2]=='a' && buffer[3]=='t' && buffer[4]=='u' && buffer[5]=='s' && buffer[6]=='\0')
            {
                UART_SendString("Status OK\r\n");
            }
            else
            {
                UART_SendString("Invalid command\r\n");
            }

            index=0;
            UART_SendString("> ");
        }
        else
        {
            if(index<49)
            {
                buffer[index] = c;
                index++;
                UART_SendChar(c);
            }
        }
    }
}
