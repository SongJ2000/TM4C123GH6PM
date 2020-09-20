#include "CHIP/ADC128D818/ADC128D818.h"

/*
1. Power on the device, then wait for at least 33ms.
2. Read the Busy Status Register (address 0Ch). If the 'Not Ready' bit = 1, then increase the wait time until 'Not
Ready' bit = 0 before proceeding to the next step.
3. Program the Advanced Configuration Register ¡ª Address 0Bh:
¨C a. Choose to use the internal or external VREF (bit 0).
¨C b. Choose the mode of operation (bits [2:1]).
4. Program the Conversion Rate Register (address 07h).
5. Choose to enable or disable the channels using the Channel Disable Register (address 08h).
6. Using the Interrupt Mask Register (address 03h), choose to mask or not to mask the interrupt status from
propagating to the interrupt output pin, INT.
7. Program the Limit Registers (addresses 2Ah ¨C 39h).
8. Set the ¡®START¡¯ bit of the Configuration Register (address 00h, bit 0) to 1.
9. Set the 'INT_Clear' bit (address 00h, bit 3) to 0. If needed, program the 'INT_Enable' bit (address 00h, bit 1)
to 1 to enable the INT output.
*/

void ADC128D818Init(void)
{
	unsigned char temp;
	Delay_ms(50);
	myIICReadLen(ADC128D818Addr, BusyStatus, 1, &temp);
	while(temp&0x02)
	{
		myIICReadLen(ADC128D818Addr, BusyStatus, 1, &temp);
	}
	temp = (1<<0); myIICWriteLen(ADC128D818Addr, AdvancedConfiguration, 1, &temp);
	temp = (1<<0); myIICWriteLen(ADC128D818Addr, ConversionRate, 1, &temp);
	//temp = (1<<4|1<<5|1<<6|1<<7); myIICWriteLen(ADC128D818Addr, AdvancedConfiguration, 1, &temp); //Ö»¿ª4Â·IN0-IN3
	temp = (1<<0); myIICWriteLen(ADC128D818Addr, Configuration, 1, &temp);
	temp = (1<<0); myIICWriteLen(ADC128D818Addr, ConversionRate, 1, &temp);
}

float ADC128D818_GetADCValue(uint8_t ch)
{
	uint8_t temp[2] = {0, 0};
	if(ch <= 6 && ch >= 0)
		myIICReadLen(ADC128D818Addr, ChannelBase + ch, 2, temp);
	uint16_t val = ((temp[0]<<8|temp[1])>>4)/4095*Vref;
	return val;
}

float ADC128D818_GetTemperatureValue(void)
{
	uint8_t temp[2] = {0, 0};
	myIICReadLen(ADC128D818Addr, ChannelBase + 7, 2, temp);
	return ((temp[0]<<1|temp[1]>>7))/2.0f;
}
