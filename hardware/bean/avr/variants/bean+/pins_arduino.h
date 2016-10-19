/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: wiring.h 249 2007-02-03 16:52:51Z mellis $
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

#define NUM_DIGITAL_PINS            10
#define NUM_ANALOG_INPUTS           6
#define analogInputToDigitalPin(p)  ((p < NUM_DIGITAL_PINS) ? (p) + NUM_DIGITAL_PINS : -1)

#define digitalPinHasPWM(p)         ((p) == 2 || (p) == 5 || (p) == 6 || (p) == 7)

#define IS_BEAN (0)

static const uint8_t SS   = 6;
static const uint8_t MOSI = 7;
static const uint8_t MISO = 8;
static const uint8_t SCK  = 9;

static const uint8_t SDA = 14;
static const uint8_t SCL = 15;
//static const uint8_t LED_BUILTIN = 13;

static const uint8_t A0 = 10;
static const uint8_t A1 = 11;
static const uint8_t A2 = 12;
static const uint8_t A3 = 13;
static const uint8_t A4 = 14;
static const uint8_t A5 = 15;

static const uint8_t CC_INTERRUPT_PIN = 16;

// TODO - I don't know what this stuff is
#define digitalPinToPCICR(p)    (((p) >= 0 && (p) <= 21) ? (&PCICR) : ((uint8_t *)0))
#define digitalPinToPCICRbit(p) (((p) <= 7) ? 2 : (((p) <= 13) ? 0 : 1))
#define digitalPinToPCMSK(p)    (((p) <= 7) ? (&PCMSK2) : (((p) <= 13) ? (&PCMSK0) : (((p) <= 21) ? (&PCMSK1) : ((uint8_t *)0))))
#define digitalPinToPCMSKbit(p) (((p) <= 7) ? (p) : (((p) <= 13) ? ((p) - 8) : ((p) - 14)))

#ifdef ARDUINO_MAIN

// On the Arduino board, digital pins are also used
// for the analog output (software PWM).  Analog input
// pins are a separate set.

// ATMEL ATMEGA8 & 168 / ARDUINO
//
//                  +-\/-+
//            PC6  1|    |28  PC5 (AI 5)
//      (D 0) PD0  2|    |27  PC4 (AI 4)
//      (D 1) PD1  3|    |26  PC3 (AI 3)
//      (D 2) PD2  4|    |25  PC2 (AI 2)
// PWM+ (D 3) PD3  5|    |24  PC1 (AI 1)
//      (D 4) PD4  6|    |23  PC0 (AI 0)
//            VCC  7|    |22  GND
//            GND  8|    |21  AREF
//            PB6  9|    |20  AVCC
//            PB7 10|    |19  PB5 (D 13)
// PWM+ (D 5) PD5 11|    |18  PB4 (D 12)
// PWM+ (D 6) PD6 12|    |17  PB3 (D 11) PWM
//      (D 7) PD7 13|    |16  PB2 (D 10) PWM
//      (D 8) PB0 14|    |15  PB1 (D 9) PWM
//                  +----+
//
// (PWM+ indicates the additional PWM pins on the ATmega168.)

// ATMEL ATMEGA1280 / ARDUINO
//
// 0-7 PE0-PE7   works
// 8-13 PB0-PB5  works
// 14-21 PA0-PA7 works
// 22-29 PH0-PH7 works
// 30-35 PG5-PG0 works
// 36-43 PC7-PC0 works
// 44-51 PJ7-PJ0 works
// 52-59 PL7-PL0 works
// 60-67 PD7-PD0 works
// A0-A7 PF0-PF7
// A8-A15 PK0-PK7


// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &DDRB,
	(uint16_t) &DDRC,
	(uint16_t) &DDRD,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PINB,
	(uint16_t) &PINC,
	(uint16_t) &PIND,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PD, 	// D0
	PD,     // D1
	PD,     // D2	PWM
	PD,     // D3
	PB,     // D4
	PB,     // D5	PWM
	PB,     // D6   PWM
	PB,	    // D7	PWM
	PB,	    // D8
	PB, 	// D9
	PC,     // A0
	PC,	    // A1
	PC,     // A2
	PC, 	// A3
	PC,	    // A4
	PC,	    // A5
	PD,     // CC_INTERRUPT
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(2),	// D0
	_BV(4),	// D1
	_BV(6),	// D2	PWM
	_BV(7),	// D3
	_BV(0),	// D4
	_BV(1),	// D5	PWM
	_BV(2),	// D6	PWM
	_BV(3),	// D7	PWM
	_BV(4),	// D8
	_BV(5),	// D9
	_BV(0),	// A0
	_BV(1),	// A1
	_BV(2),	// A2
	_BV(3),	// A3
	_BV(4),	// A4
	_BV(5),	// A5
	_BV(5), // D5
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER, // D0
	NOT_ON_TIMER, // D1
	TIMER0A,      // D2	PWM
	NOT_ON_TIMER, // D3
	NOT_ON_TIMER, // D4
	TIMER1A,      // D5	PWM
	TIMER1B,      // D6	PWM
	TIMER2A,      // D7	PWM
	NOT_ON_TIMER, // D8
	NOT_ON_TIMER, // D9
	NOT_ON_TIMER, // A0
	NOT_ON_TIMER, // A1
	NOT_ON_TIMER, // A2
	NOT_ON_TIMER, // A3
	NOT_ON_TIMER, // A4
	NOT_ON_TIMER, // A5
	NOT_ON_TIMER, // CC_INTERRUPT
};

#endif  // ARDUINO_MAIN
#endif  // Pins_Arduino_h
