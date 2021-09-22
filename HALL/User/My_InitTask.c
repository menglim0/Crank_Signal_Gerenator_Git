
/*  -------------------------Includes-----------------------------------------*/
#include "My_InitTask.h" 
#include "CAM_Pulse.h"
#include "usart.h"
#include "FLEE.h"
#include "vios.h"
#include "stdbool.h"
#include "timer.h"

#define ADC1_DR_Address    ((u32)0x4001244C)

unsigned int AD_value;
unsigned int AD_value_group[2];



#define  N   50      //The sample freq is 50 
#define  M  12      //12 adc channel



//#define Crank_out_Port GPIOB
//#define Crank_out_Pin GPIO_Pin_0

//#define CAM_output_Port GPIOA
//#define CAM_output_PIN GPIO_Pin_7

//#define CAMIN_Port GPIOA
//#define CAMIN_PIN GPIO_Pin_6

#define Crank_out_Port GPIOB
#define Crank_out_Pin GPIO_Pin_10

#define CAM_output_Port GPIOB
#define CAM_output_PIN GPIO_Pin_12

#define Toggle_Port GPIOD
#define Toggle_PIN GPIO_Pin_12

#define CAMIN_Port GPIOB
#define CAMIN_PIN GPIO_Pin_11

#define SetVIOS_KNOCK_LOW() GPIO_ResetBits(GPIOA,GPIO_Pin_0)

/* Private variables ---------------------------------------------------------*/


uint16_t ADC_ConvertedValue[4];
ErrorStatus HSEStartUpStatus;

uint16_t timer4_It_Cnt,timer4_It_Cnt_Raw;
uint16_t MINT_Knock_WinGate,MINT_Cyl_Num,MINT_Cyl_Num_Old;
uint16_t Debug_Crank_Freq_Display;
/*  -------------------------Functions-----------------------------------------*/
/**** stm32�ĳ�ʼ������ȫ�����������  *****/
void My_InitTask(void)
{
	uint8_t Flash_Read_Buffer[32];
   /* Configure the system clocks */
  	RCC_Configuration();
  	SysTick_Configuration();
    
  /* NVIC Configuration */
  	NVIC_Configuration();

  /* Configure the GPIO ports */
  	GPIO_Configuration();
		uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	
/* DMA configuration ------------------------------------------------------*/
  //DMA_Configuration();
  /* ADC configuration ------------------------------------------------------*/
  ADC_Configuration();
	
	Init_TIMER(7200);
	
		TIM3_PWM_Init(6009,1000);	 //����Ƶ��PWMƵ��=72000/900=8Khz
		TIM_SetCompare4(TIM3,800);	
	
	 	TIM2_Int_Init(6009,1000);	
		TIM_SetCompare1(TIM2,800);         //����ռ�ձ�Ϊ1/3 
		
		TIM1_Int_Init(10,10);	
//		TIM_SetCompare1(TIM1,800);         //����ռ�ձ�Ϊ1/3
	//Init_PWM(0x01F0);
	printf("Start_OK");
	FLASH_Read(0x0800F800,Flash_Read_Buffer,32);
	Usart1_Send(Flash_Read_Buffer,32);
	//Flash_Write_16byte_with_Check();
}



