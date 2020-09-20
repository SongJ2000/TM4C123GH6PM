#include "TFT/TFT18.h"

#include "TFT/TFT18_Font.h"

static void tft_delay(long t) {
  while (t--)
    ;
}

#ifndef USING_HARDWARE_SPI

static void Lcd_WriteIndex(uint8_t dat) {
  uint8_t i = 8;
  TFT_CS(0);
  TFT_DC(0);
  TFT_SCL(0);
  while (i--) {
    if (dat & 0x80) {
      TFT_SDA(1);
    } else {
      TFT_SDA(0);
    }
    TFT_SCL(1);

    TFT_SCL(0);
    dat <<= 1;
  }
  TFT_CS(1);
}

static void Lcd_WriteData(uint8_t dat)  //写数据
{
  uint8_t i = 8;

  TFT_CS(0);
  TFT_DC(1);
  TFT_SCL(0);
  while (i--) {
    if (dat & 0x80) {
      TFT_SDA(1);
    } else {
      TFT_SDA(0);
    }
    TFT_SCL(1);
    TFT_SCL(0);
    dat <<= 1;
  }
  TFT_CS(1);
}

static void LCD_WriteData_16Bit(uint16_t dat)  //向液晶屏写一个16位数据
{
  uint8_t i = 16;
  TFT_CS(0);
  TFT_DC(1);
  TFT_SCL(0);
  while (i--) {
    if (dat & 0x8000) {
      TFT_SDA(1);
    } else {
      TFT_SDA(0);
    }
    TFT_SCL(1);
    TFT_SCL(0);
    dat <<= 1;
  }
  TFT_CS(1);
}

#else

static void Lcd_WriteIndex(uint8_t dat)  //写命令
{
  TFT_DC(0);
  SSIDataPut(SSI0_BASE, dat);
  while (SSIBusy(SSI0_BASE))
    ;
}

static void Lcd_WriteData(uint8_t dat)  //写数据
{
  TFT_DC(1);
  SSIDataPut(SSI0_BASE, dat);
  while (SSIBusy(SSI0_BASE))
    ;
}

static void LCD_WriteData_16Bit(uint16_t dat)  //向液晶屏写一个16位数据
{
  uint8_t h, l;
  h = dat >> 8;
  l = (uint8_t)dat;

  TFT_DC(1);
  SSIDataPut(SSI0_BASE, h);
  while (SSIBusy(SSI0_BASE))
    ;
  SSIDataPut(SSI0_BASE, l);
  while (SSIBusy(SSI0_BASE))
    ;
}

