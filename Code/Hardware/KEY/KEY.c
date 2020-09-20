#include "KEY/KEY.h"

void Key_Init(void) {
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    ;
  GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
  GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA,
                   GPIO_PIN_TYPE_STD_WPU);

  HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;  //解锁
  HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;            //确认PF0
  HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;              //重新锁定

  GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
  GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA,
                   GPIO_PIN_TYPE_STD_WPU);
}

uint32_t Key1(void) {
  static uint32_t LastState = 1;
  uint32_t CurrentState = 1, result = 0;
  CurrentState = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
  if (CurrentState == 0) {
    Delay_ms(10);
    CurrentState = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
    if (CurrentState == 0) {
      if (CurrentState != LastState)
        result = 1;
      else
        result = 0;
    }
  }
  LastState = CurrentState;
  return result;
}

uint32_t Key2(void) {
  static uint32_t LastState = 1;
  uint32_t CurrentState = 1, result = 0;
  CurrentState = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);
  if (CurrentState == 0) {
    Delay_ms(10);
    CurrentState = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);
    if (CurrentState == 0) {
      if (CurrentState != LastState)
        result = 1;
      else
        result = 0;
    }
  }
  LastState = CurrentState;
  return result;
}
