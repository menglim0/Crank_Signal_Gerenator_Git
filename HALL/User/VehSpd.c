#include "timer.h"
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "VEHSPD.h"
#include "My_InitTask.h" 



void VSPD_Output_WSS(uint16_t freq)
{

				PWM_Freq_DC(0,50,freq);
			
}


void VSPD_Output_VSS(uint16_t freq)
{
			
				PWM_Freq_DC(2,50,freq);
}
