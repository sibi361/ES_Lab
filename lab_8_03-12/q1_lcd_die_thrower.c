#include <LPC17xx.h>

#define REG_SEL 1 << 27
#define ENABLE 1 << 28
#define BUFF_LEN 16

unsigned long int LCD_INIT_CMDS[9] = {0x30, 0x30, 0x30, 0x20, 0x28, 0x0C, 0x06, 0x01, 0x80};

void delay(unsigned int top)
{
    for (i = 0; i < top; i++)
        ;
}

void lcd_send(unsigned int cmd, unsigned int data)
{
    LPC_GPIO0->FIOPIN = (data & 0xF0) << 19;

    cmd ? (LPC_GPIO0->FIOCLR = REG_SEL) : (LPC_GPIO0->FIOSET = REG_SEL);

    // provide negative edge
    LPC_GPIO0->FIOSET = ENABLE;
    delay(100);
    LPC_GPIO0->FIOCLR = ENABLE;

    delay(15000);

    if (data != 0x20 && data != 0x30)
    {
        LPC_GPIO0->FIOPIN = (data & 0xF) << 23;

        cmd ? (LPC_GPIO0->FIOCLR = REG_SEL) : (LPC_GPIO0->FIOSET = REG_SEL);

        // provide negative edge
        LPC_GPIO0->FIOSET = ENABLE;
        delay(100);
        LPC_GPIO0->FIOCLR = ENABLE;

        delay(15000);
    }
}

void display(char *str)
{
    unsigned int i;
    char temp;

    for (i = 0; i < sizeof(LED_INIT_CMDS) / sizeof(LED_INIT_CMDS[0]); i++)
    {
        lcd_send(1, LED_INIT_CMDS[i]); // send command
    }

    i = 0;
    while ((temp = str[i++]) != '\0')
    {
        lcd_send(0, temp); // send data
    }
}

int main()
{
    char buffer[BUFF_LEN] = "Die toss: ";
    unsigned int die;

    SystemInit();
    SystemCoreClockUpdate();

    // LCD
    LPC_GPIO0->FIODIR |= (0xF << 23) | REG_SEL | ENABLE;

    display(buffer);

    while (1)
    {
        if (!(LPC_GPIO0->FIOPIN & 1 << 21)) // throw die when SW2 is pressed
        {
            die = rand() % 6 + 1;
            buffer[10] = '0' + die;
        }

        display(buffer);
    }

    return 0;
}
