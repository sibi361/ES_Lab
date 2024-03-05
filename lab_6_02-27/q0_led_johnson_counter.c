#include <LPC17xx.h>

unsigned int i, j;
unsigned long LED = 0x00000FF0;

void delay(int n)
{
	for (j = 0; j < n; j++)
		;
}

int main(void)
{
	SystemInit();
	SystemCoreClockUpdate();

	LPC_PINCON->PINSEL0 = 0x00000000;
	LPC_GPIO0->FIODIR = 0x00000FF0;

	while (1)
	{
		LED = 0x00000010;
		for (i = 0; i < 9; i++)
		{
			LPC_GPIO0->FIOSET = LED;
			LED <<= 1;
			delay(100000);
		}

		LED = 0x00000010;
		for (i = 0; i < 9; i++)
		{
			LPC_GPIO0->FIOCLR = LED;
			LED <<= 1;
			delay(100000);
		}
	}
}
