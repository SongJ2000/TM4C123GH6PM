#include "CHIP/DAC7811/DAC7811.h"

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

void DAC7811_Init(void) {
  //【1】开启端口时钟
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
  while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC))
    ;  //等待时钟开启
  //【2】配置GPIO
  GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);
  SetSYNC(HEIGH);
}

void DAC7811_SetOutput(uint16_t val) {
  if (val < 0 || val > 4095) return;
  val = val | 0x1000;
  SetSCLK(HEIGH);
  SetSYNC(LOW);
  for (int i = 0; i < 16; i++) {
    SetSCLK(HEIGH);
    if ((val << i) & 0x8000)
      SetSDIN(HEIGH);
    else
      SetSDIN(LOW);
    SetSCLK(LOW);
  }
  SetSYNC(HEIGH);
}
