#ifndef __CAM_PULSE__
#define __CAM_PULSE__

#include "My_InitTask.h"  


extern uint16_t CAM_out_Phase[]; 
extern uint16_t CAM_in_Phase[]; 



void CAM_Pulse_Output(uint16_t CAM_Phase_ofst[],uint16_t CAM_FirstTooth_Offset,GPIO_TypeDef* CAM_out_Port, uint16_t CAM_out_Pin);

#endif