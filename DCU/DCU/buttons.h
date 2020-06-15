
#ifndef BUTTONS_H_
#define BUTTONS_H_

int button_flag[4];

void button_function_0(void)
{
	if(button_flag[0] == 1)
	{
		//INSERT button function here
		button_flag[0] = 0;
	}
}

void button_function_1(void)
{
	if(button_flag[1] == 1)
	{
		//INSERT button function here
		button_flag[1] = 0;
	}
}


void button_function_2(void)
{
	if(button_flag[2] == 1)
	{
		//INSERT button function here
		button_flag[2] = 0;
	}
}


void button_function_3(void)
{
	if(button_flag[3] == 1)
	{
		//INSERT button function here
		button_flag[3] = 0;
	}
}

//INTERUPT for buttons
ISR(PCINT1_vect)
{
	if (!(PINC & (1<<PINC0))) //NOTE: The setup used to create this has the buttons pulled off when not pressed. Change logic if this is not the case.
	{
		button_flag[0] =1;
	}
	if (!(PINC & (1<<PINC1)))
	{
		button_flag[1] =1;
	}
	if (!(PINC & (1<<PINC2)))
	{
		button_flag[2] =1;
	}
	if (!(PINC & (1<<PINC3)))
	{
		button_flag[3] =1;
	}
}

#endif /* BUTTONS_H_ */