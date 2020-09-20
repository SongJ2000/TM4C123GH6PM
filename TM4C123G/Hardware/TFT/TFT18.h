/**
 * @Name    TFT18.h
 * @Brief   1.8寸 TFT液晶库函数
 * @Author  ILCE姜崧
 * @Date    2020/7/18
 * @note		1.8 TFT液晶接线定义：
 *          ----------------------------------------------------
 *          模拟SIP：
 *            模块管脚            单片机管脚
 *            SCL                 B0
 *            SDA                 B1
 *            RES                 E4
 *            DC                  E5
 *            CS                  B4
 *            BL                  A5
 *          硬件SPI：
 *            SCL                 A2
 *            SDA                 A5
 *            RES                 D6
 *            DC                  D7
 *            CS                  A3
 *
 *           电源引脚
 *            BL  3.3V电源（背光控制引脚，也可以接PWM来控制亮度）
 *            VCC 3.3V电源
 *            GND 电源地
 *          --------------------------------------------------
 */

#ifndef __TFT18_H
#define __TFT18_H

#include "system.h"

#define USING_HARDWARE_SPI

#define TFT_X_MAX 128  //液晶X方宽度
#define TFT_Y_MAX 160  //液晶Y方宽度

////-------常用颜色----------
//#define RED     	0XF800    //红色
//#define GREEN   	0X07E0    //绿色
//#define BLUE    	0X001F    //蓝色
//#define BRED    	0XF81F
//#define GRED    	0XFFE0    //灰色
//#define GBLUE   	0X07FF    //
//#define BLACK   	0X0000    //黑色
//#define WHITE   	0XFFFF    //白色
//#define YELLOW  	0xFFE0    //黄色

//定义写字笔的颜色
#define PENCOLOR RED

//定义背景颜色
#define BGCOLOR WHITE

/* 不使用硬件SPI */
#ifndef USING_HARDWARE_SPI

#define SCL_GPIO_PERIPH SYSCTL_PERIPH_GPIOB
#define SDA_GPIO_PERIPH SYSCTL_PERIPH_GPIOB
#define RES_GPIO_PERIPH SYSCTL_PERIPH_GPIOE
#define DC_GPIO_PERIPH SYSCTL_PERIPH_GPIOE
#define CS_GPIO_PERIPH SYSCTL_PERIPH_GPIOB
#define BL_GPIO_PERIPH SYSCTL_PERIPH_GPIOA

#define SCL_GPIO_PIN GPIO_PIN_0  //时钟
#define SDA_GPIO_PIN GPIO_PIN_1  //数据
#define RES_GPIO_PIN GPIO_PIN_4  //复位
#define DC_GPIO_PIN GPIO_PIN_5   //命令
#define CS_GPIO_PIN GPIO_PIN_4   //片选
#define BL_GPIO_PIN GPIO_PIN_5   //背光

#define SCL_GPIO_BASE GPIO_PORTB_BASE  //时钟
#define SDA_GPIO_BASE GPIO_PORTB_BASE  //数据
#define RES_GPIO_BASE GPIO_PORTE_BASE  //复位
#define DC_GPIO_BASE GPIO_PORTE_BASE   //命令
#define CS_GPIO_BASE GPIO_PORTB_BASE   //片选
#define BL_GPIO_BASE GPIO_PORTA_BASE   //背光

#define TFT_SCL(x)                                             \
  {                                                            \
    if (x)                                                     \
      GPIOPinWrite(SCL_GPIO_BASE, SCL_GPIO_PIN, SCL_GPIO_PIN); \
    else                                                       \
      GPIOPinWrite(SCL_GPIO_BASE, SCL_GPIO_PIN, 0);            \
  }
#define TFT_SDA(x)                                             \
  {                                                            \
    if (x)                                                     \
      GPIOPinWrite(SDA_GPIO_BASE, SDA_GPIO_PIN, SDA_GPIO_PIN); \
    else                                                       \
      GPIOPinWrite(SDA_GPIO_BASE, SDA_GPIO_PIN, 0);            \
  }
#define TFT_RES(x)                                             \
  {                                                            \
    if (x)                                                     \
      GPIOPinWrite(RES_GPIO_BASE, RES_GPIO_PIN, RES_GPIO_PIN); \
    else                                                       \
      GPIOPinWrite(RES_GPIO_BASE, RES_GPIO_PIN, 0);            \
  }
#define TFT_DC(x)                                           \
  {                                                         \
    if (x)                                                  \
      GPIOPinWrite(DC_GPIO_BASE, DC_GPIO_PIN, DC_GPIO_PIN); \
    else                                                    \
      GPIOPinWrite(DC_GPIO_BASE, DC_GPIO_PIN, 0);           \
  }
#define TFT_CS(x)                                           \
  {                                                         \
    if (x)                                                  \
      GPIOPinWrite(CS_GPIO_BASE, CS_GPIO_PIN, CS_GPIO_PIN); \
    else                                                    \
      GPIOPinWrite(CS_GPIO_BASE, CS_GPIO_PIN, 0);           \
  }
#define TFT_BL(x)                                           \
  {                                                         \
    if (x)                                                  \
      GPIOPinWrite(BL_GPIO_BASE, BL_GPIO_PIN, BL_GPIO_PIN); \
    else                                                    \
      GPIOPinWrite(CS_GPIO_BASE, BL_GPIO_PIN, 0);           \
  }

/* 使用硬件SPI */
#else

#define RES_GPIO_PERIPH SYSCTL_PERIPH_GPIOD
#define DC_GPIO_PERIPH SYSCTL_PERIPH_GPIOD

#define RES_GPIO_PIN GPIO_PIN_6  //复位
#define DC_GPIO_PIN GPIO_PIN_7   //命令

#define RES_GPIO_BASE GPIO_PORTD_BASE  //复位
#define DC_GPIO_BASE GPIO_PORTD_BASE   //命令

#define TFT_RES(x)                                             \
  {                                                            \
    if (x)                                                     \
      GPIOPinWrite(RES_GPIO_BASE, RES_GPIO_PIN, RES_GPIO_PIN); \
    else                                                       \
      GPIOPinWrite(RES_GPIO_BASE, RES_GPIO_PIN, 0);            \
  }
#define TFT_DC(x)                                           \
  {                                                         \
    if (x)                                                  \
      GPIOPinWrite(DC_GPIO_BASE, DC_GPIO_PIN, DC_GPIO_PIN); \
    else                                                    \
      GPIOPinWrite(DC_GPIO_BASE, DC_GPIO_PIN, 0);           \
  }

#endif

extern void LCDInit(void);
extern void LCDClear(int color);
extern void LCDShowChar(uint16_t x, uint16_t y, uint8_t dat);
extern void LCDShowString(uint16_t x, uint16_t y, uint8_t* dat);
extern void LCDShowInt8(uint16_t x, uint16_t y, char dat);
extern void LCDShowInt16(uint16_t x, uint16_t y, int dat);
extern void LCDShowImage(const unsigned char* p);

#endif
