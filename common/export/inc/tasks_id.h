/******************************************************************************
 ** File Name:      block_id.h                                                *
 ** Author:         Richard Yang                                              *
 ** DATE:           16/10/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 16/10/2001     Richard.Yang     Create.                                   *
 ******************************************************************************/

#ifndef _TASKS_ID_H
#define _TASKS_ID_H

#include "tasks_def.h"
#include "oscfg.h"

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
// User can define other static task ids 
// from USER_CUSTOMIZE_TASK_ID_START 
// to   USER_CUSTOMIZE_TASK_ID_END
// NOTE: The number of static tasks user defined 
//       MUST be no more than USER_CUSTOMIZE_TASK_MAX_NUM
#define USER_CUSTOMIZE_TASK_MAX_NUM		7
#define USER_CUSTOMIZE_TASK_ID_START  	93
#define USER_CUSTOMIZE_TASK_ID_END    	99 

#define SYSTEM_STATIC_THREAD_MAX_NUM	93

typedef enum
{
/* Here is for Refence design. From 0 to 11*/	
    CM = 0,          			// Channel manager thread defination.
    DIAG = 1,            	    // Diagnostic thread defination.   
    KPDSVR = 2, 
    RESERVED3 = 3,				
    AUDIO = 4,                  // AUDIO,
    VIDEO = 5,                  // VIDEO,
    EFS = 6,              
    CHR_SVR = 7,   	
    UDISK = 8,			  	
    TIMESVR = 9,
    FORBIDDEN_USE10,			// Reserved for MIDI(for D platform) and MP3,      	
    P_TPP,			            // Touch Panel Task

/* Here is for Layer1 design. From 12 to 14*/	
    P_LAYER1 = 12,
    TESTDSPTOL1 = 13,  			// just for test 
    TESTPSTOL1 = 14,   			// just for test

/* Here is for simat design. From 15 to 17*/	
    P_SIMAT_IMPL = 15,
    P_SIMAT_DUM = 16,
    P_SIMAT = 17, 

/* Here is for Protocol Stack design. From 18 to 20*/	
    P_RRA = 18,			
    P_MN = 19,
    RECORD = 20,    
    
    P_APP = 21,		// used for APP
    
/* Here is for SIM design. From 22 to 23*/	
    P_SIM = 22,
    P_SIM_WIN_DUMMY = 23, 		

    P_ATC = 24,
    FFS =25,// RESERVED25 = 25, 
    MPEG = 26,    
    
    CUSTOMIZED_STATIC_TASKS		// 12 tasks id

    // Add your tasks id here.
    // But all static tasks MUST be less than SYSTEM_STATIC_THREAD_MAX_NUM !!!
    // .....
    STL_COMPACTION = 39,
	P_CPANEL = 40,
	P_PSDummy = 41,

	P_NVSMS        = 42,
	P_MN_2         = 43,
	P_MN_3         = 44,
	P_SIM_2        = 45,
	P_SIM_3        = 46,
	P_processMM_2  = 47,
	P_processMM_3  = 48,
	P_GMM_2        = 49,
	P_GMM_3        = 50,
	P_SMR_2        = 51,
	P_SMR_3        = 52,
	P_RRC_2        = 53,
	P_RRC_3        = 54,
	P_cmrouter_2   = 55,
	P_cmrouter_3   = 56,
	P_LOGSAVE      = 57,
	P_MN_4         = 58,
	P_SIM_4        = 59,
	P_processMM_4  = 60,
	P_GMM_4        = 61,
	P_SMR_4        = 62,
	P_RRC_4        = 63,
	P_cmrouter_4   = 64,
    P_SMR          = 65,
    P_SMC 	       = 66,
    P_SMC_2        = 67,
	P_SMC_3        = 68,
	P_SMC_4        = 69,
	
	P_NSWTH_1   = 70,
	P_NSWTH_2   = 71,
	P_NSWTH_3   = 72,
	P_NSWTH_4   = 73,
	P_TD_CALIBRATION = 74,
#ifdef FLASH_SUPPORT
	P_FLASH		 = 75,//FLASH_SUPPORT
#endif
   P_L1SIM = 76, //for ueit test
    P_SS             = 77,
    P_SS_2           = 78,
    P_SS_3           = 79,
    P_SS_4           = 80,
    
    P_CC            = 81,
    P_CC_2          = 82,
    P_CC_3          = 83,
    P_CC_4          = 84,
    P_SM            = 85,
#if !defined(PRODUCT_DM) && !defined(TIANLONG_MOCOR)
    P_processMM     = 86,
    P_GMM           = 87,
#endif
    P_SNDCP         = 88,
    P_LLC           = 89,
    P_DLC_SAPI3     = 90,
    
    #if defined(PRODUCT_DM) || defined(TIANLONG_MOCOR)
    P_DLC           = 91,//if it is dual mode code,define P_DLC task id here        
    #endif
    
    MAX_BLOCK_NUM = USER_CUSTOMIZE_TASK_ID_START
} thread_id_type;

#define UPM UDISK   // UPM is used for M platform
                    // UDISK is used for D platform
#define MIDI FORBIDDEN_USE10 // MIDI is used for D platform

#define P_TP P_TPP // Touch Panel task used in SC6800 platform

typedef enum
{
    STARUP_DYNA_TASK_ID = USER_CUSTOMIZE_TASK_ID_END + 1,
    CUSTOMIZED_DYNAMIC_TASKS
    P_TL,
    P_SMS, 
    P_NETWORK,
    MSG_BUF,
    SIO,
	P_SN,
	P_DSP,
    GPRS_LOG_MSG_ID = 0x90
}dyna_thread_id_type;


#if (!defined( _SUPPORT_GPRS_ )) && (!defined( PRODUCT_DM ))     
#ifndef P_Llentity
#define P_Llentity	0
#endif

#ifndef P_SMentity
#define P_SMentity	0
#endif

#ifndef P_SNDCPEntity
#define P_SNDCPEntity	0
#endif
#endif

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif /* End block_id.h*/
