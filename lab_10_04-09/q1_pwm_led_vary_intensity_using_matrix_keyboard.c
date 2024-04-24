#include <LPC17xx.h>

/*
Intensity Level      Key Pressed
	0%                  0
	33%                 1
	66%                 2
	100%                3
*/

#define BRIGHTNESS_MR_STEP 30000
#define WAIT_DELAY_1_MS_TC 3000

void init_pwm(void);
void update_width(unsigned int);
void delay_1_ms(void);
unsigned int scan_matrix_keyboard(void);

unsigned int temp, key;

int main(void)
{
	SystemInit();
	SystemCoreClockUpdate();

	// set P0.8 - P0.11 as output for matrix keyboard rows (CNA)
	LPC_GPIO0->FIODIR = 0xF << 9;

	init_pwm();

	while (1)
	{
		key = scan_matrix_keyboard();

		switch (key)
		{
		case 0x1:
			update_width(0);
			break;
		case 0x2:
			update_width(BRIGHTNESS_MR_STEP);
			break;
		case 0x4:
			update_width(BRIGHTNESS_MR_STEP * 2);
			break;
		case 0x8:
			update_width(BRIGHTNESS_MR_STEP * 3);
			break;
		}

		delay_1_ms();
	}
}

void init_pwm(void)
{
	// configure the only PWM pin available on board P1.23 as PWM1.4 (CNB -> LED)
	LPC_PINCON->PINSEL3 = 2 << 14; // function 2

	LPC_PWM1->TCR = 1 << 1;	 // stop timer and reset both TC and PC
	LPC_PWM1->PCR = 1 << 12; // set PWMENA4: enable PWN1.4 flipflop

	LPC_PWM1->PR = 0;
	LPC_PWM1->MR0 = BRIGHTNESS_MR_STEP * 4;
	LPC_PWM1->LER = 0xFF; // enable shadow register copying

	LPC_PWM1->MCR = 1 << 1;				 // reset TC on match event
	LPC_PWM1->TCR = (1 << 3) | (1 << 0); // enable PWN mode and start timer
}

void update_width(unsigned int new_pulse_width)
{
	LPC_PWM1->MR4 = new_pulse_width; // set PWM1.4 width
	LPC_PWM1->LER = 0xFF;			 // enable shadow register copying
}

unsigned int scan_matrix_keyboard(void)
{
	LPC_GPIO0->FIOSET = 1 << 8; // send 1 on first row

	// poll all columns until result is non-zero
	while (!(temp = (LPC_GPIO0->FIOPIN >> 4 & 0xF)))
		;

	return temp;
}

void delay_1_ms()
{
	LPC_TIM0->TCR = 1 << 1;
	LPC_TIM0->PR = 0;
	LPC_TIM0->TCR = 1 << 0;
	while (LPC_TIM0->TC < WAIT_DELAY_1_MS_TC)
		;
}
