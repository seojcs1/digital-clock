#include "main.h"

////두개짜리 fnd는 제어에 1 세그먼트에 0을 주면 불이 들어옴 -> 공통 음극
////네개짜리 fnd는 제어에 0 세그먼트에 1을 주면 불이 들어옴 -> 공통 양극
////PA0~PA7까지 세그먼트 연결, PC0~3까지 세그먼트 위치 연결
////PD0~PD7까지 세그먼트 연결, PC4,5까지 세그먼트 위치 연결

int main(void)
{
	timer0_init();
	timer1_init();
	timer2_init();
	//포트B OC1핀 출력 설정
	DDRB |= (1<<PB5); 
	//포트F sw입력 설정
	DDRF = 0x00; 
	
	//포트C FND 제어 위치설정
	DDRC = 0xFF; 
	
	//포트A,D FND 세그먼트 설정
	DDRA = 0xFF; 
	DDRD = 0XFF; 
	
	sei();
	
	while (1)
	{
		;
	}
}
