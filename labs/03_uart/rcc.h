#ifndef LOADER_REGS_H
#define LOADER_REGS_H

#include <stdint.h>

//---------------
// RCC Registers
//---------------

// Register addresses:
#define REG_RCC_CR       (volatile uint32_t*)(uintptr_t)0x40021000U // Clock Control Register
#define REG_RCC_CFGR     (volatile uint32_t*)(uintptr_t)0x40021004U // PLL Configuration Register
#define REG_RCC_CIR      (volatile uint32_t*)(uintptr_t)0x40021008U // Clock Configuration Register
#define REG_RCC_APB2RSTR (volatile uint32_t*)(uintptr_t)0x4002100CU // APB2 Peripheral Reset Register
#define REG_RCC_APB1RSTR (volatile uint32_t*)(uintptr_t)0x40021010U // APB1 Peripheral Reset Register
#define REG_RCC_AHBENR   (volatile uint32_t*)(uintptr_t)0x40021014U // AHB1 Peripheral Clock Enable Register
#define REG_RCC_APB2ENR  (volatile uint32_t*)(uintptr_t)0x40021018U // APB1 Peripheral Clock Enable Register
#define REG_RCC_APB1ENR  (volatile uint32_t*)(uintptr_t)0x4002101CU // APB1 Peripheral Clock Enable Register
#define REG_RCC_BDCR     (volatile uint32_t*)(uintptr_t)0x40021020U // Backup domain control register
#define REG_RCC_CSR      (volatile uint32_t*)(uintptr_t)0x40021024U // Control/status register
#define REG_RCC_CFGR2    (volatile uint32_t*)(uintptr_t)0x4002102CU // Clock configuration register 2
#define REG_RCC_CFGR3    (volatile uint32_t*)(uintptr_t)0x40021030U // Clock configuration register 2

// RCC_CR register bits:
#define RCC_CR_HSION  (1U <<  0U) // HSI Oscillator Enable
#define RCC_CR_HSIRDY (1U <<  1U) // HSI Oscillator Ready Flag
#define RCC_CR_HSEON  (1U << 16U) // HSE Oscillator Enable
#define RCC_CR_HSERDY (1U << 17U) // HSE Oscillator Ready Flag
#define RCC_CR_HSEBP  (1U << 18U) // HSE Oscillator Bypass
#define RCC_CR_PLLON  (1U << 24U) // PLL Enable Flag
#define RCC_CR_PLLRDY (1U << 25U) // PLL Locked Flag

// RCC_CFGR register bits:
#define RCC_CFGR_SW     (0b11U << 0U) // System clock switch:
#define RCC_CFGR_SW_HSI (0b00U << 0U) // - HSI
#define RCC_CFGR_SW_HSE (0b01U << 0U) // - HSE
#define RCC_CFGR_SW_PLL (0b10U << 0U) // - PLL

#define RCC_CFGR_SWS     (0b11U << 2U) // System clock switch status:
#define RCC_CFGR_SWS_HSI (0b00U << 2U) // - HSI
#define RCC_CFGR_SWS_HSE (0b01U << 2U) // - HSE
#define RCC_CFGR_SWS_PLL (0b10U << 2U) // - PLL

#define RCC_CFGR_SW_HPRE_Set(reg, val) (reg) = ((reg) & ~(0xFU <<  4U)) | (((val) & 0xFU) <<  4U)
#define RCC_CFGR_SW_PPRE_Set(reg, val) (reg) = ((reg) & ~(0x7U <<  8U)) | (((val) & 0x7U) <<  8U)

#define RCC_CFGR_PLLSRC_HSI (0U << 16U) // PLL entry clock source: HSI/2
#define RCC_CFGR_PLLSRC_HSE (1U << 16U) // PLL entry clock source: HSE

#define RCC_CFGR_PLLMUL_Set(reg, val) (reg) |= (((val) & 0xFU) << 18U)

// RCC_AHBENR register bits:
#define RCC_AHBENR_IOPAEN (1U << 17U) // I/O port A clock enable
#define RCC_AHBENR_IOPCEN (1U << 19U) // I/O port C clock enable

// REG_RCC_APB2ENR register bits:
#define RCC_APB2ENR_USART1EN (1U << 14U) // USART1 clock enable

// RCC_CFGR2 register bits:
#define RCC_CFGR2_PREDIV_Set(reg, val) (reg) |= ((val) & 0xFU) // PREDIV division factor

// RCC_CFGR3 register bits:
#define RCC_CFGR3_USART1SW        0b11U
#define RCC_CFGR3_USART1SW_PCLK   0b00U // PCLK selected as USART1 clock source
#define RCC_CFGR3_USART1SW_SYSCLK 0b01U // System clock (SYSCLK) selected as USART1 clock
#define RCC_CFGR3_USART1SW_LSE    0b10U // LSE clock selected as USART1 clock
#define RCC_CFGR3_USART1SW_HSI    0b11U // HSI clock selected as USART1 clock

#endif // LOADER_REGS_H
