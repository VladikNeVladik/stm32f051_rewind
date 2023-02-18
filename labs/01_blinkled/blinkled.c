#include <stdint.h>

#define PLL_SRC (0b10U)
#define AHB_FREQ_48 0U
#define HSI_SRC (1U << 15)

//------
// Masks
//------
#define REG_RCC_CFGR_PLL_SET_SRC_MASK (0b11U << 15)
#define REG_RCC_CFGR_PLLMUL_MASK (15U << 18)
#define REG_RCC_CFGR_AHB_MASK (0b1111U << 4)
#define REG_RCC_CFGR_SYSCLK_MASK (0b11U)
#define REG_RCC_CFGR_PCLK_PRESCALER_MASK (0b111U << 8)
#define REG_RCC_CFGR2_PLL_MASK (0b11U)
//---------------
// Bit aryphmetic 
//---------------
#define SET_BIT(REG, BIT) (*REG |= 1u << BIT)                   // выставление бита BIT в регистре REG в 1
#define CLEAR_BIT(REG, BIT) (*REG &= (~(1u << BIT)))            // выставление бита BIT в регистре REG в 0
#define READ_BIT(REG, BIT) ((*REG >> BIT) & 1u)                 // возвращает бит BIT регистра REG
#define WRITE_REG(REG, VAL) (*REG = VAL)                        // выставление регистра REG в VAL
#define READ_REG(REG) (*REG)                                    // возвращает регистр REG

#define MODIFY_REG(REG, MODIFYMASK, VALUE) (*REG = ((*REG & !MODIFYMASK) | (VALUE & MODIFYMASK)))


#define REG_RCC_CR_HSEON(REG) ({SET_BIT(REG, 16U); while ((READ_BIT(REG, 17) != 1));})
#define REG_RCC_CFGR2_CONF_PLL(REG, DIV) (MODIFY_REG(REG, REG_RCC_CFGR2_PLL_MASK, (DIV - 1))) //DIV - divider
#define REG_RCC_CFGR_PLL_SET_SRC(REG, SRC) (MODIFY_REG(REG, REG_RCC_CFGR_PLL_SET_SRC_MASK, SRC))
#define REG_RCC_CFGR_PLLMUL_SET_MULT(REG, MULT) (MODIFY_REG(REG, REG_RCC_CFGR_PLLMUL_MASK, (MULT - 2U) << 18))
#define REG_RCC_CR_PLL_ENABLE(REG) ({SET_BIT(REG, 24); while ((READ_BIT(REG, 25)) != 1);})
#define REG_RCC_CFGR_SET_AHB(REG, FREQ) (MODIFY_REG(REG, REG_RCC_CFGR_AHB_MASK, FREQ << 4))
#define REG_RCC_CFGR_SET_SYSCLK_SRC(REG, SRC) ({MODIFY_REG(REG, REG_RCC_CFGR_SYSCLK_MASK, SRC); while (READ_BIT(REG, 3) != 1);})
#define REG_RCC_CFGR_PCLK_PRESCALER_SET_DIV_2(REG) (MODIFY_REG(REG, REG_RCC_CFGR_PCLK_PRESCALER_MASK, 0b100 << 8))
#define REG_RCC_AHBENR_PORT_C_ENABLE(REG) (SET_BIT(REG_RCC_AHBENR, 19))
#define GPIOC_MODER_PORT_SET_MODE_OUTPUT(REG, PORT) (SET_BIT(REG, 2*PORT))
#define GPIOC_TYPER_PORT_SET_PUSH_PULL(REG, PORT) (CLEAR_BIT(REG, PORT))
#define GPIOC_ODR_PORT_ENABLE(REG, PORT) (SET_BIT(REG, PORT))
#define GPIOC_ODR_PORT_DISABLE(REG, PORT) (CLEAR_BIT(REG, PORT))

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

#define GPIOC_MODER (volatile uint32_t*)(uintptr_t)0x48000800U      // GPIO port mode register
#define GPIOC_TYPER (volatile uint32_t*)(uintptr_t)0x48000804U      // GPIO port output type register

#define GPIOC_ODR (volatile uint32_t*)(uintptr_t) 0x48000814U       // GPIO port output register

//------
// Main
//------

#define CPU_FREQENCY 48000000U // CPU frequency: 48 MHz
#define ONE_MILLISECOND (CPU_FREQENCY / 1000U)

void board_clocking_init()
{
    // (1) Clock HSE and wait for oscillations to setup.
    REG_RCC_CR_HSEON(REG_RCC_CR);

    // (2) Configure PLL:
    // PREDIV output: HSE/2 = 4 MHz
    REG_RCC_CFGR2_CONF_PLL(REG_RCC_CFGR2, 2U);

    // (3) Select PREDIV output as PLL input (4 MHz):
    REG_RCC_CFGR_PLL_SET_SRC(REG_RCC_CFGR, HSI_SRC);

    // (4) Set PLLMUL to 12:
    // SYSCLK frequency = 48 MHz
    REG_RCC_CFGR_PLLMUL_SET_MULT(REG_RCC_CFGR, 12);

    // (5) Enable PLL:
    REG_RCC_CR_PLL_ENABLE(REG_RCC_CR);

    // (6) Configure AHB frequency to 48 MHz:
    REG_RCC_CFGR_SET_AHB(REG_RCC_CFGR, AHB_FREQ_48);

    // (7) Select PLL as SYSCLK source:
    REG_RCC_CFGR_SET_SYSCLK_SRC(REG_RCC_CFGR, PLL_SRC);

    // (8) Set APB frequency to 24 MHz
    REG_RCC_CFGR_PCLK_PRESCALER_SET_DIV_2(REG_RCC_CFGR);
}


void board_gpio_init()
{
    // (1) Enable GPIOC clocking:
    REG_RCC_AHBENR_PORT_C_ENABLE(REG_RCC_AHBENR);

    // (2) Configure PC8 and PC9 mode:
    GPIOC_MODER_PORT_SET_MODE_OUTPUT(GPIOC_MODER, 8);
    GPIOC_MODER_PORT_SET_MODE_OUTPUT(GPIOC_MODER, 9);


    // (3) Configure PC8 and PC9 type output push-pull set:
    GPIOC_TYPER_PORT_SET_PUSH_PULL(GPIOC_TYPER, 8);
    GPIOC_TYPER_PORT_SET_PUSH_PULL(GPIOC_TYPER, 9);

}

void totally_accurate_quantum_femtosecond_precise_super_delay_3000_100ms()
{
    for (uint32_t i = 0; i < 100U * ONE_MILLISECOND; ++i)
    {
        // Insert NOP for power consumption:
        __asm__ volatile("nop");
    }
}

int main()
{
#ifndef INSIDE_QEMU
    board_clocking_init();
#endif

    board_gpio_init();

    while (1)
    {
        GPIOC_ODR_PORT_ENABLE(GPIOC_ODR, 8);
        totally_accurate_quantum_femtosecond_precise_super_delay_3000_100ms();
        GPIOC_ODR_PORT_DISABLE(GPIOC_ODR, 8);
        totally_accurate_quantum_femtosecond_precise_super_delay_3000_100ms();
        GPIOC_ODR_PORT_ENABLE(GPIOC_ODR, 9);
        totally_accurate_quantum_femtosecond_precise_super_delay_3000_100ms();
        GPIOC_ODR_PORT_DISABLE(GPIOC_ODR, 9);
        totally_accurate_quantum_femtosecond_precise_super_delay_3000_100ms();
    }
}

