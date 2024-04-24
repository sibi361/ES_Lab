#include <LPC17xx.H>

#define STEP_SPEED_DELAY 3000
#define WAIT_DELAY 65000
#define ROTATE_COUNT 3

void rotate_clockwise(void);
void rotate_anti_clockwise(void);
void delay(unsigned int);

unsigned int i, j, k, temp;

int main(void)
{
	SystemInit();
	SystemCoreClockUpdate();

	LPC_GPIO0->FIODIR = 0xF << 4; // P0.4 to P0.7 stepper motor output (CNA)

	while (1)
	{
		// read SW2 - 7nth pin of CNB
		if (LPC_GPIO2->FIOPIN & (1 << 12))
		{
			for (i = 0; i < ROTATE_COUNT; i++)
			{
				rotate_clockwise();
			}
		}
		else
		{
			for (i = 0; i < ROTATE_COUNT; i++)
			{
				rotate_anti_clockwise();
			}
		}

		// Delay to show rotation
		delay(WAIT_DELAY);
	}
}

void rotate_clockwise(void)
{
	temp = 1 << 3;

	// for A B C D stepping: 4 -> 5 -> 6 -> 7
	for (j = 0; j < 4; j++)
	{
		LPC_GPIO0->FIOPIN = ~(temp <<= 1);

		// for motor speed variation
		delay(STEP_SPEED_DELAY);
	}
}

void rotate_anti_clockwise(void)
{
	temp = 1 << 8;

	// for A B C D stepping: 7 -> 6 -> 5 -> 4
	for (k = 0; k < 4; k++)
	{
		LPC_GPIO0->FIOPIN = ~(temp >>= 1);

		// for motor speed variation
		delay(STEP_SPEED_DELAY);
	}
}

void delay(unsigned int n)
{
	unsigned int m;

	for (m = 0; m < n; m++)
		;
}
