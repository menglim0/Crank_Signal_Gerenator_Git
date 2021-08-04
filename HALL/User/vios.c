#include "stm32f10x.h"
#include "vios.h"
#include "stdbool.h"

#define ADC_Filter_Length 32
#define ADC_Zero_Offset 90

uint32_t ADC_SimpleConvertValue[9];
uint32_t ADC_TempConvertValue[9][ADC_Filter_Length+1];

u8 VIOS_Misfire_Enable;
u8 VIOS_Misfire_Cylinder;

u16 VIOS_Misfire_Freq;


u8 VIOS_KNOCK_Enable;
u8 VIOS_KNOCK_Cylinder;

u16 VIOS_KNOCK_Freq;

/*---------------------------------------------
Variables for CRANK signals
----------------------------------------------*/
u16 VIOS_Crank_Freq;


/*----------75CH165 Def-------------------*/
uint32_t  DA_75CH165[1],VIOS_VVT_In_Phase_Final,VIOS_VVT_Exh_Phase_Final;
uint32_t Main_74HC165;

uint32_t VIOS_Crank_Frequency;
uint32_t VIOS_Misfire_Frequency;
uint32_t VIOS_Knock_Frequency;

uint8_t VIOS_Knock_EnableBit[4];
uint8_t VIOS_Misfire_EnableBit[4];	
uint32_t Misfire_EnableStatus;
uint32_t Knock_EnableStatus;

uint32_t VIOS_VehSpd_WSS,VIOS_VehSpd_VSS;
uint8_t VIOS_ADC_Filter_Index=0;


#define HC165_CLK_Set() GPIO_SetBits(GPIOB, GPIO_Pin_8)
#define HC165_CLK_Clr() GPIO_ResetBits(GPIOB, GPIO_Pin_8)

#define HC165_SH_Set() GPIO_ResetBits(GPIOB, GPIO_Pin_9)
#define HC165_SH_Clr() GPIO_SetBits(GPIOB, GPIO_Pin_9)

#define HC165_DS GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)
/*----------Enc of 75CH165 Def---------------*/

void VIOS_GPIO_LED_Test()
{

				if(!GPIO_ReadOutputDataBit(GPIOG,GPIO_Pin_14))
						GPIO_SetBits( GPIOG,GPIO_Pin_14);             //  点亮LED 
				else
					GPIO_ResetBits(GPIOG,GPIO_Pin_14);            //  熄灭LED
}


/*--------------------------
ADC convert function
AD0 not used
AD1 Crank Freq=AD*2 max 8K
AD2 Knock Frequency=AD*7 max 28K
AD3 Misfire Ratio = Crank Freq *Value /4096
AD4 WSS = Value *5 max 20K  Timer 0
AD5 VSS = Value *5 max 20K  Timer 2
-------------------*/
void VIOS_ADC_Convert_Result()
{
	uint8_t ADC_Filter_Index,ADC_Ch_Index;
	uint32_t ADC_ConvertValue_Sum=0;
	
	
				//AD_ConvertFunction();
				//ADC_SimpleConvertValue[0]=Get_Adc(ADC_Channel_0);

	for(ADC_Ch_Index=1;ADC_Ch_Index<9;ADC_Ch_Index++)
	{
			for(ADC_Filter_Index=0;ADC_Filter_Index<ADC_Filter_Length;ADC_Filter_Index++)
			{
				ADC_TempConvertValue[ADC_Ch_Index][ADC_Filter_Index]=Get_Adc(ADC_Ch_Index);
				ADC_ConvertValue_Sum=ADC_ConvertValue_Sum+ADC_TempConvertValue[ADC_Ch_Index][ADC_Filter_Index];
			}
			ADC_ConvertValue_Sum=ADC_ConvertValue_Sum/ADC_Filter_Length;
			ADC_SimpleConvertValue[ADC_Ch_Index]+=ADC_ConvertValue_Sum;
			ADC_SimpleConvertValue[ADC_Ch_Index]=ADC_SimpleConvertValue[ADC_Ch_Index]/2;
			
			if(ADC_SimpleConvertValue[ADC_Ch_Index]>ADC_Zero_Offset)
			{
			ADC_SimpleConvertValue[ADC_Ch_Index]=ADC_SimpleConvertValue[ADC_Ch_Index]-ADC_Zero_Offset;
			}
			else
			{
				ADC_SimpleConvertValue[ADC_Ch_Index]=0;
			}
		
	}
	

	VIOS_ADC_Filter_Index++;
	if(VIOS_ADC_Filter_Index>=ADC_Filter_Length)
	{
			VIOS_ADC_Filter_Index=0;
	}

				VIOS_Crank_Frequency=ADC_SimpleConvertValue[1]*2;
				VIOS_Knock_Frequency=ADC_SimpleConvertValue[2]*7;
				VIOS_Misfire_Frequency=ADC_SimpleConvertValue[3];
				VIOS_VehSpd_WSS=ADC_SimpleConvertValue[4]*5;
				VIOS_VehSpd_VSS=ADC_SimpleConvertValue[5]*5;
	
	
}



/******AD input Convert The par is ADC_Channel_x***********/
u16 Get_Adc(u8 ch) 
{
//Set ADC regerular channel
ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );  ////  
ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //
while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//
return ADC_GetConversionValue(ADC1);  //
}


u16 Get_AD_Filter(uint16_t inputValue)
{

	return 0;

}

void VIOS_Set_Parameters()
{


}


u8 VIOS_Set_Misfire_Cylinder(u8 cylinder)
{
			VIOS_Misfire_Cylinder=cylinder;
	return VIOS_Misfire_Cylinder;
	
}

