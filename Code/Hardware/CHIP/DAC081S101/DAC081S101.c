/**
 * DAC:
 *  - SYNC - PC6
 *  - SCLK - PC5
 *  - SDIN - PC4
 */
#include "CHIP/DAC081S101/DAC081S101.h"

#define LOW 0
#define HEIGH 1

static void SetSYNC(bool state) {
  GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, state * GPIO_PIN_6);
}

static void SetSCLK(bool state) {
  GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, state * GPIO_PIN_5);
}

static void SetSDIN(bool state) {
  GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, state * GPIO_PIN_4);
}

static void DAC081_Delay(void) {
  for (int i = 0; i < 10; i++)
    ;
}

void DAC081S101_Init(void) {
  //【1】开启端口时钟
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
  while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC))
    ;  //等待时钟开启
  //【2】配置GPIO
  GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);
  SetSYNC(HEIGH);
}

void DAC081S101_SetOutput(uint16_t val) {
  if (val > 255) return;
  val = (val << 4) & 0x0FF0;
  SetSYNC(LOW);
  for (int i = 0; i < 16; i++) {
    SetSCLK(HEIGH);
    if ((val << i) & 0x8000)
      SetSDIN(HEIGH);
    else
      SetSDIN(LOW);
    // Delay_us(1);
    DAC081_Delay();
    SetSCLK(LOW);
  }
  SetSYNC(HEIGH);
}
