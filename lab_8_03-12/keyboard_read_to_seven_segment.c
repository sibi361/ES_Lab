#include <LPC17xx.h>

#define KB_ROWS 4
#define KB_COLS 4

#define DELAY 1         // seconds
#define REFRESH_RATE 50 // hertz

unsigned int SEVEN_SEG_DATA_CODES[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x40};

unsigned char KB_KEYS_MAP[KB_ROWS][KB_COLS] = {
    {
        '1',
        '2',
        '3',
        '+',
    },
    {
        '4',
        '5',
        '6',
        '-',
    },
    {
        '7',
        '8',
        '9',
        '*',
    },
    {
        'C',
        '0',
        '=',
        '/',
    },
};

unsigned int k, temp, mr;

void delay()
{
    LPC_TIM0->TCR = 0x2;  // DISABLE and set RESET flag
    LPC_TIM0->EMR = 0x20; // Set EM0 to 1 on match event
    LPC_TIM0->MCR = 0x4;  // Stop PC and TC on match event

    LPC_TIM0->PR = 0x0;
    LPC_TIM0->MR0 = mr;

    LPC_TIM0->TCR = 0x1;           // ENABLE and clear RESET flag
    while (!(LPC_TIM0->EMR & 0x1)) // wait for match event i.e. until EM0 (bit 0) is 1
        ;
}

void display(unsigned int idx)
{
    for (k = 0; k < REFRESH_RATE * DELAY; k++) // timer is set to count only one second, hence multiply by delay
    {
        LPC_GPIO0->FIOPIN = SEVEN_SEG_DATA_CODES[idx] << 4;
        delay();
    }
}

char read_from_keyboard()
{
    unsigned int i, j, temp;

    temp = i = 0;

    while (!temp) // pressed switch will cause temp to be 0
    {
        for (i = 0; i < KB_ROWS; i++)
        {
            // write row
            LPC_GPIO2->FIOCLR = 0xF << 10; // clear P2.13 - P2.10
					  //LPC_GPIO2->FIOCLR = 0xFFFFFFFF;
            LPC_GPIO2->FIOSET = 1 << (10 + i);

            // read cols in while condition
					  temp = (LPC_GPIO1->FIOPIN >> 23) & 0xF;
        }
    }

		
    /*j = 0;
    while (j < KB_COLS && !(temp & 0x1))
    {
        temp >>= 1;
        j++;
    }

    // impossible edge case
    if (j >= KB_COLS)
    {
        j = 0;
    }*/

    return KB_KEYS_MAP[i][j];
}

int main()
{
    unsigned int key_pressed;

    SystemInit();
    SystemCoreClockUpdate();

    // matrix keyboard
    //LPC_GPIO2->FIODIRL |= 0xF << 10; // P2.10 -13
	  LPC_GPIO2->FIODIR = 0xFFFFFFFF;

    // seven segment
    //LPC_GPIO0->FIODIR |= 0xFF << 4; // Set P0.11 - P0.4 as output for seven segment data lines

		LPC_GPIO0->FIODIR = 0xFFFFFFFF; // Set P0.23 - P0.26 as output for seven segment data lines

    // REFRESH_RATE * (PR + 1) * (MR + 1) / (3 * 10 ^ 6) = 1 second
    // => (MR + 1) = (3 * 10 ^ 6) / [(PR + 1) * REFRESH_RATE]
    // => (MR + 1) = (3 * 10 ^ 6) / [(0 + 1) * 4 * REFRESH_RATE]
    mr = 3000000 / REFRESH_RATE - 1;
    
    key_pressed = 10; // display "-"

    while (1)
    {
			  display(key_pressed);

        key_pressed = read_from_keyboard();

        if (key_pressed >= '0' && key_pressed <= '9')
        {
            // displaying 0 - 9 only
            key_pressed -= '0';
        }
    }

    return 0;
}
