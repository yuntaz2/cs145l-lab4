/*
 * keypad.c
 *
 * Created: 5/16/2022 5:01:36 PM
 *  Author: Yun
 */
#include "avr.h"

unsigned char is_pressed(int r, int c)
{
	// set all 8 GPIOs to N/C
	DDRA = 0;
	PORTA = 0;

	c = 3 - c;
	r = 7 - r;
	// set r to "0"
	SET_BIT(DDRA, r);

	// set c to "w1"
	SET_BIT(PORTA, c);
	avr_wait(1);
	return GET_BIT(PINA, c) == 0;
}

unsigned char get_key(void)
{
	int i, j;
	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			if (is_pressed(i, j))
			{
				return i * 4 + j + 1;
			}
		}
	}
	return 0;
}

unsigned mapping(unsigned char i)
{
	switch (i)
	{
	case (1):
	case (2):
	case (3):
		return i;
	case (5):
	case (6):
	case (7):
		return i - 1;
	case (9):
	case (10):
	case (11):
		return i - 2;
	case (14):
		return 0;
	default:
		return 0;
	}
}
