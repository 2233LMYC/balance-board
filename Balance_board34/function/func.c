#include "func.h"
#include "mpu6050.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"

const uint8_t str1[] = "A7:00001";//左低
const uint8_t str2[]  = "A7:00002";//右低
const uint8_t str3[]  = "A7:00003";//右极低
const uint8_t str4[]  = "A7:00004";//左极低
const uint8_t str5[]  = "A7:00005";//平衡
const uint8_t str6[]  = "A7:00006";//摇呀摇
const uint8_t str7[]  = "A7:00007";//初始化
const uint8_t str8[]  = "A7:00008";//初始化成功
const uint8_t str9[]  = "A7:00009";//初始化失败
const uint8_t str10[] = "A7:00010";//重启设备
const uint8_t str11[] = "A7:00011";//语音模式
const uint8_t str12[] = "A7:00012";//游戏模式

uint8_t busy;
uint8_t cnt = 0;
uint8_t board_state_old = 0;
uint8_t stop = 0;
extern float pitch, roll, yaw;

void Voice_mode(int i)
{
	switch(i)
	{
		case 1:HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen((const char*)str1),1000);
		break;
		case 2:HAL_UART_Transmit(&huart2,(uint8_t*)str2,strlen((const char*)str2),1000);
		break;
		case 3:HAL_UART_Transmit(&huart2,(uint8_t*)str3,strlen((const char*)str3),1000);
		break;
		case 4:HAL_UART_Transmit(&huart2,(uint8_t*)str4,strlen((const char*)str4),1000);
		break;
		case 5:HAL_UART_Transmit(&huart2,(uint8_t*)str5,strlen((const char*)str5),1000);
		break;
		case 6:HAL_UART_Transmit(&huart2,(uint8_t*)str6,strlen((const char*)str6),1000);
		break;
		case 7:HAL_UART_Transmit(&huart2,(uint8_t*)str7,strlen((const char*)str7),1000);
		break;
		case 8:HAL_UART_Transmit(&huart2,(uint8_t*)str8,strlen((const char*)str8),1000);
		break;
		case 9:HAL_UART_Transmit(&huart2,(uint8_t*)str9,strlen((const char*)str9),1000);
		break;
		case 10:HAL_UART_Transmit(&huart2,(uint8_t*)str10,strlen((const char*)str10),1000);
		break;
		case 11:HAL_UART_Transmit(&huart2,(uint8_t*)str11,strlen((const char*)str11),1000);
		break;
		case 12:HAL_UART_Transmit(&huart2,(uint8_t*)str12,strlen((const char*)str12),1000);
		break;

	}
}

void YUYIN_func(void)
{
	uint8_t board_state = 111;
	
	//printf("%d\r\n",(int)mpu_Angle.pitch);
	
	if((pitch<10)&&(pitch>5))
			board_state = 3;	//右边低
	
	else if(pitch>10)
			board_state = 4;	//右边极低

	else if((pitch<(-5))&&(pitch>(-10)))
			board_state = 1;  //左边低

	else if(pitch<(-10))
			board_state = 2;	//左边极低

	else if((pitch<4)&&(pitch>(-4)))//平衡
			board_state = 5;	

	
	if(board_state == board_state_old)
	{
		stop = 1;
	}
	else
	{
		board_state_old = board_state;
		stop = 0;
	}
	
	
	if(stop == 0)
	{
		switch(board_state)
		{
			case 1:Voice_mode(2);
				break;
			case 2:Voice_mode(3);
				break;
			case 3:Voice_mode(1);
				break;
			case 4:Voice_mode(4);
				break;
			case 5:Voice_mode(5);
				break;
			case 6:Voice_mode(11);
				break;
			case 7:Voice_mode(6);
				break;
			default :break;
		}
	}
}



