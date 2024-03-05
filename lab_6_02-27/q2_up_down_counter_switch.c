#include <LPC17xx.h>

unsigned int counter, j;

void delay(int n)
{
	for (j = 0; j < n; j++)
		;
}

int main(void)
{
	SystemInit();
	SystemCoreClockUpdate();

	LPC_PINCON->PINSEL0 = 0x0; // Set P0.15 - P0.0 to GPIO
	LPC_PINCON->PINSEL4 = 0x0; // Set P2.15 - P2.0 to GPIO

	LPC_GPIO0->FIODIR |= 0xFF << 4; // Set P0.11 - P0.4 as output for LED
	LPC_GPIO2->FIODIR |= 0x0 << 12; // Set P2.12 as input for SW2

	counter = 0;

	while (counter <= 0xFFFFFFFF)
	{
		!(LPC_GPIO2->FIOPIN & 1 << 12)
			? counter--	 // Down counter if SW2 is pressed
			: counter++; // Else up counter

		if (counter >= 256)
		{
			counter = 0;
		}
		else if (counter <= 0)
		{
			counter = 255;
		}

		LPC_GPIO0->FIOPIN = counter << 4;

		delay(30000);
	}

	return 0;
}
