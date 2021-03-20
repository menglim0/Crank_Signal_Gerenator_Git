#include "stm32f10x.h"
#include "vios.h"
#include "stdbool.h"


u16 ADC_SimpleConvertValue[4];


u8 VIOS_Misfire_Enable;
u8 VIOS_Misfire_Cylinder;

u16 VIOS_Misfire_Freq;
/******AD input Convert The par is ADC_Channel_x***********/
u16 Get_Adc(u8 ch) 
{
//Set ADC regerular channel
ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );  ////  
ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //
while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//
return ADC_GetConversionValue(ADC1);  //
}


u16 Get_AD_Filter(uint16_t inputValue)
{

	return 0;

}

u8 VIOS_Set_Misfire_Cylinder(u8 cylinder)
{
			VIOS_Misfire_Cylinder=cylinder;
	
}

u8 VIOS_Get_Misfire_Cylinder()
{

		return VIOS_Misfire_Cylinder;
}


u8 VIOS_Set_Misfire_Enable(u8 cylinder)
{
	u8 Misfire_Enable=1;
	if((Misfire_Enable>>cylinder)&0x01)
		return true;
	else
		return false;
			
	
}

u8 VIOS_Get_Misfire_Enable()
{

		return VIOS_Misfire_Enable;
}

u16 VIOS_Set_Misfire_Freq(u16 Misfire_Freq)
{
			VIOS_Misfire_Freq=Misfire_Freq;
	
}

u16 VIOS_Get_Misfire_Freq(u16 Misfire_Freq)
{
			
	return VIOS_Misfire_Freq;
	
}


