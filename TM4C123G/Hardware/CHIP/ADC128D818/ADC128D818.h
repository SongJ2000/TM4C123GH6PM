#ifndef __ADC128D818_H
#define __ADC128D818_H

#include "system.h"

#define 	Vref										5.0f

#define		ADC128D818Addr					0x1D
#define		Configuration 					0x00
#define		InterruptStatus 				0x01
#define		InterruptMask						0x03 
#define		ConversionRate					0x07
#define		ChannelDisable					0x08
#define		OneShot									0x09
#define		DeepShutdown 						0x0A
#define		AdvancedConfiguration		0x0B
#define		BusyStatus							0x0C
#define		ChannelBase							0x20 //0x20 - 0x27
#define		LimitBase								0x2A //0x2A - 0x39
#define		ManufacturerID					0x3E
#define		RevisionID 							0x3F

void ADC128D818Init(void);
float ADC128D818_GetADCValue(uint8_t ch);
float ADC128D818_GetTemperatureValue(void);

#endif
