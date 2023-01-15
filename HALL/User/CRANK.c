#include "timer.h"
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "CRANK.h"
#include "My_InitTask.h"

bool Timer4_Enable_Flag;

void CRANK_OutPut_Function(uint16_t freq)
{
	freq=VIOS_Crank_Frequency;
	
			if(freq>10)
			{
				if(Timer4_Enable_Flag==false)
				{
					TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_Trigger,ENABLE );
					TIM_Cmd(TIM4, ENABLE);
					Timer4_Enable_Flag=true;
				}

								
//				if(VIOS_Misfire_EnableBit[MINT_Cyl_Num]==1)
//				{
//					CRANK_Misfire_Frequency=(uint32_t)(freq*VIOS_Misfire_Frequency)/4096;
//					if(CRANK_Misfire_Frequency<10)
//					{
//						CRANK_Misfire_Frequency=10;
//					}
//					CRANK_Freq_DC(CRANK_Misfire_Frequency);
//				}
				else if(freq<8000)
				{ 
					//Time4 只触发中断，不做PWM out
				CRANK_Freq_DC(VIOS_Crank_Frequency);
				}
			

			}
			else 
			{

				if(Timer4_Enable_Flag==true)
				{
					Timer4_Enable_Flag=false;
				}
				TIM_Cmd(TIM4, DISABLE); 
				if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
				{
					TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
				}
					
			 }


}



void CRANK_Freq_DC(uint16_t freq_Temp)
{
	uint16_t arr_peroid,Var_psc=0,i;
	
	uint32_t arr_peroid_long,arr_peroid_long_temp;
//	freq_Temp=VIOS_Crank_Frequency;
	if(freq_Temp>0)
	{
		TIM_Cmd(TIM4, ENABLE); 
			arr_peroid_long = 72000000/(freq_Temp*6);
			arr_peroid_long_temp=arr_peroid_long;
			for(i=0;i<100;i++)
			{
				if(arr_peroid_long_temp>65535)
				{
					Var_psc++;
					arr_peroid_long_temp	=arr_peroid_long/(Var_psc+1);
				}
				else
				{
				break;
				}
			}
			
			arr_peroid_long	=arr_peroid_long/	(Var_psc+1);
			arr_peroid = arr_peroid_long;	
			
				TIM4->ARR = arr_peroid;
				TIM4->PSC =Var_psc;
		}
	else
	{
		TIM_Cmd(TIM4, DISABLE); 
	
	}
}
