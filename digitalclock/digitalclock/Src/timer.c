#include "main.h"

////////timer0////////
volatile uint8_t step; // switch case, check

////////timer2////////
volatile uint16_t count2; //1ms check
volatile uint8_t sw_time; //SW check
volatile uint8_t state;
volatile uint8_t pre_state;
volatile uint16_t times;

////////timer0, timer2 같이 사용////////
volatile uint8_t time_stop;
volatile uint16_t time_sec;
volatile uint16_t time_sec_set;
volatile uint8_t flag;

////////FND control////////
volatile uint8_t fnd[255];
volatile uint8_t fnd_pos[255] = { 0xDF, 0xEF, 0xFE, 0xFD, 0xFB, 0xF7}; 
volatile uint8_t number_m[255] = { 0xEB, 0x28, 0xB3, 0xBA, 0X78, 0XDA, 0XDB, 0XE8, 0XFB, 0XF8, 0x00};
volatile uint8_t number_s[255] = { 0x03, 0x3F, 0x4A, 0x2A, 0x36, 0xA2, 0x82, 0x33, 0x02, 0x32, 0xFF};

ISR(TIMER0_OVF_vect) //100us 인터럽트 발생
{
	TCNT0 = 256 - 206;
	PORTA = 0x00; //FND reset
	PORTD = 0xFF;
	PORTG = 0X01;
	
	if (++step >= 6) //문장 시작시 ++...
	{
		step = 0;
	}
	
	switch (step) //step0~5 FND 값 표현..
	{
		case 0:
		if (time_stop == 1 && flag == 1) // time_stop = 1 -> 2FND blink mode
		{
			PORTD = number_s[10]; // FND off
		}
		else if (time_stop == 2 && flag == 1) // time_stop = 2 -> 4FND blink mode(ooxx) <- 뒤에 두개
		{
			PORTD = number_s[fnd[step]]; // FND on
			PORTC = fnd_pos[step]; 
		}
		else if (time_stop == 3 && flag == 1) // time_stop = 3 -> 4FND blink mode(xxoo) <- 앞에 두개
		{
			PORTD = number_s[fnd[step]]; // FND on
			PORTC = fnd_pos[step];
		}
		else // time_stop == 0 -> 일반 모드 동작..
		{
			PORTD = number_s[fnd[step]]; // FND on
			PORTC = fnd_pos[step]; 
		}
		break;
		//
		case 1:
		if (time_stop == 1 && flag == 1) 
		{
			PORTD = number_s[10]; 
		}
		else if (time_stop == 2 && flag == 1) 
		{
			PORTD = number_s[fnd[step]];
			PORTC = fnd_pos[step]; 
		}
		else if (time_stop == 3 && flag == 1)
		{
			PORTD = number_s[fnd[step]]; 
			PORTC = fnd_pos[step];
		}
		else 
		{
			PORTD = number_s[fnd[step]];
			PORTC = fnd_pos[step]; 
		}
		break;
		//
		case 2:
		if (time_stop == 1 && flag == 1) 
		{
			PORTA = number_m[fnd[step]];
			PORTC = fnd_pos[step];
		}
		else if (time_stop == 2 && flag == 1) 
		{
			PORTA = number_m[10]; 
		}
		else if (time_stop == 3 && flag == 1)
		{
			PORTA = number_m[fnd[step]];
			PORTC = fnd_pos[step];
		}
		else  
		{
			PORTA = number_m[fnd[step]];
			PORTC = fnd_pos[step];
		}
		break;
		//
		case 3:
		if (time_stop == 1 && flag == 1) 
		{
			PORTA = number_m[fnd[step]];
			PORTC = fnd_pos[step];
		}
		else if (time_stop == 2 && flag == 1) 
		{
			PORTA = number_m[10];
		}
		else if (time_stop == 3 && flag == 1)
		{
			PORTA = number_m[fnd[step]];
			PORTC = fnd_pos[step];
		}
		else 
		{
			PORTA = number_m[fnd[step]];
			PORTC = fnd_pos[step];
		}
		break;
		//
		case 4:
		if (time_stop == 1 && flag == 1)
		{
			PORTA = number_m[fnd[step]];
			PORTC = fnd_pos[step];
		}
		else if (time_stop == 2 && flag == 1) 
		{
			PORTA = number_m[fnd[step]];
			PORTC = fnd_pos[step];
		}
		else if (time_stop == 3 && flag == 1)
		{
			PORTA = number_m[10];
		}
		else 
		{
			PORTA = number_m[fnd[step]];
			PORTC = fnd_pos[step];
		}
		break;
		//
		case 5:
		if (time_stop == 1 && flag == 1) 
		{
			PORTA = number_m[fnd[step]];
			PORTC = fnd_pos[step];
		}
		else if (time_stop == 2 && flag == 1) 
		{
			PORTA = number_m[fnd[step]];
			PORTC = fnd_pos[step];
		}
		else if (time_stop == 3 && flag == 1)
		{
			PORTA = number_m[10];
		}
		else
		{
			PORTA = number_m[fnd[step]];
			PORTC = fnd_pos[step];
		}
		break;
	}
	
	fnd[0] = time_sec%10; // 0단위 sec
	fnd[1] = (time_sec/10)%6; // 10단위 sec
	fnd[2] = (time_sec/60)%10;  // 0단위 min
	fnd[3] = (time_sec/600)%6; // 10단위 min
	fnd[4] = (time_sec/3600)%10; // 0단위 hour
	fnd[5] = (time_sec/36000)%10; // 10단위 hour
	
}