#endif

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶坐标设置
//  @param      x_start     	坐标x方向的起点
//  @param      y_start     	坐标y方向的起点
//  @param      x_end       	坐标x方向的终点
//  @param      y_end       	坐标y方向的终点
//  @return     void
//  @since      v1.0
//  Sample usage:               Lcd_SetRegion(0,0,10,10);//
//  x、y的起点都是0，终点都是10
//-------------------------------------------------------------------------------------------------------------------
void Lcd_SetRegion(unsigned int x_start, unsigned int y_start,
                   unsigned int x_end, unsigned int y_end) {
  Lcd_WriteIndex(0x2a);
  Lcd_WriteData(0x02);
  Lcd_WriteData(x_start + 2);
  Lcd_WriteData(0x02);
  Lcd_WriteData(x_end + 2);
  Lcd_WriteIndex(0x2b);
  Lcd_WriteData(0x01);
  Lcd_WriteData(y_start + 1);
  Lcd_WriteData(0x01);
  Lcd_WriteData(y_end + 1);
  Lcd_WriteIndex(0x2c);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶清屏函数
//  @param      color     		颜色设置
//  @return     void
//  @since      v1.0
//  Sample usage:      LCDClear(YELLOW);// 全屏设置为黄色
//-------------------------------------------------------------------------------------------------------------------
void LCDClear(int color) {
  uint8_t i, j;
  Lcd_SetRegion(0, 0, TFT_X_MAX - 1, TFT_Y_MAX - 1);
  for (i = 0; i < TFT_Y_MAX; i++)
    for (j = 0; j < TFT_X_MAX; j++) LCD_WriteData_16Bit(color);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶初始化
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void LCDInit(void) {
#ifndef USING_HARDWARE_SPI
  SysCtlPeripheralEnable(SCL_GPIO_PERIPH);
  GPIOPinTypeGPIOOutput(SCL_GPIO_BASE, SCL_GPIO_PIN);

  SysCtlPeripheralEnable(SDA_GPIO_PERIPH);
  GPIOPinTypeGPIOOutput(SDA_GPIO_BASE, SDA_GPIO_PIN);

  SysCtlPeripheralEnable(RES_GPIO_PERIPH);
  GPIOPinTypeGPIOOutput(RES_GPIO_BASE, RES_GPIO_PIN);

  SysCtlPeripheralEnable(DC_GPIO_PERIPH);
  GPIOPinTypeGPIOOutput(DC_GPIO_BASE, DC_GPIO_PIN);

  SysCtlPeripheralEnable(CS_GPIO_PERIPH);
  GPIOPinTypeGPIOOutput(CS_GPIO_BASE, CS_GPIO_PIN);

  SysCtlPeripheralEnable(BL_GPIO_PERIPH);
  GPIOPinTypeGPIOOutput(BL_GPIO_BASE, BL_GPIO_PIN);
#else
  SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  GPIOPinConfigure(GPIO_PA2_SSI0CLK);
  GPIOPinConfigure(GPIO_PA3_SSI0FSS);
  GPIOPinConfigure(GPIO_PA4_SSI0RX);
  GPIOPinConfigure(GPIO_PA5_SSI0TX);
  GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_2);
  SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,
                     SSI_MODE_MASTER, 40 * 1000 * 1000, 8);
  SSIEnable(SSI0_BASE);  //使能

  SysCtlPeripheralEnable(RES_GPIO_PERIPH);
  GPIOPinTypeGPIOOutput(RES_GPIO_BASE, RES_GPIO_PIN);
  /* Unlock PD7 */
  HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
  HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= GPIO_PIN_7;
  HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0x00;
  SysCtlPeripheralEnable(DC_GPIO_PERIPH);
  GPIOPinTypeGPIOOutput(DC_GPIO_BASE, DC_GPIO_PIN);
//  GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0);
#endif

  TFT_DC(1);
  tft_delay(999999);
  TFT_DC(0);
  tft_delay(999999);
  TFT_DC(1);
  tft_delay(999999);
  TFT_DC(0);
  TFT_RES(0);
  tft_delay(999999);
  TFT_RES(1);
  tft_delay(999999);
  Lcd_WriteIndex(0x11);
  tft_delay(999999);
  Lcd_WriteIndex(0xB1);
  Lcd_WriteData(0x01);
  Lcd_WriteData(0x2C);
  Lcd_WriteData(0x2D);
  Lcd_WriteIndex(0xB2);
  Lcd_WriteData(0x01);
  Lcd_WriteData(0x2C);
  Lcd_WriteData(0x2D);
  Lcd_WriteIndex(0xB3);
  Lcd_WriteData(0x01);
  Lcd_WriteData(0x2C);
  Lcd_WriteData(0x2D);
  Lcd_WriteData(0x01);
  Lcd_WriteData(0x2C);
  Lcd_WriteData(0x2D);
  Lcd_WriteIndex(0xB4);
  Lcd_WriteData(0x07);
  Lcd_WriteIndex(0xC0);
  Lcd_WriteData(0xA2);
  Lcd_WriteData(0x02);
  Lcd_WriteData(0x84);
  Lcd_WriteIndex(0xC1);
  Lcd_WriteData(0xC5);
  Lcd_WriteIndex(0xC2);
  Lcd_WriteData(0x0A);
  Lcd_WriteData(0x00);
  Lcd_WriteIndex(0xC3);
  Lcd_WriteData(0x8A);
  Lcd_WriteData(0x2A);
  Lcd_WriteIndex(0xC4);
  Lcd_WriteData(0x8A);
  Lcd_WriteData(0xEE);
  Lcd_WriteIndex(0xC5);
  Lcd_WriteData(0x0E);
  Lcd_WriteIndex(0x36);
  Lcd_WriteData(0xC0);
  Lcd_WriteData(0xC8);
  Lcd_WriteIndex(0xe0);
  Lcd_WriteData(0x0f);
  Lcd_WriteData(0x1a);
  Lcd_WriteData(0x0f);
  Lcd_WriteData(0x18);
  Lcd_WriteData(0x2f);
  Lcd_WriteData(0x28);
  Lcd_WriteData(0x20);
  Lcd_WriteData(0x22);
  Lcd_WriteData(0x1f);
  Lcd_WriteData(0x1b);
  Lcd_WriteData(0x23);
  Lcd_WriteData(0x37);
  Lcd_WriteData(0x00);
  Lcd_WriteData(0x07);
  Lcd_WriteData(0x02);
  Lcd_WriteData(0x10);
  Lcd_WriteIndex(0xe1);
  Lcd_WriteData(0x0f);
  Lcd_WriteData(0x1b);
  Lcd_WriteData(0x0f);
  Lcd_WriteData(0x17);
  Lcd_WriteData(0x33);
  Lcd_WriteData(0x2c);
  Lcd_WriteData(0x29);
  Lcd_WriteData(0x2e);
  Lcd_WriteData(0x30);
  Lcd_WriteData(0x30);
  Lcd_WriteData(0x39);
  Lcd_WriteData(0x3f);
  Lcd_WriteData(0x00);
  Lcd_WriteData(0x07);
  Lcd_WriteData(0x03);
  Lcd_WriteData(0x10);
  Lcd_WriteIndex(0x2a);
  Lcd_WriteData(0x00);
  Lcd_WriteData(0x00 + 2);
  Lcd_WriteData(0x00);
  Lcd_WriteData(0x80 + 2);
  Lcd_WriteIndex(0x2b);
  Lcd_WriteData(0x00);
  Lcd_WriteData(0x00 + 3);
  Lcd_WriteData(0x00);
  Lcd_WriteData(0x80 + 3);
  Lcd_WriteIndex(0xF0);
  Lcd_WriteData(0x01);
  Lcd_WriteIndex(0xF6);
  Lcd_WriteData(0x00);
  Lcd_WriteIndex(0x3A);
  Lcd_WriteData(0x05);
  Lcd_WriteIndex(0x29);
  LCDClear(0xFFFF);  //全白
}

