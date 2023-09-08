#ifndef _FUNC_H_
#define _FUNC_H_

#include "main.h"

extern const uint8_t str1[];//左低
extern const uint8_t str2[];//右低
extern const uint8_t str3[];//右极低
extern const uint8_t str4[];//左极低
extern const uint8_t str5[];//平衡
extern const uint8_t str6[];//摇呀摇
extern const uint8_t str7[];//初始化
extern const uint8_t str8[];//初始化成功
extern const uint8_t str9[];//初始化失败
extern const uint8_t str10[];//重启设备

extern uint8_t busy;
extern uint8_t cnt;
extern uint8_t board_state_old;
extern uint8_t stop;


void Voice_mode(int i);
void YUYIN_func(void);




#endif
