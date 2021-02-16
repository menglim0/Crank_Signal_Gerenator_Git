#include "FLEE.h"
#include "stm32f10x.h"
#include "stm32f10x_flash.h"


#include "stdbool.h"
#include "stdlib.h"
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
 
//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.

//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else 
#define STM_SECTOR_SIZE	2048
#endif		 
uint64_t STMFLASH_BUF[STM_SECTOR_SIZE/2];//�����2K�ֽ�

#define STM32FLASH_PAGE_SIZE 1024
#define STM32FLASH_PAGE_NUM 64

#define STM32FLASH_BASE 0x08000000
#define STM32FLASH_END 0x08010000

#define FLASH_TYPEERASE_PAGES     0x00U  /*!<Pages erase only*/
#define FLASH_TYPEERASE_MASSERASE 0x02U  /*!<Flash mass erase activation*/

#define FLASH_TYPEPROGRAM_HALFWORD             0x01U  /*!<Program a half-word (16-bit) at a specified address.*/
#define FLASH_TYPEPROGRAM_WORD                 0x02U  /*!<Program a word (32-bit) at a specified address.*/
#define FLASH_TYPEPROGRAM_DOUBLEWORD           0x03U  /*!<Program a double word (64-bit) at a specified address*/

#define FLASH_BANK_1     1U /*!< Bank 1   */


uint32_t writeFlashData = 0x0;
uint32_t addr = 0x0800F800;
uint8_t bufr[10];
uint16_t FlashBuffer[STM32FLASH_PAGE_SIZE >> 1];
uint8_t write_date[32] = {1,2,3,4,5,6,6,7,8,9,10,11,12,13,14,15};

uint8_t First_index;
uint32_t start_addr,offset;
uint16_t i;


typedef struct
{
  uint32_t TypeErase;   /*!< TypeErase: Mass erase or page erase.
                             This parameter can be a value of @ref FLASHEx_Type_Erase */
  
  uint32_t Banks;       /*!< Select banks to erase when Mass erase is enabled.
                             This parameter must be a value of @ref FLASHEx_Banks */    
  
  uint32_t PageAddress; /*!< PageAdress: Initial FLASH page address to erase when mass erase is disabled
                             This parameter must be a number between Min_Data = 0x08000000 and Max_Data = FLASH_BANKx_END 
                             (x = 1 or 2 depending on devices)*/
  
  uint32_t NbPages;     /*!< NbPages: Number of pagess to be erased.
                             This parameter must be a value between Min_Data = 1 and Max_Data = (max number of pages - value of initial page)*/
                                                          
} FLASH_EraseInitTypeDef;

typedef enum
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;


FLASH_EraseInitTypeDef F_Addr;

uint8_t Flash_used_page;
bool Flash_Page_00_Full,Flash_Page_01_Full;

//FLASHд�����ݲ���

uint32_t Flash_Find_Address()
{
	uint32_t Data_Group[4];
	uint32_t Flash_Find_UsedIndex;
	for(Flash_Find_UsedIndex=0;Flash_Find_UsedIndex<1024;Flash_Find_UsedIndex=Flash_Find_UsedIndex+4)
	{
		Flash_ReadOnePage(0x0800F800+Flash_Find_UsedIndex,1,Data_Group);
		if(Data_Group[0]==0xFFFFFFFF)
		{
			break;
		}
	}
	
	return (0x0800F800+Flash_Find_UsedIndex);
	
}

void Flash_Write_16byte_with_Check(void)
{

	
		F_Addr.TypeErase = FLASH_TYPEERASE_PAGES;
		F_Addr.PageAddress = addr;
		F_Addr.NbPages = 1;
	
	   FLASH_Read(addr, bufr, sizeof(bufr));//��������1��ֵ
	
	for(i=0;i<1024;i+=16)
	{
		if(bufr[i]==0xff)
		{
			offset=i;
			start_addr=addr+offset;
			F_Addr.PageAddress = start_addr;
			First_index=i/16;
			write_date[0]=First_index;
			break;
		}
		else if(i>=1024-16)
		{
			Flash_used_page=1;
			Flash_Page_00_Full=true;
		}
	}
	writeFlashTest(F_Addr.PageAddress,write_date,16);//�̶�16�ֽڳ���

}


