#include <LPC17xx.h>

#define DELAY 60		// seconds
#define REFRESH_RATE 50 // hertz

#define START_HOUR 14
#define START_MINUTE 28

#define COUNT_DIGITS 4

unsigned int SEVEN_SEG_DECODER_CODES[] = {0xF87FFFFF, 0xF8FFFFFF, 0xF97FFFFF, 0xF9FFFFFF};
unsigned int SEVEN_SEG_DATA_CODES[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

unsigned int i, j, k, mr, hour, minute, concat;
unsigned int digit_array[4];

void split_num_into_digit_array_reversed(unsigned int input)
{
	// e.g. 9876 becomes {6, 7, 8, 9}

	for (i = 0; i < COUNT_DIGITS; i++)
	{
		digit_array[i] = input % 10;
		input /= 10;
	}
}

void delay()
{
	LPC_TIM0->TCR = 0x2;  // DISABLE and set RESET flag
	LPC_TIM0->EMR = 0x20; // Set EM0 to 1 on match event
	LPC_TIM0->MCR = 0x4;  // Stop PC and TC on match event

	LPC_TIM0->PR = 0x0;
	LPC_TIM0->MR0 = mr;

	LPC_TIM0->TCR = 0x1;		   // ENABLE and clear RESET flag
	while (!(LPC_TIM0->EMR & 0x1)) // wait for match event i.e. until EM0 (bit 0) is 1
		;
}

void display()
{
	for (j = 0; j < REFRESH_RATE * DELAY; j++) // timer is set to count only one second, hence multiply by delay
	{
		for (k = 0; k < COUNT_DIGITS; k++)
		{
			LPC_GPIO1->FIOPIN = SEVEN_SEG_DECODER_CODES[k]; // select seven segment display amongst four displays
			LPC_GPIO0->FIOPIN = (SEVEN_SEG_DATA_CODES[digit_array[k]] | (k == 2
																			 ? 0x80 // display current digit with decimal point
																			 : 0x0	// display current digit
																		 ))
								<< 4;
			delay();
		}
	}
}

int main()
{
	SystemInit();
	SystemCoreClockUpdate();

	LPC_PINCON->PINSEL0 = 0x0; // Set P0.15 - P0.0 to GPIO
	LPC_PINCON->PINSEL3 = 0x0; // Set P1.31 - P1.16 to GPIO
	LPC_PINCON->PINSEL5 = 0x0; // Set P2.31 - P2.16 to GPIO

	LPC_GPIO0->FIODIR |= 0xFF << 4; // Set P0.11 - P0.4 as output for seven segment data lines
	LPC_GPIO1->FIODIRH |= 0xF << 7; // Set P1.26 - P1.23 as output for seven segement decoder selection lines
	LPC_GPIO0->FIODIRH |= 0x0 << 5; // Set P0.21 (7nth pin in CNC) as input for SW2

	// REFRESH_RATE * COUNT_DIGITS * (PR + 1) * (MR + 1) / (3 * 10 ^ 6) = 1 second
	// => (MR + 1) = (3 * 10 ^ 6) / [(PR + 1) * COUNT_DIGITS * REFRESH_RATE]
	// => (MR + 1) = (3 * 10 ^ 6) / [(0 + 1) * 4 * REFRESH_RATE]
	mr = 3000000 / (COUNT_DIGITS * REFRESH_RATE) - 1;

	hour = START_HOUR;
	minute = START_MINUTE;

	while (1)
	{
		minute += !(LPC_GPIO0->FIOPIN & 1 << 21)
					  ? 10 // fast forward clock on SW2 press
					  : 1;

		if (minute > 59)
		{
			minute = 0;
			hour += 1;
		}

		if (hour > 23)
		{
			hour = minute = 0;
		}

		concat = hour * 100 + minute;

		split_num_into_digit_array_reversed(concat);

		display();
	}

	return 0;
}
