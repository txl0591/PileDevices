/*********************************************************
 Copyright (C),2011-2017,Electronic Technology Co.,Ltd.
 File name: 		hw_flash.c
 Author: 			Txl
 Version: 			1.0
 Date: 				2014-12-23
 Description: 		
 History: 			
 					
   1.Date:	 		2014-12-23
 	 Author:	 	Txl
 	 Modification:  Created file
 	 
*********************************************************/
#include "hw_define.h"

/*************************************************
 Function:		hw_flash_init
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void hw_flash_init(void)
{
	FLASH_UnlockBank1();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
}

/*************************************************
 Function:		hw_flash_deinit
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void hw_flash_deinit(void)
{
	FLASH_LockBank1();
}

/*************************************************
 Function:		hw_flash_erase
 Descroption:	 
 Input: 
	1.SectorAddress
 Output: 
 Return: 	
 Other:  
*************************************************/
uint32_t hw_flash_erase(uint32_t SectorAddress)
{
	#if defined(STM32L1XX_MD) || defined(STM32L1XX_HD)|| defined(STM32L1XX_MD_PLUS)
	FLASH_ClearFlag(FLASH_FLAG_PGAERR | FLASH_FLAG_OPTVERR);
	return FLASH_ErasePage(SectorAddress);
	#else
	return FLASH_ErasePage(SectorAddress);
	#endif /* STM32L1XX_XD */
}


/*************************************************
 Function:		hw_flash_program
 Descroption:	 
 Input: 
	1.SectorAddress
	2.*buf
	3.buflen
 Output: 
 Return: 	
 Other:  
*************************************************/
int32_t hw_flash_program(uint32_t SectorAddress, uint8_t *buf, uint32_t buflen) 
{
    uint32_t i = 0;
    uint32_t BaseAddress = SectorAddress;
	volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
	if(NULL == buf || 0 == buflen)
	{
		return -1;
	}
	
    while((i < buflen) && (FLASHStatus == FLASH_COMPLETE))
    {
      FLASHStatus = FLASH_ProgramHalfWord(BaseAddress, *(uint16_t*)buf);
      i = i+2;
      BaseAddress = BaseAddress + 2;
      buf = buf + 2;
    }

    return BaseAddress;
}

/*************************************************
 Function:		hw_flash_read
 Descroption:	 
 Input: 
	1.SectorAddress
	2.*buf
	3.buflen
 Output: 
 Return: 	
 Other:  
*************************************************/
uint32_t hw_flash_read(uint32_t SectorAddress, uint8_t *buf , uint32_t buflen)
{
	uint32_t size = buflen;
	uint32_t len = 0;
	
	if(NULL == buf || 0 == buflen)
	{
		return 0;
	}

	while(size > 0)
	{
		*(buf+len) = *(uint8_t*)(SectorAddress+len);
		len++;
		size--;
	}
        
    return len;
}
