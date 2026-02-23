/*
 *          Radio Driver: A simple program that takes four button inputs
 *                        and changes the receiver frequency accordingly
 *
 *          Version: 0.1
 *          Created by: Evan Joshua Jones III (KL5GB)
 *          Initial Draft: 12/18/2022
 *          Last Updated: 12/18/2022
 *
 */

#include <msp430g2553.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <i2c.h>
#include <lcd.h>
#include <si5351.h>

/* Declare Constants */
#define XTAL_FREQ   (7998300)                           // Crystal ladder filter center frequency

/* Declare Variables */
unsigned char button;                                   // Interrupt flag information
unsigned char cursor;                                   // Cursor location
uint32_t frequency;                                     // Transceiver frequency
unsigned char freq[8];                                  // frequency character array

/* Declare Functions */
uint32_t int_pow(uint32_t x, uint8_t y);              // integer power function

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	                        // stop watchdog timer

    /* Set Up System */
	DCOCTL = 0;                                         // calibrate and set DCO to 1 MHz
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	i2c_init();                                         // setup i2c

	pll_a_init();                                       // setup both Si5351 PLLs
	pll_b_init();

	lcd_init(TWO_LINE);                                 // setup LCD screen in two line mode

	/* Setup Button Inputs */
    P2IN |= BIT0 + BIT3 + BIT4 + BIT5;                  // Declare inputs Up = P2.0, Down = P2.5, Left = P2.4, Right = P2.3
    P2REN |= BIT0 + BIT3 + BIT4 + BIT5;                 // Enable Pull Up/Down Resistors
    P2OUT |= BIT0 + BIT3 + BIT4 + BIT5;                 // Configure Resistors as pull up
    //P2IES |= BIT0 + BIT3 + BIT4 + BIT5;                 // Read falling edge
    P2IE |= BIT0 + BIT3 + BIT4 + BIT5;                  // Allows interrupts when input triggered
    __enable_interrupt();                               // Enable interrupts

    /* Setup LCD Screen */
    lcd_write('K',0x40);
    lcd_write('L',0x41);
    lcd_write('5',0x42);
    lcd_write('G',0x43);
    lcd_write('B',0x44);

    lcd_write(0x32,0x00);
    lcd_write(0x38,0x01);
    lcd_write(0x36,0x02);
    lcd_write(0x30,0x03);
    lcd_write(0x30,0x04);
    lcd_write(0x30,0x05);
    lcd_write(0x30,0x06);
    lcd_write(0x30,0x07);

    lcd_write('H',0x09);
    lcd_write('z',0x0A);

    cursor = 0x00;
    lcd_c(cursor);

    /* Set frequency, and frequency array */
    frequency = 28600000;

    si_set_freq(XTAL_FREQ, SYNTH_MS_0);
    si_set_freq((frequency - XTAL_FREQ), SYNTH_MS_1);

    freq[0] = 0x02;
    freq[1] = 0x08;
    freq[2] = 0x06;

    char i;
    for(i = 3; i < 8; i++){
        freq[i] = 0x00;
    }

	/* Main Loop */
    while(1){
        LPM0;
        switch(button){
            case BIT4:  // Move cursor left
                if(cursor == 0x00){
                    cursor = 0x07;
                    lcd_c(cursor);
                }
                else{
                    cursor--;
                    lcd_c(cursor);
                }
            break;

            case BIT3:  // Move cursor right
                if(cursor == 0x07){
                    cursor = 0x00;
                    lcd_c(cursor);
                }
                else{
                    cursor++;
                    lcd_c(cursor);
                }
            break;

            case BIT0:  // Increment value
                if(freq[cursor] == 0x09){
                    freq[cursor] = 0x00;
                    lcd_write((freq[cursor]|0x30),cursor);
                    lcd_c(cursor);
                    frequency = frequency - 9*int_pow(10,(0x07-cursor));
                    si_set_freq((frequency - XTAL_FREQ), SYNTH_MS_1);

                }
                else{
                    freq[cursor] = freq[cursor] + 0x01;
                    lcd_write((freq[cursor]|0x30),cursor);
                    lcd_c(cursor);
                    frequency = frequency + int_pow(10,(0x07-cursor));
                    si_set_freq((frequency - XTAL_FREQ), SYNTH_MS_1);
                }
            break;

            case BIT5:  // Decrement value
                if(freq[cursor] == 0x00){
                    freq[cursor] = 0x09;
                    lcd_write((freq[cursor]|0x30),cursor);
                    lcd_c(cursor);
                    frequency = frequency + 9*int_pow(10,(0x07-cursor));
                    si_set_freq((frequency - XTAL_FREQ), SYNTH_MS_1);
                }
                else{
                    freq[cursor] = freq[cursor] - 0x01;
                    lcd_write((freq[cursor]|0x30),cursor);
                    lcd_c(cursor);
                    frequency = frequency - int_pow(10,(0x07-cursor));
                    si_set_freq((frequency - XTAL_FREQ), SYNTH_MS_1);
                }
            break;
        }

    }
	
	//return 0;
}

#pragma vector = PORT2_VECTOR                           // Port 2 push button interrupts
__interrupt void PORT2_ISR (void){
    LPM0_EXIT;
    if(P2IFG == BIT4){
        button = BIT4;
    }

    if(P2IFG == BIT3){
        button = BIT3;
    }

    if(P2IFG == BIT0){
        button = BIT0;
    }

    if(P2IFG == BIT5){
        button = BIT5;
    }
    P2IFG &= ~P2IFG;
}

uint32_t int_pow(uint32_t x, uint8_t y){
    uint8_t j;
    uint32_t result;
    result = x;
    for(j = 1;j < y; j++){
        result = result * x;
    }
    return result;
}

