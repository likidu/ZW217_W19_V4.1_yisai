/****************************************************************************
** File Name:      mail_welcom.c                                            *
** Author:                                                                    *
** Date:           11/10/2010                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:                                                               *
*****************************************************************************
**                         Important Edit History                            *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 11/2010      Jiaoyou.wu        Create
** 
****************************************************************************/
#define _MAIL_WELCOM_C_  

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_push_mail_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include <string.h>
#include "sci_types.h"
#include "mail_utility.h"
#include "mail_message.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                           *
 **--------------------------------------------------------------------------*/ 

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

PUBLIC void  MMIeMAIL_Test(void)
{
	MAIL_MESSAGE_T *message_ptr = PNULL;
    //wchar name[15] = {'d',':','\\','a','b','c','d','.','t','x','t'};
    //wchar name[15] = {'d',':','\\','a','.','t','x','t'};
    //wchar name[15] = {'d',':','\\','a','b','.','t','x','t'};
	wchar name[15] = L"d:\\ab.txt";
    //wchar name[15] = {'d',':','\\','1','.','t','x','t'};
    
    //wchar * name = "d:\\a.txt";

	//SCI_TRACE_LOW:"\"\"\"\"\"\"\"\"\"\"1"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_WELCOM_57_112_2_18_2_48_5_194,(uint8*)"");
	message_ptr =(MAIL_MESSAGE_T *) MMIEMAIL_CreateMessage(name, 1000, Inbox);

    

	if(message_ptr)
    {
    	message_ptr->fine = TRUE;
    	ParseMessage(message_ptr, TRUE);
    }

	MMIEMAIL_DestroyMessage(message_ptr);

    /*AAA *aaa = PNULL;
	uint16 gg = MAX_MAIL_LENGTH;
    aaa = (AAA *)SCI_ALLOC(sizeof(AAA));

    
	//SCI_TRACE_LOW:"\"\"\"\"\"\"\"\"\"\""
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_WELCOM_75_112_2_18_2_48_5_195,(uint8*)"");
	aaa = testtest(aaa);
	//SCI_TRACE_LOW:"\"\"\"\"\"\"\"\"\"\"%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_WELCOM_77_112_2_18_2_48_5_196,(uint8*)"d", aaa);
    */
    
}






