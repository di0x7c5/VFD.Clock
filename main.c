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

#define F_CPU 8000000U

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "i2c.h"
#include "vfd_clock.h"

static unsigned char RTChun = 0;
static unsigned char RTCsec = 0;
static unsigned char RTCmin = 0;
static unsigned char RTChrs = 0;
static unsigned char HourDEC = 0;
static unsigned char HourUNI = 0;
static unsigned char MinuteDEC = 0;
static unsigned char MinuteUNI = 0;
static unsigned char SecondDEC = 0;
static unsigned char SecondUNI = 0;
static unsigned char HundredDEC = 0;
static unsigned char HundredUNI = 0;

static unsigned int tSign[10] =     // Signs' Board
    { CHAR_0, CHAR_1, CHAR_2, CHAR_3, CHAR_4, CHAR_5, CHAR_6, CHAR_7, CHAR_8, CHAR_9 };
static unsigned int tcurrentNet[8] =       // currentNet's Board
    { ~(1<<7), ~(1<<6), ~(1<<5), ~(1<<4), ~(1<<3), ~(1<<2), ~(1<<1), ~(1<<0) };

DisplayFormat displayFormat = DF_HM;

/*
 * Function return true if given key is pressed.
 */
unsigned char isPressed(Button btn) {
    return !(KEYPIN & btn);
}

unsigned char isNotPressed(Button btn) {
    return (KEYPIN & btn);
}

/*
 * Get unit and decimal from bsd number.
 */
void Bcd2UniDec(unsigned char* bcd, unsigned char* uni, unsigned char* dec) {
    *uni = *bcd & 0x0f;
    *dec = (*bcd & 0xf0) >> 4;
}

/*
 * Get current time from PCF8583
 */
void getCurrentTime(void) {
    RTChun = RTC_READ(0x01);
    RTCsec = RTC_READ(0x02);
    RTCmin = RTC_READ(0x03);
    RTChrs = RTC_READ(0x04);

    Bcd2UniDec(&RTChun, &HundredUNI, &HundredDEC);
    Bcd2UniDec(&RTCsec, &SecondUNI, &SecondDEC);
    Bcd2UniDec(&RTCmin, &MinuteUNI, &MinuteDEC);
    Bcd2UniDec(&RTChrs, &HourUNI, &HourDEC);
}

/*
 * Key manager thread.
 */
void keyManagerThread(void) {
    // If we pressed P1 button then...
    if(isPressed(P1)) {
        _delay_ms(10);

        if(isPressed(P1)) {
            ++HourUNI;

            if(HourUNI>9) {
                HourDEC++;
                HourUNI=0;
            }

            if(HourDEC==2 && HourUNI==4) {
                RTC_WRITE(0x04,0x00);
            } else {
                RTChrs = (HourDEC<<4) | HourUNI;
                RTC_WRITE(0x04,RTChrs);
            }

            _delay_ms(100);
        }
    }

    // If we pressed P2 button then...
    if(isPressed(P2)) {
        _delay_ms(10);

        if(isPressed(P2)) {
            ++MinuteUNI;

            if(MinuteUNI>9) {
                MinuteDEC++;
                MinuteUNI=0;
            }

            if(MinuteDEC>5) {
                RTC_WRITE(0x03,0x00);
            } else {
                RTCmin = (MinuteDEC<<4) | MinuteUNI;
                RTC_WRITE(0x03,RTCmin);
            }

            _delay_ms(100);
        }
    }

    // If we pressed P3 button then...
    if(isPressed(P3)) {
        _delay_ms(10);

        if(isPressed(P3)) {
            RTC_WRITE(0x01,0x00); // set value of hundrets second to 0
            RTC_WRITE(0x02,0x00); // set value of seconds to 0
        }
    }

    // If we pressed P4 button then...
    if(isPressed(P4)) {
        _delay_ms(10);

        if(isPressed(P4)) {
            switch(displayFormat) {
                case DF_HM: displayFormat = DF_HMS;
                    break;
                case DF_HMS: displayFormat = DF_HM;
                    break;
                default: /* nothing here */
		    break;
            }

            _delay_ms(200);
        }
    }
}

/*
 * The main function.
 */
int main(void) {

    /*
     * Set the ports directions
     * 0 - input
     * 1 - output
     */
    ANODEDDR = 0xff;
    NETDDR   = 0xff;
    KEYDDR   = 0xf0;
    KEYPORT  = 0x0f;

    /*
     * Timer 0 configuration
     */
    TCCR0 = 1<<CS00;    // Timer/Counter Control Register
    TIMSK = 1<<TOIE0;   // Timer/Counter Interrupt Mask Register

    /*
     * Global switch on Interruption
     */
    sei();

    /*
     * Main loop of programm.
     */
    for(FOREVER) {
        getCurrentTime();   // get current time
        keyManagerThread(); // run key manager
    }

    return 0;
}

/*
 * Interrupt from timer 0 overflow.
 */
ISR(TIMER0_OVF_vect) {
    static unsigned char currentNet = 0;

    // let's light turn off
    NETPORT = 0xff;

    /*
     * Display Formats:
      *      01234567
     * HM:   x13-12xx
     * HMS:  13-12-32
     * HMSU: 12213212
     *       01234567
     */

    // Turn On the Right Sign
    switch(currentNet) {
        case 0:
            if(displayFormat == DF_HM) {
                ANODEPORT = 0xff;
            } else  {
                ANODEPORT = (HourDEC != 0) ? tSign[HourDEC] : 0xff;
	    } break;
        case 1:
            if(displayFormat == DF_HM) {
                ANODEPORT = (HourDEC != 0) ? tSign[HourDEC] : 0xff;
            } else {
                ANODEPORT = tSign[HourUNI];
            } break;
        case 2:
            if(displayFormat == DF_HM) {
		ANODEPORT = tSign[HourUNI];
            } else {
		ANODEPORT = ~(SEG_G);
	    } break;
        case 3:
            if(displayFormat == DF_HM) {
		ANODEPORT = ~(SEG_G);
            } else {
		ANODEPORT = tSign[MinuteDEC];
	    } break;
        case 4:
            if(displayFormat == DF_HM) {
                ANODEPORT = tSign[MinuteDEC];
            } else {
		ANODEPORT = tSign[MinuteUNI];
	    } break;
        case 5:
            if(displayFormat == DF_HM) {
                ANODEPORT = tSign[MinuteUNI];
            } else {
		ANODEPORT = ~(SEG_G);
	    } break;
        case 6:
            if(displayFormat == DF_HM) {
                ANODEPORT = 0xff;
            } else {
		ANODEPORT = tSign[SecondDEC];
	    } break;
        case 7:
            if(displayFormat == DF_HM) {
                ANODEPORT = 0xff;
            } else {
                ANODEPORT = tSign[SecondUNI];
            } break;
        default: /* empty here */
            break;
    }

    // turn on the tube
    NETPORT = tcurrentNet[currentNet++];

    if(currentNet > 7) {
        currentNet = 0;
    }
}
/*
 * AND THIS IS THE END OF THIS FANTASTIC PROGRAM :-)
 *
 * Program is dedicated for two persons: AP & NB
 * Without who, my world would be gray and boring...
 * Thank You Very Much !!!
 *
 * :-)
 */
