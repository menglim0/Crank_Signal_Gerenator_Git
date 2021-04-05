#ifndef __VEHSPD_H__
#define __VEHSPD_H__
#include "stm32f10x.h"
#include "stdint.h"
#include "My_InitTask.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//处理车速信号输出						  
//////////////////////////////////////////////////////////////////////////////////
void VSPD_Output_WSS(uint16_t freq);
void VSPD_Output_VSS(uint16_t freq);

#endif
