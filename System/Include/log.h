/*********************************************************
 Copyright (C),2012-2018,Electronic Technology Co.,Ltd.
 File name: 		log.h
 Author: 			Txl
 Version: 			1.0
 Date: 				2015-1-10
 Description: 		
 History: 			
 					
   1.Date:	 		2015-1-10
 	 Author:	 	Txl
 	 Modification:  Created file
 	 
*********************************************************/
#ifndef _LOG_H_
#define _LOG_H_

#include "hw_define.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define _DEBUG_             1    

#define LEVEL_NET           0x01
#define LEVEL_DRIVER        0x02
#define LEVEL_STORAGE       0x04
#define LEVEL_LOGIC         0x08
#define LEVEL_SCAN          0x10
#define LEVEL_EXPER         0x20

#define LEVEL_ALL           (LEVEL_NET|LEVEL_DRIVER|LEVEL_STORAGE|LEVEL_LOGIC|LEVEL_SCAN|LEVEL_EXPER)
#if _DEBUG_
#define LEVEL_DEBUG         (LEVEL_DRIVER|LEVEL_STORAGE|LEVEL_SCAN|LEVEL_LOGIC|LEVEL_EXPER|LEVEL_DRIVER)
#else
#define LEVEL_DEBUG         0x00
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* ifndef _LOG_H_ Edit By Txl 2015-1-10 */

