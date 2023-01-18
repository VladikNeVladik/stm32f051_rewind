#include "rcc.h"
#include "uart.h"
#include "gpio.h"

//-------------------
// RCC configuration
//-------------------

#define CPU_FREQENCY 48000000U // CPU frequency: 48 MHz
#define ONE_MILLISECOND CPU_FREQENCY/1000U

void board_clocking_init()
{
    // (1) Clock HSE and wait for oscillations to setup.
    // Also disable all clocks but HSE (8 MHz) and HSI (8 MHz).
    *REG_RCC_CR = RCC_CR_HSEON;
    while ((*REG_RCC_CR & RCC_CR_HSERDY) != RCC_CR_HSERDY);

    // (2) Configure PLL:
    // PREDIV output: HSI/2 = 4 MHz
    RCC_CFGR2_PREDIV_Set(*REG_RCC_CFGR2, 1U);

    // (3) Select PREDIV output as PLL input (4 MHz):
    *REG_RCC_CFGR |= RCC_CFGR_PLLSRC_HSE;

    // (4) SYSCLK frequency = 48 MHz
    RCC_CFGR_PLLMUL_Set(*REG_RCC_CFGR, 12U-1U);

    // (5) Enable PLL:
    *REG_RCC_CR |= RCC_CR_PLLON;
    while ((*REG_RCC_CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);

    // (6) Configure AHB frequency to 48 MHz:
    RCC_CFGR_SW_HPRE_Set(*REG_RCC_CFGR, 0b000U);

    // (7) Select PLL as SYSCLK source:
    *REG_RCC_CFGR |= RCC_CFGR_SW_PLL;
    while ((*REG_RCC_CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

    // (8) Set APB frequency to 48 MHz:
    RCC_CFGR_SW_PPRE_Set(*REG_RCC_CFGR, 0b000U);
}

void delay_1ms()
{
    __asm__ volatile("ldr r0, =__num_cycles");  // r0 = &num_cycles;
    __asm__ volatile("ldr r0, [r0]");           // r0 = *r0;

    __asm__ volatile("__cycle_start:");    // < while (r0 != 0) r0--;
    __asm__ volatile("sub r0, r0, #1");    // <
    __asm__ volatile("cmp r0, #0");        // <
    __asm__ volatile("bne __cycle_start"); // <

    __asm__ volatile("b __cycle_end");

    __asm__ volatile("__num_cycles:");   // num_cycles =  1 * ONE_MILLISECOND/5
    __asm__ volatile(".word 9600");      //

    __asm__ volatile("__cycle_end:");
}

//--------------------
// GPIO configuration
//--------------------

void board_gpio_init()
{
    // (1) Configure PC8 as LED:
    *REG_RCC_AHBENR |= RCC_AHBENR_IOPCEN;

    // Configure PC8 mode:
    uint32_t reg_gpioc_moder = 0U;

    GPIO_MODER_MODE_Set(reg_gpioc_moder, 8U, GPIO_MODER_MODE_InOut);

    *GPIOC_MODER = reg_gpioc_moder;

    // Configure PC8 type:
    uint32_t reg_gpioc_otyper = 0U;

    GPIO_OTYPER_Set(reg_gpioc_otyper, 8U, GPIO_OTYPER_PushPull);

    *GPIOC_TYPER = reg_gpioc_otyper;

    // (2) Configure pins PA9 and PA10 for UART
    *REG_RCC_AHBENR |= RCC_AHBENR_IOPAEN;

    // Set alternate functions:
    uint32_t reg_gpioa_afrh = 0U;

    GPIO_AFRH_Set(reg_gpioa_afrh,  9U, 1U);
    GPIO_AFRH_Set(reg_gpioa_afrh, 10U, 1U);

    *GPIOA_AFRH = reg_gpioa_afrh;

    // Configure pin operating speed:
    uint32_t reg_gpioa_ospeedr = 0U;

    GPIO_OSPEEDR_Set(reg_gpioa_ospeedr,  9U, GPIO_OSPEEDR_High);
    GPIO_OSPEEDR_Set(reg_gpioa_ospeedr, 10U, GPIO_OSPEEDR_High);

    *GPIOA_OSPEEDR = reg_gpioa_ospeedr;

    // Configure mode register:
    uint32_t reg_gpioa_moder = 0U;

    GPIO_MODER_MODE_Set(reg_gpioa_moder,  9U, GPIO_MODER_MODE_Alternate);
    GPIO_MODER_MODE_Set(reg_gpioa_moder, 10U, GPIO_MODER_MODE_Alternate);

    *GPIOA_MODER |= reg_gpioa_moder;
}

//------
// Main
//------

int main()
{
    board_clocking_init();

    board_gpio_init();

    UART_init(9600U, CPU_FREQENCY);

    print_string("Hello, world!\n\r");

    while (1);
}

// What is expected:
// Text    = Hello, world!\n\r
// ASCII   = 72 101 108 108 111 44 32 119 111 114 108 100 33
// What is printed:
// Text    = Hello,��Kױ��
// ASCII   = 72 101 108 108 111 44 239 191 189 239 191 189 75 215 177 239 191 189 239 191 189 10
// 239 191 = 1110 1111 1011 1111
// 239 191 189 = 1110 1111 1011 1111 1011 1101

//
// With parity check:
// 95 = 0101 1111 0 _
// 47 = 0010 1111 1 /
// Without parity check:
// 95 = 0101 1111 _
//      0010 1111 1
