#ifndef __VIOS_H
#define __VIOS_H

#include "stm32f10x.h"
extern u16 ADC_SimpleConvertValue[4];
extern u8 VIOS_Misfire_Enable;

u16 Get_Adc(u8 ch); 
u16 Get_Adc(u8 ch);
u8 VIOS_Set_Misfire_Cylinder(u8 cylinder);
u8 VIOS_Get_Misfire_Cylinder();

u8 VIOS_Set_Misfire_Enable(u8 Cyl_en);
u8 VIOS_Get_Misfire_Enable();

u16 VIOS_Set_Misfire_Freq(u16 Misfire_Freq);
u16 VIOS_Get_Misfire_Freq(u16 Misfire_Freq);


#endif
