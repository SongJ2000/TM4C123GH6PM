/*
 * 按键
 * KEY1 PF4
 * KEY2 PF0
 */

#ifndef HARDWARE_KEY_H_
#define HARDWARE_KEY_H_

#include "system.h"

void Key_Init(void);
uint32_t Key1(void);  //按键检测
uint32_t Key2(void);
uint32_t Key3(void);  //按键检测
uint32_t Key4(void);

#endif /* HARDWARE_KEY_H_ */
