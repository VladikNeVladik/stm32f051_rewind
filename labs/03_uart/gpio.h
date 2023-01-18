#ifndef GPIO_H
#define GPIO_H

//----------------
// GPIO Registers
//----------------

#define GPIOA_MODER   (volatile uint32_t*)(uintptr_t)0x48000000U // GPIO port mode register
#define GPIOA_OSPEEDR (volatile uint32_t*)(uintptr_t)0x48000008U // GPIO port output speed register
#define GPIOA_AFRH    (volatile uint32_t*)(uintptr_t)0x48000024U // GPIO alternate function high register


#define GPIOC_MODER   (volatile uint32_t*)(uintptr_t)0x48000800U // GPIO port mode register
#define GPIOC_TYPER   (volatile uint32_t*)(uintptr_t)0x48000804U // GPIO port output type register
#define GPIOC_OSPEEDR (volatile uint32_t*)(uintptr_t)0x48000808U // GPIO port output speed register
#define GPIOC_PUPDR   (volatile uint32_t*)(uintptr_t)0x4800080CU // GPIO port pull-up/pull-down register
#define GPIOC_IDR     (volatile uint32_t*)(uintptr_t)0x48000810U // GPIO port input  data register
#define GPIOC_ODR     (volatile uint32_t*)(uintptr_t)0x48000814U // GPIO port output data register
#define GPIOC_BSRR    (volatile uint32_t*)(uintptr_t)0x48000818U // GPIO port bit set/reset register
#define GPIOC_LCKR    (volatile uint32_t*)(uintptr_t)0x4800081CU // GPIO port configuration lock register
#define GPIOC_AFRL    (volatile uint32_t*)(uintptr_t)0x48000820U // GPIO alternate function low register
#define GPIOC_AFRH    (volatile uint32_t*)(uintptr_t)0x48000824U // GPIO alternate function high register
#define GPIOC_BRR     (volatile uint32_t*)(uintptr_t)0x48000828U // GPIO port bit reset register

// GPIO_MODER register bits:
#define GPIO_MODER_MODE_Set(reg, bit, mode) (reg) |= ((mode) << (2U*(bit)))
#define GPIO_MODER_MODE_Input     0b00U // Input mode (reset state)
#define GPIO_MODER_MODE_InOut     0b01U // General purpose output mode
#define GPIO_MODER_MODE_Alternate 0b10U // Alternate function mode
#define GPIO_MODER_MODE_Analog    0b11U // Analog mode

// GPIO_TYPER register bits:
#define GPIO_OTYPER_Set(reg, bit, val) (reg) |= ((val) << (bit))
#define GPIO_OTYPER_PushPull  0U
#define GPIO_OTYPER_OpenDrain 1U

// GPIO_OSPEEDR register bits:
#define GPIO_OSPEEDR_Set(reg, bit, speed) (reg) |= ((speed) << (2U*(bit)))
#define GPIO_OSPEEDR_Low    0b00U
#define GPIO_OSPEEDR_Medium 0b01U
#define GPIO_OSPEEDR_High   0b11U

// GPIO_PUPDR register bits:
#define GPIO_PUPDR_Set(reg, bit, val) (reg) |= ((val) << (2U*(bit)))
#define GPIO_PUPDR_NoPull     0b00U // No pull-up, pull-down
#define GPIO_PUPDR_PullUp     0b01U // Pull-up
#define GPIO_PUPDR_PullDown   0b10U // Pull-down

// GPIO_ODR bit:
#define GPIO_ODR_BitSet(reg, bit)   (reg) |=  (1U << (bit))
#define GPIO_ODR_BitReset(reg, bit) (reg) &= ~(1U << (bit))

// GPIO_IDR bit:
#define GPIO_IDR_BitIsSet(reg, bit) !!((reg) & (1U << (bit)))

// GPIO_AFRL register bits:
#define GPIO_AFRL_Set(reg, bit, val) ((reg) |= ((val) & 0x7U) << (4U*((bit) - 0U)))

// GPIO_AFRH register bits:
#define GPIO_AFRH_Set(reg, bit, val) ((reg) |= ((val) & 0x7U) << (4U*((bit) - 8U)))

// GPIO pin masks:
#define GPIO_PIN_0  ((uint16_t)0x0001U)
#define GPIO_PIN_1  ((uint16_t)0x0002U)
#define GPIO_PIN_2  ((uint16_t)0x0004U)
#define GPIO_PIN_3  ((uint16_t)0x0008U)
#define GPIO_PIN_4  ((uint16_t)0x0010U)
#define GPIO_PIN_5  ((uint16_t)0x0020U)
#define GPIO_PIN_6  ((uint16_t)0x0040U)
#define GPIO_PIN_7  ((uint16_t)0x0080U)
#define GPIO_PIN_8  ((uint16_t)0x0100U)
#define GPIO_PIN_9  ((uint16_t)0x0200U)
#define GPIO_PIN_10 ((uint16_t)0x0400U)
#define GPIO_PIN_11 ((uint16_t)0x0800U)
#define GPIO_PIN_12 ((uint16_t)0x1000U)
#define GPIO_PIN_13 ((uint16_t)0x2000U)
#define GPIO_PIN_14 ((uint16_t)0x4000U)
#define GPIO_PIN_15 ((uint16_t)0x8000U)

#endif // GPIO_H
