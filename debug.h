/**************************************************************
**	FileName:debug.h
**	Date: Mar 17 2010 Ver 0.1
**	Author: amaork+work@gmail.com
**	Description: 打印调试信息调试语句
**
**************************************************************/

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>

#define		DEBUG_MQ	0	
#define		DEBUG_PWB	1
#define		DEBUG_IMG	2
#define		DEBUG_SYNC	3
#define		DEBUG_EXTD	4
#define		DEBUG_RCCP	5
#define		DEBUG_FPGA	6
#define 	DEBUG_RKEY	7
#define 	DEBUG_CLIST	8
#define		DEBUG_THREAD	9
#define 	DEBUG_SCANNER	10
#define		DEBUG_DOTCLK	11
#define		DEBUG_RDISPLAY	12
#define		DEBUG_CONFIGURE	13

typedef	struct{
	unsigned set;
	char name[10];
}DEBUG_INFO, *P_DEBUG_INFO;

/* Global*/
extern DEBUG_INFO debug[];

int debug_cmd_resolve(int argc, char **argv);


/* If define _DEBUG_ then will print error message to screen */

#ifdef _DEBUG_

	/* Show which file and line message */
	#define __DEBUG_MESG__(mesg)			do { fprintf(stderr, "%s[%s] --> [%d] --> %s\n", __FILE__,__func__, __LINE__, mesg);} while(0)
	#define __debug_mesg__(format, args...)	fprintf(stderr, format, args)
#else

	#define __DEBUG_MESG__(mesg)			do {} while(0)
	#define __debug_mesg__(format, args...)	do {} while(0) 
#endif

#endif

