/**
 * 头文件集合
 */
#ifndef __SYSTEM_H
#define __SYSTEM_H

/*----------标准库----------*/
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*----------驱动库----------*/
#include "driverlib/adc.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/tm4c123gh6pm.h"
#include "utils/uartstdio.h"

/*----------外设库----------*/
#include "ADC/ADC.h"
#include "CHIP/DAC081S101/DAC081S101.h"
#include "CHIP/DAC7811/DAC7811.h"
#include "IIC/myIIC.h"
#include "KEY/KEY.h"
#include "OLED/oled.h"
#include "PWM/myPWM.h"
#include "SPI/mySPI.h"
#include "TFT/TFT18.h"
#include "UART/myUART.h"

/* 系统定时器初始化 */
void SysTickInit(void);

/* 获取系统时间(ms) */
uint32_t GetSysTime_ms(void);

/* 获取系统时间(us) */
uint32_t GetSysTime_us(void);

/* 毫秒级延时 */
void Delay_ms(uint32_t ms);

/* 微秒级延时 */
void Delay_us(uint32_t us);

#endif
