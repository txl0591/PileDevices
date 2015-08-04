/*********************************************************
  Copyright (C), 2013-2016
  File name:	logic_scan.h
  Author:   	
  Version:   	1.0
  Date: 		13.11.23
  Description:  
  History:                   
    1. Date:
       Author:
       Modification:
    2. ...
*********************************************************/

#ifndef _LOGIC_SCAN_H_
#define _LOGIC_SCAN_H_

#define MAX_VOLAGE         (4.0664)       
#define MAX_VOLAGE_BASE    0x7FFFF

/*************************************************
 Function:		logic_sensor_update
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void logic_sensor_update(uint8_t Id, uint8_t Channel);

/*************************************************
 Function:		logic_sensor_read_drift
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
float logic_sensor_read_drift(uint8_t Index);

/*************************************************
  Function:		logic_scan_proc
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void logic_scan_proc(void);

/*************************************************
  Function:		logic_scan_init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void logic_scan_init(void);

/*************************************************
 Function:		logic_sensor_get_voltage
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void logic_sensor_get_voltage(uint8_t Id);


#endif