ISR(TIMER2_OVF_vect) //1ms 인터럽트 발생
{
	TCNT2 = 256 - 250; 
	
	
	if (++sw_time >= 50) //50ms...
	{
		sw_time = 0; //sw reset
		state = (PINF & ((1<<PF0) | (1<<PF1) | (1<<PF2) | (1<<PF3) | (1<<PF4) | (1<<PF5))); // PF(0,1,2,3,4,5)_sw

		if(pre_state != state)
		{
			if (state == 0x01) //(state == (PINF & (1<<PF0)))
			{
				if (time_stop == 1) // 2FND blink mode
				{
					time_sec += 1;
				}
			}
			else if (state == 0x02) //(state == (PINF & (1<<PF2)))
			{
				if (time_stop == 1)
				{
					time_sec += 10;
				}
			}
			else if (state == 0x04) //(state == (PINF & (1<<PF2)))
			{
				if (time_stop == 2)  //4FND blink mode(ooxx) <- 뒤에 두개
				{
					time_sec += 60;
				}
			}
			else if (state == 0x08) //(state == (PINF & (1<<PF3)))
			{
				if (time_stop == 2)
				{
					time_sec += 600;
				}
			}
			else if (state == 0x10) //(state == (PINF & (1<<PF4)))
			{
				if (time_stop == 3) //4FND blink mode(xxoo) <- 앞에 두개
				{
					time_sec += 3600;
				}
			}
			else if (state == 0x20) //(state == (PINF & (1<<PF5)))
			{
				time_stop++;
				time_stop %= 4; //time_stop -> 0,1,2,3 change
			}
		}
		pre_state = state;
	}
	
	if (time_stop == 0)
	{
		if (++count2 >= 1000) //1s time_sec++ -> 1초 증가
		{
			count2 = 0;
			time_sec++;
		}
	}
	else if (time_stop == 1 || time_stop == 2 || time_stop == 3)
	{
		if (++times >= 500) //0.5s 깜빡임 
		{
			times = 0; // 500ms count reset

			if (flag > 0)
			{
				flag = 0; // OFF
			}
			else
			{
				flag = 1; // on
			}
		}
	}
}

void timer0_init() //100us 구현
{
	TCNT0 = 256 - 206; // TCNT0값 50 설정
	TCCR0 = (0<<CS02) | (1<<CS01) | (0<<CS00); //분주비 8 설정
	TIMSK = (1<<TOIE0); // 오버플로 인터럽트 허용
}

void timer1_init()
{
	TCCR1A = (0<<COM1A1) | (1<<COM1A0) | (0<<WGM11) | (0<<WGM10);
	TCCR1B = (0<<WGM13) | (1<<WGM12) | (1<<CS12) | (0<<CS11) | (1<<CS10); //분주비 1024 CTC모드 OC1A값 토글
	TIMSK = (1<<OCIE1A);
}

void timer2_init() //1ms 구현
{
	TCNT2 = 256 - 250; // TNCT2값 6설정
	TCCR2 = (0<<CS22) | (1<<CS21) | (1<<CS20); //분주비 64
	TIMSK |= (1<<TOIE2);
}

void delay(uint16_t ms) //delay(1ms)
{
	count2 = 0;
	while(count2 <= ms); 
}
