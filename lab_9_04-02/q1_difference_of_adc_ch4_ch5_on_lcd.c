#include <LPC17xx.h>
#include <stdio.h>

#define LCD_INIT_DELAY 10000

#define REFERENCE_VOLTAGE 3.300
#define FULL_SCALE 0xFFF // 12 bit ADC

// LCD connected to CNC
#define DATA_CTRL 0xF << 15
#define RS_CTRL 1 << 19
#define EN_CTRL 1 << 20

void lcd_init(void);
void lcd_puts(unsigned char *);
void lcd_write(unsigned int);
void port_write(unsigned int);
void clear_ports(void);
void delay(unsigned int);

unsigned int temp, temp1, temp2, adc_4_reading, adc_5_reading;
float digital_diff, analog_diff;

unsigned char lcd_line_1[110] = "ADC CH4 - CH5";
unsigned char lcd_line_2[100] = "Difference Calc";

void ADC_IRQHandler(void)
{
	// clear all interrupt flags
	LPC_ADC->ADINTEN = 0x1FF;

	// check bit 31 done flag
	if (!((temp = LPC_ADC->ADGDR) & (1 << 31)))
	{
		return;
	}
	// check if channel 4 has been polled
	else if ((temp >> 24 & 0x7) == 0x4)
	{
		adc_4_reading = (temp >> 4) & 0xFFF;
	}
	// check if channel 5 has been polled
	else if ((temp >> 24 & 0x7) == 0x5)
	{
		adc_5_reading = (temp >> 4) & 0xFFF;
	}
	else
	{
		return;
	}

	digital_diff = adc_4_reading - adc_5_reading;
	analog_diff = ((float)digital_diff * (float)REFERENCE_VOLTAGE) / (float)FULL_SCALE;

	sprintf(lcd_line_1, "%3.0f           ", digital_diff);

	// move cursor to first line and write string
	temp1 = 0x80;
	lcd_write(0);
	delay(800);
	lcd_puts(&lcd_line_1[0]);

	sprintf(lcd_line_2, "%3.2fV        ", analog_diff);

	// move cursor to second line and write string
	temp1 = 0xC0;
	lcd_write(0);
	delay(800);
	lcd_puts(&lcd_line_2[0]);
}

int main(void)
{
	SystemInit();
	SystemCoreClockUpdate();

	LPC_GPIO0->FIODIR = RS_CTRL | EN_CTRL | DATA_CTRL;

	lcd_init();
	delay(LCD_INIT_DELAY);

	// P1.31 as AD0.5 & P1.30 as AD0.4 Function 3
	LPC_PINCON->PINSEL3 = (3 << 30) | (3 << 28);
	LPC_SC->PCONP |= (1 << 12);

	LPC_ADC->ADCR = (1 << 4) | (1 << 5) | (1 << 21) | (1 << 16);

	LPC_ADC->ADINTEN = (1 << 4) | (1 << 5); // enable interrupt on ADC CH4 & CH5 scan
	NVIC_EnableIRQ(ADC_IRQn);				// enable ADC interrupts globally

	while (1)
		;
}

void lcd_init()
{
	clear_ports();
	delay(3200);
	temp2 = (0x30 << 15);
	port_write(0);
	delay(30000);
	temp2 = (0x30 << 15);
	port_write(0);
	delay(30000);
	temp2 = (0x30 << 15);
	port_write(0);
	delay(30000);
	temp2 = (0x20 << 15);
	port_write(0);
	delay(30000);
	temp1 = 0x28;
	lcd_write(0);
	delay(30000);
	temp1 = 0x0C;
	lcd_write(0);
	delay(800);
	temp1 = 0x06;
	lcd_write(0);
	delay(800);
	temp1 = 0x01;
	lcd_write(0);
	delay(10000);
	temp1 = 0x80;
	lcd_write(0);
	delay(800);
	return;
}

void lcd_write(unsigned int is_data)
{
	temp2 = temp1 & 0xF0;
	temp2 = temp2 << 11;
	port_write(is_data);
	temp2 = temp1 & 0x0F;
	temp2 = temp2 << 15;
	port_write(is_data);
	delay(1000);
	return;
}

void port_write(unsigned int is_data)
{
	clear_ports();
	LPC_GPIO0->FIOPIN = temp2;

	(is_data) ? (LPC_GPIO0->FIOSET = RS_CTRL) : (LPC_GPIO0->FIOCLR = RS_CTRL);

	LPC_GPIO0->FIOSET = EN_CTRL;
	delay(25);
	LPC_GPIO0->FIOCLR = EN_CTRL;
	return;
}

void clear_ports(void)
{
	LPC_GPIO0->FIOCLR = RS_CTRL | EN_CTRL | DATA_CTRL;
	return;
}

void lcd_puts(unsigned char *buffer)
{
	unsigned int i;
	while (buffer[i] != '\0' && i++ < 16)
	{
		temp1 = buffer[i];
		lcd_write(1);
	}
	return;
}

void delay(unsigned int n)
{
	unsigned int i;

	for (i = 0; i < n; i++)
		;
}
