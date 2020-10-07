/**
 * Keil5 MDK Complier6 尚不支持
 */

#include "system.h"

int main(void) {
  BasicSystemInit();
  while (1) {
    myPrintf("Time: %d  Running...\r", GetSysTime_ms() / 1000);
    Delay_ms(1000);
  }
}
