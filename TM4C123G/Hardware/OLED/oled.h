#ifndef __OLED_H
#define __OLED_H

#include "system.h"

#define Protocal_IIC 0
#define Protocal_SPI 1

#define OLED_CS_Port GPIO_PORTD_BASE
#define OLED_CS_Pin GPIO_PIN_0

#define OLED_DC_Port GPIO_PORTD_BASE
#define OLED_DC_Pin GPIO_PIN_1

#define OLED_RST_Port GPIO_PORTD_BASE
#define OLED_RST_Pin GPIO_PIN_2

void OLED_GPIOInit(void);
void OLED_setCS(bool state);
void OLED_setDC(bool state);
void OLED_setRST(bool state);

#define OLED_MODE 0
#define SIZE 8
#define XLevelL 0x00
#define XLevelH 0x10
#define Max_Column 128
#define Max_Row 64
#define Brightness 0xFF
#define X_WIDTH 128
#define Y_WIDTH 64

//------------------可修改------------------
#define SSD1306_I2C_ADDRESS 0x3c  // oled地址
#define HARD_SSD1306_I2C hi2c1    // hard_i2c对象

#define OLED_CMD 0   //写命令
#define OLED_DATA 1  //写数据

// OLED控制用函数
void OLED_WR_Byte(unsigned char dat, unsigned char cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(unsigned char x, unsigned char y, unsigned char t);
void OLED_Fill(unsigned char x1, unsigned char y1, unsigned char x2,
               unsigned char y2, unsigned char dot);
void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char chr,
                   unsigned char Char_Size);
void OLED_ShowNum(unsigned char x, unsigned char y, unsigned int num,
                  unsigned char len, unsigned char size);
void OLED_ShowString(unsigned char x, unsigned char y, char *p,
                     unsigned char Char_Size);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(unsigned char x, unsigned char y, unsigned char no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1,
                  unsigned char y1, unsigned char BMP[]);
void fill_picture(unsigned char *fill_Data);
void Picture(void);
void IIC_Start(void);
void IIC_Stop(void);

void OLED_Write_IIC_Command(unsigned char IIC_Command);
void OLED_Write_IIC_Data(unsigned char IIC_Data);

void IIC_Wait_Ack(void);

#endif
