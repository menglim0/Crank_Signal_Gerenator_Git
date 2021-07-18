#include "timer.h"
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "VEHSPD.h"
#include "My_InitTask.h" 



void VSPD_Output_WSS(uint16_t freq)
{
if(freq>0)
	{
				TIM_Cmd(TIM2, ENABLE); 
				PWM_Freq_DC(1,50,freq);
	}
	else
	{
			PWM_Freq_DC(1,0,1);
			TIM_Cmd(TIM1, DISABLE); 
	}
			
}


void VSPD_Output_VSS(uint16_t freq)
{		
			if(freq>0)
			{
				TIM_Cmd(TIM3, ENABLE);			
				PWM_Freq_DC(2,50,freq);
			}
			else
			{
					PWM_Freq_DC(2,0,1);
					TIM_Cmd(TIM3, DISABLE); 
			}
			
}