void writeFlashTest(uint32_t start_address,uint8_t *data,uint8_t length)
{
	//1������FLASH
	uint8_t write_i,data_i;  
	uint32_t data_temp[4];
	uint32_t PageError = 0;

	//2������FLASH
	//��ʼ��FLASH_EraseInitTypeDef
	FLASH_EraseInitTypeDef f;
	
	
	f.TypeErase = FLASH_TYPEERASE_PAGES;
	f.PageAddress = start_address;
	f.NbPages = 1;
	//����PageError
	FLASH_Unlock();
	//���ò�������
	if(Flash_Page_00_Full==true)
	{
		FLASH_ErasePage(f.PageAddress);
		Flash_Page_00_Full=false;
	}
	
	
	//3����FLASH��д
	for(write_i=0;write_i<8;write_i++)
	{
		for(data_i=0;data_i<4;data_i++)
		{
			data_temp[data_i]=data[data_i+write_i*4];
			//writeFlashData=writeFlashData+data[data_i+write_i*4]<<8;
			
		}
		
		writeFlashData=(data_temp[0])+(data_temp[1]<<8)+(data_temp[2]<<16)+(data_temp[3]<<24);
		FLASH_ProgramWord(f.PageAddress+4*write_i, writeFlashData);
		writeFlashData=0;
	}
//	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, writeFlashData);
//	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr+4, writeFlashData);
	//4����סFLASH
  FLASH_Lock();
}




uint32_t FLASH_Read(uint32_t Address, void *Buffer, uint32_t Size)
{
	uint32_t nread = Size;
	uint8_t* d = (uint8_t *)Buffer;
	const uint8_t* s = (const uint8_t *)Address;

	if (!Buffer || Address < STM32FLASH_BASE || Address >= STM32FLASH_END)
		return 0;

	while (nread >= sizeof(uint32_t) && (((uint32_t)s) <= (STM32FLASH_END - 4)))
	{
		*(uint32_t *)d = *(uint32_t *)s;
		d += sizeof(uint32_t);
		s += sizeof(uint32_t);
		nread -= sizeof(uint32_t);
	}

	while (nread && (((uint32_t)s) < STM32FLASH_END))
	{
		*d++ = *s++;
		nread--;
	}

	return Size - nread;
}




