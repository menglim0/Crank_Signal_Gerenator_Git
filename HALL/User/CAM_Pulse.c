#include "CAM_Pulse.h"
#include "My_InitTask.h"  

#define CAM_out GPIOA
#define CAM_out GPIO_Pin_0


//uint16_t CAM_out_Phase[]={10,20,30,40,50,60,70,80,90,100}; 
//uint16_t CAM_in_Phase[]={5,10,15,20,25,30,35,40,45,50}; 
uint16_t CAM_Count=0;


uint16_t CAM_in_Phase[]={22,45,52,75,82,89,112,119}; 
uint16_t CAM_out_Phase[]={12,35,42,65,72,79,102,109}; 
//uint16_t CAM_in_Phase[]={72,210,252,390,432,474,612}; 
//uint16_t CAM_out_Phase[]={132,270,312,450,492,534,672}; 
void CAM_Pulse_Output(uint16_t CAM_Phase_ofst[],uint16_t CAM_FirstTooth_Offset,GPIO_TypeDef* CAM_out_Port, uint16_t CAM_out_Pin)
{
	
	uint16_t CAM_Phase_Angle_AfterOffset[10],CAM_Phase_Angle_Offset_Limit; 
  uint8_t CAM_index=0;
	
	if(CAM_FirstTooth_Offset<CAM_Phase_ofst[0]*6)
	{
		CAM_Phase_Angle_Offset_Limit=CAM_FirstTooth_Offset;
	}
	else
	{
		CAM_Phase_Angle_Offset_Limit= CAM_Phase_ofst[0]*6-1;
	}
	
	
	for(CAM_index=0;CAM_index<8;CAM_index++)
	{
		CAM_Phase_Angle_AfterOffset[CAM_index]=CAM_Phase_ofst[CAM_index]*6-CAM_Phase_Angle_Offset_Limit -1;
		//CAM_Phase_Angle_AfterOffset[CAM_index]=CAM_Phase_ofst[CAM_index]*6;
	}
//	                                                                 

						if((timer4_It_Cnt_Raw>(CAM_Phase_Angle_AfterOffset[0]))&&(timer4_It_Cnt_Raw<=(CAM_Phase_Angle_AfterOffset[1])))
						{
							GPIO_SetBits(CAM_out_Port, CAM_out_Pin);
							CAM_Count=0;
						}
						else if((timer4_It_Cnt_Raw>(CAM_Phase_Angle_AfterOffset[1]))&&(timer4_It_Cnt_Raw<=(CAM_Phase_Angle_AfterOffset[2])))
						{
							GPIO_ResetBits(CAM_out_Port, CAM_out_Pin);
							CAM_Count++;
							
			
						}
							else if((timer4_It_Cnt_Raw>(CAM_Phase_Angle_AfterOffset[2]))&&(timer4_It_Cnt_Raw<=(CAM_Phase_Angle_AfterOffset[3])))
					{
							GPIO_SetBits(CAM_out_Port, CAM_out_Pin);
							CAM_Count++;
						}
						else if((timer4_It_Cnt_Raw>CAM_Phase_Angle_AfterOffset[3])&&(timer4_It_Cnt_Raw<=CAM_Phase_Angle_AfterOffset[4]))
						{
							GPIO_ResetBits(CAM_out_Port, CAM_out_Pin);
								CAM_Count++;
						}
							else if((timer4_It_Cnt_Raw>CAM_Phase_Angle_AfterOffset[4])&&(timer4_It_Cnt_Raw<=CAM_Phase_Angle_AfterOffset[5]))
						{
							GPIO_SetBits(CAM_out_Port, CAM_out_Pin);
								CAM_Count++;
						}
						else if((timer4_It_Cnt_Raw>CAM_Phase_Angle_AfterOffset[5])&&(timer4_It_Cnt_Raw<=CAM_Phase_Angle_AfterOffset[6]))
						{
							GPIO_ResetBits(CAM_out_Port, CAM_out_Pin);
								CAM_Count++;
						}
						else if((timer4_It_Cnt_Raw>CAM_Phase_Angle_AfterOffset[6])&&(timer4_It_Cnt_Raw<=CAM_Phase_Angle_AfterOffset[7]))
						{
							GPIO_SetBits(CAM_out_Port, CAM_out_Pin);
								CAM_Count++;
						}
//						else if((timer4_It_Cnt_Raw>CAM_Phase_Angle_AfterOffset[7])&&(timer4_It_Cnt_Raw<=CAM_Phase_Angle_AfterOffset[8]))
//						{
//							GPIO_ResetBits(CAM_out_Port, CAM_out_Pin);
//						}
//							else if((timer4_It_Cnt_Raw>CAM_Phase_Angle_AfterOffset[8])&&(timer4_It_Cnt_Raw<=CAM_Phase_Angle_AfterOffset[9]))
//						{
//							GPIO_SetBits(CAM_out_Port, CAM_out_Pin);
//								CAM_Count++;
//						}
//					
						else if(timer4_It_Cnt_Raw>CAM_Phase_Angle_AfterOffset[7])
						{
							GPIO_ResetBits(CAM_out_Port, CAM_out_Pin);
								CAM_Count++;
						}
		
						
}
