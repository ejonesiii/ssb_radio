/*
 * si5351.h
 *
 *  Created on: Jun 7, 2022
 *      Author: Evan Joshua Jones III   (KL5GB)
 */
#include <stdint.h>


#ifndef SI5351_H_
#define SI5351_H_

/* Hardware Register Definitions */
#define SYNTH_ADD   (0x60)

#define CLK_ENABLE_CONTROL  (3)
#define PLLX_SRC            (15)
#define CLK0_CONTROL        (16)
#define CLK1_CONTROL        (17)
#define CLK2_CONTROL        (18)
#define SYNTH_PLL_A         (26)
#define SYNTH_PLL_B         (34)
#define SYNTH_MS_0          (42)
#define SYNTH_MS_1          (50)
#define SYNTH_MS_2          (58)
#define PLL_RESET           (177)

#define XTAL_LOAD_CAP       (183)
#define XTAL_6_PF           (0b01010010)
#define XTAL_8_PF           (0b10010010)
#define XTAL_10_PF          (0b11010010)        // Default setting for the SI5351 (ONLY USE THIS, UNLESS YOU KNOW WHAT YOU'RE DOING)

/* Multisynth Divisors */
#define DIV_1               (0x00)
#define DIV_2               (0x10)
#define DIV_4               (0x20)
#define DIV_8               (0x30)
#define DIV_16              (0x40)
#define DIV_32              (0x50)
#define DIV_64              (0x60)
#define DIV_128             (0x70)

/* Function Declarations and Relevant Configuration Definitions */
void pll_a_init(void);
void pll_b_init(void);
void si_set_freq(uint32_t freq, unsigned int addr);




#endif /* SI5351_H_ */
