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

	LPC_PINCON->PINSEL0 = 0x0;		// Set P0.15 - P0.0 to GPIO
	LPC_GPIO0->FIODIR |= 0xFF << 4; // Set P0.11 - P0.4 as output

	counter = 0;

	while (1)
	{
		LPC_GPIO0->FIOPIN = counter++ << 0x4;

		delay(15000);
	}

	return 0;
}