u8 VIOS_Get_Misfire_Cylinder()
{

		return VIOS_Misfire_Cylinder;
}


u8 VIOS_Set_Misfire_Enable(u8 cylinder)
{
	u8 Misfire_Enable=0;
	if((Misfire_Enable>>cylinder)&0x01)
		return true;
	else
		return false;
			
	
}

u8 VIOS_Get_Misfire_Enable()
{

		return VIOS_Misfire_Enable;
}

u16 VIOS_Set_Misfire_Freq(u16 Misfire_Freq)
{
			VIOS_Misfire_Freq=Misfire_Freq;
		return VIOS_Misfire_Freq;
	
}

u16 VIOS_Get_Misfire_Freq(u16 Misfire_Freq)
{
			
	return VIOS_Misfire_Freq;
	
}

/*---------------------------------------------
Interface to CRANK signals
----------------------------------------------*/
u16 VIOS_Set_Crank_Freq(u16 Crank_Freq)
{
			VIOS_Crank_Freq=Crank_Freq;
		return VIOS_Crank_Freq;
	
}

u16 VIOS_Get_Crank_Freq(u16 Misfire_Freq)
{
			
	return VIOS_Crank_Freq;
	
}

/*----------75CH165 Function-------------------*/
uint32_t VIOS_Read_HC165(unsigned char Cascades_Num)
{
 unsigned char i;    //临时变量
 uint32_t DATA1=0;	 //临时数据
	
	
	HC165_CLK_Set(); 
	HC165_SH_Set();	//启动 74HC165
	HC165_SH_Clr();
//-------------------------------------------------//	

	if(Cascades_Num<4)
	{
		for(i=0;i<Cascades_Num*8;i++)	//读8次数据
		{
			//低电平时读取数据 
				
			if(HC165_DS)		//数据高位在前
				DATA1 |= 0x800000>>i;//读取到电平数据
			
			HC165_CLK_Clr();
			HC165_CLK_Set(); 	//数据移位

		}
		DA_75CH165[0]=DATA1;			//读取到一个数据 并存储
		HC165_SH_Clr();	 
	}
	
VIOS_VVT_In_Phase_Final=VIOS_Read_VVT_In_Phase(0);
VIOS_VVT_Exh_Phase_Final=VIOS_Read_VVT_Exh_Phase(0);
	
	VIOS_Read_Knock_EnableStatus();
	VIOS_Read_Misfire_EnableStatus();
	return DA_75CH165[0];
}

uint32_t VVT_In_Phase,VIOS_VVT_In_Phase,VIOS_VVT_In_Phase_2;
uint32_t VIOS_Read_VVT_In_Phase(unsigned char bytePos)
{

	
	VVT_In_Phase= 0xFF-(DA_75CH165[0]>>16)&0xFF;
	
	VIOS_VVT_In_Phase=(VVT_In_Phase>>4)&0xF;
	VIOS_VVT_In_Phase_2=(VVT_In_Phase)&0xF;
	
return VIOS_VVT_In_Phase*10+VIOS_VVT_In_Phase_2;
}


uint32_t VVT_Exh_Phase,sT_VIOS_VVT_Exh_Phase,sT_VIOS_VVT_Exh_Phase_2;

uint32_t VIOS_Read_VVT_Exh_Phase(unsigned char bytePos)
{

	
	VVT_Exh_Phase= 0xFF-(DA_75CH165[0]>>8)&0xFF;
	
		sT_VIOS_VVT_Exh_Phase=(VVT_Exh_Phase>>4)&0xF;
	sT_VIOS_VVT_Exh_Phase_2=(VVT_Exh_Phase)&0xF;
	
return sT_VIOS_VVT_Exh_Phase*10+sT_VIOS_VVT_Exh_Phase_2;
	

}

void VIOS_Read_Misfire_EnableStatus()
{
	
	
	Misfire_EnableStatus= (DA_75CH165[0])&0xF;
	
	VIOS_Misfire_EnableBit[0]=(Misfire_EnableStatus>>0)&0x1;
	VIOS_Misfire_EnableBit[1]=(Misfire_EnableStatus>>1)&0x1;
	VIOS_Misfire_EnableBit[2]=(Misfire_EnableStatus>>2)&0x1;
	VIOS_Misfire_EnableBit[3]=(Misfire_EnableStatus>>3)&0x1;
	

}

void VIOS_Read_Knock_EnableStatus()
{
	

	
	Knock_EnableStatus= (DA_75CH165[0]>>4)&0xF;
	
	VIOS_Knock_EnableBit[0]=(Knock_EnableStatus>>0)&0x1;
	VIOS_Knock_EnableBit[1]=(Knock_EnableStatus>>1)&0x1;
	VIOS_Knock_EnableBit[2]=(Knock_EnableStatus>>2)&0x1;
	VIOS_Knock_EnableBit[3]=(Knock_EnableStatus>>3)&0x1;

}



/***--------
KNOCK
----------*/
u8 VIOS_Set_KNOCK_Cylinder(u8 cylinder)
{
			VIOS_KNOCK_Cylinder=cylinder;
	return VIOS_KNOCK_Cylinder;
	
}

u8 VIOS_Get_KNOCK_Cylinder()
{

		return VIOS_KNOCK_Cylinder;
}


u8 VIOS_Set_KNOCK_Enable(u8 cylinder,u8 Enable_Cyl)
{
	//u8 KNOCK_Enable=Enable_Cyl;
//	if((KNOCK_Enable>>cylinder)&0x01)
//		return true;
	if((VIOS_Knock_EnableBit[cylinder])==0x01)
		return true;
	else
		return false;
			
	
}

u8 VIOS_Get_KNOCK_Enable()
{

		return VIOS_KNOCK_Enable;
}





