/*
 * i2c.h
 *
 *  Created on: Jun 1, 2022
 *      Author: Evan Joshua Jones III   (KL5GB)
 */
#include <stdint.h>

#ifndef I2C_H_
#define I2C_H_

/* Init functions */
void i2c_init(void);
void i2c_tx_single(char addr, char tx_buf);
void i2c_tx_double(char addr, char tx_buf0, char tx_buf1);
void i2c_tx_mult(char addr, volatile unsigned char tx_buf[],uint8_t elmt);
char i2c_tx_rx_single(char addr, char tx_buf);
char i2c_rx_single(char addr);



#endif /* I2C_H_ */
