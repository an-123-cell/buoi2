#include <stdint.h>

#define RCC_BASE        0x40021000UL
#define RCC_CR          (*(volatile uint32_t *)(RCC_BASE + 0x00))
#define RCC_CFGR        (*(volatile uint32_t *)(RCC_BASE + 0x04))
#define RCC_APB2ENR     (*(volatile uint32_t *)(RCC_BASE + 0x18))

#define FLASH_BASE      0x40022000UL
#define FLASH_ACR       (*(volatile uint32_t *)(FLASH_BASE + 0x00))

#define GPIOA_BASE      0x40010800UL
#define GPIOA_CRH       (*(volatile uint32_t *)(GPIOA_BASE + 0x04))

#define TIM1_BASE       0x40012C00UL
#define TIM1_CR1        (*(volatile uint32_t *)(TIM1_BASE + 0x00))
#define TIM1_EGR        (*(volatile uint32_t *)(TIM1_BASE + 0x14))
#define TIM1_CCMR1      (*(volatile uint32_t *)(TIM1_BASE + 0x18))
#define TIM1_CCMR2      (*(volatile uint32_t *)(TIM1_BASE + 0x1C))
#define TIM1_CCER       (*(volatile uint32_t *)(TIM1_BASE + 0x20))
#define TIM1_PSC        (*(volatile uint32_t *)(TIM1_BASE + 0x28))
#define TIM1_ARR        (*(volatile uint32_t *)(TIM1_BASE + 0x2C))
#define TIM1_CCR1       (*(volatile uint32_t *)(TIM1_BASE + 0x34))
#define TIM1_CCR2       (*(volatile uint32_t *)(TIM1_BASE + 0x38))
#define TIM1_CCR3       (*(volatile uint32_t *)(TIM1_BASE + 0x3C))
#define TIM1_CCR4       (*(volatile uint32_t *)(TIM1_BASE + 0x40))
#define TIM1_BDTR       (*(volatile uint32_t *)(TIM1_BASE + 0x44))


void Clock_Init(void)
{
    FLASH_ACR = 0x02;

    RCC_CR |= (1 << 16);
    while ((RCC_CR & (1 << 17)) == 0);

    RCC_CFGR = 0;

    RCC_CFGR |= (4 << 8);
    RCC_CFGR |= (1 << 16);
    RCC_CFGR |= (7 << 18);

    RCC_CR |= (1 << 24);
    while ((RCC_CR & (1 << 25)) == 0);

    RCC_CFGR &= ~(3 << 0);
    RCC_CFGR |= (2 << 0);

    while ((RCC_CFGR & (3 << 2)) != (2 << 2));
}


void GPIO_Init(void)
{
    RCC_APB2ENR |= (1 << 0);
    RCC_APB2ENR |= (1 << 2);
    RCC_APB2ENR |= (1 << 11);

    GPIOA_CRH = 0x0000BBBB;
}


void TIM1_PWM_Init(void)
{
    TIM1_PSC = 71;
    TIM1_ARR = 999;

    TIM1_CCMR1 = 0;

    TIM1_CCMR1 |= (6 << 4);
    TIM1_CCMR1 |= (1 << 3);

    TIM1_CCMR1 |= (6 << 12);
    TIM1_CCMR1 |= (1 << 11);

    TIM1_CCMR2 = 0;

    TIM1_CCMR2 |= (6 << 4);
    TIM1_CCMR2 |= (1 << 3);

    TIM1_CCMR2 |= (6 << 12);
    TIM1_CCMR2 |= (1 << 11);

    TIM1_CCR1 = 10;
    TIM1_CCR2 = 50;
    TIM1_CCR3 = 200;
    TIM1_CCR4 = 900;

    TIM1_CCER = 0;

    TIM1_CCER |= (1 << 0);
    TIM1_CCER |= (1 << 4);
    TIM1_CCER |= (1 << 8);
    TIM1_CCER |= (1 << 12);

    TIM1_BDTR |= (1 << 15);

    TIM1_EGR |= (1 << 0);

    TIM1_CR1 |= (1 << 7);
    TIM1_CR1 |= (1 << 0);
}


int main(void)
{
    Clock_Init();
    GPIO_Init();
    TIM1_PWM_Init();

    while (1)
    {
    }
}
