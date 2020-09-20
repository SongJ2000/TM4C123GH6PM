#include "system.h"

uint8_t Buf[30];

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

int main(void) {
  BasicSystemInit();
  while (1) {
    myPrintf("Time: %d  Running...\r", GetSysTime_ms() / 1000);
    Delay_ms(1000);
  }
}
