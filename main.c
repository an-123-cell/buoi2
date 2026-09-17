#include <stdint.h>

#define RCC_APB2ENR  (*(volatile uint32_t*)0x40021018)

#define GPIOA_CRL    (*(volatile uint32_t*)0x40010800)
#define GPIOA_CRH    (*(volatile uint32_t*)0x40010804)

#define USART1_SR    (*(volatile uint32_t*)0x40013800)
#define USART1_DR    (*(volatile uint32_t*)0x40013804)
#define USART1_BRR   (*(volatile uint32_t*)0x40013808)
#define USART1_CR1   (*(volatile uint32_t*)0x4001380C)

#define ADC1_SR      (*(volatile uint32_t*)0x40012400)
#define ADC1_CR2     (*(volatile uint32_t*)0x40012408)
#define ADC1_SMPR2   (*(volatile uint32_t*)0x40012410)
#define ADC1_SQR1    (*(volatile uint32_t*)0x4001242C)
#define ADC1_SQR3    (*(volatile uint32_t*)0x40012434)
#define ADC1_DR      (*(volatile uint32_t*)0x4001244C)

#define SYST_CSR     (*(volatile uint32_t*)0xE000E010)
#define SYST_RVR     (*(volatile uint32_t*)0xE000E014)
#define SYST_CVR     (*(volatile uint32_t*)0xE000E018)

#define MA_LOP  "Lop01"
#define MA_NHOM "- N09"

void UART1_Init(void)
{
    RCC_APB2ENR |= (1 << 2);
    RCC_APB2ENR |= (1 << 14);

    /* PA9 TX */
    GPIOA_CRH &= ~(0xF << 4);
    GPIOA_CRH |= (0xB << 4);

    /* PA10 RX */
    GPIOA_CRH &= ~(0xF << 8);
    GPIOA_CRH |= (0x4 << 8);

    /* 115200 baud - clock 8MHz */
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

void UART1_SendNumber(uint32_t num)
{
    char buffer[10];
    uint32_t i = 0;

    if (num == 0)
    {
        UART1_SendChar('0');
        return;
    }

    while (num > 0)
    {
        buffer[i] = (num % 10) + '0';
        num = num / 10;
        i++;
    }

    while (i > 0)
    {
        i--;
        UART1_SendChar(buffer[i]);
    }
}

void ADC1_Init(void)
{
    /* Enable ADC1 clock */
    RCC_APB2ENR |= (1 << 9);

    /* PA0 = Analog input */
    GPIOA_CRL &= ~(0xF << 0);

    /* Channel 0 sample time = 239.5 cycles */
    ADC1_SMPR2 &= ~(0x7 << 0);
    ADC1_SMPR2 |= (0x7 << 0);

    /* One conversion */
    ADC1_SQR1 = 0;

    /* First conversion = Channel 0 */
    ADC1_SQR3 = 0;

    /* ADC ON */
    ADC1_CR2 = (1 << 0);

    /* Wait */
    for (volatile uint32_t i = 0; i < 10000; i++);

    /* Reset calibration */
    ADC1_CR2 |= (1 << 3);

    while (ADC1_CR2 & (1 << 3));

    /* Start calibration */
    ADC1_CR2 |= (1 << 2);

    while (ADC1_CR2 & (1 << 2));

    /* Software trigger */
    ADC1_CR2 |= (1 << 20);
    ADC1_CR2 |= (7 << 17);
}

uint16_t ADC1_Read(void)
{
    ADC1_SR &= ~(1 << 1);

    ADC1_CR2 |= (1 << 22);

    while (!(ADC1_SR & (1 << 1)));

    return (uint16_t)(ADC1_DR & 0xFFF);
}

void SysTick(void)
{
    /* 8MHz / 8000 = 1kHz = 1ms */
    SYST_RVR = 8000 - 1;
    SYST_CVR = 0;

    /* Enable SysTick, processor clock */
    SYST_CSR = 5;

    /* 1000 x 1ms = 1 second */
    for (volatile uint32_t i = 0; i < 1000; i++)
    {while (!(SYST_CSR & (1 << 16)));
    }

    SYST_CSR = 0;
}

int main(void)
{
    uint16_t adc_value;
    uint32_t voltage;

    UART1_Init();
    ADC1_Init();

    UART1_SendString("\r\n");
    UART1_SendString("================================\r\n");
    UART1_SendString("BAI 3 - CAM BIEN ANH SANG\r\n");
    UART1_SendString(MA_LOP);
    UART1_SendString(MA_NHOM);
    UART1_SendString("\r\n");
    UART1_SendString("================================\r\n");

    while (1)
    {
        adc_value = ADC1_Read();

        voltage = ((uint32_t)adc_value * 3300) / 4095;

        UART1_SendString("ADC = ");
        UART1_SendNumber(adc_value);

        UART1_SendString(" | Voltage = ");
        UART1_SendNumber(voltage);

        UART1_SendString(" mV\r\n");

        SysTick();
    }

    return 0;
}
