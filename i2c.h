/*
 * VFD.Clock - the clock build with Vacuum Fluorescent Display.
 * Copyright (C) 2008 Dariusz Iwanoczko (ivanek_dar@wp.pl)
 * Copyright (C) 2012 Dariusz Iwanoczko (dariusz.iwanoczko@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef I2C_H
#define I2C_H

#define RTC_ADR_R   0xA1    // PCF8583 Address to Read data
#define RTC_ADR_W   0xA0    // PCF8583 Address to Write data
#define NOP()   __asm__ __volatile__ ("nop")    // assemblers NOP instruction

#define SDA_0 I2CPORT = (I2CPORT & 0xBF )               // Set SDA bit to L state
#define SDA_1 I2CPORT = ( ( I2CPORT & 0xBF ) | 0x40 )   // Set SDA bit to H state
#define SCL_0 I2CPORT = ( I2CPORT & 0x7F )              // Set SCL bit to L state
#define SCL_1 I2CPORT = ( ( I2CPORT & 0x7F ) | 0x80 )   // set SCL bit to H state

#define SDA_IN  I2CDDR = ( I2CDDR & 0xBF )              // Set SDA pin as Input
#define SDA_OUT I2CDDR = ( ( I2CDDR & 0xBF ) | 0x40 )   // Set SDA pin as Output
#define SCL_IN  I2CDDR = ( I2CDDR & 0x7F )              // Set SCL pin as Input
#define SCL_OUT I2CDDR = ( ( I2CDDR & 0x7F ) | 0x80 )   // Set SCL pin as Output

/*
 * Delay Definitions.
 */
#define uDLY 1      // Delay for I2C-bus

/*
 * Let's do some simple reduction
 */
#define WRITE   1
#define READ    0

/*
 * Declaration of second-rate Functions
 */
void i2c_start(void);               // START condition
void i2c_stop(void);                // STOP  condition
void i2c_init(unsigned int wr);     // Initialisation
void i2c_send(unsigned char byte);  // Send a value on I2C-bus
unsigned char i2c_take(void);       // Take a value from I2C-bus
void i2c_delay(unsigned int p);     // Delay for I2C-bus

/*
 * Declaration of first-rate Functions for PCF8583
 */
unsigned char RTC_READ(unsigned char WordAdr);
void RTC_WRITE(unsigned char WordAdr, unsigned char Data);

#endif // I2C_H
