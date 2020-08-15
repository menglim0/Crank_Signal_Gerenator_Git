#ifndef MY_INITTASK_H
#define MY_INITTASK_H


#include "stm32f10x.h"
	






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
//void RCC_Configuration(void);
//void GPIO_Configuration(void);
//void NVIC_Configuration(void);
void DMA_Configuration(void);

void AD_ConvertFunction(void);

extern uint16_t Number_Of_Teeth;



#endif /* MY_INITTASK_H */
















