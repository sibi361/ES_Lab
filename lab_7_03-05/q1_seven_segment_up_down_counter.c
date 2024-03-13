#include <LPC17xx.h>

#define COUNT_DIGITS 4

unsigned long int SEVEN_SEG_DECODER_CODES[4] = {0xF87FFFFF, 0xF8FFFFFF, 0xF97FFFFF, 0xF9FFFFFF};
unsigned int SEVEN_SEG_DATA_CODES[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

unsigned int i, j, k;
unsigned int digit_array[4] = {0};
int counter;

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

	LPC_TIM0->PR = 149;
	LPC_TIM0->MR0 = 99; // 50 * 4 * (149 + 1) * (99 + 1) / (3 * 10 ^ 6) = 1 second

	LPC_TIM0->TCR = 0x1;		   // ENABLE and clear RESET flag
	while (!(LPC_TIM0->EMR & 0x1)) // wait until EM0 bit is 1
		;
}

void display()
{
	for (j = 0; j < 50; j++) // refresh rate: 50 Hz
	{
		for (k = 0; k < COUNT_DIGITS; k++)
		{
			LPC_GPIO1->FIOPIN = SEVEN_SEG_DECODER_CODES[k];				   // select seven segment display amongst four displays
			LPC_GPIO0->FIOPIN = SEVEN_SEG_DATA_CODES[digit_array[k]] << 4; // send current digit code to the selected display
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

	counter = -1;

	while (1)
	{
		!(LPC_GPIO0->FIOPIN & 1 << 21)
			? counter--	 // Down counter if SW2 is pressed
			: counter++; // Else up counter

		counter = counter >= 10000
					  ? 0
				  : counter < 0
					  ? 9999
					  : counter;

		split_num_into_digit_array_reversed(counter);

		display();
	}

	return 0;
}
