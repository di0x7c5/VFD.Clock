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

#include <avr/io.h>

#include "i2c.h"
#include "vfd_clock.h"

/*
 * TODO!
 */
void i2c_delay(unsigned int p) {
    unsigned int i;
    unsigned char j;

    for(i=0; i<p; i++) {
        for (j=0; j<10; j++) {
            NOP();
        }
    }
}

/*
 * TODO!
 */
void i2c_init(unsigned int wr) {
    // Set SCL pin as output...
    SCL_OUT;
    // ...And Set SDA pin as Output or Input in dependence from WR
    if(wr == WRITE) {SDA_OUT;} else {SDA_IN;}
}

/*
 * TODO!
 */
void i2c_start(void) {
    i2c_init(WRITE);    // Initialization
    SCL_1;              // HIGHT state SCL and...
    SDA_1;              // ...HIGHT state SDA, of course
    i2c_delay(uDLY);    // let's wait a bit...
    SDA_0;              // WoW!!! LOW state SDA (a START Condition)
    i2c_delay(uDLY);    // ... again ...
    SCL_0;              // And lets end the START condition-LOW state SCL
    i2c_delay(uDLY);    // ...and one more
}

/*
 * TODO!
 */
void i2c_stop(void) {
    i2c_init(WRITE);    // Initialization
    SDA_0;              // A LOW state on SDA line
    i2c_delay(uDLY);    // Just We Wait...
    SCL_1;              // Now HIGHT state on SCL line
    i2c_delay(uDLY);    // ...still wait...
    SDA_1;              // And SDA go to HIGHT state
    i2c_delay(uDLY);    // ...And Again
}

/*
 * TODO!
 */
void i2c_send(unsigned char byte) {
    unsigned int i;

    // Initialization SDA & SCL pins
    i2c_init(WRITE);
    // And now we can send data bit-by-bit
    for (i=0; i<8; i++) {
        // We check the state of the oldest bit
        if(byte>127) {      // If byte > 10000000b
            SDA_1;          // Set SDA to HIGH state
        } else {            // If byte is not > 10000000b
            SDA_0;          // Set SDA to LOW state
        }                   //

        // Now We must generate a clock signal
        i2c_delay(uDLY);    // Wait...
        SCL_1;              // SCL to HIGHT state
        i2c_delay(uDLY);    // ...Again wait...
        SCL_0;              // SCL to LOW state
        i2c_delay(uDLY);    // ...And Again

        // In the end, we get next byte's bit
        byte <<= 1;
    }

    // Handling of acknowledgement
    SDA_1;
    i2c_delay(uDLY);
    SCL_1;
    i2c_delay(uDLY);
    SCL_0;
    i2c_delay(uDLY);
}

/*
 * TODO!
 */
unsigned char i2c_take(void) {
    unsigned int i;         // Help to 'for' loop
    unsigned char byte = 0; // Our Taken Back Byte

    i2c_init(WRITE);
    SDA_0;
    i2c_delay(uDLY);

    // If We want read a byte from I2C-bus,
    // We must set a SDA pin as input
    i2c_init(READ);

    // Take byte bit-by-bit
    for (i=0; i<8; i++) {
        // Scroll buffer right
        byte <<= 1;
        // And set least significant bit when required
        if ((I2CPIN & 0x40) == 0x40) {
            byte |= 1;
        }

        // Clock signal
        i2c_delay(uDLY);    // Wait...
        SCL_1;              // SCL to HIGHT state
        i2c_delay(uDLY);    // ...Again wait...
        SCL_0;              // SCL to LOW state
        i2c_delay(uDLY);    // ...And Again
    }

    // Handling of acknowledgement
    i2c_init(WRITE);
    SDA_1;
    i2c_delay(uDLY);
    SCL_1;
    i2c_delay(uDLY);
    SCL_0;
    i2c_delay(uDLY);

    return(byte);
}

/*
 * TODO!
 */
unsigned char RTC_READ(unsigned char WordAdr) {
    unsigned char byte;

    i2c_start();                // START condition
    i2c_send(RTC_ADR_W);        // Send a PCF8583 address to write
    i2c_send(WordAdr);          // Send on bus Word Address
    i2c_start();                // START condition
    i2c_send(RTC_ADR_R);        // Send a PCF8583 address to read
    byte = i2c_take();          // Read the byte from I2C-bus
    i2c_stop();                 // STOP condition

    return(byte);               // Results
}

/*
 * TODO!
 */
void RTC_WRITE(unsigned char WordAdr, unsigned char Data) {
    i2c_start();                // START condition
    i2c_send(RTC_ADR_W);        // Send a PCF8583 address to write
    i2c_send(WordAdr);          // Send on bus Word Address
    i2c_send(Data);             // Send on bus Data
    i2c_stop();                 // STOP condition
}
