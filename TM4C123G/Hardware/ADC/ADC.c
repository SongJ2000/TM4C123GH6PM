#include "ADC.h"

void ADC_Init(void) {
  SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
  while (!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)) {
  }
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
  while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)) {
  }
  GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3 | GPIO_PIN_2);
  //
  // Enable the first sample sequencer to capture the value of channel 0 when
  // the processor trigger occurs.
  //
  ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
  ADCSequenceStepConfigure(ADC0_BASE, 0, 4,
                           ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0);
  ADCSequenceEnable(ADC0_BASE, 0);

  ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
  ADCSequenceStepConfigure(ADC0_BASE, 3, 0,
                           ADC_CTL_CH1 | ADC_CTL_IE | ADC_CTL_END);
  ADCSequenceEnable(ADC0_BASE, 3);
}

uint32_t ADC0_Get(void) {
  uint32_t ADCValue[5] = {0, 0, 0, 0, 0};
  static uint32_t TempAvg[3] = {0, 0, 0};
  uint32_t result = 0;

  // 向后移动 1
  TempAvg[2] = TempAvg[1];
  TempAvg[1] = TempAvg[0];

  ADCProcessorTrigger(ADC0_BASE, 0);
  while (!ADCIntStatus(ADC0_BASE, 0, false))
    ;
  ADCIntClear(ADC0_BASE, 0);
  ADCSequenceDataGet(ADC0_BASE, 0, ADCValue);
  TempAvg[0] =
      (ADCValue[0] + ADCValue[1] + ADCValue[2] + ADCValue[3] + ADCValue[4]) / 5;

  result = 0.7f * TempAvg[0] + 0.2f * TempAvg[1] + 0.1f * TempAvg[2];

  return result;
}

uint32_t ADC1_Get(void) {
  uint32_t ADCValue[1];
  static uint32_t TempAvg[3] = {0, 0, 0};
  uint32_t result = 0;

  // 向后移动 1
  TempAvg[2] = TempAvg[1];
  TempAvg[1] = TempAvg[0];

  ADCProcessorTrigger(ADC0_BASE, 3);
  while (!ADCIntStatus(ADC0_BASE, 3, false))
    ;
  ADCIntClear(ADC0_BASE, 3);
  ADCSequenceDataGet(ADC0_BASE, 3, ADCValue);
  TempAvg[0] = ADCValue[0];

  result = 0.7f * TempAvg[0] + 0.2f * TempAvg[1] + 0.1f * TempAvg[2];

  return result;
}
