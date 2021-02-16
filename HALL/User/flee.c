#include "FLEE.h"
#include "stm32f10x.h"
#include "stm32f10x_flash.h"


#include "stdbool.h"
#include "stdlib.h"
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
 
//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.

//WriteAddr:起始地址(此地址必须为2的倍数!!)
//pBuffer:数据指针
//NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //字节
#else 
#define STM_SECTOR_SIZE	2048
#endif		 
uint64_t STMFLASH_BUF[STM_SECTOR_SIZE/2];//最多是2K字节

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

//FLASH写入数据测试

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
	
	   FLASH_Read(addr, bufr, sizeof(bufr));//读出扇区1的值
	
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
	writeFlashTest(F_Addr.PageAddress,write_date,16);//固定16字节长度

}


void writeFlashTest(uint32_t start_address,uint8_t *data,uint8_t length)
{
	//1、解锁FLASH
	uint8_t write_i,data_i;  
	uint32_t data_temp[4];
	uint32_t PageError = 0;

	//2、擦除FLASH
	//初始化FLASH_EraseInitTypeDef
	FLASH_EraseInitTypeDef f;
	
	
	f.TypeErase = FLASH_TYPEERASE_PAGES;
	f.PageAddress = start_address;
	f.NbPages = 1;
	//设置PageError
	FLASH_Unlock();
	//调用擦除函数
	if(Flash_Page_00_Full==true)
	{
		FLASH_ErasePage(f.PageAddress);
		Flash_Page_00_Full=false;
	}
	
	
	//3、对FLASH烧写
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
	//4、锁住FLASH
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
	uint32_t pagepos = 0;         // 页位置
	uint32_t pageoff = 0;         // 页内偏移地址
	uint32_t pagefre = 0;         // 页内空余空间
	uint32_t offset = 0;          // Address在FLASH中的偏移
	uint32_t nwrite = NumToWrite; // 记录剩余要写入的数据量

	/* 非法地址 */
	if (Address < STM32FLASH_BASE || Address > (STM32FLASH_END - 2) || NumToWrite == 0 || Buffer == NULL)
		return 0;

	/* 解锁FLASH */
	FLASH_Unlock();

	/* 计算偏移地址 */
	offset = Address - STM32FLASH_BASE;

	/* 计算当前页位置 */
	pagepos = offset / STM32FLASH_PAGE_SIZE;

	/* 计算要写数据的起始地址在当前页内的偏移地址 */
	pageoff = ((offset % STM32FLASH_PAGE_SIZE) >> 1);

	/* 计算当前页内空余空间 */
	pagefre = ((STM32FLASH_PAGE_SIZE >> 1) - pageoff);

	/* 要写入的数据量低于当前页空余量 */
	if (nwrite <= pagefre)
		pagefre = nwrite;

	while (nwrite != 0)
	{
		/* 检查是否超页 */
		if (pagepos >= STM32FLASH_PAGE_NUM)
			break;

		/* 读取一页 */
		FLASH_Read(STM32FLASH_BASE + pagepos * STM32FLASH_PAGE_SIZE, FlashBuffer, STM32FLASH_PAGE_SIZE);

		/* 检查是否需要擦除 */
		for (i = 0; i < pagefre; i++)
		{
			if (*(FlashBuffer + pageoff + i) != 0xFFFF) /* FLASH擦出后默认内容全为0xFF */
				break;
		}

		if (i < pagefre)
		{
			uint32_t count = 0;
			uint32_t index = 0;
			uint32_t PageError = 0;
			FLASH_EraseInitTypeDef pEraseInit;

			/* 擦除一页 */
			pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
			pEraseInit.PageAddress = STM32FLASH_BASE + pagepos * STM32FLASH_PAGE_SIZE;
			pEraseInit.Banks = FLASH_BANK_1;
			pEraseInit.NbPages = 1;
			if (FLASH_ErasePage(pEraseInit.PageAddress) != FLASH_COMPLETE)
				break;

			/* 复制到缓存 */
			for (index = 0; index < pagefre; index++)
			{
				*(FlashBuffer + pageoff + index) = *(Buffer + index);
			}

			/* 写回FLASH */
			count = FLASH_WriteNotCheck(STM32FLASH_BASE + pagepos * STM32FLASH_PAGE_SIZE, FlashBuffer, STM32FLASH_PAGE_SIZE >> 1);
			if (count != (STM32FLASH_PAGE_SIZE >> 1))
			{
				nwrite -= count;
				break;
			}
		}
		else
		{
			/* 无需擦除，直接写 */
			uint32_t count = FLASH_WriteNotCheck(Address, Buffer, pagefre);
			if (count != pagefre)
			{
				nwrite -= count;
				break;
			}
		}

		Buffer += pagefre;         /* 读取地址递增         */
		Address += (pagefre << 1); /* 写入地址递增         */
		nwrite -= pagefre;         /* 更新剩余未写入数据量 */

		pagepos++;     /* 下一页           */
		pageoff = 0;   /* 页内偏移地址置零  */

		/* 根据剩余量计算下次写入数据量 */
		pagefre = nwrite >= (STM32FLASH_PAGE_SIZE >> 1) ? (STM32FLASH_PAGE_SIZE >> 1) : nwrite;
	}

	/* 加锁FLASH */
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

//READ_START_ADDR 	-- 开始写的地址
//SizeOf_u32 		-- 要读出的32位（双字）的数量
//DataAddress		-- 等待被读出的数据数组（32位的）
///
void Flash_ReadOnePage(uint32_t READ_START_ADDR,u16 SizeOf_u32,uint32_t *DataAddress)
{	
	uint32_t Address;
	u16 Remain_u32 = SizeOf_u32;
	/* 检查写入的数据是否正确 */
	Address = READ_START_ADDR;
	while ((Remain_u32 > 0)) 
	{
		 DataAddress[SizeOf_u32-Remain_u32] = (*(__IO uint32_t*) Address);
		Address += 4;
		Remain_u32--;	
	}
}
 
//注意事项： 
//1、对flash进行写入操作，一定要遵循“先擦除，后写入”的原则 
//2、注意到stm32内置flash的擦除操作都是以页为单位进行，而写入操作则必须以16位半字宽度数据为单位，允许跨页写，尝试写入非16位半字数据将导致stm32内部总线错误。 
//3、进行stm32的内置flash编程操作时（写或擦除），必须打开内部的RC振荡器（HSI） 
//4、注意stm32的内置flash最多只有10万次重复擦写的生命周期，谨记切勿在程序中放任死循环对flash进行持续地重复擦写。










