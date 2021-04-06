#ifndef __VIOS_H
#define __VIOS_H

#include "stm32f10x.h"
extern uint32_t ADC_SimpleConvertValue[9];
extern uint8_t VIOS_Misfire_Enable;
extern uint32_t VIOS_VehSpd_WSS,VIOS_VehSpd_VSS;
extern uint32_t VIOS_Crank_Frequency;
extern uint8_t VIOS_KNOCK_Enable;
extern uint8_t VIOS_KNOCK_Cylinder;
extern uint16_t VIOS_KNOCK_Freq;
extern uint8_t VIOS_Knock_EnableBit[4];
extern uint8_t VIOS_Misfire_EnableBit[4];
extern uint32_t Misfire_EnableStatus;

extern uint32_t VIOS_Misfire_Frequency;
extern uint32_t VIOS_Knock_Frequency;

extern uint32_t  VIOS_VVT_In_Phase_Final,VIOS_VVT_Exh_Phase_Final;

uint16_t Get_Adc(u8 ch); 
uint16_t Get_Adc(u8 ch);
void VIOS_ADC_Convert_Result(void);
void VIOS_GPIO_LED_Test(void);

uint8_t VIOS_Set_Misfire_Cylinder(u8 cylinder);
uint8_t VIOS_Get_Misfire_Cylinder(void);

uint8_t VIOS_Set_Misfire_Enable(u8 Cyl_en);
uint8_t VIOS_Get_Misfire_Enable(void);

uint16_t VIOS_Set_Misfire_Freq(u16 Misfire_Freq);
uint16_t VIOS_Get_Misfire_Freq(u16 Misfire_Freq);
void VIOS_Read_Misfire_EnableStatus(void);


/* KNOCK interface*/
uint8_t VIOS_Set_KNOCK_Cylinder(u8 cylinder);
uint8_t VIOS_Get_KNOCK_Cylinder(void);
uint8_t VIOS_Set_KNOCK_Enable(u8 cylinder,u8 Enable_Cyl);
uint8_t VIOS_Get_KNOCK_Enable(void);
void VIOS_Read_Knock_EnableStatus(void);
/*---------------------------------------------
Interface to CRANK signals
----------------------------------------------*/
uint16_t VIOS_Set_Crank_Freq(u16 Crank_Freq);
uint16_t VIOS_Get_Crank_Freq(u16 Misfire_Freq);

/*---------------------------------------------
Interface to 74HC165 signals
----------------------------------------------*/
uint32_t VIOS_Read_HC165(unsigned char Cascades_Num);
uint32_t VIOS_Read_VVT_In_Phase(unsigned char bytePos);
uint32_t VIOS_Read_VVT_Exh_Phase(unsigned char bytePos);
extern uint32_t Main_74HC165;

#endif
