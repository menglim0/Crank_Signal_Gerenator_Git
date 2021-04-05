#ifndef __FLEE_H__
#define __FLEE_H__
#include "stm32f10x.h"
#include "stdint.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//STM32 FLASH 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/13
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
//用户根据自己的需要设置
#define STM32_FLASH_SIZE 64 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1              //使能FLASH写入(0，不是能;1，使能)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
//FLASH解锁键值

uint32_t Flash_Find_Address(void);

extern uint8_t write_date[32];
 
void Flash_Write(void);
uint32_t FLASH_Read(uint32_t Address, void *Buffer, uint32_t Size);
void writeFlashTest(uint32_t start_address,uint8_t *data,uint8_t length);
static uint32_t FLASH_WriteNotCheck(uint32_t Address, const uint16_t *Buffer, uint32_t NumToWrite);
void Flash_ReadOnePage(uint32_t READ_START_ADDR,u16 SizeOf_u32,uint32_t *DataAddress);


#endif

















