#ifndef MY_INITTASK_H
#define MY_INITTASK_H


#include "stm32f10x.h"
#include "vios.h"
#include "CRANK.h"	
#include "VEHSPD.h"
#include "MISF.h"
#include "stdbool.h"
#include "VEHSPD.h"
extern uint16_t timer4_It_Cnt_Raw,MINT_Cyl_Num;
extern uint32_t  DA_75CH165[4];
extern uint32_t Main_74HC165;
extern uint16_t Debug_Crank_Freq_Display;

void My_InitTask(void); 
   
void RCC_Configuration(void);
void NVIC_Configuration(void);
void GPIO_Configuration(void);
void SysTick_Configuration(void);

/*:::::::Enable Timer4 and PWM output::::::::::::::*/
void Init_TIMER(uint16_t period);
void Init_PWM(uint16_t Dutyfactor);

/* Private function prototypes -----------------------------------------------*/
void ADC_Configuration(void);
void DMA_Configuration(void);
void AD_ConvertFunction(void);





#endif /* MY_INITTASK_H */
















