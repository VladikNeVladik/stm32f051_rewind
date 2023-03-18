#include <stdint.h>
#include <stddef.h>

//---------------
// RCC Registers
//---------------

#define REG_RCC_CR      (volatile uint32_t*)(uintptr_t)0x40021000U // Clock Control Register
#define REG_RCC_CFGR    (volatile uint32_t*)(uintptr_t)0x40021004U // PLL Configuration Register
#define REG_RCC_AHBENR  (volatile uint32_t*)(uintptr_t)0x40021014U // AHB1 Peripheral Clock Enable Register
#define REG_RCC_APB2ENR (volatile uint32_t*)(uintptr_t)0x40021018U // APB1 Peripheral Clock Enable Register
#define REG_RCC_CFGR2   (volatile uint32_t*)(uintptr_t)0x4002102CU // Clock configuration register 2
#define REG_RCC_CFGR3   (volatile uint32_t*)(uintptr_t)0x40021030U // Clock configuration register 2

//----------------
// GPIO Registers
//----------------

#define GPIOA_MODER   (volatile uint32_t*)(uintptr_t)0x48000000U // GPIO port mode register
#define GPIOA_OSPEEDR (volatile uint32_t*)(uintptr_t)0x48000008U // GPIO port output speed register
#define GPIOA_AFRH    (volatile uint32_t*)(uintptr_t)0x48000024U // GPIO alternate function high register

#define GPIOC_MODER   (volatile uint32_t*)(uintptr_t)0x48000800U // GPIO port mode register
#define GPIOC_TYPER   (volatile uint32_t*)(uintptr_t)0x48000804U // GPIO port output type register

//----------------
// UART Registers
//----------------

#define USART1_CR1 (volatile uint32_t*)(uintptr_t)0x40013800U // Control register 1
#define USART1_CR2 (volatile uint32_t*)(uintptr_t)0x40013804U // Control register 2
#define USART1_BRR (volatile uint32_t*)(uintptr_t)0x4001380CU // Baud rate register
#define USART1_ISR (volatile uint32_t*)(uintptr_t)0x4001381CU // Interrupt and status register
#define USART1_TDR (volatile uint32_t*)(uintptr_t)0x40013828U // Transmit data register

//-------------------
// RCC configuration
//-------------------

#define CPU_FREQENCY 48000000U // CPU frequency: 48 MHz
#define ONE_MILLISECOND CPU_FREQENCY/1000U

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

//--------------------
// GPIO configuration
//--------------------

void board_gpio_init()
{
    // (1) Configure PC8 as LED
    *REG_RCC_AHBENR |= (1U << 19U);

    // Configure PC8 mode:
    *GPIOC_MODER |= (0b01U << (2U * 8U));

    // Configure PC8 type:
    *GPIOC_TYPER |= (0U << 8U);

    // (2) Configure pins PA9 and PA10 for UART
    *REG_RCC_AHBENR |= (1U << 17U);

    // Set alternate functions:
    *GPIOA_AFRH |= (1U << 4U * ( 9U - 8U));
    *GPIOA_AFRH |= (1U << 4U * (10U - 8U));

    // Configure pin operating speed:
    *GPIOA_OSPEEDR |= (0b11U << (2U *  9U));
    *GPIOA_OSPEEDR |= (0b11U << (2U * 10U));

    // Configure mode register:
    *GPIOA_MODER |= (0b10U << (2U *  9U));
    *GPIOA_MODER |= (0b10U << (2U * 10U));
}

//--------------------
// GPIO configuration
//--------------------

void uart_init(size_t baudrate, size_t frequency)
{
    // (1) Configure USART1 clocking:
    *REG_RCC_APB2ENR |= (1U << 14U);
    *REG_RCC_CFGR3   |= 0b00U;

    // (2) Set USART1 parameters:
    uint32_t reg_usart_cr1 = 0U;
    uint32_t reg_usart_cr2 = 0U;

    reg_usart_cr1 |= 0x00000000U;  // Data length: 8 bits
    reg_usart_cr1 |=  (0U << 15U); // Use oversampling by 16
    reg_usart_cr1 &= ~(1U << 10U); // Parity control: disabled
    reg_usart_cr1 |=  (1U <<  3U); // Transmitter: enabled

    reg_usart_cr2 |= (0U << 19U);    // Endianness: LSB first
    reg_usart_cr2 |= (0b10U << 12U); // Number of stop bits: 2 stop bit

    *USART1_CR1 = reg_usart_cr1;
    *USART1_CR2 = reg_usart_cr2;

    // (3) Configure USART baud rate:
    uint32_t usartdiv = (frequency + baudrate/2)/baudrate;

    *USART1_BRR = usartdiv;

    // (4) Enable UART:
    *USART1_CR1 |= (1U << 0U);

    // (5) Wait for TX to enable:
    while ((*USART1_ISR & (1U << 21U)) == 0U);
}

void uart_send_byte(char sym)
{
    // Wait for TXE:
    while ((*USART1_ISR & (1U <<  7U)) == 0U);

    // Put data into DR:
    *USART1_TDR = sym;
}

void print_string(const char* buf)
{
    for (size_t i = 0; buf[i] != '\0'; i++)
    {
        uart_send_byte(buf[i]);
    }
}

//------
// Main
//------

#define UART_BAUDRATE 9600
#define UART_BAUDRATE_FIX -300

int main()
{
    board_clocking_init();

    board_gpio_init();

    uart_init(UART_BAUDRATE + UART_BAUDRATE_FIX, CPU_FREQENCY);

    print_string("Hello, world!\r");

    while (1);
}
