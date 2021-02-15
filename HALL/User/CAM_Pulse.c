#include "CAM_Pulse.h"
#include "My_InitTask.h"  

#define CAM_out GPIOA
#define CAM_out GPIO_Pin_0


uint16_t CAM_out_Phase[]={15,19,23,26,45,49,75,79,83,109}; 
uint16_t CAM_in_Phase[]={5,9,35,39,43,69,95,99,103,106}; 

void CAM_Pulse_Output(uint16_t CAM_Phase_ofst[],uint16_t CAM_FirstTooth_Offset,GPIO_TypeDef* CAM_out_Port, uint16_t CAM_out_Pin)
{

						if((timer4_It_Cnt_Raw>(CAM_Phase_ofst[0]*6))&&(timer4_It_Cnt_Raw<=(CAM_Phase_ofst[1]*6)))
						{
							GPIO_ResetBits(CAM_out_Port, CAM_out_Pin);
						}
						else if((timer4_It_Cnt_Raw>(CAM_Phase_ofst[1]*6))&&(timer4_It_Cnt_Raw<=(CAM_Phase_ofst[2]*6)))
						{
							GPIO_SetBits(CAM_out_Port, CAM_out_Pin);
						}
							else if((timer4_It_Cnt_Raw>(CAM_Phase_ofst[2]*6))&&(timer4_It_Cnt_Raw<=(CAM_Phase_ofst[3]*6)))
					{
							GPIO_ResetBits(CAM_out_Port, CAM_out_Pin);
						}
						else if((timer4_It_Cnt_Raw>CAM_Phase_ofst[3]*6)&&(timer4_It_Cnt_Raw<=CAM_Phase_ofst[4]*6))
						{
							GPIO_SetBits(CAM_out_Port, CAM_out_Pin);
						}
							else if((timer4_It_Cnt_Raw>CAM_Phase_ofst[4]*6)&&(timer4_It_Cnt_Raw<=CAM_Phase_ofst[5]*6))
						{
							GPIO_ResetBits(CAM_out_Port, CAM_out_Pin);
						}
						else if((timer4_It_Cnt_Raw>CAM_Phase_ofst[5]*6)&&(timer4_It_Cnt_Raw<=CAM_Phase_ofst[6]*6))
						{
							GPIO_SetBits(CAM_out_Port, CAM_out_Pin);
						}
						else if((timer4_It_Cnt_Raw>CAM_Phase_ofst[6]*6)&&(timer4_It_Cnt_Raw<=CAM_Phase_ofst[7]*6))
						{
							GPIO_ResetBits(CAM_out_Port, CAM_out_Pin);
						}
						else if((timer4_It_Cnt_Raw>CAM_Phase_ofst[7]*6)&&(timer4_It_Cnt_Raw<=CAM_Phase_ofst[8]*6))
						{
							GPIO_SetBits(CAM_out_Port, CAM_out_Pin);
						}
							else if((timer4_It_Cnt_Raw>CAM_Phase_ofst[8]*6)&&(timer4_It_Cnt_Raw<=CAM_Phase_ofst[9]*6))
						{
							GPIO_ResetBits(CAM_out_Port, CAM_out_Pin);
						}
							else if(timer4_It_Cnt_Raw>CAM_Phase_ofst[9]*6)
						{
							GPIO_SetBits(CAM_out_Port, CAM_out_Pin);
						}
						
}
