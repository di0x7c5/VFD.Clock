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

#ifndef VFD_CLOCK_H
#define VFD_CLOCK_H

/*
 * Port intended to anode control
 */
#define ANODEPORT   PORTA
#define ANODEDDR    DDRA
#define ANODEPIN    PINA

/*
 * Port intended to net control
 */
#define NETPORT     PORTC
#define NETDDR      DDRC
#define NETPIN      PINC

/*
 * Port intended to keyboard control
 */
#define KEYPORT     PORTB
#define KEYDDR      DDRB
#define KEYPIN      PINB

/*
 * Port intended to I2C-bus control
 */
#define I2CPORT     PORTD
#define I2CDDR      DDRD
#define I2CPIN      PIND

/*
 * Define segments for VFD tube.
 */                         // .gfedcba
#define SEG_A   1<<0        // 00000001b
#define SEG_B   1<<1        // 00000010b
#define SEG_C   1<<2        // 00000100b
#define SEG_D   1<<3        // 00001000b
#define SEG_E   1<<4        // 00010000b
#define SEG_F   1<<5        // 00100000b
#define SEG_G   1<<6        // 01000000b
#define SEG_H   1<<7        // 10000000b

/*
 * Define chars for VFD tube.
 */
#define CHAR_0 ~(SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F)
#define CHAR_1 ~(SEG_B | SEG_C)
#define CHAR_2 ~(SEG_A | SEG_B | SEG_D | SEG_E | SEG_G)
#define CHAR_3 ~(SEG_A | SEG_B | SEG_C | SEG_D | SEG_G)
#define CHAR_4 ~(SEG_B | SEG_C | SEG_F | SEG_G)
#define CHAR_5 ~(SEG_A | SEG_C | SEG_D | SEG_F | SEG_G)
#define CHAR_6 ~(SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G)
#define CHAR_7 ~(SEG_A | SEG_B | SEG_C)
#define CHAR_8 ~(SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G)
#define CHAR_9 ~(SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G)
#define CHAR_A ~(SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G)
#define CHAR_B ~(SEG_C | SEG_D | SEG_E | SEG_F | SEG_G)
#define CHAR_C ~(SEG_A | SEG_D | SEG_E | SEG_F )
#define CHAR_D ~(SEG_B | SEG_C | SEG_D | SEG_E | SEG_G)
#define CHAR_E ~(SEG_A | SEG_D | SEG_E | SEG_F | SEG_G)
#define CHAR_F ~(SEG_A | SEG_E | SEG_F | SEG_G)
#define CHAR_DOT ~(SEG_H)
#define CHAR_MINUS ~(SEG_G)
#define CHAR_ON 0xff
#define CHAR_OFF 0x00

/*
 * Buttons
 */
typedef enum eButton {
    P1 = 0x01,
    P2 = 0x02,
    P3 = 0x04,
    P4 = 0x08
} Button;

/*
 * Display format
 */
typedef enum eDisplayFormat {
    DF_HM,
    DF_HMS,
    DF_HMSU
} DisplayFormat;

/*
 * Delay Definitions
 */
#define SAWtube   1     // Set And Wait for TimeMode
#define SAWanim   20    // Set And Wait for DemoMode
#define EXIT_MODE 18000 // Delay generated during pass to different mode

/*
 * This is like never ending story.
 */
#define FOREVER ;;

#endif // VFD_CLOCK_H
