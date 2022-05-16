#include <stdio.h>
#include <xc.h>
#include "avr.h"
#include "keypad.h"
#include "lcd.h"

#define LCDSIZE 17
#define SEC 1000
#define INS 0
#define MAX 1
#define MIN 2
#define AVG 3

unsigned char state = 8;

float Data[4] = {0.12, 0.23, 0.34, 0.45};

void reset(void)
{
	for (int i = 0; i < 4; ++i)
		Data[i] = 0.00;
}

void display(void)
{
	char buf[LCDSIZE];

	lcd_pos(0, 0);
	sprintf(buf, "%d:%1.2f MA:%1.2f", state, Data[INS], Data[MAX]);
	lcd_puts2(buf);

	lcd_pos(1, 0);
	sprintf(buf, "MI:%1.2f aV:%1.2f", Data[MIN], Data[AVG]);
	lcd_puts2(buf);
}

int short get_sample()
{
	int result;
	ADMUX = 0b01000000;
	ADCSRA = 0b11000000;
	while (GET_BIT(ADCSRA, 6))
		; // wait for a change in the voltage
	result = ADC;
	return result;
}

void normal(void)
{
	Data[INS] = (get_sample() / 1023.0) * 5;
}

void hold_avg(void)
{
	reset();
	while (1)
	{
		avr_wait(500);

		if ((state = get_key()) == 8)
			break;
		Data[INS] = (get_sample() / 1023.0) * 5;
		if (Data[MAX] < Data[INS])
			Data[MAX] = Data[INS];
		if (Data[MIN] > Data[INS])
			Data[MIN] = Data[INS];
		Data[AVG] = (Data[AVG] + Data[INS]) / 2.0;

		display();
	}
}

int main(void)
{
	lcd_init();
	avr_wait(1000);
	lcd_clr();
	while (1)
	{
		avr_wait(500);
		unsigned char k = get_key();
		if (k)
			state = k;
		switch (state)
		{
		case 4:
			reset();
			state = 8;
			break;
		case 3:
			hold_avg();
			break;
		case 8:
			normal();
		}
		display();
	}
}