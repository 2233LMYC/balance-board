
#include "AT.h"
#include "string.h"
#include "usart.h"
#include "stdio.h"



#if Server

uint8_t Str1[] = {"AT\r\n",};//AT测试
uint8_t Str2[] = {"AT+CWMODE=3\r\n"};//设置模式为混合模式
uint8_t Str3[] = {"AT+CWSAP=\"ESP8266\",\"123456789\",5,3\r\n"};//建立WiFi信息，ip:192.168.4.1
uint8_t Str4[] = {"AT+CIPMUX=1\r\n"};//建立多连接
uint8_t Str5[] = {"AT+CIPSERVER=1,6666\r\n"};//建立服务器，端口6666

int ESP8266_Server_Init(void)
{
	static uint8_t Step = 0;
	switch(Step)
	{
		case 0: HAL_UART_Transmit(&huart2,Str1,sizeof(Str1),1000);
						HAL_Delay(1000);
						if(strstr((const char*)USART2_REC_Buff,(const char*)"OK") != NULL)
						{
							memset(USART2_REC_Buff,0,strlen((const char*)USART2_REC_Buff));
							Step = 1;
							printf("AT OK!\r\n");
						}
						else 
						{
							Step = 0;
							printf("AT ERROR!\r\n");
							return -1;
						}
						break;
		case 1: HAL_UART_Transmit(&huart2,Str2,sizeof(Str2),1000);
						HAL_Delay(1000);
						if(strstr((const char*)USART2_REC_Buff,(const char*)"OK") != NULL)
						{
							memset(USART2_REC_Buff,0,strlen((const char*)USART2_REC_Buff));
							Step = 2;
							printf("CWMODE OK!\r\n");
						}
						else 
						{
							Step = 1;
							printf("CWMODE ERROR!\r\n");
							return -2;
						}
						break;
		case 2: HAL_UART_Transmit(&huart2,Str3,sizeof(Str3),1000);
						HAL_Delay(1000);
						if(strstr((const char*)USART2_REC_Buff,(const char*)"OK") != NULL)
						{
							memset(USART2_REC_Buff,0,strlen((const char*)USART2_REC_Buff));
							Step = 3;
							printf("WIFI CREATE OK!\r\n");
						}
						else 
						{
							Step = 2;
							printf("WIFI CREATING...\r\n");
							return -3;
						}
						break;
		case 3: HAL_UART_Transmit(&huart2,Str4,sizeof(Str4),1000);
						HAL_Delay(1000);
						if(strstr((const char*)USART2_REC_Buff,(const char*)"OK") != NULL)
						{
							memset(USART2_REC_Buff,0,strlen((const char*)USART2_REC_Buff));
							Step = 4;
							printf("MORE CONNECT OK!\r\n");
						}
						else 
						{
							Step = 3;
							printf("MORE CONNECT ERROR!\r\n");
							return -4;
						}
						break;
		case 4: HAL_UART_Transmit(&huart2,Str5,sizeof(Str5),1000);
						HAL_Delay(1000);
						if(strstr((const char*)USART2_REC_Buff,(const char*)"OK") != NULL)
						{
							memset(USART2_REC_Buff,0,strlen((const char*)USART2_REC_Buff));
							Step = 5;
							printf("SERVER CREATE OK!\r\n");
							return 0;
						}
						else 
						{
							Step = 4;
							printf("SERVER CREATE ERROR!\r\n");
							return -5;
						}
	}	
	return 1;
}

#elif	Client

uint8_t Str1[] = {"AT\r\n",};//AT测试
uint8_t Str2[] = {"AT+CWMODE=1\r\n"};//设置模式为station
uint8_t Str3[] = {"AT+CWJAP=\"ESP8266ttt\",\"123456789\"\r\n"};//连接WIFI
uint8_t Str4[] = {"AT+CIPSTART=\"TCP\",\"192.168.4.1\",2233\r\n"};//连接服务器IP
uint8_t Str5[] = {"AT+CIPMODE=1\r\n"};//开启透传
uint8_t Str6[] = {"AT+CIPSEND\r\n"};//发送数据，开始透传


int ESP8266_Client_Init(void)
{
	static uint8_t Step = 0;
	switch(Step)
	{
		case 0: HAL_UART_Transmit(&huart2,Str1,sizeof(Str1),1000);
						HAL_Delay(1000);
						if(strstr((const char*)USART2_REC_Buff,(const char*)"OK") != NULL)
						{
							memset(USART2_REC_Buff,0,strlen((const char*)USART2_REC_Buff));
							Step = 1;
							printf("AT OK!\r\n");
						}
						else 
						{
							Step = 0;
							printf("AT ERROR!\r\n");
							return -1;
						}
						break;
		case 1: HAL_UART_Transmit(&huart2,Str2,sizeof(Str2),1000);
						HAL_Delay(1000);
						if(strstr((const char*)USART2_REC_Buff,(const char*)"OK") != NULL)
						{
							memset(USART2_REC_Buff,0,strlen((const char*)USART2_REC_Buff));
							Step = 2;
							printf("MODE STATION OK!\r\n");
						}
						else 
						{
							Step = 1;
							printf("MODE STATION ERROR!\r\n");
							return -2;
						}
						break;
		case 2: HAL_UART_Transmit(&huart2,Str3,sizeof(Str3),1000);
						HAL_Delay(1000);
						if(strstr((const char*)USART2_REC_Buff,(const char*)"OK") != NULL)
						{
							memset(USART2_REC_Buff,0,strlen((const char*)USART2_REC_Buff));
							Step = 3;
							printf("WIFI CONNECTED OK!\r\n");
						}
						else 
						{
							Step = 2;
							printf("CONNECTING...\r\n");
							return -3;
						}
						break;
		case 3: HAL_UART_Transmit(&huart2,Str4,sizeof(Str4),1000);
						HAL_Delay(1000);
						if(strstr((const char*)USART2_REC_Buff,(const char*)"OK") != NULL)
						{
							memset(USART2_REC_Buff,0,strlen((const char*)USART2_REC_Buff));
							Step = 4;
							printf("IP CONNECTED OK!\r\n");
						}
						else 
						{
							Step = 3;
							printf("IP CONNECTED ERROR!\r\n");
							return -4;
						}
						break;
		case 4: HAL_UART_Transmit(&huart2,Str5,sizeof(Str5),1000);
						HAL_Delay(1000);
						if(strstr((const char*)USART2_REC_Buff,(const char*)"OK") != NULL)
						{
							memset(USART2_REC_Buff,0,strlen((const char*)USART2_REC_Buff));
							Step = 5;
							printf("CIPMODE OK!\r\n");
						}
						else 
						{
							Step = 4;
							printf("CIPMODE ERROR!\r\n");
							return -5;
						}
						break;
	 case 5: HAL_UART_Transmit(&huart2,Str6,sizeof(Str6),1000);
					 HAL_Delay(1000);
					 memset(USART2_REC_Buff,0,strlen((const char*)USART2_REC_Buff));
					 printf("CIPSEND OK!\r\n");
					 printf("ESP8266 Client Init OK!\r\n");
					 return 0;
					 
	}	
	return 1;
}

#endif

