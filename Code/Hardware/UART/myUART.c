#include "UART/myUART.h"

void UART0_IntHandler(void) {
  uint32_t ui_Status = UARTIntStatus(UART0_BASE, true);
  UARTIntClear(UART0_BASE, ui_Status);

  while (UARTCharsAvail(UART0_BASE)) {
    int32_t i_UartReceiveData = UARTCharGetNonBlocking(UART0_BASE);
    UARTCharPutNonBlocking(UART0_BASE, i_UartReceiveData);
  }
}

void UART0Init(void) {
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
  GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
  UARTStdioConfig(0, 115200, SysCtlClockGet());
  UARTIntRegister(UART0_BASE, UART0_IntHandler);
  IntPrioritySet(INT_UART0, 0);
  IntMasterEnable();
  IntEnable(INT_UART0);
  UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
}

void myPrintf(const char *format, ...) {
  char loc_buf[64];
  char *temp = loc_buf;
  va_list arg;
  va_list copy;
  va_start(arg, format);
  va_copy(copy, arg);
  int len = vsnprintf(temp, sizeof(loc_buf), format, copy);
  va_end(copy);
  if (len < 0) {
    va_end(arg);
    return;
  };
  if (len >= sizeof(loc_buf)) {
    temp = (char *)malloc(len + 1);
    if (temp == NULL) {
      va_end(arg);
      return;
    }
    len = vsnprintf(temp, len + 1, format, arg);
  }
  va_end(arg);
  UARTwrite(temp, len);
  if (temp != loc_buf) {
    free(temp);
  }
}
