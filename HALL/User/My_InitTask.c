
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
/**** stm32的初始化工作全部在这里完成  *****/
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
		uart_init(115200);	 //串口初始化为115200
	
/* DMA configuration ------------------------------------------------------*/
  //DMA_Configuration();
  /* ADC configuration ------------------------------------------------------*/
  ADC_Configuration();
	
	Init_TIMER(7200);
	
		TIM3_PWM_Init(6009,1000);	 //不分频。PWM频率=72000/900=8Khz
		TIM_SetCompare4(TIM3,800);	
	
	 	TIM2_Int_Init(6009,1000);	
		TIM_SetCompare1(TIM2,800);         //设置占空比为1/3 
		
		TIM1_Int_Init(10,10);	
//		TIM_SetCompare1(TIM1,800);         //设置占空比为1/3
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
	GPIO_InitTypeDef GPIO_InitStructure;					//定义一个GPIO结构体变量

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE| RCC_APB2Periph_GPIOG |RCC_APB2Periph_AFIO,ENABLE);	
															//使能各个端口时钟，重要！！！
	
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8; 			//配置LED D5端口挂接到13端口
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   		//复用功能输出推挽
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//配置端口速度为50M
  	GPIO_Init(GPIOA, &GPIO_InitStructure);				   	//将端口GPIOA进行初始化配置
	
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13; 			//配置LED D5端口挂接到13端口
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   		//复用功能输出推挽
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//配置端口速度为50M
  	GPIO_Init(GPIOB, &GPIO_InitStructure);				   	//将端口GPIOB进行初始化配置
	
					GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 			//配置LED D5端口挂接到13端口
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	   		//复用功能输出推挽
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//配置端口速度为50M
  	GPIO_Init(GPIOB, &GPIO_InitStructure);				   	//将端口GPIOB进行初始化配置
	
		/*  配置输入端口*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ; 			//配置LED E0端口挂接到0端口
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	   		//复用功能上拉输入
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	   	//配置端口速度为50M
  	GPIO_Init(GPIOB, &GPIO_InitStructure);				   	//将端口GPIOE进行初始化配置
	
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 			//配置LED D5端口挂接到13端口
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   		//复用功能输出推挽
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//配置端口速度为50M
  	GPIO_Init(GPIOD, &GPIO_InitStructure);				   	//将端口GPIOD进行初始化配置
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;             //配置LED D2端口挂接到PG14端口
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   	//通用输出推挽
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//配置端口速度为50M
  	GPIO_Init(GPIOG, &GPIO_InitStructure);				   	//将端口GPIOD进行初始化配置

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ; 			//配置LED D5端口挂接到13端口
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   		//复用功能输出推挽
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//配置端口速度为50M
  	GPIO_Init(GPIOD, &GPIO_InitStructure);				   	//将端口GPIOD进行初始化配置

	/*  配置输入端口*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ; 			//配置LED E0端口挂接到0端口
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	   		//复用功能上拉输入
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	   	//配置端口速度为50M
  	GPIO_Init(GPIOE, &GPIO_InitStructure);				   	//将端口GPIOE进行初始化配置
	 
	 //PA0/1/2 ??????????
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //??????
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //??????
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ; 			//配置LED D5端口挂接到13端口
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	   		//复用功能输出推挽
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//配置端口速度为50M
  	GPIO_Init(GPIOD, &GPIO_InitStructure);				   	//将端口GPIOD进行初始化配置
		
	GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE);			//将定时器4通道2重映射到PD13引脚，重要！！
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
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
     NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruc
  
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
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;					//独立模式  CR1
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;						    //扫描模式使能
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;					//连续转换模式使能
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//启动转换的外部事件--无  CR2
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				//转换后的数据右对齐
  ADC_InitStructure.ADC_NbrOfChannel = 1;								//转换的通道数为1
  ADC_Init(ADC1, &ADC_InitStructure);

  //ADC_TempSensorVrefintCmd(ENABLE);
  /* ADC1 regular channel14 configuration */ 
  //ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5 );
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5 );

	/* Enable ADC1 DMA */															  
  ADC_DMACmd(ADC1, DISABLE);		                          						  //允许ADC1进行DMA传送
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);														  //使能ADC1

  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);													  //允许ADC1复位校准寄存器 
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));									  //检测校准寄存器是否复位完成 

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);													  //启动ADC1 校准
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));									      //检测校准是否完成 
     
  /* Start ADC1 Software Conversion */ 
  //ADC_SoftwareStartConvCmd(ADC1, ENABLE);										  //软件触发启动ADC1转换

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
** 函数名称: TIMER_Init
** 功能描述: 定时器2初始化配置
** 参数描述：无
** 作  　者: Dream
** 日　  期: 2014年11月07日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Init_TIMER(uint16_t period)
{
	TIM_TimeBaseInitTypeDef	 TIM_BaseInitStructure;			//定义一个定时器结构体变量

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);   	//使能定时器4，重要！！

	TIM_DeInit(TIM4);                              			//将IM2定时器初始化位复位值

	TIM_InternalClockConfig(TIM4); 							//配置 TIM4 内部时钟
	   
	TIM_BaseInitStructure.TIM_Period = 10800-1; 				//设置自动重载寄存器值为最大值	0~65535之间  1000000/1000=1000us=1ms													
															//TIM_Period（TIM1_ARR）=7200，计数器向上计数到7200后产生更新事件，
															//计数值归零 也就是 1MS产生更新事件一次
	TIM_BaseInitStructure.TIM_Prescaler = 0;  				//自定义预分频系数为0，即定时器的时钟频率为72M提供给定时器的时钟	0~65535之间
															//设置预分频器分频系数0
	TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //时钟分割为0
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;   
															//TIM向上计数模式 从0开始向上计数，计数到1000后产生更新事件
	TIM_TimeBaseInit(TIM4, &TIM_BaseInitStructure); 		//根据指定参数初始化TIM时间基数寄存器	
      
 	TIM_ARRPreloadConfig(TIM4, ENABLE);						//使能TIMx在 ARR 上的预装载寄存器 
	
	TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_Trigger,ENABLE );// enable interrupt

	//TIM_Cmd(TIM4, ENABLE); 		//TIM4总开关：开启 
}


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: PWM_Init
** 功能描述: 配置PWM通道及占空比
** 参数描述：Dutyfactor 定义占空比大小
** 作  　者: Dream
** 日　  期: 2014年11月07日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Init_PWM(uint16_t Dutyfactor)
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;					//定义一个通道输出结构


	TIM_OCStructInit(&TIM_OCInitStructure);					//设置缺省值

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	   	//PWM 模式 1 输出 	
	TIM_OCInitStructure.TIM_Pulse = Dutyfactor; 			//设置占空比，占空比=(CCRx/ARR)*100%或(TIM_Pulse/TIM_Period)*100%
															//PWM的输出频率为Fpwm=72M/7200=1Mhz；  
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
															//TIM 输出比较极性高   	    
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    
															//使能输出状态  需要PWM输出才需要这行代码
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);				//根据参数初始化PWM寄存器    

	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);	   	//使能 TIMx在 CCR2 上的预装载寄存器

    TIM_CtrlPWMOutputs(TIM4,ENABLE);  						//设置TIM4 的PWM 输出为使能  
}



void AD_ConvertFunction(void)
{
	AD_value=ADC_GetConversionValue(ADC1);
 	//while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
		AD_value_group[0] = ADC_ConvertedValue[0];
		AD_value_group[1] = ADC_ConvertedValue[1];
}

void TIM4_IRQHandler(void)   //TIM3中断
{
		uint32_t Init_Misfire_Frequency;
   if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
        {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
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









