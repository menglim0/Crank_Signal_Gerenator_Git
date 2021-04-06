#include "timer.h"
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "KNOCK.h"
#include "My_InitTask.h"

bool VbKNOCK_Enable_Flag;

void KNOCK_Output_Freq(uint16_t freq,bool enable)
{
	if(freq>0&&enable==1)
	{
				TIM_Cmd(TIM2, ENABLE); 
				PWM_Freq_DC(1,50,freq);
	}
	else if(enable==0)
	{
			PWM_Freq_DC(1,0,freq);
			//TIM_Cmd(TIM2, DISABLE); 
	}
			
}


