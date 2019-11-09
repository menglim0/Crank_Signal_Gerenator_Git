/*
    FreeRTOS V7.0.0 - Copyright (C) 2011 Real Time Engineers Ltd.
	

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS tutorial books are available in pdf and paperback.        *
     *    Complete, revised, and edited pdf reference manuals are also       *
     *    available.                                                         *
     *                                                                       *
     *    Purchasing FreeRTOS documentation will not only help you, by       *
     *    ensuring you get running as quickly as possible and with an        *
     *    in-depth knowledge of how to use FreeRTOS, it will also help       *
     *    the FreeRTOS project to continue with its mission of providing     *
     *    professional grade, cross platform, de facto standard solutions    *
     *    for microcontrollers - completely free of charge!                  *
     *                                                                       *
     *    >>> See http://www.FreeRTOS.org/Documentation for details. <<<     *
     *                                                                       *
     *    Thank you for using FreeRTOS, and thank you for your support!      *
     *                                                                       *
    ***************************************************************************


    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    >>>NOTE<<< The modification to the GPL is included to allow you to
    distribute a combined work that includes FreeRTOS without being obliged to
    provide the source code for proprietary components outside of the FreeRTOS
    kernel.  FreeRTOS is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/
	

/* Standard includes. */
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


/* Library includes. */
#include "stm32f10x.h"
#include "stm32f10x_it.h"

/* user files  		*/
#include "My_InitTask.h"
#include "vios.h"


void Freq_Convert(uint16_t AD_Value);
void Task1 (void *data);
void Task2 (void *data);
void Task3 (void *data);
void Task4 (void *data);
/*void Task5 (void *data);
void Task6 (void *data);*/

uint16_t Freq_PreARR[5]={180,1500,2000,2500,3000};
uint16_t Freq_Prefres[5]={1,2,4,8,16};
uint16_t Freq_Offset[5]={0,750,660,585,573};



/*-----------------------------------------------------------*/
/*-----------------------------------------------------------*/
int main( void )				 
{

	My_InitTask();

	xTaskCreate( Task1, ( signed portCHAR *) "Task1", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY+1), NULL);
	//xTaskCreate( Task2, ( signed portCHAR *) "Task2", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY+1), NULL);
	//xTaskCreate( Task3, ( signed portCHAR *) "Task3", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY), 	NULL);
   //	xTaskCreate( Task4, ( signed portCHAR *) "Task4", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY), NULL);
	/*xTaskCreate( Task5, ( signed portCHAR *) "Task5", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY+1), NULL);
	xTaskCreate( Task6, ( signed portCHAR *) "Task6", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY+1), NULL);*/
	

	/* Start the scheduler. */
	vTaskStartScheduler();
	
	/* Will only get here if there was not enough heap space to create the idle task. */
	return 0;

}


/*-----------------------------------------------------------*/
/*-----------------------------------------------------------*/	
void Task1 (void *data)
{
	portTickType xLastExecutionTime;
	char LoopCnt;
	uint16_t CrankPeroid=720;
	data = data;
	xLastExecutionTime = xTaskGetTickCount();
	 
	while (1) 
	{
		 vTaskDelayUntil( &xLastExecutionTime, 2/ portTICK_RATE_MS );
		LoopCnt++;
		if(LoopCnt>15)
		{LoopCnt=0;
if(!GPIO_ReadOutputDataBit(GPIOG,GPIO_Pin_14))
    GPIO_SetBits( GPIOG,GPIO_Pin_14);             //  µ„¡¡LED 
		//vTaskDelay( 1 / portTICK_RATE_MS ); 
		else
		GPIO_ResetBits(GPIOG,GPIO_Pin_14);            //  œ®√LED

		//vTaskDelay( 1 / portTICK_RATE_MS ); 
		
		//AD_ConvertFunction();
		ADC_SimpleConvertValue[0]=Get_Adc(ADC_Channel_0);
		//TIM_SetCompare1(TIM4,ADC_SimpleConvertValue[0]>>2);
		ADC_SimpleConvertValue[1]=Get_Adc(ADC_Channel_1);
		//TIM4->ARR =(ADC_SimpleConvertValue[1]-400)<<4;
		//TIM_SetCompare2(TIM4,(ADC_SimpleConvertValue[1]-400)<<4);
		ADC_SimpleConvertValue[2]=Get_Adc(ADC_Channel_2);
		ADC_SimpleConvertValue[3]=Get_Adc(ADC_Channel_3);
		
			//TIM4->ARR = 60000-1;
		Freq_Convert(ADC_SimpleConvertValue[1]);
		
		//Init_TIMER(ADC_SimpleConvertValue[1]);
//		CrankPeroid++;
//		if(CrankPeroid>7200)
//		{
//		CrankPeroid=7200;
//		}
	}
	}
}

void Freq_Convert(uint16_t AD_Value)
{
	uint8_t index;
	uint16_t psc,arr;
	for(index=1;index<5;index++)
	{
		if(AD_Value<Freq_PreARR[index]&&AD_Value>Freq_PreARR[index-1])
		{
			psc=Freq_Prefres[index-1];
			arr=AD_Value-Freq_Offset[index-1];
			
		}
		else if(AD_Value>Freq_PreARR[4])
		{
			psc=Freq_Prefres[4];
			arr=AD_Value-Freq_Offset[4];
		}
		else if(AD_Value<Freq_PreARR[0])
	 {
			psc=Freq_Prefres[0];
			arr=Freq_PreARR[0];
	 }
		
	}
	TIM4->ARR=arr;
	TIM4->PSC=(psc*8-1);

}

/*
void Task2 (void *data)
{
	data = data;
			
	while (1) 
	{
		if(!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0))
		{
					GPIO_SetBits( GPIOG,GPIO_Pin_14);             //  µ„¡¡LED 
			
		}
		else
		{
			GPIO_ResetBits(GPIOG, GPIO_Pin_14);        //  œ®√LED 
		}
     		 
		vTaskDelay( 100 / portTICK_RATE_MS ); 


	   	vTaskDelay( 100 / portTICK_RATE_MS );
	}
}


void Task3 (void *data)
{
	data = data;

	while (1) 
	{
		 
    GPIO_SetBits( GPIOF,GPIO_Pin_8);             //  µ„¡¡LED 
		vTaskDelay( 1000 / portTICK_RATE_MS ); 
		GPIO_ResetBits(GPIOF, GPIO_Pin_8);            //  œ®√LED

    
		vTaskDelay( 1000 / portTICK_RATE_MS ); 


	}
}


void Task4 (void *data)
{
	data = data;

	while (1) 
	{
		GPIO_ResetBits(GPIOF, GPIO_Pin_9);            //  œ®√LED
		
		vTaskDelay( 1000 / portTICK_RATE_MS ); 
		GPIO_SetBits( GPIOF,GPIO_Pin_9);             //  µ„¡¡LED 


		vTaskDelay( 1000 / portTICK_RATE_MS ); 
	}
}
*/
/*
void Task5 (void *data)
{
	data = data;

	while (1) 
	{
		

		vTaskDelay( 1000 / portTICK_RATE_MS ); 
	}
}


void Task6 (void *data)
{
	data = data;

	while (1) 
	{
		

		vTaskDelay( 1000 / portTICK_RATE_MS ); 
	}
}
*/





/*-----------------------------------------------------------*/	
/*-----------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/***********************  End of File ***************************************/