void LCDShowChar(uint16_t x, uint16_t y, uint8_t dat) {
  uint8_t i, j;
  uint8_t temp;
  for (i = 0; i < 16; i++) {
    Lcd_SetRegion(x, y + i, x + 7, y + i);
    temp = tft_ascii[dat - 32]
                    [i];  //减32因为是取模是从空格开始取得 空格在ascii中序号是32
    for (j = 0; j < 8; j++) {
      if (temp & 0x01)
        LCD_WriteData_16Bit(PENCOLOR);
      else
        LCD_WriteData_16Bit(BGCOLOR);
      temp >>= 1;
    }
  }
}

void LCDShowString(uint16_t x, uint16_t y, uint8_t* dat) {
  uint16_t j;

  j = 0;
  while (dat[j] != '\0') {
    LCDShowChar(x + 8 * j, y * 16, dat[j]);
    j++;
  }
}

void LCDShowInt8(uint16_t x, uint16_t y, char dat) {
  uint8_t a[3];
  uint8_t i;
  if (dat < 0) {
    LCDShowChar(x, y * 16, '-');
    dat = -dat;
  } else
    LCDShowChar(x, y * 16, ' ');

  a[0] = dat / 100;
  a[1] = dat / 10 % 10;
  a[2] = dat % 10;
  i = 0;
  while (i < 3) {
    LCDShowChar(x + (8 * (i + 1)), y * 16, '0' + a[i]);
    i++;
  }
}

void lcd_showuint8_t(uint16_t x, uint16_t y, uint8_t dat) {
  uint8_t a[3];
  uint8_t i;

  a[0] = dat / 100;
  a[1] = dat / 10 % 10;
  a[2] = dat % 10;
  i = 0;
  while (i < 3) {
    LCDShowChar(x + (8 * (i + 1)), y * 16, '0' + a[i]);
    i++;
  }
}

void LCDShowInt16(uint16_t x, uint16_t y, int dat) {
  uint8_t a[5];
  uint8_t i;
  if (dat < 0) {
    LCDShowChar(x, y * 16, '-');
    dat = -dat;
  } else
    LCDShowChar(x, y * 16, ' ');

  a[0] = dat / 10000;
  a[1] = dat / 1000 % 10;
  a[2] = dat / 100 % 10;
  a[3] = dat / 10 % 10;
  a[4] = dat % 10;

  i = 0;
  while (i < 5) {
    LCDShowChar(x + (8 * (i + 1)), y * 16, '0' + a[i]);
    i++;
  }
}

void lcd_showuint16_t(uint16_t x, uint16_t y, uint16_t dat) {
  uint8_t a[5];
  uint8_t i;

  a[0] = dat / 10000;
  a[1] = dat / 1000 % 10;
  a[2] = dat / 100 % 10;
  a[3] = dat / 10 % 10;
  a[4] = dat % 10;

  i = 0;
  while (i < 5) {
    LCDShowChar(x + (8 * (i + 1)), y * 16, '0' + a[i]);
    i++;
  }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      显示40*40 QQ图片
//  @param      *p     			图像数组地址
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void LCDShowImage(const unsigned char* p) {
  int i, j, k;
  unsigned char picH, picL;
  LCDClear(WHITE);  //清屏

  for (k = 0; k < 4; k++) {
    for (j = 0; j < 3; j++) {
      Lcd_SetRegion(40 * j, 40 * k, 40 * j + 39, 40 * k + 39);  //坐标设置
      for (i = 0; i < 40 * 40; i++) {
        picL = *(p + i * 2);  //数据低位在前
        picH = *(p + i * 2 + 1);
        LCD_WriteData_16Bit(picH << 8 | picL);
      }
    }
  }
}
