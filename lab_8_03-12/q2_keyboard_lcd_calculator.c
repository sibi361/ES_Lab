#include <LPC17xx.h>

#define KB_ROWS 4
#define KB_COLS 4

#define REG_SEL 1 << 27
#define ENABLE 1 << 28
#define BUFF_LEN 16

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
        'X',
        '0',
        'C',
        '/',
    },
};

unsigned long int LCD_INIT_CMDS[] = {0x30, 0x30, 0x30, 0x20, 0x24, 0x0F, 0x06, 0x01, 0x80};

void lcd_send(unsigned int cmd, unsigned int data)
{
    unsigned int m;

    LPC_GPIO0->FIOPIN = (data & 0xF0) << 19;
    LPC_GPIO0->FIOPIN = (data & 0xF) << 23;

    cmd ? (LPC_GPIO0->FIOCLR = REG_SEL) : (LPC_GPIO0->FIOSET = REG_SEL);

    // provide negative edge
    LPC_GPIO0->FIOSET = ENABLE;
    for (m = 0; m < 100; m++)
        ;
    LPC_GPIO0->FIOCLR = ENABLE;
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

char read_from_keyboard()
{
    unsigned int i, j, temp;

    temp = i = 0;
    while (!(temp = (LPC_GPIO1->FIOPIN >> 23) & 0xF)) // pressed switch will cause temp to be 0
    {
        for (i = 0; i < KB_ROWS; i++)
        {
            // write row
            LPC_GPIO2->FIOCLR = 0xF << 10; // clear P2.13 - P2.10
            LPC_GPIO2->FIOSET = 1 << (10 + i);

            // read cols in while condition
        }
    }

    j = 0;
    while (j < KB_COLS && !(temp & 0x1))
    {
        temp >>= 1;
        j++;
    }

    // impossible edge case
    if (j >= KB_COLS)
    {
        j = 0;
    }

    return KB_KEYS_MAP[i][j];
}

char get_result(char op1, char op2, char operator)
{
    unsigned int op1_num, op2_num, result;
    op1_num = op1 - '0';
    op2_num = op2 - '0';

    switch (operator)
    {
    case '+':
        result = op1_num + op2_num;
        break;
    case '-':
        result = op1_num - op2_num;
        break;
    case '*':
        result = op1_num * op2_num;
        break;
    case '/':
        result = op1_num / op2_num;
        break;

    default:
        return 'X';
    }

    return '0' + result;
}

int main()
{
    char buffer[BUFF_LEN];
    char temp, operand, operation;
    unsigned int state, idx = 0;

    SystemInit();
    SystemCoreClockUpdate();

    // matrix keyboard
    LPC_GPIO2->FIODIRL |= 0xF << 10; // P2.10 -13

    // LCD
    LPC_GPIO0->FIODIR |= (0xF << 23) | REG_SEL | ENABLE;

    display("C4LCUL4T0R");

    state = 0; // awaiting operand

    while (1)
    {
        temp = read_from_keyboard();

        if (temp == 'X')
        {
            buffer[0] = '\0';
            display(buffer);
            idx = state = 0;
            continue;
        }
        else if (temp == 'C')
        {
            buffer[--idx] = '\0';
            display(buffer);
            if (state)
                state--;
            continue;
        }

        switch (state)
        {
        case 0:
            if (!(temp >= '0' && temp <= '9'))
            {
                continue;
            }

            buffer[idx++] = operand = temp;
            buffer[idx] = '\0';
            state = 1; // awaiting operator
            break;
        case 1:
            if (!(temp == '+' || temp == '-' || temp == '*' || temp == '/'))
            {
                continue;
            }

            buffer[idx++] = ' ';
            buffer[idx++] = operation = temp;
            buffer[idx] = '\0';
            state = 2; // awaiting second operand
            break;
        case 2:
            if (!(temp >= '0' && temp <= '9'))
            {
                continue;
            }

            buffer[idx++] = ' ';
            buffer[idx++] = temp;
            buffer[idx++] = ' ';
            buffer[idx++] = '=';

            buffer[idx++] = get_result(operand, temp, operation);

            buffer[idx++] = ' ';
            buffer[idx] = '\0';

            display(buffer);
            idx = state = 0;
            continue;
        default:
            display("?????");
            idx = state = 0;
            continue;
        }

        display(buffer);
    }

    return 0;
}
