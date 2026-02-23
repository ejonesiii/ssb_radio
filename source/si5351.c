/*
 * si5351.c
 *
 *  Created on: Jun 7, 2022
 *      Author: Evan Joshua Jones III   (KL5GB)
 */

#include <si5351.h>
#include <i2c.h>
#include <stdint.h>
#include <math.h>

#define F_VCO   (900)                                               // in MHz, only int values
#define F_XTAL  (25)                                                // in MHz, only int values
#define DIV     (DIV_1)                                             // Setting multisynth divisor, allows lower frequency limits at upper frequency limit expense
#define divisor (1)                                                 // Setting divisor compensation, must be the same as the "DIV" divisor
#define MAX_RES (1048575)                                           // Maximum fractional resolution [1/1048575 or 1/(2^20 - 1)]

void si_tx(uint32_t P1, uint32_t P2, uint32_t P3, uint32_t divs, unsigned int addr){
    unsigned char tx_buf[8];                                        // I2C tx buffer

    tx_buf[0] = (P3 >> 8) & 0xFF;                                   // P3[15:8]
    tx_buf[1] = P3 & 0xFF;                                          // P3[7:0]
    tx_buf[2] = (0b00000011 & (P1 >> 16)) | (divs);                 // P1[17:16]
    tx_buf[3] = (P1 >> 8) & 0xFF;                                   // P1[15:8]
    tx_buf[4] = P1 & 0xFF;                                          // P1[7:0]
    tx_buf[5] = (((P3 >> 16) & 0x0F) << 4) | ((P2 >> 16) & 0x0F);   // P3[19:16] & P2[19:16]
    tx_buf[6] = (P2 >> 8) & 0xFF;                                   // P2[15:8]
    tx_buf[7] = P2 & 0xFF;                                          // P2[7:0]


    i2c_tx_double(SYNTH_ADD, addr, tx_buf[0]);                      // Transmit all the data
    i2c_tx_double(SYNTH_ADD, addr+1, tx_buf[1]);
    i2c_tx_double(SYNTH_ADD, addr+2, tx_buf[2]);
    i2c_tx_double(SYNTH_ADD, addr+3, tx_buf[3]);
    i2c_tx_double(SYNTH_ADD, addr+4, tx_buf[4]);
    i2c_tx_double(SYNTH_ADD, addr+5, tx_buf[5]);
    i2c_tx_double(SYNTH_ADD, addr+6, tx_buf[6]);
    i2c_tx_double(SYNTH_ADD, addr+7, tx_buf[7]);
}

void pll_a_init(void){
    volatile uint32_t a, b, c;                                      // Frequency Constants
    volatile uint32_t P1, P2, P3;                                   // Final calculated values

    i2c_tx_double(SYNTH_ADD, CLK_ENABLE_CONTROL, 0x00);             // Enable all clock outputs
    i2c_tx_double(SYNTH_ADD, CLK0_CONTROL, 0x0F);                   // Map to Multisynth 0 with max drive power
    i2c_tx_double(SYNTH_ADD, PLL_RESET, 0x20);                      // Reset PLL A

    a = F_VCO / F_XTAL;                                             // First constant, VCO feedback frequency divider: 900 MHz / 25 MHz
    b = 0;                                                          // Numerator
    c = MAX_RES;                                                    // Maximum resolution

    P1 = 128*a + (128)*(b/c) - 512;
    P2 = 128*a - c*(128)*(b/c);
    P3 = c;

    si_tx(P1, P2, P3, DIV, SYNTH_PLL_A);

}



//TODO: FINISH MODIFYING TO RUN PLL B
void pll_b_init(void){
    volatile uint32_t a, b, c;                                      // Frequency Constants
    volatile uint32_t P1, P2, P3;                                   // Final calculated values

    i2c_tx_double(SYNTH_ADD, CLK_ENABLE_CONTROL, 0x00);             // Enable all clock outputs
    i2c_tx_double(SYNTH_ADD, CLK1_CONTROL, 0x2F);                   // Map to Multisynth 1 with max drive power
    i2c_tx_double(SYNTH_ADD, CLK2_CONTROL, 0x2F);                   // Map to Multisynth 2 with max drive power
    i2c_tx_double(SYNTH_ADD, PLL_RESET, 0x80);                      // Reset PLL B



    a = F_VCO / F_XTAL;                                             // First constant, VCO feedback frequency divider: 900 MHz / 25 MHz
    b = 0;                                                          // Numerator
    c = MAX_RES;                                                    // Maximum resolution

    P1 = 128*a + (128)*(b/c) - 512;
    P2 = 128*a - c*(128)*(b/c);
    P3 = c;

    si_tx(P1, P2, P3, DIV, SYNTH_PLL_B);

}

void si_set_freq(uint32_t freq, unsigned int addr){
    volatile float a, b, c;                                     // Frequency Constants
    volatile uint32_t P1, P2, P3;                                   // Final calculated values
    //const uint32_t f_xtal = 25000000;

    //volatile float fdiv = (f_xtal * (F_VCO / 25)) / (freq * divisor);     // Calculate the frequency divisor from the PLL
    volatile float fdiv = (F_VCO * 1.0 * MAX_RES)/(((freq*divisor)/1000000.0));     // Calculate the frequency divisor from the PLL


    a = (F_VCO*1000000) / (freq*divisor);                                                   // First constant, VCO feedback frequency divider: 900 MHz / 25 MHz
    c = MAX_RES;                                                    // Third constant, Maximum resolution
    b = (uint32_t)(fdiv - (a*c));                                                   // Second constant, numerator

    P1 = (uint32_t) (128*a + floor((128)*(b/c)) - 512);
    P2 = (uint32_t) (128*a - c*floor((128)*(b/c)));
    P3 = (uint32_t) c;

    si_tx(P1, P2, P3, DIV, addr);

}

