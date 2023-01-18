#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stddef.h>

#include "rcc.h"
#include "uart.h"

//----------------
// UART registers
//----------------

#define USART1_CR1 (volatile uint32_t*)(uintptr_t)0x40013800U // Control register 1
#define USART1_CR2 (volatile uint32_t*)(uintptr_t)0x40013804U // Control register 2
#define USART1_BRR (volatile uint32_t*)(uintptr_t)0x4001380CU // Baud rate register
#define USART1_ISR (volatile uint32_t*)(uintptr_t)0x4001381CU // Interrupt and status register (USART_ISR)
#define USART1_RDR (volatile uint32_t*)(uintptr_t)0x40013824U // Receive data register
#define USART1_TDR (volatile uint32_t*)(uintptr_t)0x40013828U // Transmit data register

// USART_CR1 register bits:
#define USART_CR1_M       0x10001000U // Number of data bits:
#define USART_CR1_M_8bits 0x00000000U // 8 data bits
#define USART_CR1_M_9bits 0x00001000U // 9 data bits
#define USART_CR1_M_7bits 0x10000000U // 7 data bits

#define USART_CR1_OVER16 (0U << 15U) // Oversampling by 16
#define USART_CR1_OVER8  (1U << 15U) // Oversampling by 8

#define USART_CR1_PCE     (1U << 10U) // Parity control enable

#define USART_CR1_PS_EVEN (0U <<  9U) // Parity: even
#define USART_CR1_PS_ODD  (1U <<  9U) // Parity: odd

#define USART_CR1_TE (1U << 3U) // Transmitter enable
#define USART_CR1_RE (1U << 2U) // Receiver enable
#define USART_CR1_UE (1U << 0U) // UART enable

// USART_CR2 register bits:
#define USART_CR2_LSBFIRST (0U << 19U) // Least significant bit first
#define USART_CR2_MSBFIRST (1U << 19U) // Most  significant bit first

#define USART_CR2_STOP_BITS         (0b11U << 12U)
#define USART_CR2_STOP_BITS_1bit    (0b00U << 12U)
#define USART_CR2_STOP_BITS_0_5bits (0b01U << 12U)
#define USART_CR2_STOP_BITS_2bits   (0b10U << 12U)
#define USART_CR2_STOP_BITS_1_5bits (0b11U << 12U)

// USART_BRR register bits:
#define USART_BRR_BRR16_Set(reg, usartdiv16) ((reg) = (usartdiv16) & 0xFFFFU)
// Warning: this (^^^) is only suitable for oversampling by 16

// USART_ISR register bits:
#define USART_ISR_REACK (1U << 22U) // Receive enable acknowledge flag
#define USART_ISR_TEACK (1U << 21U) // Transmit enable acknowledge flag
#define USART_ISR_TXE   (1U <<  7U) // Transmit data register empty
#define USART_ISR_RXE   (1U <<  5U) // Read data register not empty

//----------------
// UART interface
//----------------

void UART_init(size_t baudrate, size_t frequency)
{
    // (1) Configure USART1 clocking:
    *REG_RCC_APB2ENR |= RCC_APB2ENR_USART1EN;
    *REG_RCC_CFGR3   |= RCC_CFGR3_USART1SW_PCLK;

    // (2) Set USART1 parameters:
    uint32_t reg_usart_cr1 = 0U;
    uint32_t reg_usart_cr2 = 0U;

    reg_usart_cr1 |= USART_CR1_M_8bits; // Data length: 8 bits
    reg_usart_cr1 |= USART_CR1_OVER16;  // Use oversampling by 16
    reg_usart_cr1 &= ~USART_CR1_PCE;    // Parity control: disabled
    reg_usart_cr1 |= USART_CR1_TE;      // Transmitter: enabled

    reg_usart_cr2 |= USART_CR2_LSBFIRST;        // Endianness: LSB first
    reg_usart_cr2 |= USART_CR2_STOP_BITS_2bits; // Number of stop bits: 2 stop bit

    *USART1_CR1 = reg_usart_cr1;
    *USART1_CR2 = reg_usart_cr2;

    // (3) Configure USART baud rate:
    uint32_t usartdiv = (frequency + baudrate/2)/baudrate;

    USART_BRR_BRR16_Set(*USART1_BRR, usartdiv);

    // (4) Enable UART:
    *USART1_CR1 |= USART_CR1_UE;

    // (5) Wait for TX to enable:
    while ((*USART1_ISR & USART_ISR_TEACK) == 0U);
}

void UART_send_byte(char sym)
{
    // Wait for TXE:
    while ((*USART1_ISR & USART_ISR_TXE) == 0U);

    // Put data into DR:
    *USART1_TDR = sym;
}

void print_string(const char* buf)
{
    for (size_t i = 0; buf[i] != '\0'; i++)
    {
        UART_send_byte(buf[i]);
    }
}


#endif // UART_H