void RCC_Configuration(void)
{
  ErrorStatus HSEStartUpStatus;

  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
   
  /* Enable GPIOB, GPIOF and AFIO clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOF | 
                         RCC_APB2Periph_AFIO, ENABLE);
	
	/* Enable DMA clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/* Enable ADC1 and GPIOA clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
}


/*void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
 
  // Configure PC. as Output push-pull 
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  //GPIO_Init(GPIOF, &GPIO_InitStructure);     
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
}
*/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//����һ��GPIO�ṹ�����

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE| RCC_APB2Periph_GPIOG |RCC_APB2Periph_AFIO,ENABLE);	
															//ʹ�ܸ����˿�ʱ�ӣ���Ҫ������
	
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8; 			//����LED D5�˿ڹҽӵ�13�˿�
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   		//���ù����������
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//���ö˿��ٶ�Ϊ50M
  	GPIO_Init(GPIOA, &GPIO_InitStructure);				   	//���˿�GPIOA���г�ʼ������
	
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13; 			//����LED D5�˿ڹҽӵ�13�˿�
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   		//���ù����������
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//���ö˿��ٶ�Ϊ50M
  	GPIO_Init(GPIOB, &GPIO_InitStructure);				   	//���˿�GPIOB���г�ʼ������
	
					GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 			//����LED D5�˿ڹҽӵ�13�˿�
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	   		//���ù����������
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//���ö˿��ٶ�Ϊ50M
  	GPIO_Init(GPIOB, &GPIO_InitStructure);				   	//���˿�GPIOB���г�ʼ������
	
		/*  ��������˿�*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ; 			//����LED E0�˿ڹҽӵ�0�˿�
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	   		//���ù�����������
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	   	//���ö˿��ٶ�Ϊ50M
  	GPIO_Init(GPIOB, &GPIO_InitStructure);				   	//���˿�GPIOE���г�ʼ������
	
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 			//����LED D5�˿ڹҽӵ�13�˿�
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   		//���ù����������
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//���ö˿��ٶ�Ϊ50M
  	GPIO_Init(GPIOD, &GPIO_InitStructure);				   	//���˿�GPIOD���г�ʼ������
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;             //����LED D2�˿ڹҽӵ�PG14�˿�
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   	//ͨ���������
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//���ö˿��ٶ�Ϊ50M
  	GPIO_Init(GPIOG, &GPIO_InitStructure);				   	//���˿�GPIOD���г�ʼ������

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ; 			//����LED D5�˿ڹҽӵ�13�˿�
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   		//���ù����������
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//���ö˿��ٶ�Ϊ50M
  	GPIO_Init(GPIOD, &GPIO_InitStructure);				   	//���˿�GPIOD���г�ʼ������

	/*  ��������˿�*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ; 			//����LED E0�˿ڹҽӵ�0�˿�
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	   		//���ù�����������
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	   	//���ö˿��ٶ�Ϊ50M
  	GPIO_Init(GPIOE, &GPIO_InitStructure);				   	//���˿�GPIOE���г�ʼ������
	 
	 //PA0/1/2 ??????????
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //??????
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //??????
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ; 			//����LED D5�˿ڹҽӵ�13�˿�
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	   		//���ù����������
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//���ö˿��ٶ�Ϊ50M
  	GPIO_Init(GPIOD, &GPIO_InitStructure);				   	//���˿�GPIOD���г�ʼ������
		
	GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE);			//����ʱ��4ͨ��2��ӳ�䵽PD13���ţ���Ҫ����
}

void NVIC_Configuration(void)
{
 
 NVIC_InitTypeDef NVIC_InitStructure;
  
#ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif

  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
     NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruc
  
}

void SysTick_Configuration(void)
{
  /* Select AHB clock(HCLK) as SysTick clock source */
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);

}


void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
 /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;					//����ģʽ  CR1
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;						    //ɨ��ģʽʹ��
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;					//����ת��ģʽʹ��
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//����ת�����ⲿ�¼�--��  CR2
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				//ת����������Ҷ���
  ADC_InitStructure.ADC_NbrOfChannel = 1;								//ת����ͨ����Ϊ1
  ADC_Init(ADC1, &ADC_InitStructure);

  //ADC_TempSensorVrefintCmd(ENABLE);
  /* ADC1 regular channel14 configuration */ 
  //ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5 );
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5 );

	/* Enable ADC1 DMA */															  
  ADC_DMACmd(ADC1, DISABLE);		                          						  //����ADC1����DMA����
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);														  //ʹ��ADC1

  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);													  //����ADC1��λУ׼�Ĵ��� 
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));									  //���У׼�Ĵ����Ƿ�λ��� 

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);													  //����ADC1 У׼
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));									      //���У׼�Ƿ���� 
     
  /* Start ADC1 Software Conversion */ 
  //ADC_SoftwareStartConvCmd(ADC1, ENABLE);										  //�����������ADC1ת��

}

void DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  /* DMA channel1 configuration ----------------------------------------------*/
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 6;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  /* Enable DMA channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: TIMER_Init
** ��������: ��ʱ��2��ʼ������
** ������������
** ��  ����: Dream
** �ա�  ��: 2014��11��07��
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Init_TIMER(uint16_t period)
{
	TIM_TimeBaseInitTypeDef	 TIM_BaseInitStructure;			//����һ����ʱ���ṹ�����

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);   	//ʹ�ܶ�ʱ��4����Ҫ����

	TIM_DeInit(TIM4);                              			//��IM2��ʱ����ʼ��λ��λֵ

	TIM_InternalClockConfig(TIM4); 							//���� TIM4 �ڲ�ʱ��
	   
	TIM_BaseInitStructure.TIM_Period = 10800-1; 				//�����Զ����ؼĴ���ֵΪ���ֵ	0~65535֮��  1000000/1000=1000us=1ms													
															//TIM_Period��TIM1_ARR��=7200�����������ϼ�����7200����������¼���
															//����ֵ���� Ҳ���� 1MS���������¼�һ��
	TIM_BaseInitStructure.TIM_Prescaler = 0;  				//�Զ���Ԥ��Ƶϵ��Ϊ0������ʱ����ʱ��Ƶ��Ϊ72M�ṩ����ʱ����ʱ��	0~65535֮��
															//����Ԥ��Ƶ����Ƶϵ��0
	TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ʱ�ӷָ�Ϊ0
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;   
															//TIM���ϼ���ģʽ ��0��ʼ���ϼ�����������1000����������¼�
	TIM_TimeBaseInit(TIM4, &TIM_BaseInitStructure); 		//����ָ��������ʼ��TIMʱ������Ĵ���	
      
 	TIM_ARRPreloadConfig(TIM4, ENABLE);						//ʹ��TIMx�� ARR �ϵ�Ԥװ�ؼĴ��� 
	
	TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_Trigger,ENABLE );// enable interrupt

	//TIM_Cmd(TIM4, ENABLE); 		//TIM4�ܿ��أ����� 
}


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: PWM_Init
** ��������: ����PWMͨ����ռ�ձ�
** ����������Dutyfactor ����ռ�ձȴ�С
** ��  ����: Dream
** �ա�  ��: 2014��11��07��
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Init_PWM(uint16_t Dutyfactor)
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;					//����һ��ͨ������ṹ


	TIM_OCStructInit(&TIM_OCInitStructure);					//����ȱʡֵ

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	   	//PWM ģʽ 1 ��� 	
	TIM_OCInitStructure.TIM_Pulse = Dutyfactor; 			//����ռ�ձȣ�ռ�ձ�=(CCRx/ARR)*100%��(TIM_Pulse/TIM_Period)*100%
															//PWM�����Ƶ��ΪFpwm=72M/7200=1Mhz��  
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
															//TIM ����Ƚϼ��Ը�   	    
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    
															//ʹ�����״̬  ��ҪPWM�������Ҫ���д���
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);				//���ݲ�����ʼ��PWM�Ĵ���    

	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);	   	//ʹ�� TIMx�� CCR2 �ϵ�Ԥװ�ؼĴ���

    TIM_CtrlPWMOutputs(TIM4,ENABLE);  						//����TIM4 ��PWM ���Ϊʹ��  
}



void AD_ConvertFunction(void)
{
	AD_value=ADC_GetConversionValue(ADC1);
 	//while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
		AD_value_group[0] = ADC_ConvertedValue[0];
		AD_value_group[1] = ADC_ConvertedValue[1];
}

void TIM4_IRQHandler(void)   //TIM3�ж�
{
		uint32_t Init_Misfire_Frequency;
   if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
        {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
				timer4_It_Cnt_Raw++;
					
					if(timer4_It_Cnt_Raw>=720)
					{
						timer4_It_Cnt_Raw=0;						
										
					}
					
						
					if(timer4_It_Cnt_Raw>=120)
					{
							MINT_Cyl_Num=VIOS_Set_Misfire_Cylinder((timer4_It_Cnt_Raw-120)/180);
							VIOS_Set_KNOCK_Cylinder(MINT_Cyl_Num);	
					}
					else
					{
						VIOS_Set_Misfire_Cylinder(0);
						VIOS_Set_KNOCK_Cylinder(0);
					}
					
					if((VIOS_Misfire_EnableBit[MINT_Cyl_Num]&0x01)==1)
					{
						
						Init_Misfire_Frequency=(uint32_t)(VIOS_Get_Crank_Freq(0)*VIOS_Misfire_Frequency)/4096;
						if(Init_Misfire_Frequency<200)
						{
								Init_Misfire_Frequency=200;
						}
						CRANK_Freq_DC(Init_Misfire_Frequency);
							
					}
					else
					{
						if(VIOS_Get_Crank_Freq(0)>0)
						{
								CRANK_Freq_DC(VIOS_Get_Crank_Freq(0));
						}
					}


					
					if(VIOS_Knock_EnableBit[MINT_Cyl_Num]==1 && MINT_Knock_WinGate<60&&(VIOS_Get_Crank_Freq(0)>200))
					{
						GPIO_SetBits(Toggle_Port,Toggle_PIN);
						KNOCK_Output_Freq(VIOS_Knock_Frequency,1);				
						MINT_Knock_WinGate++; 
						
					}
					else
					{
						KNOCK_Output_Freq(VIOS_Knock_Frequency,0);	
							GPIO_ResetBits(Toggle_Port,Toggle_PIN);
											
					}									
					
					
					if(MINT_Cyl_Num_Old!=MINT_Cyl_Num)
					{
							MINT_Knock_WinGate=0;
					}
					
					
					timer4_It_Cnt=timer4_It_Cnt_Raw%360;
					
					if((( timer4_It_Cnt>=348) && (timer4_It_Cnt<360)))
					{
						GPIO_SetBits(Crank_out_Port, Crank_out_Pin);
					}
					else if((timer4_It_Cnt/3)%2==1)
					{
					GPIO_ResetBits(Crank_out_Port, Crank_out_Pin);
					}
					else
					{GPIO_SetBits(Crank_out_Port, Crank_out_Pin);
					}
	
					
					CAM_Pulse_Output(CAM_out_Phase,VIOS_VVT_Exh_Phase_Final,CAM_output_Port,CAM_output_PIN);
					
					CAM_Pulse_Output(CAM_in_Phase,VIOS_VVT_In_Phase_Final,CAMIN_Port,CAMIN_PIN);
					
					MINT_Cyl_Num_Old=MINT_Cyl_Num;
			
        }
 }









