#ifndef __FLEE_H__
#define __FLEE_H__
#include "stm32f10x.h"
#include "stdint.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//STM32 FLASH ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/13
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 64 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1              //ʹ��FLASHд��(0��������;1��ʹ��)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ
//FLASH������ֵ

uint32_t Flash_Find_Address(void);

extern uint8_t write_date[32];
 
void Flash_Write(void);
uint32_t FLASH_Read(uint32_t Address, void *Buffer, uint32_t Size);
void writeFlashTest(uint32_t start_address,uint8_t *data,uint8_t length);
static uint32_t FLASH_WriteNotCheck(uint32_t Address, const uint16_t *Buffer, uint32_t NumToWrite);
void Flash_ReadOnePage(uint32_t READ_START_ADDR,u16 SizeOf_u32,uint32_t *DataAddress);


#endif

