uint32_t FLASH_Write(uint32_t Address, const uint16_t *Buffer, uint32_t NumToWrite)
{
	uint32_t i = 0;
	uint32_t pagepos = 0;         // ҳλ��
	uint32_t pageoff = 0;         // ҳ��ƫ�Ƶ�ַ
	uint32_t pagefre = 0;         // ҳ�ڿ���ռ�
	uint32_t offset = 0;          // Address��FLASH�е�ƫ��
	uint32_t nwrite = NumToWrite; // ��¼ʣ��Ҫд���������

	/* �Ƿ���ַ */
	if (Address < STM32FLASH_BASE || Address > (STM32FLASH_END - 2) || NumToWrite == 0 || Buffer == NULL)
		return 0;

	/* ����FLASH */
	FLASH_Unlock();

	/* ����ƫ�Ƶ�ַ */
	offset = Address - STM32FLASH_BASE;

	/* ���㵱ǰҳλ�� */
	pagepos = offset / STM32FLASH_PAGE_SIZE;

	/* ����Ҫд���ݵ���ʼ��ַ�ڵ�ǰҳ�ڵ�ƫ�Ƶ�ַ */
	pageoff = ((offset % STM32FLASH_PAGE_SIZE) >> 1);

	/* ���㵱ǰҳ�ڿ���ռ� */
	pagefre = ((STM32FLASH_PAGE_SIZE >> 1) - pageoff);

	/* Ҫд������������ڵ�ǰҳ������ */
	if (nwrite <= pagefre)
		pagefre = nwrite;

	while (nwrite != 0)
	{
		/* ����Ƿ�ҳ */
		if (pagepos >= STM32FLASH_PAGE_NUM)
			break;

		/* ��ȡһҳ */
		FLASH_Read(STM32FLASH_BASE + pagepos * STM32FLASH_PAGE_SIZE, FlashBuffer, STM32FLASH_PAGE_SIZE);

		/* ����Ƿ���Ҫ���� */
		for (i = 0; i < pagefre; i++)
		{
			if (*(FlashBuffer + pageoff + i) != 0xFFFF) /* FLASH������Ĭ������ȫΪ0xFF */
				break;
		}

		if (i < pagefre)
		{
			uint32_t count = 0;
			uint32_t index = 0;
			uint32_t PageError = 0;
			FLASH_EraseInitTypeDef pEraseInit;

			/* ����һҳ */
			pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
			pEraseInit.PageAddress = STM32FLASH_BASE + pagepos * STM32FLASH_PAGE_SIZE;
			pEraseInit.Banks = FLASH_BANK_1;
			pEraseInit.NbPages = 1;
			if (FLASH_ErasePage(pEraseInit.PageAddress) != FLASH_COMPLETE)
				break;

			/* ���Ƶ����� */
			for (index = 0; index < pagefre; index++)
			{
				*(FlashBuffer + pageoff + index) = *(Buffer + index);
			}

			/* д��FLASH */
			count = FLASH_WriteNotCheck(STM32FLASH_BASE + pagepos * STM32FLASH_PAGE_SIZE, FlashBuffer, STM32FLASH_PAGE_SIZE >> 1);
			if (count != (STM32FLASH_PAGE_SIZE >> 1))
			{
				nwrite -= count;
				break;
			}
		}
		else
		{
			/* ���������ֱ��д */
			uint32_t count = FLASH_WriteNotCheck(Address, Buffer, pagefre);
			if (count != pagefre)
			{
				nwrite -= count;
				break;
			}
		}

		Buffer += pagefre;         /* ��ȡ��ַ����         */
		Address += (pagefre << 1); /* д���ַ����         */
		nwrite -= pagefre;         /* ����ʣ��δд�������� */

		pagepos++;     /* ��һҳ           */
		pageoff = 0;   /* ҳ��ƫ�Ƶ�ַ����  */

		/* ����ʣ���������´�д�������� */
		pagefre = nwrite >= (STM32FLASH_PAGE_SIZE >> 1) ? (STM32FLASH_PAGE_SIZE >> 1) : nwrite;
	}

	/* ����FLASH */
	FLASH_Lock();

	return ((NumToWrite - nwrite) << 1);
}


static uint32_t FLASH_WriteNotCheck(uint32_t Address, const uint16_t *Buffer, uint32_t NumToWrite)
{
	uint32_t nwrite = NumToWrite;
	uint32_t addrmax = STM32FLASH_END - 2;

	while (nwrite)
	{
		if (Address > addrmax)
			break;

		FLASH_ProgramHalfWord(Address, *Buffer);
		if ((*(__IO uint16_t*) Address) != *Buffer)
			break;

		nwrite--;
		Buffer++;
		Address += 2;
	}
	return (NumToWrite - nwrite);
}

//READ_START_ADDR 	-- ��ʼд�ĵ�ַ
//SizeOf_u32 		-- Ҫ������32λ��˫�֣�������
//DataAddress		-- �ȴ����������������飨32λ�ģ�
///
void Flash_ReadOnePage(uint32_t READ_START_ADDR,u16 SizeOf_u32,uint32_t *DataAddress)
{	
	uint32_t Address;
	u16 Remain_u32 = SizeOf_u32;
	/* ���д��������Ƿ���ȷ */
	Address = READ_START_ADDR;
	while ((Remain_u32 > 0)) 
	{
		 DataAddress[SizeOf_u32-Remain_u32] = (*(__IO uint32_t*) Address);
		Address += 4;
		Remain_u32--;	
	}
}
 
//ע����� 
//1����flash����д�������һ��Ҫ��ѭ���Ȳ�������д�롱��ԭ�� 
//2��ע�⵽stm32����flash�Ĳ�������������ҳΪ��λ���У���д������������16λ���ֿ������Ϊ��λ�������ҳд������д���16λ�������ݽ�����stm32�ڲ����ߴ��� 
//3������stm32������flash��̲���ʱ��д���������������ڲ���RC������HSI�� 
//4��ע��stm32������flash���ֻ��10����ظ���д���������ڣ����������ڳ����з�����ѭ����flash���г������ظ���д��










