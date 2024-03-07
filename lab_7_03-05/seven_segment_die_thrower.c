#include <LPC17xx.h>

#define DELAY 1			// seconds
#define REFRESH_RATE 50 // hertz

unsigned int SEVEN_SEG_DATA_CODES[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D};

unsigned int i, mr, die;

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
	for (i = 0; i < REFRESH_RATE * DELAY; i++) // timer is set to count only one second, hence multiply by delay
	{
		LPC_GPIO0->FIOPIN = SEVEN_SEG_DATA_CODES[die] << 4;
		delay();
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
	LPC_GPIO0->FIODIRH |= 0x0 << 5; // Set P0.21 (7nth pin in CNC) as input for SW2

	// REFRESH_RATE * (PR + 1) * (MR + 1) / (3 * 10 ^ 6) = 1 second
	// => (MR + 1) = (3 * 10 ^ 6) / [(PR + 1) * REFRESH_RATE]
	// => (MR + 1) = (3 * 10 ^ 6) / [(0 + 1) * 4 * REFRESH_RATE]
	mr = 3000000 / REFRESH_RATE - 1;

	die = 0;

	while (1)
	{
		if (!(LPC_GPIO0->FIOPIN & 1 << 21)) // throw die when SW2 is pressed
		{
			die = rand() % 6 + 1;
		}

		display();
	}

	return 0;
}
