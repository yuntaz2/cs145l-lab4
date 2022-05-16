/*
 * avr.h
 *
 * Created: 4/4/2022 5:37:26 PM
 *  Author: Yun
 */ 


#ifndef AVR_H_
#define AVR_H_

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/io.h>

#define XTAL_FRQ 8000000lu

#define SET_BIT(p,i) ((p) |=  (1 << (i)))
#define CLR_BIT(p,i) ((p) &= ~(1 << (i)))
#define GET_BIT(p,i) ((p) &   (1 << (i)))

#define NOP() asm volatile("nop"::)

void avr_wait(unsigned short msec);

#endif /* AVR_H_ */