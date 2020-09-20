/**
 * SPI:
 *  - SSI0Clk - PA2
 *  - SSI0Fss - PA3
 *  - SSI0Rx  - PA4
 *  - SSI0Tx  - PA5
 */
#include "SPI/mySPI.h"

// 1.5clk,1.6mosi
//![Simple SPI Config]
void mySPIInit(void) {
  SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);  //打开时钟
  GPIOPinConfigure(GPIO_PA2_SSI0CLK);
  GPIOPinConfigure(GPIO_PA3_SSI0FSS);
  GPIOPinConfigure(GPIO_PA4_SSI0RX);
  GPIOPinConfigure(GPIO_PA5_SSI0TX);  //初始化引脚
  GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 |
                                      GPIO_PIN_2);  //配置引脚供SSI外设使用。
  SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,
                     SSI_MODE_MASTER, 1000000, 8);
  SSIEnable(SSI0_BASE);  //使能
}

void mySPITransmit(unsigned char *data, unsigned int len) {
  while (len) {
    len--;
    SSIDataPut(SSI0_BASE, *data);
    data++;
  }
  while (SSIBusy(SSI0_BASE))
    ;
}
