/*********************************************************
 Copyright (C),2011-2017,Electronic Technology Co.,Ltd.
 File name: 		appmain.c
 Author: 			Tangxl
 Version: 			1.0
 Date: 				2017-5-18
 Description: 		
 History: 			
 					
   1.Date:	 		2017-5-18
 	 Author:	 	Tangxl
 	 Modification:  Created file
   2. ... 
*********************************************************/
#include "logic.h"

/*************************************************
 Function:		appmain_proc
 Descroption:	app proc
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void appmain_proc(void)
{
	logic_proc();
}

/*************************************************
  Function:		appmain_init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void appmain_init(void)
{
    storage_init();
	logic_init();  
}

