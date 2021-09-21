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
#include <stdbool.h>

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
#include "usart.h"
#include "FLEE.h"
#include "timer.h"

void Freq_Convert(uint16_t AD_Value);

void Task1 (void *data);
void Task2 (void *data);
void Task3 (void *data);
void Task4 (void *data);


uint16_t Freq_PreARR[5]={180,1500,2000,2500,2770};
uint16_t Freq_Prefres[5]={1,2,4,8,16};
uint16_t Freq_Offset[5]={0,750,660,585,573};

uint16_t Freq_Misfire = 300;

uint16_t Debug_Crank_Freq_Display_Org;




#define AD_MIN 0
#define INCLUDE_uxTaskGetStackHighWaterMark 1
uint32_t Task_Stack_MaxSize;

uint16_t Filter_index=0,Filter_SumValue,Filter_length=16,Filter_AD_Temp[16]={0},Filter_Delete;



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
	char LoopCnt,Filt_cnt,index_ii;
	uint16_t freq_input;
	data = data;
	

	
	xLastExecutionTime = xTaskGetTickCount();
	 
	while (1) 
	{
		 vTaskDelayUntil( &xLastExecutionTime, 2/ portTICK_RATE_MS );
		LoopCnt++;
		
		if(Usart1_Receive_Complete==true)
		{
			for (index_ii=0;index_ii<Usart1_Rec_Cnt;index_ii++)
			{
					write_date[index_ii]=DMA_Rece_Buf[index_ii];
			}
				Flash_Write_16byte_with_Check(); //DMA接收串口数据缓冲区

				Usart1_Receive_Complete=false;
		}
		
		if(LoopCnt>15)
		{
				LoopCnt=0;
			VIOS_GPIO_LED_Test();
			
			Main_74HC165=VIOS_Read_HC165(3);		
			VIOS_ADC_Convert_Result();

//			freq_input=VIOS_Crank_Frequency;			
//			Debug_Crank_Freq_Display_Org=freq_input;
//			Debug_Crank_Freq_Display=freq_input;
			VIOS_Set_Crank_Freq(VIOS_Crank_Frequency);
			CRANK_OutPut_Function(VIOS_Crank_Frequency);
			VSPD_Output_WSS(VIOS_VehSpd_WSS);
			VSPD_Output_VSS(VIOS_VehSpd_VSS);

			if(Task_Stack_MaxSize<xPortGetFreeHeapSize())
			{
					Task_Stack_MaxSize=xPortGetFreeHeapSize();
			}

		}
		

	}
}





/*
void Task2 (void *data)
{
	data = data;
			
	while (1) 
	{
		if(!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0))
		{
					GPIO_SetBits( GPIOG,GPIO_Pin_14);             //  点亮LED 
			
		}
		else
		{
			GPIO_ResetBits(GPIOG, GPIO_Pin_14);        //  熄灭LED 
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
		 
    GPIO_SetBits( GPIOF,GPIO_Pin_8);             //  点亮LED 
		vTaskDelay( 1000 / portTICK_RATE_MS ); 
		GPIO_ResetBits(GPIOF, GPIO_Pin_8);            //  熄灭LED

    
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
