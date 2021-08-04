#include "timer.h"
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "CRANK.h"
#include "My_InitTask.h"

bool Timer4_Enable_Flag;

void CRANK_OutPut_Function(uint16_t Crank_Freq)
{
	
	uint16_t temp_Crank_Freq;
	
	
			if(Crank_Freq>10)
			{
					if(Timer4_Enable_Flag==false)
					{
						TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_Trigger,ENABLE );
						TIM_Cmd(TIM4, ENABLE);
						Timer4_Enable_Flag=true;
					}

					if(Crank_Freq<8000)
					{
						//Time4 只触发中断，不做PWM out
						temp_Crank_Freq=Crank_Freq;
					CRANK_Freq_DC(temp_Crank_Freq);
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



void CRANK_Freq_DC(uint16_t CRK_freq)
{
	uint32_t arr_peroid,Var_psc=0,i;
	
	uint32_t arr_peroid_long=0,arr_peroid_long_temp;
	
	if(VIOS_Crank_Frequency>10)
	{
		  TIM_Cmd(TIM4, ENABLE); 
			arr_peroid_long = 72000000/(VIOS_Crank_Frequency*6);
			arr_peroid_long_temp=arr_peroid_long;
		
			for(i=0;i<100;i++)
			{
				if(arr_peroid_long_temp>=65535)
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

//			PWM_Freq_DC(3,50,VIOS_Crank_Frequency*6);
		}
	else
	{
		TIM_Cmd(TIM4, DISABLE); 
	
	}
}
