/*
 * lcd.h
 *
 *  Created on: Jun 6, 2022
 *      Author: Evan Joshua Jones III   (KL5GB)
 */

#ifndef LCD_H_
#define LCD_H_

/* Hardware Definitions */
#define SCREEN_ADD  (0x27)

/* Function Declarations and Relevant Configuration Definitions */
// Turn single byte into two nibbles, declared for development and debugging reasons
unsigned char * nibbler(unsigned char command, unsigned char RS);
#define RS_ON       (0x01)
#define RS_OFF      (0x00)


// Transmit byte to the screen, declared for development and debugging reasons
void lcd_tx(unsigned char tx_byte, unsigned char RS);
#define CLR_SRN     (0x01)
#define HOME        (0x02)
#define SRN_R       (0x1C)
#define SRN_L       (0x18)
#define CSR_R       (0x14)
#define CSR_L       (0x10)


// Initializes LCD screen. WARNING: ONLY DO ONCE PER POWER CYCLE
void lcd_init(unsigned char screen);
#define ONE_LINE_8  (0x20)          // Sets screen to 5x8 matrix, top line only
#define ONE_LINE_10 (0x24)          // Sets screen to 5x10 matrix, uses some of both lines
#define TWO_LINE    (0x28)          // Sets screen to 5x8 matrix, fully uses both lines
//#define CURSOR_ON ()              // TODO: choose whether to include a cursor, and whether to make it blink

// Moves cursor to position at 'location' variable
void lcd_c(unsigned char location);


void lcd_write(unsigned char letter, unsigned char addr);
void lcd_c_left(void);
void lcd_c_right(void);


#endif /* LCD_H_ */

