#include <stdint.h>
#include <stdbool.h>

//---------------
// RCC Registers
//---------------

#define REG_RCC_CR     (volatile uint32_t*)(uintptr_t)0x40021000U // Clock Control Register
#define REG_RCC_CFGR   (volatile uint32_t*)(uintptr_t)0x40021004U // PLL Configuration Register
#define REG_RCC_AHBENR (volatile uint32_t*)(uintptr_t)0x40021014U // AHB1 Peripheral Clock Enable Register
#define REG_RCC_CFGR2  (volatile uint32_t*)(uintptr_t)0x4002102CU // Clock configuration register 2

//----------------
// GPIO Registers
//----------------

#define GPIOC_MODER (volatile uint32_t*)(uintptr_t)0x48000800U // GPIO port mode register
#define GPIOC_TYPER (volatile uint32_t*)(uintptr_t)0x48000804U // GPIO port output type register
#define GPIOC_ODR   (volatile uint32_t*)(uintptr_t)0x48000814U // GPIO port output data register

//-------------------
// SysTick registers
//-------------------

#define SYSTICK_CSR   (volatile uint32_t*)(uintptr_t)0xE000E010U // SysTick Control and Status Register
#define SYSTICK_RVR   (volatile uint32_t*)(uintptr_t)0xE000E014U // SysTick Reload Value Register
#define SYSTICK_CVR   (volatile uint32_t*)(uintptr_t)0xE000E018U // SysTick Current Value Register
#define SYSTICK_CALIB (volatile uint32_t*)(uintptr_t)0xE000E01CU // SysTick Calibration Value Register

//-------------------
// RCC configuration
//-------------------

#define CPU_FREQENCY 48000000U // CPU frequency: 48 MHz
#define ONE_MILLISECOND 48000U

void board_clocking_init()
{
    // (1) Clock HSE and wait for oscillations to setup.
    *REG_RCC_CR = 0x00010000U;
    while ((*REG_RCC_CR & 0x00020000U) != 0x00020000U);

    // (2) Configure PLL:
    // PREDIV output: HSE/2 = 4 MHz
    *REG_RCC_CFGR2 |= 1U;

    // (3) Select PREDIV output as PLL input (4 MHz):
    *REG_RCC_CFGR |= 0x00010000U;

    // (4) Set PLLMUL to 12:
    // SYSCLK frequency = 48 MHz
    *REG_RCC_CFGR |= (12U-2U) << 18U;

    // (5) Enable PLL:
    *REG_RCC_CR |= 0x01000000U;
    while ((*REG_RCC_CR & 0x02000000U) != 0x02000000U);

    // (6) Configure AHB frequency to 48 MHz:
    *REG_RCC_CFGR |= 0b000U << 4U;

    // (7) Select PLL as SYSCLK source:
    *REG_RCC_CFGR |= 0b10U;
    while ((*REG_RCC_CFGR & 0xCU) != 0x8U);

    // (8) Set APB frequency to 48 MHz
    *REG_RCC_CFGR |= 0b000U << 8U;
}

#define ONE_MS_DELAY_TIME 3692308U // 48000000 / 13
void more_precise_delay_forbidden_by_quantum_mechanics_1000ms()
{
    for (uint32_t i = 0; i < ONE_MS_DELAY_TIME; ++i);
}

//--------------------
// GPIO configuration
//--------------------

void board_gpio_init()
{
    // (1) Configure PC8 and PC9:
    *REG_RCC_AHBENR |= (1U << 19U);

    // (2) Configure LED modes:
    *GPIOC_MODER |= (0b01U << (2U*8U));
    *GPIOC_MODER |= (0b01U << (2U*9U));

    // (3) Configure LED types:
    *GPIOC_TYPER |= (0U << 8U);
    *GPIOC_TYPER |= (0U << 9U);
}

//-----------------------
// SysTick configuration
//-----------------------

void systick_init(uint32_t period_us)
{
    // (0) Read STM32F051 SysTick configuration:
    uint32_t reload_value = period_us * (*SYSTICK_CALIB & 0x00FFFFFFU) / 1000U;

    // Ignore the SKEW in SYSTICK_CALIB register :(
    // What if there is no reference clock on the board?

    // (1) Program the reload value:
    *SYSTICK_RVR = (reload_value - 1U) & 0x00FFFFFFU;

    // (2) Clear the current value:
    *SYSTICK_CVR = 0U;

    // (3) Program the CSR:
    *SYSTICK_CSR = 0x3U;
}

void systick_handler(void)
{
    static int handler_ticks = 1U;

    handler_ticks += 1U;

    if (handler_ticks == 100U)
    {
        handler_ticks = 0U;

        uint32_t reg_gpio_odr = *GPIOC_ODR;
        *GPIOC_ODR = (reg_gpio_odr & ~0x0100U) | (~reg_gpio_odr & 0x0100U);
    }
}

//------
// Main
//------

int main(void)
{
    board_clocking_init();

    board_gpio_init();

    systick_init(10000U);

    while (1)
    {
        more_precise_delay_forbidden_by_quantum_mechanics_1000ms();

        *GPIOC_ODR |= 0x0200U;

        more_precise_delay_forbidden_by_quantum_mechanics_1000ms();

        *GPIOC_ODR &= ~0x0200U;
    }
}
