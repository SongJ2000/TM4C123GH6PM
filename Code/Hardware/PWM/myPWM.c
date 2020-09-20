/**
 * PWM:
 *  普通PWM
 *  - M1PWM6 - PF2
 *  - M1PWM7 - PF3
 *  带死区
 *  - M0PWM0 - PB6
 *  - M0PWM1 - PB7
 */
#include "PWM/myPWM.h"

void PWM_Init_withDeadBand(void) {
  //【1】配置时钟，系统时钟8分频，即80M/2 = 40M
  SysCtlPWMClockSet(SYSCTL_PWMDIV_2);
  //【2】开启时钟，PWM & GPIO
  SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
  //【3】配置引脚PWM通道
  GPIOPinConfigure(GPIO_PB6_M0PWM0);
  GPIOPinConfigure(GPIO_PB7_M0PWM1);
  //【4】配置引脚复用PWM功能
  GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
  GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);
  //【5】配置PWM发生器
  PWMGenConfigure(PWM0_BASE, PWM_GEN_0,
                  PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
  //【6】配置PWM周期
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 800);
  //【7】配置PWM占空比
  PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,
                   PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0) / 2);
  //【8】设置死区时间
  PWMDeadBandEnable(PWM0_BASE, PWM_GEN_0, 4, 4);
  //【9】使能PWM模块1输出
  PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT | PWM_OUT_0_BIT, true);
  //【10】使能PWM发生器
  PWMGenEnable(PWM0_BASE, PWM_GEN_0);
}

void PWM_Init_Common(void) {
  //【1】配置时钟，系统时钟8分频，即80M/2 = 40M
  SysCtlPWMClockSet(SYSCTL_PWMDIV_2);
  //【2】开启时钟，PWM & GPIO
  SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  //【3】配置引脚复用PWM功能
  GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);
  GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);
  //【4】配置引脚PWM通道
  GPIOPinConfigure(GPIO_PF2_M1PWM6);  // PF2->PWM模块1信号6
  GPIOPinConfigure(GPIO_PF3_M1PWM7);  // PF3->PWM模块1信号7
  //【5】配置PWM发生器
  PWMGenConfigure(PWM1_BASE, PWM_GEN_3,
                  PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
  //【6】配置PWM周期
  PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3,
                  800);  // 800/40000000 = 20us
  //【7】配置PWM占空比
  PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,
                   PWMGenPeriodGet(PWM1_BASE, PWM_GEN_3) * 0.5);
  PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,
                   PWMGenPeriodGet(PWM1_BASE, PWM_GEN_3) * 0.5);
  //【8】使能PWM模块1输出
  PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);
  PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);
  //【9】使能PWM发生器
  PWMGenEnable(PWM1_BASE, PWM_GEN_3);
}

void Set_M1PWM6_Duty(float duty) {
  if (duty <= 0.95f && duty > 0.05f) {
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,
                     PWMGenPeriodGet(PWM1_BASE, PWM_GEN_3) * duty);
    PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);
  } else if (duty == 0)
    PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, false);
}

void Set_M1PWM7_Duty(float duty) {
  if (duty <= 0.95f && duty > 0.05f) {
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,
                     PWMGenPeriodGet(PWM1_BASE, PWM_GEN_3) * duty);
    PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);
  } else if (duty == 0)
    PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, false);
}
