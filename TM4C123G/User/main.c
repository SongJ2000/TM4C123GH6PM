/**
 * 外设接口：
 * IIC:
 *  - SCL - PB2
 *  - SDA - PB3
 * SPI:
 *  - SSI0Clk - PA2
 *  - SSI0Fss - PA3
 *  - SSI0Rx  - PA4
 *  - SSI0Tx  - PA5
 * OLED:
 *  - D0  - PA2(SSI0Clk)
 *  - D1  - PA5(SSI0Tx)
 *  - DC  - PD1
 *  - RST - PD2
 *  - CS  - PD0
 * PWM:
 *  普通PWM
 *  - M1PWM6 - PF2
 *  - M1PWM7 - PF3
 *  带死区
 *  - M0PWM0 - PB6
 *  - M0PWM1 - PB7
 * DAC:
 *  - SYNC - PC6
 *  - SCLK - PC5
 *  - SDIN - PC4
 */

#include "system.h"

uint8_t Buf[30];
float ADC0Val = 0, ADC1Val = 0, DACVal = 100;

void BasicSystemInit(void) {
  /* 使能浮点运算单元 */
  FPUEnable();
  FPULazyStackingEnable();
  /* 设置主频为80MHz */
  SysCtlClockSet(SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL |
                 SYSCTL_SYSDIV_2_5);
  /* SysTick初始化 */
  SysTickInit();
  /* 调试串口初始化 */
  UART0Init();
}

/* 串口打印任务 */
void UartPrintEvent(void) {
  sprintf((char*)Buf, "Input:  %.2f  ", ADC0Val);
  LCDShowString(10, 2, (uint8_t*)Buf);
  sprintf((char*)Buf, "Output: %.2f  ", ADC1Val);
  LCDShowString(10, 3, (uint8_t*)Buf);
}

int main(void) {
  BasicSystemInit();
  /* ADC初始化 */
  ADC_Init();
  /* LCD初始化 */
  LCDInit();
  LCDShowString(50, 0, (uint8_t*)"C-04");
  /* KEY初始化 */
  Key_Init();
  while (1) {
    ADC0Val = ADC0_Get() * 3.3f / 4095;
    ADC1Val = ADC1_Get() * 3.3f / 4095;
    if (Key1()) {
			myPrintf("key1\r");
    }
    if (Key2()) {
			myPrintf("key2\r");
    }
		if(Key3()) {
			myPrintf("key3\r");
		}
		if(Key4()) {
			myPrintf("key4\r");
		}
    UartPrintEvent();
    Delay_ms(200);
  }
  return 0;
}
