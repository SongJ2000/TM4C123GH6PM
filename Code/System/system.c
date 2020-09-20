#include "system.h"

#include "driverlib/interrupt.h"
#include "driverlib/systick.h"

#define TickPerSecond (1000)  //每秒Tick数，即SysTick中断次数
#define usPerTick (1000000 / TickPerSecond)  //每个Tick对应的微秒数

void SysTickIntHandler(void);

/**
 * 系统时间
 * 由于每秒被分为1000份，因此系统时间单位为ms
 */
static volatile uint32_t SysTime_ms;  //系统时锟斤拷(ms)

// SysTick初始化
void SysTickInit(void) {
  // Sets the period of the SysTick counter.
  SysTickPeriodSet(SysCtlClockGet() / TickPerSecond);
  // Registers an interrupt handler for the SysTick interrupt.
  SysTickIntRegister(SysTickIntHandler);
  // Enables the processor interrupt.
  IntMasterEnable();
  // Enables the SysTick interrupt.
  SysTickIntEnable();
  // Enables the SysTick counter.
  SysTickEnable();
}

// SysTick中断服务函数
void SysTickIntHandler(void) { SysTime_ms++; }

// 获取系统时间毫秒
uint32_t GetSysTime_ms(void) { return SysTime_ms; }

// 获取系统时间微秒
uint32_t GetSysTime_us(void) {
  register uint32_t ms, us;

  do {
    ms = SysTime_ms;
    us =
        ((SysTickValueGet() / SysTickPeriodGet()) * usPerTick) + ms * usPerTick;
  } while (ms != SysTime_ms);

  return us;
}

//毫秒级延时
void Delay_ms(uint32_t ms) {
  uint32_t delayTime = GetSysTime_ms() + ms;
  while (GetSysTime_ms() < delayTime)
    ;
}

//微秒级延时
void Delay_us(uint32_t us) {
  uint32_t delayTime = GetSysTime_us() + us;
  while (GetSysTime_us() < delayTime)
    ;
}
