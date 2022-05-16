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

unsigned char state = 0;

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
	sprintf(buf, "iV:%1.2f MA:%1.2f", Data[INS], Data[MAX]);
	lcd_puts2(buf);

	lcd_pos(1, 0);
	sprintf(buf, "MI:%1.2f aV:%1.2f", Data[MIN], Data[AVG]);
	lcd_puts2(buf);
}

int short get_sample()
{
	int result;
	ADMUX = 0b010
	while (ADCSRA & (1 << ADSC))
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

		if (state = get_key() == 2)
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
		case 1:
			reset();
			state = 2;
			break;
		case 3:
			hold_avg();
			break;
		case 2:
			normal();
		}
		display();
	}
}