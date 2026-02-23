/*
 * lcd.c
 *
 *  Created on: Jun 6, 2022
 *      Author: Evan Joshua Jones III   (KL5GB)
 *
 *      Functions to handle the startup and management of the 16x2 LCD screen
 */

#include <lcd.h>
#include <stdint.h>
#include <i2c.h>


unsigned char * nibbler(unsigned char command, unsigned char RS){
    static unsigned char tx[4];
    tx[0] = (((command >> 0x04) & 0x0F) << 0x04) | RS | 0x0C;   // First shift MSB down 4, truncate LSB, shift up 4, 'OR' with RS, E, & LED
    tx[1] = 0x08 | RS;                                      // Disable E, 'OR' with RS
    tx[2] = ((command & 0x0F) << 0x04) | RS | 0x0C;         // Truncate MSB, shift up 4, 'OR' with RS, E, & LED
    tx[3] = 0x08 | RS;                                      // Disable E, 'OR' with RS

    return tx;
}

void lcd_tx(unsigned char tx_byte, unsigned char RS){
    unsigned char *tx_buf;                                  // Declare the pointer variable

    tx_buf = nibbler(tx_byte, RS);                          // Set Tx buffer to the pointer address
    i2c_tx_single(SCREEN_ADD,*tx_buf);                      // Transmit byte. Pointer must be in parenthesis when adding 'i' *(tx_buf+1)
    i2c_tx_single(SCREEN_ADD,*(tx_buf+1));
    i2c_tx_single(SCREEN_ADD,*(tx_buf+2));
    i2c_tx_single(SCREEN_ADD,*(tx_buf+3));

}

void lcd_init(unsigned char screen){

    i2c_tx_single(SCREEN_ADD, 0b00101100);                  // Sets to 4 line input (MUST BE DONE FIRST AND SEPARATELY FROM THE LCD_TX FUNCTION)
    i2c_tx_single(SCREEN_ADD, 0b00001000);

    lcd_tx(screen, RS_OFF);                                 // Sets screen to either 1 line or 2 line
    lcd_tx(0x0F, RS_OFF);                                   // Turn on screen and cursor
    lcd_tx(CLR_SRN, RS_OFF);                                // Clear screen
    lcd_tx(HOME, RS_OFF);                                   // Return to home position

}

void lcd_write(unsigned char letter, unsigned char addr){
    unsigned char *tx_buf;

    tx_buf = nibbler((addr | 0x80), RS_OFF);
    i2c_tx_single(SCREEN_ADD,*tx_buf);                      // Transmit byte. Pointer must be in parenthesis when adding 'i' *(tx_buf+1)
    i2c_tx_single(SCREEN_ADD,*(tx_buf+1));
    i2c_tx_single(SCREEN_ADD,*(tx_buf+2));
    i2c_tx_single(SCREEN_ADD,*(tx_buf+3));

    tx_buf = nibbler(letter, RS_ON);
    i2c_tx_single(SCREEN_ADD,*tx_buf);                      // Transmit byte. Pointer must be in parenthesis when adding 'i' *(tx_buf+1)
    i2c_tx_single(SCREEN_ADD,*(tx_buf+1));
    i2c_tx_single(SCREEN_ADD,*(tx_buf+2));
    i2c_tx_single(SCREEN_ADD,*(tx_buf+3));
}

void lcd_c_left(void){                                      // Move cursor Left
    lcd_tx(CSR_L,RS_OFF);
}

void lcd_c_right(void){                                     // Move cursor right
    lcd_tx(CSR_R,RS_OFF);
}

void lcd_c(unsigned char location){                         // Move cursor to arbitrary position
    unsigned char *tx_buf;

    tx_buf = nibbler((location | 0x80), RS_OFF);
    i2c_tx_single(SCREEN_ADD,*tx_buf);                      // Transmit byte. Pointer must be in parenthesis when adding 'i' *(tx_buf+1)
    i2c_tx_single(SCREEN_ADD,*(tx_buf+1));
    i2c_tx_single(SCREEN_ADD,*(tx_buf+2));
    i2c_tx_single(SCREEN_ADD,*(tx_buf+3));
}
