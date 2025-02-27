/******************************************************************************
 ** File Name:      atc.c                                                     *
 ** Author:         Steven & Ivan                                             *
 ** Date:           05/24/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file is the main file of ATC module. It contains     *
 **                 most of the functions which control the ATC line read,    *
 **                 process, error handle and others.                         *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 05/24/2002     Steven           Create.                                   *
 ** 06/20/2002     Ivan             start to edit and modity                  * 
 ** 08/20/2002     Xueli            Modify and add   *
 ******************************************************************************/

/*******************************************************************************
 **                        ATC Header Files                                    *
 ******************************************************************************/
#include "base_trc.h"
#include "atc.h"
#include "atc_common.h"
#include "atc_malloc.h"
#include "atc_gsm_ss.h"
#include "atc_gsm.h"

#include "atc_gsm_sms.h"

 
#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)
#include "atc_phonebook.h"
#endif

#include "atc_basic_cmd.h"
#include "dal_chr.h"
#include "mnclassmark_api.h"
#include "atc_tcp_if.h"
#include "env_atc_signal.h"
#include "sig_code.h"
#include "atc_port.h"
#include "atc_ppp.h"
#include "atc_tcp_if.h"
#include "dal_audio.h"
#include "in_message.h"
#include "socket_types.h"

#ifdef _MUX_ENABLE_
#include "atc_mux.h"
#endif

#ifdef _SUPPORT_GPRS_
    #include "mn_api.h"
#endif

#include "sci_service.h"
#include "dal_power.h"

#ifdef ATC_SAT_ENABLE
#ifndef _GSM_ONLY_
#include "atc_gprs_modem.h"
#endif
#endif

#if defined (BLUETOOTH_SUPPORT_SPRD_BT)
#include "bt_abs.h"
#if defined(BLE_INCLUDED) || defined(BT_BLE_SUPPORT)
#include "bt_ble_abs.h"
#endif
#endif

#ifdef _MNGPS_SUPPORT_
#include "atc_gps.h"
#endif
#ifdef PPP_USB
#include "ppp_mp_api.h"
#endif
/**---------------------------------------------------------------------------*
 **                         Constant                                   *
 **---------------------------------------------------------------------------*/
#if defined (BLUETOOTH_SUPPORT_SPRD_BT)
typedef struct _BT_SIGNAL
{
    uint16           signalCode;        // the signalCode
    uint16           signalSize;        // signal size
    uint32           pre;               // previous signal
    uint32           succ;              // succ signal
    uint32           send;
    BT_MSG_T         content;           // message content
} BT_SIGNAL;

typedef void*					DPARAM;				/*!< param data pointer type */
#endif

/**---------------------------------------------------------------------------*
 **                         Extern Variable                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local Variable                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Extern Function Declaration                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local Function Declaration                                  *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Declare Variable                                  *
 **---------------------------------------------------------------------------*/
#ifndef _GSM_ONLY_
#ifdef ATC_SAT_ENABLE 
#ifdef TCPIP_SUPPORT
extern  void ATC_ProcessSocketEvent(ATC_CONFIG_T *atc_config_ptr,xSignalHeaderRec *sig_ptr);
#endif
#endif/*#ifdef ATC_SAT_ENABLE */
#endif
MN_DUAL_SYS_E       g_current_card_id = MN_DUAL_SYS_1;

#if defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)
BOOLEAN g_is_hang_up_data_call = FALSE;  // used as +++ATH flag
#endif

extern const ATC_INFO_T     g_atc_info_table[];
extern ATC_Func_Handle_F    g_atc_handle_table[];
extern const uint8*         g_atc_result_code_text[];

extern SCI_TMID             T_ATC_CALL_EXP_IND;

#ifndef _ULTRA_LOW_CODE_
extern SCI_TMID             T_ATC_POWEROFF_EXP_IND;
#if 0
extern BOOLEAN             g_is_stk_sendsms[MN_SYS_NUMBER];
#endif
#endif

#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)
const ATC_RSP_STR_T         g_atc_cpin_rsp[] =
{
    "+CPIN: READY",         // No farther entry needed
    "+CPIN: SIM PIN",       // ME is waiting for SIM PIN
    "+CPIN: SIM PIN2",      // ME is waiting for SIM PUK
    "+CPIN: SIM PUK",      // ME is waiting for SIM2
    "+CPIN: SIM PUK2",      // ME is waiting for SIM PUK2
    "+CPIN: BLOCK",          // The SIM card is blocked for the user
    "+CPIN: NOT READY"     
};
#endif

#if !defined( _ULTRA_LOW_CODE_) || defined(_ATC_SYNC_TOOL_)
static const ATC_SMS_RSP_STR_T s_sms_verbos_info[] =
{
    {300,   "ME failure"},
    {301,   "SMS service of ME reserved"},
    {302,   "Operation not allowed"},
    {303,   "Operation not supported"},
    {304,   "Invalid PDU mode parameter"},
    {305,   "Invalid text mode parameter"},
    {310,   "SIM not inserted"},
    {311,   "SIM PIN required"},
    {312,   "PH-SIM PIN required"},
    {313,   "SIM failure"},
    {315,   "SIM wrong"},
    {316,   "SIM PUK required"},
    {317,   "SIM PIN2 required"},
    {318,   "SIM PUK2 required"},
    {320,   "memory failure"},
    {321,   "Invalid memory index"},
    {322,   "SIM memory full"},
    {330,   "SC address unknown"},
    {331,   "no network service"},
    {332,   "network timeout"},
    {340,   "no +CNMA acknowledgement expected"},
    {500,   "unknown error"},
    {512,   "sms conflict with mt call"}
};
#endif

RSP_RESULT_STR     g_rsp_str;  
static ATC_GLOBAL_INFO_T        s_atc_global_info;
ATC_CONFIG_T             s_config_info;
static BOOLEAN is_close_atc_queue = FALSE;
ATC_CALLS_STATE_T               g_calls_state[MN_SYS_NUMBER] = {0};
/*lint -e752*/
extern MN_DUAL_SYS_E                g_atc_modem_dial_sys;
/*lint -e752*/

#ifdef _MUX_ENABLE_
extern uint16               s_setup_menu_len[MN_DUAL_SYS_MAX];

extern BOOLEAN              ppp_dialing_in_progress;
extern BOOLEAN              g_atc_international_call;

extern BOOLEAN is_cmux_recv;
extern MUX_SYS_PARA_T cmux_sys_para;

BOOLEAN is_first_sync = TRUE;

uint8 g_call_exp_link_id = 0;
uint8 g_mo_call_link_id = 0;
uint8 g_poweroff_exp_link_id =0;

ATC_LINK_CONFIG_T  s_link_config_info[ATC_MAX_LINK_ID];

ATC_REC_BUFFER atc_rec_buffer0;//default at buffer
ATC_REC_BUFFER atc_rec_bufferdebug;//default debug at buffer
ATC_BUF_T    atc_buffer[ATC_MAX_LINK_ID];

static uint32 cur_atc_cmd_size[ATC_MAX_LINK_ID];
static BOOLEAN     atc_is_echo;

static char   atc_end_ch1[ATC_MAX_LINK_ID];
static char   atc_end_ch2[ATC_MAX_LINK_ID];
static char   atc_bs_ch[ATC_MAX_LINK_ID];       // Backspace char of ATC
BOOLEAN g_atc_ignore_one_detach_cnf = FALSE;

extern BOOLEAN  RM_GetCalibrationPostMode(void);

#ifdef _MUX_REDUCED_MODE_
extern void MUX_EnableFlowCtl(void);
#endif
#endif

uint8 atc_urc_link_id = 0;

#ifndef _MUX_ENABLE_
extern BOOLEAN g_is_diag_cmd;
#endif

#define S_CONFIG_INFO_PTR       &s_config_info

#define MN_ATC_GPRS_RESUME_IND APP_MN_PUSH_GPRS_DATA_CNF

#ifdef ATC_SAT_ENABLE 
#ifndef _GSM_ONLY_
extern BOOLEAN  g_atc_ip_detach;
#endif
#endif

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
// Description : This function is the callback of CHR
// Global resource dependence : 
// Author:       Xueli
// Note:
/*****************************************************************************/
LOCAL void ATC_ChrCallBack(uint32 taskID, uint32 argc, void *argv);

/******************************************************************************/
// Description : identify if the message is the needed msg on pending state
// Global resource dependence : None
// Author : Ivan
// Note : None
/******************************************************************************/
LOCAL ATC_STATUS ATC_IdentifyMsg( // the identify result 
         int msg_type, 
         ATC_GLOBAL_INFO_T * atc_global_info_ptr // point to the ATC_GLOBAL_INFO_T struct
                          );

#endif

/******************************************************************************/
// Description : This function do some initialization work for ATC
// Global resource dependence : g_atc_info_table
//                              g_atc_handle_table
//                              g_atc_result_code_text
// Author : Steven  
// Note : This function should be called first on starting ATC task
/******************************************************************************/
LOCAL ATC_STATUS ATC_Initialize(void);  

/******************************************************************************/
// Description : This function reads a new AT command line from sio unit and 
//               parses it
// Global resource dependence : 
// Author : Steven
// Note :   Modify: Xueli
/******************************************************************************/
LOCAL ATC_STATUS ATC_RecNewLineSig(  // Return S_ATC_SUCCESS if success,
                                     // otherwise return error code.
        ATC_GLOBAL_INFO_T      *atc_global_info_ptr, // point to the ATC_GLOBAL_INFO_T struct.
        const xSignalHeaderRec *sig_ptr         // point to signal struct 
        );
/******************************************************************************/
// Description : This function processes the AT commands stored in the AT 
//               config struct one by one.
// Global resource dependence : None
// Author : Steven
// Note : None
/******************************************************************************/
LOCAL ATC_STATUS ATC_ProcessCmds( // Return S_ATC_SUCCESS if success,
                            // otherwise return error code.
        ATC_GLOBAL_INFO_T     *atc_global_info_ptr // point to the ATC_GLOBAL_INFO_T struct.
        );
/******************************************************************************/
// Description : This function despatches AT command to its handle.
// Global resource dependence : 
// Author : Steven
// Note : None
/******************************************************************************/
LOCAL ATC_STATUS ATC_DespatchCmd(  // Return S_ATC_STATUS if success,
                                   // otherwise return error code.
        ATC_GLOBAL_INFO_T   *atc_global_info_ptr, // point to the ATC_GLOBAL_INFO_T struct.
        ATC_CUR_INFO_LIST_T *atc_cur_info_ptr // Point to the info struct of
                                               // AT command
        );
/******************************************************************************/
// Description : This function checks the current received AT command.
// Global resource dependence : None
// Author : Steven
// Note : None
/******************************************************************************/
LOCAL ATC_STATUS ATC_CheckCmd(  // Return S_ATC_SUCCESS if command is valid,
                                // otherwise return error code.
        ATC_GLOBAL_INFO_T   *atc_global_info_ptr, // point to the ATC_GLOBAL_INFO_T struct.
        ATC_CUR_INFO_LIST_T *atc_cur_info_ptr  // Point to the info struct of AT command.
        );
/******************************************************************************/
// Description : This function builds test command response.
// Global resource dependence : None
// Author : Steven
// Note : None
/******************************************************************************/
LOCAL ATC_STATUS ATC_BuildTestCmdResponse(  // Return S_ATC_SUCCESS if success,
                                      // otherwise return error code.
        ATC_GLOBAL_INFO_T   * atc_global_info_ptr, // point to the ATC_GLOBAL_INFO_T struct.
        ATC_CUR_INFO_LIST_T * atc_cur_info_ptr // Point the info struct of cmd
        );
/******************************************************************************/
// Description : This function get the error string
// Global resource dependence : None
// Author : Ivan
// Note : None
// History:
/******************************************************************************/
LOCAL void ATC_GetErrVerbose(
        int32 err_idx,
        char *err_verbose
                       );
/******************************************************************************/
// Description : register the event to MN
// Global resource dependence : None
// Author : Ivan
// Note : None
/******************************************************************************/
LOCAL void ATC_RegisterEvent(void);

/*******************************************************************************************/
/*****************************************************************************/
// Description : This function indicate sim id to user
// Global resource dependence : none
// Author:       William
// Note:
/*****************************************************************************/
LOCAL void   ATC_IndicateSysId(
            MN_DUAL_SYS_E  dual_sys
                );
            
#ifdef _MUX_ENABLE_
LOCAL int ATC_Ring_Space( ATC_BUF_T *pRing );
LOCAL int ATC_Ring_Add( ATC_BUF_T *pRing, int ch );
LOCAL int ATC_Ring_Remove( ATC_BUF_T *pRing );
#ifndef _MUX_REDUCED_MODE_
LOCAL const char *ATC_Get_Event_Name( uint16 event );
#endif
#endif

#if defined (BLUETOOTH_SUPPORT_SPRD_BT)
/*****************************************************************************/
//  Discription: Callback function of blue tooth
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void ATC_BtCallBackFunc(DPARAM param);
#endif


#ifdef _ATC_ONLY
/*****************************************************************************/
//  Description : Audio dummy notify callback function.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void AudioDummyNotifyCallbackAT(
                HAUDIO hAudio, 
                uint32 notify_info, 
                uint32 affix_info
                )
{
    //do nothing
}
#endif

#if !defined( _ULTRA_LOW_CODE_) || defined(_ATC_SYNC_TOOL_)
/******************************************************************************/
// Description : write the all item which need in ATC
// Global resource dependence : None
// Author : hanjun
// Note :  
/******************************************************************************/     
BOOLEAN ATC_WriteNVItem(
                               ATC_CONFIG_T        *atc_config_ptr
                               )
{
    BOOLEAN status = TRUE; 

    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/

    //write the NV.
    if ((NVERR_NONE != EFS_NvitemWrite(NV_ATC_CONFIG_ID, sizeof(ATC_CONFIG_NV_T),
                                       (uint8 *)&atc_config_ptr->atc_config_nv, FALSE)))
    {
        status = FALSE;
        //SCI_TRACE_LOW:"ATC: write nv fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_330_112_2_17_22_16_18_0,(uint8*)"");
    }
    return status;
}
#endif

/******************************************************************************/
// Description : iread the all item which need in ATC.
// Global resource dependence : 
// Author : hanjun
// Note : None
/******************************************************************************/
LOCAL BOOLEAN ATC_ReadNVItem(
                              ATC_CONFIG_T        *atc_config_ptr
                              )
{
    BOOLEAN re_value = TRUE;
    
    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/
    
    if (NVERR_NONE != EFS_NvitemRead(NV_ATC_CONFIG_ID, 
                        sizeof(ATC_CONFIG_NV_T), (uint8 *)&atc_config_ptr->atc_config_nv))
    //if fail in reading,initiate all the variable
    {
        atc_config_ptr->atc_config_nv.creg                  = DEF_CREG;
        atc_config_ptr->atc_config_nv.clip_flag            = TRUE;
        atc_config_ptr->atc_config_nv.clir_flag             = FALSE;
        atc_config_ptr->atc_config_nv.colp_flag            = FALSE;
        atc_config_ptr->atc_config_nv.cmgf_flag           = FALSE;
        atc_config_ptr->atc_config_nv.call_ring_index    = 0;
        atc_config_ptr->atc_config_nv.mux_mode          = FALSE;
        atc_config_ptr->atc_config_nv.cmee                  = NUMERIC_ERR_VALUE_MODE;
        atc_config_ptr->atc_config_nv.e_value               = FALSE;
        atc_config_ptr->atc_config_nv.battery_indicator  = FALSE;
        //atc_config_ptr->atc_config_nv.stk_actflag          = TRUE;
        //atc_config_ptr->atc_config_nv.stk_time             = ATC_STKTIME_DEFAULT_VAL;
        atc_config_ptr->atc_config_nv.wind_ind_level     = ATC_SIND_DEFAULT_VAL;
        
        //SCI_TRACE_LOW:"ATC:read atc_config_nv fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_379_112_2_17_22_16_18_1,(uint8*)"");
        re_value = FALSE;
        //write the NV.
        if (NVERR_NONE != EFS_NvitemWrite(NV_ATC_CONFIG_ID, 
                                    sizeof(ATC_CONFIG_NV_T), (uint8 *)&atc_config_ptr->atc_config_nv, FALSE))
        
        {
            //SCI_TRACE_LOW:"ATC:write NV_ATC_CONFIG_ID fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_386_112_2_17_22_16_18_2,(uint8*)"");
        }
    }
    
    return re_value;    
}

#ifdef _MUX_ENABLE_
/******************************************************************************/
// Description : which will be called by mux. if is true, mux will output the response by pass without 
//  considering the AP state.
// Global resource dependence : 
// Author : feng.shi
// Note : None
/******************************************************************************/
BOOLEAN ATC_Is_First_SyncInd()
{
    return is_first_sync;
}

/******************************************************************************/
// Description : This function setup link for one ATC entity
// Global resource dependence : 
// Author : Wang Hui
// Note :
/******************************************************************************/
void ATC_Mux_Setup_Link(uint8 link_id)
{
    if((0 == link_id) || (ATC_DEBUG_LINK == link_id))
    {
        //SCI_TRACE_LOW:"ATC: ATC_Mux_Setup_Link, WARNING! link_id:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_419_112_2_17_22_16_18_5,(uint8*)"d", link_id);
        return;
    }
    if(atc_buffer[link_id].atc_buf_ptr != NULL)
    {
        //SCI_TRACE_LOW:"ATC: ATC_Mux_Setup_Link, WARNING! link_id:%d is already setup"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_424_112_2_17_22_16_18_6,(uint8*)"d", link_id);
#if 0
        atc_buffer[link_id].size            = ATC_REC_BUF_SIZE;
        atc_buffer[link_id].start_point  = 0;
        atc_buffer[link_id].end_point   = 0;
#endif
        return;
    } 
    
    atc_buffer[link_id].atc_buf_ptr =  (uint8*)SCI_ALLOC(ATC_REC_BUF_SIZE);
    if(!atc_buffer[link_id].atc_buf_ptr)
    {
        //SCI_TRACE_LOW:"ATC: ATC_Mux_Setup_Link, ERROR! no mem to setup link_id:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_438_112_2_17_22_16_18_7,(uint8*)"d", link_id);
        return;
    }
    
    atc_buffer[link_id].size            = ATC_REC_BUF_SIZE;
    atc_buffer[link_id].start_point  = 0;
    atc_buffer[link_id].end_point   = 0;
    //SCI_TRACE_LOW:"ATC: ATC_Mux_Setup_Link, link_id: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_447_112_2_17_22_16_18_8,(uint8*)"d", link_id);

}

/******************************************************************************/
// Description : This function create link for one ATC entity
// Global resource dependence : 
// Author : Wang Hui
// Note :
/******************************************************************************/
void ATC_Mux_Release_Link(uint8 link_id, BOOLEAN free_flg)
{
    uint8 call_index;
    uint8 xcall_id;
    MN_DUAL_SYS_E sim_id;
    uint8 cid;
    
    //SCI_TRACE_LOW:"ATC: ATC_Mux_Release_Link, link_id: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_462_112_2_17_22_16_18_9,(uint8*)"d", link_id);
    if((0 == link_id) || (ATC_DEBUG_LINK == link_id))
    {
        return;
    }
        
    sim_id = ATC_GetSimIdFromLinkId(link_id);
        
    if(sim_id < MN_SYS_NUMBER)
    {
        for (call_index=0; call_index<g_calls_state[sim_id].call_amount; call_index++)
        {
            if (ATC_CALL_IDLE_STATE != g_calls_state[sim_id].call_context[call_index].call_entity_state)
            {
                xcall_id = g_calls_state[sim_id].call_context[call_index].call_id;

                MNCALL_DisconnectCallEx(sim_id, (uint8)xcall_id, MN_CAUSE_NORMAL_CLEARING,PNULL);

                ATC_ADD_EXPECTED_EVENT(&s_config_info,
                                                            (uint16)APP_MN_CALL_DISCONNECTED_IND,
                                                            0xff,
                                                            ATC_DOMAIN_CS);
            }
        }
    }
    
    cid = ATC_GetCidFromLinkId(link_id);
     
    if(ATC_INVALID_CID != cid)
    {
        ATC_DisconnectModem(cid);
    }

    if(NULL == atc_buffer[link_id].atc_buf_ptr)
    {
        //SCI_TRACE_LOW:"ATC: ATC_Mux_Release_Link ERROR!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_500_112_2_17_22_16_18_10,(uint8*)"");
        atc_buffer[link_id].atc_buf_ptr = NULL;
        atc_buffer[link_id].size            = 0;
        atc_buffer[link_id].start_point  = 0;
        atc_buffer[link_id].end_point   = 0;
        return;
    }

    /* NOTE: if free_flg is FALSE, nothing to do!!! */
    if(free_flg)
    {
        SCI_FREE(atc_buffer[link_id].atc_buf_ptr);
        atc_buffer[link_id].atc_buf_ptr = NULL;
        atc_buffer[link_id].size            = 0;
        atc_buffer[link_id].start_point  = 0;
        atc_buffer[link_id].end_point   = 0;
    }
}

/******************************************************************************/
// Description : This function set the current link id 
// Global resource dependence : 
// Author : Wang Hui
// Note :
/******************************************************************************/
void ATC_Set_Current_Link_id(ATC_CONFIG_T *atc_config_ptr,uint8 link_id)
{
    //SCI_TRACE_LOW:"ATC SET link id to %d, old %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_529_112_2_17_22_16_18_11,(uint8*)"dd", link_id, atc_config_ptr->current_link_id);

    if(atc_config_ptr->current_link_id == ATC_DEBUG_LINK)
    {
        return;
    }
    
    atc_config_ptr->current_link_id = link_id;
}

/* -----------------------------------------------------------------------------------------------

   Ring Buffer Management Routines

   Each ring buffer has:-
   An 'in' pointer, which points to the next location that new data will be inserted at.
   An 'out' pointer, which points to the next character to be removed from the buffer.
   A 'limit' value, which is the size of the data array.
   A 'data' array, of 0..(limit-1) bytes.

   When in == out, the buffer is considered empty.
   When (in + 1 % limit) == out, the buffer is considered full.

   Note that if you have a 'limit' of 10, you will only be able to store 9 characters before
   the buffer full condition is reached.

   ----------------------------------------------------------------------------------------------- */

/*
** Returns the number of characters that can be added to this ring before it becomes full.
*/
LOCAL int ATC_Ring_Space( ATC_BUF_T *pRing )
{
    int ret = pRing->start_point - pRing->end_point - 1;
    
    return (ret >= 0) ? ret : (ret + (int)pRing->size);
}

LOCAL int ATC_Ring_Add( ATC_BUF_T *pRing, int ch )
{
    if ( 0 == ATC_Ring_Space( pRing ) ) 
    {
        return -1;
    }

    pRing->atc_buf_ptr[pRing->end_point++] = ch;

    if ( pRing->end_point >= (int)pRing->size ) //lint !e737
    {
        pRing->end_point = 0;
    }

    return 0;
}

/*
** Remove a character from this ring.
** Returns -1 if there are no characters in this ring.
** Returns the character removed otherwise.
*/
LOCAL int ATC_Ring_Remove( ATC_BUF_T *pRing )
{
    int ret    = 0;

    if ( pRing->end_point == pRing->start_point )
    {
        return -1;
    }

    ret = pRing->atc_buf_ptr[pRing->start_point++];

    if ( pRing->start_point >= (int)pRing->size ) //lint !e737
    {
        pRing->start_point = 0;
    }

    return ret;
}

/******************************************************************************/
// Description : This function called by ATC to clear AT buffer
// Global resource dependence : 
// Author : Wang Hui
// Note :
/******************************************************************************/
void ATC_ClrCmdLine( uint8 link_id )
{
#if 0
    atc_buffer[link_id].end_point = atc_buffer[link_id].start_point;
#else
    atc_buffer[link_id].start_point = atc_buffer[link_id].end_point;
    cur_atc_cmd_size[link_id]       = 0;
#endif
}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                           * 
 **     VOID ATC_ReadCmdLine                                    *
 **                                                                           *
 ** DESCRIPTION                                                       *
 **     Read AT command from ATC BUFFER                    *
 **                                                                           *
 ** RETURN VALUE                                                     *
 **     None                                                               *
 **                                                                           *
 ** DEPENDENCIES                                                     *
 **---------------------------------------------------------------------------*/
void ATC_ReadCmdLine(
        uint8 link_id,
        uint8 * data_ptr,            // Output argument, point to the buf which is alloced by caller 
        uint length,                    // The length of cmd line which should be read from sio
        uint * read_length_ptr   // The actual read length
        )
{
    uint         i      = 0;
    int          ch    = 0;
    BOOLEAN end  = FALSE;

    for (i = 0; i < length; i++)
    {
        /* ATC_Ring_Remove returned value: -1, 0-255
         * So ch's type shall be "int" not "uint8" */
        ch = ATC_Ring_Remove(&atc_buffer[link_id]);
        if ( -1 != ch )
        {
            if (ch == atc_end_ch1[link_id] || ch == atc_end_ch2[link_id])
            {
                *data_ptr = (uint8)ch;
                *read_length_ptr += 1;
                
                i++;
                end = TRUE;
                break;
            }
            else if (ch == atc_bs_ch[link_id])
            {
                data_ptr--;
                *read_length_ptr -= 1;
            }
            else
            {
                *data_ptr++ = (uint8)ch;
                *read_length_ptr += 1;
            }
        }
        else
        {
            break;
        }
    }

    if(i < length)
    {
        //SCI_TRACE_LOW:"ATC: ATC_ReadCmdLine Warning: only read %d < %d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1346_112_2_17_22_16_20_38,(uint8*)"dd", i, length);
    }
    else if(FALSE == end)
    {
        //SCI_TRACE_LOW:"ATC: ATC_ReadCmdLine Warning: garbage data ???"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1350_112_2_17_22_16_20_39,(uint8*)"");
    }
}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  * 
 **     void ATC_SetEcho()                                                *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Tell CMUX to echo the char it receives                                 *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
void ATC_SetEcho( BOOLEAN IsEcho )
{
    atc_is_echo = IsEcho;
}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  * 
 **     void ATC_SetCmdLineTerminateChar( )                               *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Set terminate char of of AT command                                   *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
void ATC_SetCmdLineTerminateChar( 
        uint8 link_id,
        uint8 terminate_char1,   // Default terminate char is '\n' 
        uint8 terminate_char2    // Default terminate char is '\0'
        )
{
    atc_end_ch1[link_id] = terminate_char1;
    atc_end_ch2[link_id] = terminate_char2;
}


/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  * 
 **     void ATC_SetCmdLineBackSpaceChar()                                *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Set backspace char of the AT command                                  *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
void ATC_SetCmdLineBackSpaceChar( 
        uint8 link_id,
        uint8 back_space_char  // Default backspace char value is 8
        )
{
    atc_bs_ch[link_id] = back_space_char;
}

/******************************************************************************/
// Description : This function called by CMUX to ind ATC that a new char received
// Global resource dependence : 
// Author : Wang Hui
// Note :
/******************************************************************************/
void ATC_SendNewATInd(uint8 link_id, uint32 data_length, uint8 *data_ptr)
{
    ATC_MUX_RECV_NEW_AT_T* psig;
    unsigned char  nchar;
    int i;

    //SCI_TRACE_LOW:"ATC: ATC_SendNewATInd,link_id:%d,data_length:%d,data %x, %x, %x, %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1421_112_2_17_22_16_20_40,(uint8*)"dddddd", link_id, data_length, *data_ptr,*(data_ptr+1),*(data_ptr+2),*(data_ptr+3));
    
    //@for avoiding task queue full 
    if ( 16 <= s_atc_global_info.buffered_at_cmd_count ) // 16 = atc_task_queue_count - 4
    {
        /* 
        * 1.SIO buffer is circle buffer,put in char by adding left edge,and get out
        *  char by minus right edge
        * 2.so call  SIO_ATC_ReadCmdLine() will delete the first at command and not
        *  the newest put in at command.
        * 3.because all the at command buffer in SIO buffer(it is 1200 bytes),we
        *  buffer at command num and should buffer at command length,if it over 1200 byts
        * we should clear the first at command for very long at command
        */
        
        //SIO_ATC_ReadCmdLine(&atc_cmd_line_buf[0],ATC_REC_BUF_SIZE,&atc_read_length);
        is_close_atc_queue = TRUE;
        
        //return;
    }
    
    if(is_close_atc_queue)
    {
        for(i=0;i<ATC_MAX_LINK_ID;i++)
        {
            ATC_ClrCmdLine(i);
        }

        //SCI_TRACE_LOW:"ATC: ATC_SendNewATInd, is_close_atc_queue is TRUE, return"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1449_112_2_17_22_16_20_41,(uint8*)"");
        return;
    }

    if(!atc_buffer[link_id].atc_buf_ptr)
    {
        //SCI_TRACE_LOW:"ATC: ATC_SendNewATInd, link is not setup!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1455_112_2_17_22_16_20_42,(uint8*)"");
        return;
    }
    
    if (atc_is_echo)
    {
        //SCI_TRACE_LOW:"ATC: ATC_SendNewATInd, atc_is_echo is TRUE, send copy back"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1461_112_2_17_22_16_21_43,(uint8*)"");
        MUX_Write(data_ptr,data_length,link_id);
    }

    while(data_length)
    {
        nchar = *data_ptr++;
        data_length--;

        if ( -1 == ATC_Ring_Add( &atc_buffer[link_id], nchar ) ) 
        {
            //SCI_TRACE_LOW:"ATC: ERROR AT BUFFER FULL  LINK %d "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1472_112_2_17_22_16_21_44,(uint8*)"d",link_id);

            ATC_ClrCmdLine(link_id);
            continue;
        }

        cur_atc_cmd_size[link_id]++;
        
        if (nchar == atc_end_ch1[link_id] || nchar == atc_end_ch2[link_id])
        {
            //send start up signal to start up the sim task
            SCI_CREATE_SIGNAL(psig,ATC_MUX_RECV_NEW_AT,sizeof(ATC_MUX_RECV_NEW_AT_T),SCI_IdentifyThread());
            psig->len = cur_atc_cmd_size[link_id];
            psig->link_id = link_id;
            
            s_atc_global_info.buffered_at_cmd_count ++;
            
            SCI_SEND_SIGNAL((xSignalHeaderRec*)psig,P_ATC);
            
            cur_atc_cmd_size[link_id] = 0;
        }
    }
}

/******************************************************************************/
// Description : This function called by SIO to ind ATC that a new AT received
// Global resource dependence : 
// Author : Wang Hui
// Note : the function is called by SIO Interrupt Service Routine,so it should be
//        optimized
/******************************************************************************/
void ATC_Debug_SendATInd(uint32 len)
{
    ATC_MUX_RECV_NEW_AT_T* psig;
    unsigned char  nchar                                                         = 0;    
    uint8*            temp_cmd_line_ptr                                      = NULL;
    uint               temp_line_length                                         = 0;
    uint               temp_line_length_total                                 = 0;
    static uint8     temp_cmd_line[ATC_MAX_CMD_LINE_LENGTH] = {0};

    //SCI_TRACE_LOW:"ATC: ATC_Debug_SendATInd, len:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1510_112_2_17_22_16_21_45,(uint8*)"d", len);

    //@for avoiding task queue full 
    if ( 16 <= s_atc_global_info.buffered_at_cmd_count ) // 16 = atc_task_queue_count - 4
    {
        /* 
        * 1.SIO buffer is circle buffer,put in char by adding left edge,and get out
        *  char by minus right edge
        * 2.so call  SIO_ATC_ReadCmdLine() will delete the first at command and not
        *  the newest put in at command.
        * 3.because all the at command buffer in SIO buffer(it is 1200 bytes),we
        *  buffer at command num and should buffer at command length,if it over 1200 byts
        * we should clear the first at command for very long at command
        */

        //SIO_ATC_ReadCmdLine(&atc_cmd_line_buf[0],ATC_REC_BUF_SIZE,&atc_read_length);
        is_close_atc_queue = TRUE;
    }
    
    if(is_close_atc_queue)
    {
#ifdef PPP_USB
	   SIO_ATC_ClrCmdLine(ATC_GetSioPort());
#else
        SIO_ATC_ClrCmdLine();
#endif
        //SCI_TRACE_LOW:"ATC: ATC_Debug_SendATInd, is_close_atc_queue is TRUE, return"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1533_112_2_17_22_16_21_46,(uint8*)"");
        return;
    }

#ifdef PPP_USB
    SIO_ATC_ReadCmdLine(temp_cmd_line,len,&temp_line_length,ATC_GetSioPort());
#else
    SIO_ATC_ReadCmdLine(temp_cmd_line,len,&temp_line_length);
#endif
    //SCI_TRACE_LOW:"temp_line_length %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1538_112_2_17_22_16_21_47,(uint8*)"d",temp_line_length);
    temp_line_length_total = temp_line_length;

    temp_cmd_line_ptr = temp_cmd_line;

    SCI_ASSERT(atc_buffer[ATC_DEBUG_LINK].atc_buf_ptr != PNULL); /*assert verified check NULL pointer*/

    while(temp_line_length)
    {
        nchar = *temp_cmd_line_ptr++;
        temp_line_length--;

        if (ATC_Ring_Add( &atc_buffer[ATC_DEBUG_LINK], nchar ) == -1 ) 
        {
            //SCI_TRACE_LOW:"ATC: ERROR AT BUFFER FULL  LINK ATC_DEBUG_LINK "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1552_112_2_17_22_16_21_48,(uint8*)"");
#if 0
            atc_buffer[ATC_DEBUG_LINK].end_point = atc_buffer[ATC_DEBUG_LINK].start_point;
#endif
            ATC_ClrCmdLine(ATC_DEBUG_LINK);
            continue;
        }
    }

    //send start up signal to start up the sim task
    SCI_CREATE_SIGNAL(psig,ATC_MUX_RECV_NEW_AT,sizeof(ATC_MUX_RECV_NEW_AT_T),SCI_IdentifyThread());
    psig->len = temp_line_length_total;
    psig->link_id = ATC_DEBUG_LINK;

    s_atc_global_info.buffered_at_cmd_count ++;

    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig,P_ATC);
}

/******************************************************************************/
// Description : This function list all the expected events 
// Global resource dependence : 
// Author : Pizer Fan
// Note :
/******************************************************************************/
uint8 ATC_GetDualSimMuxLinkId(MN_DUAL_SYS_E sim_id)
{
    if(RM_GetCalibrationPostMode())  //if cal post mode 
    {
        //SCI_TRACE_LOW:"ATC: CAL POST MODE, return ATC_DEBUG_LINK"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1579_112_2_17_22_16_21_49,(uint8*)"");
        return ATC_DEBUG_LINK;
    }
    
    if(g_mux_sys_info[MUX_1].is_start==MUX_STATUS_OFF)
    {
        //SCI_TRACE_LOW:"ATC_GetDualSimMuxLinkId return ctl_link %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1585_112_2_17_22_16_21_50,(uint8*)"d", g_mux_sys_info[MUX_1].ctl_link);
        return g_mux_sys_info[MUX_1].ctl_link;
    }
    
    return (uint8)(sim_id + 1);
}

/******************************************************************************/
// Description : This function get sim id from link id. 
// Global resource dependence : 
// Author : shaohua xia
// Note :
/******************************************************************************/
MN_DUAL_SYS_E ATC_GetSimIdFromLinkId(uint8 link_id)
{
    if((MN_DUAL_SYS_1 < link_id) && (link_id <= MN_SYS_NUMBER))
    {
        return (MN_DUAL_SYS_E)(link_id - 1);
    }
    else if((MN_SYS_NUMBER < link_id) && (link_id <= (MN_SYS_NUMBER + ATC_PDP_NUM_MAX)))
    {
        return g_atc_modem_dial_sys;
    }
    else
    {
        return g_current_card_id;
    }
}

/******************************************************************************/
// Description : This function get cid from link id. 
// Global resource dependence : 
// Author : shaohua xia
// Note :
/******************************************************************************/
uint8 ATC_GetCidFromLinkId(uint8 link_id)
{
    if((MN_SYS_NUMBER < link_id) && (link_id < ATC_DEBUG_LINK))
    {
        return ((link_id - MN_SYS_NUMBER) <= MN_GPRS_MAX_PDP_CONTEXT_COUNT) ? (link_id - MN_SYS_NUMBER) : ATC_INVALID_CID;
    }
    else
    {
        return ATC_INVALID_CID;
    }
}

/******************************************************************************/
// Description : This function get link id from cid. 
// Global resource dependence : 
// Author : shaohua xia
// Note :
/******************************************************************************/
uint8 ATC_GetLinkIdFromCid(uint8 cid)
{
    if((cid < 1) || (MN_GPRS_MAX_PDP_CONTEXT_COUNT < cid))
    {
        return ATC_INVALID_LINK_ID;
    }

    return ((cid + MN_SYS_NUMBER) < ATC_DEBUG_LINK) ? (cid + MN_SYS_NUMBER) : ATC_INVALID_LINK_ID;
}

#else/*_MUX_ENABLE_*/
/******************************************************************************/
// Description : This function called by SIO to ind ATC that a new AT received
// Global resource dependence : 
// Author : Ivan
// Note : the function is called by SIO Interrupt Service Routine,so it should be
//        optimized
/******************************************************************************/
#ifdef PPP_USB
void SendNewATInd(uint32 len,uint8 sio_port)
#else
void SendNewATInd(uint32 len)
#endif
{
    SIO_RECV_NEW_AT_T* psig = PNULL;
#ifndef WIN32    
    BOOLEAN is_new_cmd = FALSE;
#endif
    int  cmd_len = len;

    //@for avoiding task queue full 
    if (s_atc_global_info.buffered_at_cmd_count >= 16) // 16 = atc_task_queue_count - 4
    {
        /* 
        * 1.SIO buffer is circle buffer,put in char by adding left edge,and get out
        *  char by minus right edge
        * 2.so call  SIO_ATC_ReadCmdLine() will delete the first at command and not
        *  the newest put in at command.
        * 3.because all the at command buffer in SIO buffer(it is 1200 bytes),we
        *  buffer at command num and should buffer at command length,if it over 1200 byts
        * we should clear the first at command for very long at command
        */

        //SIO_ATC_ReadCmdLine(&atc_cmd_line_buf[0],ATC_REC_BUF_SIZE,&atc_read_length);
        is_close_atc_queue = TRUE;
    }

    if(is_close_atc_queue)
    {
#ifdef PPP_USB
	   SIO_ATC_ClrCmdLine(sio_port);
#else
        SIO_ATC_ClrCmdLine();
#endif
        return;
    }

#ifndef WIN32
    if (g_is_diag_cmd)
    {
        is_new_cmd = TRUE;
    }
    else
    {
   #ifdef PPP_USB
     cmd_len = SIO_ATC_CheckNewCmd(&is_new_cmd, len,sio_port);
   #else
     cmd_len = SIO_ATC_CheckNewCmd(&is_new_cmd, len);
   #endif
        if (cmd_len == -1)
        {
            SCI_TRACE_LOW("ATC: SIO Buffer is empty or invalid len %d", len);
            return;
        }
    }

    if (is_new_cmd == TRUE)
#endif
    {
        /*lint -e668*/
        //send start up signal to start up the sim task
        SCI_CREATE_SIGNAL(psig,SIO_RECV_NEW_AT,sizeof(SIO_RECV_NEW_AT_T),SCI_IdentifyThread());//lint !e64
        psig->len = cmd_len;
#ifdef PPP_USB
	psig->sio_port = sio_port;
#endif		
        /*lint +e668*/
    
        s_atc_global_info.buffered_at_cmd_count++;
    
        SCI_SEND_SIGNAL((xSignalHeaderRec*)psig, P_ATC);
        SCI_TRACE_LOW("ATC: recv a new AT  len %d", cmd_len);
    }
#if 0 // only for debug
    else
    {
        SCI_TRACE_LOW("ATC: recv a cmd fragment len%d", len);
    }
#endif
}
#endif
#if !defined( _MUX_REDUCED_MODE_)||defined(_ATC_UIX8910_ENABLE_)
/******************************************************************************/
// Description : This function to get event name
// Global resource dependence : 
// Author : Pizer Fan
// Note :
/******************************************************************************/
const char *ATC_Get_Event_Name( uint16 event )
{
    const char *name = NULL;
/*
#ifdef _DEBUG
    uint i;
    for(i = 0; i < sizeof(expect_event_name) / sizeof(expect_event_name[0]); i++)
    {
        if(expect_event_name[i].event == event)
        {
            name = expect_event_name[i].name;
            break;
        }
    }
#endif
*/
    return name;
}

/******************************************************************************/
// Description : This function list all the expected events 
// Global resource dependence : 
// Author : Pizer Fan
// Note :
/******************************************************************************/
void ATC_List_Expected_Event( void )
{
#ifdef _DEBUG
    ATC_CONFIG_T   *atc_config_ptr;
    uint16    start_p                       = 0;
    uint16    end_p                         = 0;
    int         expected_event_num  = 0;

    //SCI_TRACE_LOW:"ATC: ATC_List_Expected_Event as follows:"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_574_112_2_17_22_16_18_12,(uint8*)"");
    
    atc_config_ptr = &s_config_info;
    start_p           = atc_config_ptr->atc_exp_link_info.start_point;
    end_p            = atc_config_ptr->atc_exp_link_info.end_point;

    while(start_p != end_p)
    {
        if(ATC_NO_EVENT != atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].exp_event)
        {
            const char *name = NULL;
            
            if(0 == expected_event_num)
            {
                //SCI_TRACE_LOW:"|---|-------|-----|-------|------------------------------"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_588_112_2_17_22_16_18_13,(uint8*)"");
                //SCI_TRACE_LOW:"|Idx|Link-ID| CID |Call-ID| Event"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_589_112_2_17_22_16_18_14,(uint8*)"");
                //SCI_TRACE_LOW:"|---|-------|-----|-------|------------------------------"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_590_112_2_17_22_16_18_15,(uint8*)"");
            }
            
            expected_event_num++;

            name = ATC_Get_Event_Name(atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].exp_event);
            if(name)
            {
                //SCI_TRACE_LOW:"|%-3d|   %d   | %-3d |  %-3d  | %s"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_602_112_2_17_22_16_19_16,(uint8*)"|ddds", start_p, atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].link_id,atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].cid,atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].call_id,name);
            }
            else
            {
                //SCI_TRACE_LOW:"|%-3d|   %d   | %-3d |  %-3d  | %d"
                SCI_TRACE_LOW("|%-3d|   %d   | %-3d |  %-3d  | %d",start_p, atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].link_id,atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].cid,atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].call_id,atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].exp_event);
                //SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_610_112_2_17_22_16_19_17,(uint8*)"|dddd", start_p, atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].link_id,atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].cid,atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].call_id,atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].exp_event);
            }
        }

        start_p++;
        if(ATC_MAX_EXP_RESULT == start_p)
        {
            start_p = 0;
        }
    }

    if(0 != expected_event_num)
    {
        //SCI_TRACE_LOW:"|---|-------|-----|-------|------------------------------"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_623_112_2_17_22_16_19_18,(uint8*)"");
    }

    //SCI_TRACE_LOW:"Total:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_626_112_2_17_22_16_19_19,(uint8*)"d", expected_event_num);
#endif
}

/******************************************************************************/
// Description : This function clear expected result info
// Global resource dependence : 
// Author : Wang Hui
// Note :
/******************************************************************************/
void ATC_Clear_Exp_Result(ATC_EXP_RESULT_INFO_T  *atc_exp_result_info)
{
    atc_exp_result_info->link_id             = ATC_INVALID_LINK_ID;
    atc_exp_result_info->exp_event        = ATC_NO_EVENT;
    atc_exp_result_info->ext_exp_event  = ATC_NO_EVENT;
    atc_exp_result_info->call_id             = ATC_INVALID_CALL_ID;
    atc_exp_result_info->cid                  = ATC_INVALID_CID;
}

/******************************************************************************/
// Description : This function clear all expected events of call-id(cid)
// Global resource dependence : 
// Author : Pizer Fan
// Note :
/******************************************************************************/
void ATC_Clear_All_Exp_Result(uint8 id, ATC_EVENT_DOMAIN_T domain_id)
{
    ATC_CONFIG_T *atc_config_ptr;
    uint16    start_p = 0; 
    uint16    end_p   = 0;

    atc_config_ptr = &s_config_info;
    start_p = atc_config_ptr->atc_exp_link_info.start_point;
    end_p  = atc_config_ptr->atc_exp_link_info.end_point;

    //SCI_TRACE_LOW:"ATC: ATC_Clear_All_Exp_Result id:%d,domain_id:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_657_112_2_17_22_16_19_20,(uint8*)"dd",id,domain_id);
    if(domain_id == ATC_DOMAIN_PS)
    {
        while(start_p != end_p)
        {
            /* When deactivation, clear all expected events 
             * related to call states. */
            if( (atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].cid == id) &&
                (   (atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].exp_event == APP_MN_ACTIVATE_PDP_CONTEXT_CNF) ||
                    (atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].exp_event == APP_MN_DEACTIVATE_PDP_CONTEXT_CNF) ||
                    (atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].exp_event == APP_MN_DEACTIVATE_PDP_CONTEXT_IND) ))
            {
                uint8       atc_link_id = 0;
                
                const char *name = ATC_Get_Event_Name(atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].exp_event);
                if(name)
                {
                    //SCI_TRACE_LOW:"ATC: ATC_Clear_All_Exp_Result, cid:%d, exp event: %s"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_674_112_2_17_22_16_19_21,(uint8*)"ds", id, name);
                }
                else
                {
                    //SCI_TRACE_LOW:"ATC: ATC_Clear_All_Exp_Result, cid:%d, exp event: %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_679_112_2_17_22_16_19_22,(uint8*)"dd",id, atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].exp_event);
                }
                switch(atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].exp_event)
                {
                /* For example:
                 * LINK 1: PDP activation, waiting for APP_MN_ACTIVATE_PDP_CONTEXT_CNF
                 * LINK 2: PDP deactivation
                 * When receiving APP_MN_DEACTIVATE_PDP_CONTEXT_CNF, need to clear "LINK 1"'s 
                 * expected event -- APP_MN_ACTIVATE_PDP_CONTEXT_CNF
                 */
                case APP_MN_ACTIVATE_PDP_CONTEXT_CNF:
                    /* Save current_link_id */
                    atc_link_id = atc_config_ptr->current_link_id;
                    atc_config_ptr->current_link_id = atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].link_id;
                    ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR,S_ATC_RESULT_CODE_ERROR,ERR_GPRS_OPERATION_FAILURE);
                    /* Restore current_link_id */
                    atc_config_ptr->current_link_id = atc_link_id;
                    break;
                    
                default:
                    break;
                }
                ATC_CLEAR_EXP_RESULT(&atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p]);
            }
            
            start_p++;
            if(ATC_MAX_EXP_RESULT == start_p) 
            {
                start_p = 0;
            }
        }
    }
    else if(domain_id == ATC_DOMAIN_CS)
    {
        while(start_p != end_p)
        {
            if(atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].call_id == id)
            {
                //SCI_TRACE_LOW:"ATC: ATC_Clear_All_Exp_Result, call_id:%d, exp event: %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_718_112_2_17_22_16_19_23,(uint8*)"dd",id, atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].exp_event);
                ATC_CLEAR_EXP_RESULT(&atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p]);
            }
            
            start_p++;
            if(ATC_MAX_EXP_RESULT == start_p) 
            {
                start_p = 0;
            }
        }
    }
}

/******************************************************************************/
// Description : This function handle the received the expected event and set the related link id
// Global resource dependence : 
// Author : Wang Hui
// Note :
/******************************************************************************/
BOOLEAN ATC_Receive_Expected_Event(ATC_CONFIG_T *atc_config_ptr,uint16 expected_event, uint8 id,ATC_EVENT_DOMAIN_T domain_id)
{
    BOOLEAN    ret_val      = FALSE;
    uint16        start_p      = 0;
    uint16        end_p        = 0;
    uint8          link_id        = (uint8)ATC_INVALID_LINK_ID;
    const char *name         = NULL;

    start_p = atc_config_ptr->atc_exp_link_info.start_point;
    end_p  = atc_config_ptr->atc_exp_link_info.end_point;

    SCI_ASSERT(end_p < ATC_MAX_EXP_RESULT); /*assert verified check boundary*/
    SCI_ASSERT(start_p < ATC_MAX_EXP_RESULT);  /*assert verified check boundary*/

    switch(expected_event)
    {
        case APP_MN_ACTIVATE_PDP_CONTEXT_CNF:
        case APP_MN_DEACTIVATE_PDP_CONTEXT_CNF:
        {
            while(start_p != end_p)
            {
                if(atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].exp_event == expected_event 
                   && atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].cid == id) //lint !e661
                {
                    atc_config_ptr->current_link_id = atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].link_id; //lint !e661
                    ATC_CLEAR_EXP_RESULT(&atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p]);
                    ret_val = TRUE;
                    break;
                }
                
                start_p++;
                if(ATC_MAX_EXP_RESULT == start_p)
                {
                    start_p = 0;
                }
            }
            break;
        }
        
        /* NOTE: no cid field in APP_MN_GPRS_T !!!
         * DONT care cid for APP_MN_SET_QOS_CNF & APP_MN_SET_MIN_QOS_CNF !!! */
        case APP_MN_SET_QOS_CNF:
        case APP_MN_SET_MIN_QOS_CNF:
        {
            while(start_p != end_p)
            {
                if(atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].exp_event == expected_event) //lint !e661
                {
                    atc_config_ptr->current_link_id = atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].link_id; //lint !e661
                    ATC_CLEAR_EXP_RESULT(&atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p]);
                    ret_val = TRUE;
                    break;
                }

                start_p++;
                if(ATC_MAX_EXP_RESULT == start_p)
                {
                    start_p = 0;
                }
            }
            break;
        }
        
        case APP_MN_FDN_UPDATE_CNF:
        {
            expected_event = APP_MN_ADN_UPDATE_CNF;
            //lint -fallthrough
            case APP_MN_ADN_UPDATE_CNF:
            while(start_p != end_p)
            {
                if(atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].exp_event == expected_event ) //lint !e661
                {
                    atc_config_ptr->current_link_id = atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].link_id; //lint !e661 !e662
                    ATC_CLEAR_EXP_RESULT(&atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p]);
                    ret_val = TRUE;
                    break;
                }
                
                start_p++;
                if(ATC_MAX_EXP_RESULT == start_p)
                {
                    start_p = 0;
                }
            }
            break;
        }
        
        case APP_MN_GPRS_ATTACH_RESULT:
        case APP_MN_GPRS_DETACH_RESULT:
        case APP_MN_CALL_HELD_CNF:
        case APP_MN_READ_DSP_MEM_CNF:
        case APP_MN_DEACTIVE_PS_CNF:
        case APP_MN_BUILD_MPTY_CNF:
        case APP_MN_SPLIT_MPTY_CNF:
        case APP_MN_CALL_RETRIEVED_CNF:
        case APP_MN_READ_SMS_CNF:
        case APP_MN_SEND_SMS_CNF:
        case APP_MN_WRITE_SMS_CNF:
        case APP_MN_UPDATE_SMS_STATE_CNF:
        case APP_MN_INTERROGATE_CNF:
        case APP_MN_DEACTIVATE_SS_CNF:
        case APP_MN_ACTIVATE_SS_CNF:
        case APP_MN_ERASE_SS_CNF:
        case APP_MN_REGISTER_SS_CNF:
        case APP_MN_REGISTER_PASSWORD_CNF:
        case APP_MN_SIM_PIN_FUNC_CNF:
        case APP_MN_PLMN_SELECT_CNF:
        case APP_MN_PLMN_LIST_CNF:
        case APP_MN_PLMN_READ_CNF:
        case APP_MN_PLMN_WRITE_CNF:
        case APP_MN_SET_BAND_CNF:
        case APP_MN_SETUP_COMPLETE_CNF:
        case APP_MN_PS_POWER_OFF_CNF:
        case APP_MN_PS_POWER_ON_CNF:
        case APP_MN_CRSM_CNF:
        {
            while(start_p != end_p)
            {
                if(atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].exp_event == expected_event) //lint !e661
                {
                    atc_config_ptr->current_link_id = atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].link_id; //lint !e661
                    ATC_CLEAR_EXP_RESULT(&atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p]);
                    ret_val = TRUE;
                    break;
                }

                start_p++;
                if(ATC_MAX_EXP_RESULT == start_p)
                {
                    start_p = 0;
                }
            }
            break;
        }
        
        case APP_MN_CALL_START_IND:
        {
            while(start_p != end_p)
            {
                if(atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].exp_event == expected_event) //lint !e661
                {
                    atc_config_ptr->current_link_id = atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].link_id; //lint !e661
                    atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].exp_event  = APP_MN_ALERTING_IND; //lint !e661
                    atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].call_id = id; //lint !e661
                    atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].ext_exp_event = APP_MN_CALL_DISCONNECTED_IND; //lint !e661
                    ret_val = TRUE;
                    break;                    
                }
                
                start_p++;
                if(ATC_MAX_EXP_RESULT == start_p)
                {
                    start_p = 0;
                }
            }
            break;
        }
        
        case APP_MN_ALERTING_IND:
        {
            while(start_p != end_p)
            {
                if(atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].exp_event == expected_event
                  && atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].call_id == id) //lint !e661
                {
                    atc_config_ptr->current_link_id = atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].link_id; //lint !e661
                    atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].exp_event  = APP_MN_SETUP_COMPLETE_CNF; //lint !e661
                    atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].ext_exp_event = APP_MN_CALL_DISCONNECTED_IND; //lint !e661
                    ret_val = TRUE;
                    break;
                }
                
                start_p++;
                if(ATC_MAX_EXP_RESULT == start_p)
                {
                    start_p = 0;
                }
            }
            break;
        }
        
       case APP_MN_SETUP_COMPLETE_IND:
        {
            while(start_p != end_p)
            {
                if(atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].exp_event == expected_event
                  && atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].call_id == id) //lint !e661
                {
                    atc_config_ptr->current_link_id = atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].link_id; //lint !e661
                    ATC_CLEAR_EXP_RESULT(&atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p]); //lint !e661
                    ret_val = TRUE;
                    break;
                }
                
                start_p++;
                if(ATC_MAX_EXP_RESULT == start_p)
                start_p = 0;
            }
            break;
        }

        case APP_MN_CALL_DISCONNECTED_IND:
        case APP_MN_CALL_ERR_IND:
        {
            expected_event = APP_MN_CALL_DISCONNECTED_IND;
            
            while(start_p != end_p)
            {
                if(atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].ext_exp_event == expected_event 
                  || atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].exp_event == expected_event) //lint !e661
                {
                    atc_config_ptr->current_link_id = atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].link_id; //lint !e661
                    ATC_CLEAR_EXP_RESULT(&atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p]);
                    ret_val = TRUE;
                    break;
                }
                
                start_p++;
                if(ATC_MAX_EXP_RESULT == start_p) 
                {
                    start_p = 0;
                }
            }
            
            break;
        }
        
        default:
        {
            /* Try to match expect event anyway! */
            //SCI_TRACE_LOW:"ATC: recv default event %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_965_112_2_17_22_16_19_24,(uint8*)"d", expected_event);
            
            while(start_p != end_p)
            {
                if(atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].exp_event == expected_event ) //lint !e661
                {
                atc_config_ptr->current_link_id = atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].link_id; //lint !e661 !e662
                ATC_CLEAR_EXP_RESULT(&atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p]);
                ret_val = TRUE;
                break;	
                }
                start_p++;
                if(ATC_MAX_EXP_RESULT == start_p) 
                {
                    start_p = 0;
                }
            }
            
            //SCI_TRACE_LOW:"ATC: recv event %x, not found in expected list"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_983_112_2_17_22_16_19_25,(uint8*)"d", expected_event);
            /* NOT FOUND!!! */
            break;
        }
    }

    if(ret_val)
    {
        link_id = atc_config_ptr->current_link_id;
    }
    else
    {
        link_id = ATC_Get_Link_Id(id, domain_id);
    }
    
    name = ATC_Get_Event_Name(expected_event);
    if(name)
    {
        //SCI_TRACE_LOW:"ATC: ATC_Receive_Expected_Event, id:%d,event:%s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1001_112_2_17_22_16_19_26,(uint8*)"ds", id, name);
    }
    else
    {
        //SCI_TRACE_LOW:"ATC: ATC_Receive_Expected_Event, id:%d,event:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1005_112_2_17_22_16_19_27,(uint8*)"dd", id, expected_event);
    }

   if(FALSE == ret_val)
   {
       if(link_id != (uint8)ATC_INVALID_LINK_ID)
       {
            //SCI_TRACE_LOW:"ATC: ATC_Receive_Expected_Event, WARNING! NOT FOUND, set link id: %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1012_112_2_17_22_16_19_28,(uint8*)"d", link_id);
            atc_config_ptr->current_link_id = link_id;
       }
       else
       {
            //SCI_TRACE_LOW:"ATC: ATC_Receive_Expected_Event, WARNING! NOT FOUND, set urc link id: %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1017_112_2_17_22_16_19_29,(uint8*)"d", atc_urc_link_id);
            atc_config_ptr->current_link_id = atc_urc_link_id;
       }

       //ATC_List_Expected_Event();
    }
    else
    {
        if( (atc_config_ptr->current_link_id != link_id)
            && ((ATC_DOMAIN_CS==domain_id) || (ATC_DOMAIN_PS==domain_id)) )
        {
            /* non-data-mode pdp activation, no add PSD info, so */
            if((ATC_INVALID_LINK_ID != link_id))
            {
                //SCI_TRACE_LOW:"ATC: ATC_Receive_Expected_Event, FOUND, ERROR! match_link_id:%d != call_link_id:%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1032_112_2_17_22_16_19_30,(uint8*)"dd",atc_config_ptr->current_link_id, link_id);
            }
            else
            {
                //SCI_TRACE_LOW:"ATC: ATC_Receive_Expected_Event, FOUND, WARNING! match_link_id:%d, no call_link_id, NOT ADD EARLIER?"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1037_112_2_17_22_16_19_31,(uint8*)"d",atc_config_ptr->current_link_id);
            }
        }
        else
        {
            if((ATC_INVALID_LINK_ID != link_id))
            {
                //SCI_TRACE_LOW:"ATC: ATC_Receive_Expected_Event, FOUND, match_link_id:%d, call_link_id:%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1045_112_2_17_22_16_19_32,(uint8*)"dd",atc_config_ptr->current_link_id, link_id);
            }
            else
            {
                //SCI_TRACE_LOW:"ATC: ATC_Receive_Expected_Event, FOUND, match_link_id:%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1050_112_2_17_22_16_19_33,(uint8*)"d",atc_config_ptr->current_link_id);
            }
        }
    }

    while((atc_config_ptr->atc_exp_link_info.start_point != atc_config_ptr->atc_exp_link_info.end_point) && ret_val)
    {
        if(atc_config_ptr->atc_exp_link_info.atc_exp_result_info[atc_config_ptr->atc_exp_link_info.start_point].exp_event == ATC_NO_EVENT)
        {
            atc_config_ptr->atc_exp_link_info.start_point++;

            if(ATC_MAX_EXP_RESULT <= atc_config_ptr->atc_exp_link_info.start_point)
            {
                atc_config_ptr->atc_exp_link_info.start_point = 0;
            }
        }
        else
        {
            break;
        }
    }

   ATC_List_Expected_Event();
   return ret_val;
}

/******************************************************************************/
// Description : This function add the expected event when requested the expected event
// Global resource dependence : 
// Author : Wang Hui
// Note :
/******************************************************************************/
void ATC_Add_Expected_Event(ATC_CONFIG_T *atc_config_ptr,uint16 expected_event, uint8 id,ATC_EVENT_DOMAIN_T domain_id)
{
/*
    name = ATC_Get_Event_Name(expected_event);
    if(name)
    {
        //SCI_TRACE_LOW:"ATC: ATC_Add_Expected_Event, link_id:%d, event:%s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1085_112_2_17_22_16_19_34,(uint8*)"ds", atc_config_ptr->current_link_id, name);
    }
    else
    {
        //SCI_TRACE_LOW:"ATC: ATC_Add_Expected_Event, link_id:%d, event:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1089_112_2_17_22_16_19_35,(uint8*)"dd", atc_config_ptr->current_link_id, expected_event);
    }
*/
    SCI_ASSERT(atc_config_ptr->atc_exp_link_info.end_point<ATC_MAX_EXP_RESULT);    /*assert verified check boundary*/
    SCI_ASSERT(atc_config_ptr->atc_exp_link_info.start_point<ATC_MAX_EXP_RESULT);  /*assert verified check boundary*/

    if(ATC_NO_DOMAIN == domain_id)
    {
        atc_config_ptr->atc_exp_link_info.atc_exp_result_info[atc_config_ptr->atc_exp_link_info.end_point].exp_event = expected_event;
        atc_config_ptr->atc_exp_link_info.atc_exp_result_info[atc_config_ptr->atc_exp_link_info.end_point].link_id = atc_config_ptr->current_link_id;
    }
    else if(ATC_DOMAIN_CS == domain_id)
    {
        atc_config_ptr->atc_exp_link_info.atc_exp_result_info[atc_config_ptr->atc_exp_link_info.end_point].exp_event = expected_event;
        atc_config_ptr->atc_exp_link_info.atc_exp_result_info[atc_config_ptr->atc_exp_link_info.end_point].call_id = id;
        atc_config_ptr->atc_exp_link_info.atc_exp_result_info[atc_config_ptr->atc_exp_link_info.end_point].link_id = atc_config_ptr->current_link_id;
    }
    else if(ATC_DOMAIN_PS == domain_id)
    {
        atc_config_ptr->atc_exp_link_info.atc_exp_result_info[atc_config_ptr->atc_exp_link_info.end_point].exp_event = expected_event;
        atc_config_ptr->atc_exp_link_info.atc_exp_result_info[atc_config_ptr->atc_exp_link_info.end_point].cid = id;
        atc_config_ptr->atc_exp_link_info.atc_exp_result_info[atc_config_ptr->atc_exp_link_info.end_point].link_id = atc_config_ptr->current_link_id;
    }
    else
    {
        //SCI_TRACE_LOW:"ATC: ATC_Add_Expected_Event, unknown domain_id: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1114_112_2_17_22_16_20_36,(uint8*)"d", domain_id);
    }

    atc_config_ptr->atc_exp_link_info.end_point++;

    if(ATC_MAX_EXP_RESULT <= atc_config_ptr->atc_exp_link_info.end_point)
    {
        atc_config_ptr->atc_exp_link_info.end_point = 0;
    }

    if(atc_config_ptr->atc_exp_link_info.end_point == atc_config_ptr->atc_exp_link_info.start_point)
    {
        //SCI_TRACE_LOW:"ATC: ATC_Add_Expected_Event,Error :The atc response buffer is full ,clear first message,pls check"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1126_112_2_17_22_16_20_37,(uint8*)"");

        ATC_CLEAR_EXP_RESULT(&atc_config_ptr->atc_exp_link_info.atc_exp_result_info[atc_config_ptr->atc_exp_link_info.start_point]);

        atc_config_ptr->atc_exp_link_info.start_point++;
        if(ATC_MAX_EXP_RESULT <= atc_config_ptr->atc_exp_link_info.start_point)
        {
            atc_config_ptr->atc_exp_link_info.start_point = 0;
        }

        while(atc_config_ptr->atc_exp_link_info.start_point != atc_config_ptr->atc_exp_link_info.end_point)
        {
            if(atc_config_ptr->atc_exp_link_info.atc_exp_result_info[atc_config_ptr->atc_exp_link_info.start_point].exp_event == ATC_NO_EVENT)
            {   
                atc_config_ptr->atc_exp_link_info.start_point++;
                if(ATC_MAX_EXP_RESULT <= atc_config_ptr->atc_exp_link_info.start_point)
                {
                    atc_config_ptr->atc_exp_link_info.start_point = 0;
                }
            }
            else
            {
               break;
            }
        }
    } 

    ATC_List_Expected_Event();
}
/******************************************************************************/
// Description : This function get link id on which call-id(cid) is
// Global resource dependence : 
// Author : Pizer Fan
// Note :
/******************************************************************************/
uint8 ATC_Get_Link_Id(uint8 id, ATC_EVENT_DOMAIN_T domain_id)
{
    ATC_CONFIG_T *atc_config_ptr;
    uint16        start_p   = 0;
    uint16        end_p     = 0;

    atc_config_ptr = &s_config_info;
    start_p           = atc_config_ptr->atc_exp_link_info.start_point;
    end_p             = atc_config_ptr->atc_exp_link_info.end_point;
    
    if(ATC_DOMAIN_PS == domain_id)
    {
        if( (1<=id && id <=MN_GPRS_MAX_PDP_CONTEXT_COUNT) || (0xff==id))
        {
            while(start_p != end_p)
            {
                if(atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].cid == id)
                {
                    return atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].link_id;
                }
                
                start_p++;
                if(ATC_MAX_EXP_RESULT == start_p)
                {
                    start_p = 0;
                }
            }
        }
    }
    else if(ATC_DOMAIN_CS == domain_id)
    {
        while(start_p != end_p)
        {
            if(atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].call_id == id)
            {
                return atc_config_ptr->atc_exp_link_info.atc_exp_result_info[start_p].link_id;
            }
            
            start_p++;
            if(ATC_MAX_EXP_RESULT == start_p)
            {
                start_p = 0;
            }
        }
    }

    return (uint8)ATC_INVALID_LINK_ID;
}
#endif

/******************************************************************************/
// Description : This function called by SIO and MMI to know if the state is on dialing or not
// Global resource dependence : 
// Author : king.li
// Note : 
/******************************************************************************/
ATC_MODEM_STATUS_E ATC_GetModemStatus(void)
{
    uint32 i    =0;

    while(i < MN_GPRS_MAX_PDP_CONTEXT_COUNT)
    {
        if(TRUE == s_atc_global_info.atc_config_ptr->pdp_id_arr[i++])
        {
            return ATC_MODEM_ONDIAL;
        }
    }
    
    return ATC_MODEM_NOT_ONDIAL;  
}

#ifdef PPP_USB
uint8 v_atc_sio_port=ATC_SIO_DEFAULT_PORT;

void ATC_SetSioPort(uint8 port)
{
  v_atc_sio_port= port;
}

uint8 ATC_GetSioPort(void)
{
  return v_atc_sio_port;
}
#endif


/******************************************************************************/
// Description : This function is the main entry point of ATC task.
// Global resource dependence : 
// Author : Steven,Ivan 
// Note :  
// History:
/******************************************************************************/
void ATC_Task(uint32 argc, void * argv)
{
    ATC_STATUS                status   = S_ATC_SUCCESS;
    
#if defined( _MUX_ENABLE_)||defined(_ATC_UIX8910_ENABLE_)
    BOOLEAN    recv_event_result  = FALSE;
#endif

    xSignalHeaderRec        *sig_ptr  = NULL;

#ifdef _SUPPORT_GPRS_
    uint32 i    = 0;
    uint8 pid  = 0;
#endif

#ifdef PCLINK_ENABLE
    MN_GPRS_PCLINK_CFG_T pclink_cfg;
#endif

    status = ATC_Initialize();

#ifdef _MUX_ENABLE_
    MUX_Set_Atc_CallbackFunc(0xff,ATC_SendNewATInd);
    SIO_ATC_SetCallback(ATC_Debug_SendATInd);
#else
    SIO_ATC_SetCallback(SendNewATInd);
#endif

    for (;;)
    {
        // Receive signal from queue
        SCI_RECEIVE_SIGNAL(sig_ptr, P_ATC);
        SCI_TraceLow("ATC_Task, get event %d, ",sig_ptr->SignalCode);
        switch (sig_ptr->SignalCode)
        {
#ifdef _MUX_ENABLE_
            case ATC_MUX_LINK_SETUP_REQ:
            {
                //SCI_TRACE_LOW:"ATC: ATC_Task, Signal Code: ATC_MUX_LINK_SETUP_REQ, link_id:%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1750_112_2_17_22_16_21_51,(uint8*)"d", ((ATC_MUX_LINK_SETUP_REQ_T *)sig_ptr)->link_id);
                ATC_Mux_Setup_Link(((ATC_MUX_LINK_SETUP_REQ_T *)sig_ptr)->link_id);
                break;
            }

            case ATC_MUX_LINK_RELEASE_REQ:
            {
                //SCI_TRACE_LOW:"ATC: ATC_Task, Signal Code: ATC_MUX_LINK_RELEASE_REQ, link_id:%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1757_112_2_17_22_16_21_52,(uint8*)"d", ((ATC_MUX_LINK_SETUP_REQ_T *)sig_ptr)->link_id);
                ATC_Mux_Release_Link(((ATC_MUX_LINK_SETUP_REQ_T *)sig_ptr)->link_id, FALSE);
                break;
            }

            case ATC_MUX_CLOSE_REQ:
            {
                //SCI_TRACE_LOW:"ATC: ATC_Task, Signal Code: ATC_MUX_CLOSE_REQ"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1764_112_2_17_22_16_21_53,(uint8*)"");
                break;
            }

            case ATC_MUX_RECV_NEW_AT:
            {
                //SCI_TRACE_LOW:"ATC: ATC_Task, Signal Code: ATC_MUX_RECV_NEW_AT, link_id:%d,len:%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1771_112_2_17_22_16_21_54,(uint8*)"dd", ((ATC_MUX_RECV_NEW_AT_T *)sig_ptr)->link_id, ((ATC_MUX_RECV_NEW_AT_T *)sig_ptr)->len);
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,((ATC_MUX_RECV_NEW_AT_T *)sig_ptr)->link_id);

                if(is_close_atc_queue)
                {
                    s_atc_global_info.buffered_at_cmd_count--;
                    if(0 == s_atc_global_info.buffered_at_cmd_count)
                    {
                        is_close_atc_queue = FALSE;
                    }
                    
                    //SCI_TRACE_LOW:"ATC: 2 atc_global_info config link %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1782_112_2_17_22_16_21_55,(uint8*)"d", s_atc_global_info.atc_config_ptr->current_link_id);
                    ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR,S_ATC_RESULT_CODE_ERROR,ERR_AT_COMMAND_DISCARDED);
                    break;//just discard the message
                }
                else
                {
                    s_atc_global_info.buffered_at_cmd_count -= 1;//@Ivan.Yin for avoiding task queue full 2004-03-22
                    // disable atc sleep, we will start of disable that timer after
                    // we handle all at command.
                }
                
                //SCI_TRACE_LOW:"ATC: atc_task, atc_global_info config link %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1793_112_2_17_22_16_21_56,(uint8*)"d", s_atc_global_info.atc_config_ptr->current_link_id);

                status = ATC_RecNewLineSig( &s_atc_global_info, sig_ptr );
                if (S_ATC_SUCCESS == status)
                {
                ATC_ProcessCmds( &s_atc_global_info );
                }
                else if (S_ATC_DEFAULT_HANDLE == status)
                {
                    //@Shijun.cui 2005-04-05 add for AT## command
                    break;
                }
                else
                {
                    ATCS_TRACE_STATUS( status );
                }
                break;
            }
#else
            // Receive a new command line
            case SIO_RECV_NEW_AT:
            {
                if(is_close_atc_queue)
                {
                    s_atc_global_info.buffered_at_cmd_count--;
                    if(s_atc_global_info.buffered_at_cmd_count == 0)
                    {
                        is_close_atc_queue = FALSE;
                    }

                    ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR,S_ATC_RESULT_CODE_ERROR,ERR_AT_COMMAND_DISCARDED);
                    break;//just discard the message
                }
                else
                {
                    s_atc_global_info.buffered_at_cmd_count -= 1;//@Ivan.Yin for avoiding task queue full 2004-03-22
                }

                status = ATC_RecNewLineSig( &s_atc_global_info, sig_ptr );
                if (S_ATC_SUCCESS == status)
                {
                    ATC_ProcessCmds( &s_atc_global_info );
                }
                else if (S_ATC_DEFAULT_HANDLE == status)
                {
                    //add for AT## command
                    break;
                }
                else
                {
                    ATCS_TRACE_STATUS( status );
                }
                break;
            }
#endif

#ifdef _SUPPORT_GPRS_
#if 0
            case APP_MN_ACTIVATE_PDP_CONTEXT_IND:
            {
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR, ATC_GetDualSimMuxLinkId(((APP_MN_GPRS_T*)sig_ptr)->dual_sys));
                ATC_ProcessActPdpContextInd(S_CONFIG_INFO_PTR,(APP_MN_GPRS_T*)sig_ptr);//, &s_atc_global_info);
                break;
            }
#endif
            case APP_MN_SIM_PRESENT_IND:
                ATC_ProcessSimPresentInd(S_CONFIG_INFO_PTR, (APP_MN_SIM_PRESENT_IND_T*)sig_ptr, &s_atc_global_info);
                break;
            case APP_MN_DEACTIVATE_PDP_CONTEXT_IND:
            {
                //SCI_TRACE_LOW:"atc: network deactivate PDP"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1860_112_2_17_22_16_22_57,(uint8*)"");
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR, ATC_GetDualSimMuxLinkId(((APP_MN_GPRS_T*)sig_ptr)->dual_sys));
                ATC_ProcessDeactPdpContextInd(S_CONFIG_INFO_PTR,(APP_MN_GPRS_T*)sig_ptr, &s_atc_global_info);	 
                break;
            }

            // the following signals for GPRS services
            case APP_MN_ACTIVATE_PDP_CONTEXT_CNF:
            {
                //SCI_TRACE_LOW:"atc: receive pdp activate message from network"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1869_112_2_17_22_16_22_58,(uint8*)"");
				#ifdef PLATFORM_UWS6121E
				#ifdef PPP_USB
                if(PPP_IsPPPProcessing() == TRUE)
                {
                    if((((APP_MN_GPRS_EXT_T*)sig_ptr)->result >= MN_GPRS_ERR_SUCCESS) && (((APP_MN_GPRS_EXT_T*)sig_ptr)->result <= MN_GPRS_ERR_SUCCESS_SINGLE_ONLY))
                    
                    {
                            if(TCPIP_PPPMP_OpenPppModemService(((APP_MN_GPRS_EXT_T *)sig_ptr)->nsapi, ( (APP_MN_GPRS_EXT_T*)sig_ptr)->pdp_id))
                            {
                                 PPP_Stop(((APP_MN_GPRS_EXT_T*)sig_ptr)->dual_sys,( (APP_MN_GPRS_EXT_T*)sig_ptr)->pdp_id,0xFF,PDP_ACTIVATE_REJ,0xFF);/*lint !e64, !e2706*/
                            }
                    }
                    else
                    {
                           PPP_Stop(((APP_MN_GPRS_EXT_T*)sig_ptr)->dual_sys,( (APP_MN_GPRS_EXT_T*)sig_ptr)->pdp_id,0xFF,PDP_ACTIVATE_REJ,0xFF);/*lint !e64, !e2706*/

                    }

                }
				#endif
				#endif
				recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR, sig_ptr->SignalCode, 0xff, ATC_DOMAIN_PS);
                if(!recv_event_result)
                {
					recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR, sig_ptr->SignalCode,( (APP_MN_GPRS_EXT_T*)sig_ptr)->pdp_id, ATC_DOMAIN_PS);
                }
                if(recv_event_result)
                    ATC_ProcessActPdpContextCnf(S_CONFIG_INFO_PTR,(APP_MN_GPRS_T*)sig_ptr, &s_atc_global_info);
                break;
            }

            case APP_MN_SET_QOS_CNF:
            case APP_MN_SET_MIN_QOS_CNF:
            case APP_MN_SET_EXTEND_QOS_CNF:
            case APP_MN_SET_EXTEND_MIN_QOS_CNF:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_GPRS_T);

                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR, sig_ptr->SignalCode, ((APP_MN_GPRS_EXT_T*)sig_ptr)->pdp_id, ATC_DOMAIN_PS);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR, ATC_GetDualSimMuxLinkId(((APP_MN_GPRS_EXT_T*)sig_ptr)->dual_sys));
                if(recv_event_result)
                {
                    if( ((APP_MN_GPRS_T *)sig_ptr)->result == MN_GPRS_ERR_SUCCESS) 
                    {
                        ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR,S_ATC_RESULT_CODE_OK,0);
                    }
                    else
                    {
                        ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR,S_ATC_RESULT_CODE_ERROR,((APP_MN_GPRS_T*)sig_ptr)->result);
                    } 
                }
                break;
            }

            case APP_MN_DEACTIVATE_PDP_CONTEXT_CNF:
            {
                //SCI_TRACE_LOW:"atc: user deactivate PDP "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1898_112_2_17_22_16_22_59,(uint8*)"");
#if defined( _MUX_ENABLE_)||defined(_ATC_UIX8910_ENABLE_)
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR, sig_ptr->SignalCode, 0xff, ATC_DOMAIN_PS);
                if(!recv_event_result)
                {
                    recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR, sig_ptr->SignalCode, ((APP_MN_GPRS_DEACTIVATE_CNF_T *)sig_ptr)->pdp_id[0], ATC_DOMAIN_PS);
                }
#endif

#ifdef PCLINK_ENABLE
                /* in PCLink mode, directly switch to AT mode */
                MNGPRS_GetPclinkCfg(&pclink_cfg);
                if(pclink_cfg.is_pclink_open)
                {
                    SIO_ATC_SetDataMode(FALSE);
                    //SCI_TRACE_LOW:"ATC: Switch to AT MODE for PCLink!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1913_112_2_17_22_16_22_60,(uint8*)"");
                }
                else 
#endif /* PCLINK_ENABLE */
                {
                    BOOLEAN is_pdp_belong_to_modem = FALSE;
                    for ( i=0; i<((APP_MN_GPRS_DEACTIVATE_CNF_T *)sig_ptr)->pdp_num; i++)
                    {
                        pid = ((APP_MN_GPRS_DEACTIVATE_CNF_T *)sig_ptr)->pdp_id[i];

                        ATC_FreeDataReqPendingList(pid+4);

                        if(s_atc_global_info.atc_config_ptr->pdp_id_arr[pid-1])
                        {
                            is_pdp_belong_to_modem = TRUE;
                            s_atc_global_info.atc_config_ptr->pdp_id_arr[pid-1] = FALSE;
                            //SCI_TRACE_LOW:"case APP_MN_DEACTIVATE_PDP_CONTEXT_CNF,is deactivate AT pid,pid:%d"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1927_112_2_17_22_16_22_61,(uint8*)"d", pid);
#ifdef _MUX_ENABLE_
                            MPPP_ResetPPPInterface(pid);
#else
#ifdef TCPIP_SUPPORT
                            TCPIP_ResetPPPInterface();
#endif
                            break;
                        }
                    }

                    if(is_pdp_belong_to_modem)
                    {
#ifdef _MUX_ENABLE_
                        ppp_dialing_in_progress = FALSE;
#endif
#ifndef _ATC_UIX8910_ENABLE_
                        ATC_SendDetachReq(((APP_MN_GPRS_DEACTIVATE_CNF_T*)sig_ptr)->dual_sys);//when manual attach and no pdp is activated, should send detach
#endif
                    }
#if defined( _MUX_ENABLE_)||defined(_ATC_UIX8910_ENABLE_)
                    if(recv_event_result)
#endif						
                    {
                        if( MN_GPRS_ERR_SUCCESS ==((APP_MN_GPRS_DEACTIVATE_CNF_T *)sig_ptr)->result) 
                        {
                            ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR,S_ATC_RESULT_CODE_OK,0);
                        }
                        else
                        {
                            ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR,S_ATC_RESULT_CODE_ERROR,((APP_MN_GPRS_DEACTIVATE_CNF_T *)sig_ptr)->result);
                        }
                    }
#endif
                }
#if defined(_MUX_ENABLE_)||defined(_ATC_UIX8910_ENABLE_)
                if(MN_GPRS_ERR_SUCCESS == ((APP_MN_GPRS_DEACTIVATE_CNF_T *)sig_ptr)->result)
                {
                    ATC_Clear_All_Exp_Result(((APP_MN_GPRS_DEACTIVATE_CNF_T *)sig_ptr)->pdp_id[0], ATC_DOMAIN_PS);
                }
#endif
                break;
            }

            case APP_MN_GPRS_ATTACH_RESULT:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_GPRS_ATTACH_RESULT_T);
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR, ATC_GetDualSimMuxLinkId(((APP_MN_GPRS_ATTACH_RESULT_T*)sig_ptr)->dual_sys));   
                ATC_ProcessAttResult(S_CONFIG_INFO_PTR, (APP_MN_GPRS_ATTACH_RESULT_T *)sig_ptr);
                if(recv_event_result)
                {
                    /*if( ((APP_MN_GPRS_ATTACH_RESULT_T*)sig_ptr)->is_attach_ok) 
                    {
                        ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR,S_ATC_RESULT_CODE_OK,0);
                    }
                    else
                    {
                        ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR,S_ATC_RESULT_CODE_ERROR,((APP_MN_GPRS_ATTACH_RESULT_T*)sig_ptr)->err_cause );
                    }*/
                }
                break;
            }

            case APP_MN_GPRS_DETACH_RESULT:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_GPRS_DETACH_RESULT_T);
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);

#ifdef _MUX_ENABLE_
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR, ATC_GetDualSimMuxLinkId(((APP_MN_GPRS_DETACH_RESULT_T*)sig_ptr)->dual_sys));   

                //SCI_TRACE_LOW:"ATC: g_atc_ignore_one_detach_cnf %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_1988_112_2_17_22_16_22_62,(uint8*)"d", g_atc_ignore_one_detach_cnf);
                if(g_atc_ignore_one_detach_cnf)
                {
                    g_atc_ignore_one_detach_cnf = FALSE;
                    break;
                }
#endif
#ifdef ATC_SAT_ENABLE 
#ifndef _GSM_ONLY_
                if(g_atc_ip_detach)
                {
                    g_atc_ip_detach = FALSE;
                    
                    sprintf((char *)g_rsp_str, "IPDEACT OK");
                    
                    ATC_BuildInfoRsp(S_CONFIG_INFO_PTR, g_rsp_str);
                }
#endif    
#endif    
                if(recv_event_result)
                {
                    if( ((APP_MN_GPRS_DETACH_RESULT_T*)sig_ptr)->is_detach_ok) 
                    {
                        ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR,S_ATC_RESULT_CODE_OK,0);
                    }
                    else
                    {
                        ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR,S_ATC_RESULT_CODE_ERROR,0);
                    }
                }
                break;
            }
#endif

            // the following sigals for call control 
            case APP_MN_CALL_START_IND:
            {
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,((APP_MN_CALL_START_IND_T*)sig_ptr)->call_id,ATC_DOMAIN_CS);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_CALL_START_IND_T *)sig_ptr)->dual_sys));

                ATC_IndicateSysId(((APP_MN_CALL_START_IND_T*)sig_ptr)->dual_sys);
                ATC_ProcessCallStartInd(S_CONFIG_INFO_PTR,(APP_MN_CALL_START_IND_T*)sig_ptr);
                break;
            }

#ifndef _ULTRA_LOW_CODE_
            case APP_MN_ACMMAX_IND:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_ACMMAX_IND_T *)sig_ptr)->dual_sys));
                ATC_ProcessAcmInd(S_CONFIG_INFO_PTR);
                break;
            }
#endif

            case APP_MN_SETUP_IND:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_SETUP_IND_T *)sig_ptr)->dual_sys));
                ATC_IndicateSysId(((APP_MN_SETUP_IND_T*)sig_ptr)->dual_sys);
                ATC_ProcessSetupInd(S_CONFIG_INFO_PTR,(APP_MN_SETUP_IND_T*)sig_ptr);
                break;
            }

            case APP_MN_ALERTING_IND:
            {
                if(((APP_MN_ALERTING_IND_T*)sig_ptr)->alerting_type == MN_CALL_REMOTE_USER_ALERTING)
                {
                    recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,(((APP_MN_ALERTING_IND_T*)sig_ptr))->call_id,ATC_DOMAIN_CS);
                    ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_ALERTING_IND_T *)sig_ptr)->dual_sys));
                }
                else
                {
                    ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_ALERTING_IND_T *)sig_ptr)->dual_sys));
                }

                ATC_ProcessAlertingInd(S_CONFIG_INFO_PTR, (APP_MN_ALERTING_IND_T*)sig_ptr);
                break;
            }

        case APP_MN_SET_SMS_RECEIVE_FUNC_CNF:
        {
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR, sig_ptr->SignalCode, 0xff, ATC_NO_DOMAIN);
                if(recv_event_result)
                    ATC_ProcessSetSmsRcvFunCnf(S_CONFIG_INFO_PTR, (APP_MN_SET_SMS_RECEIVE_FUNC_CNF_T *)sig_ptr);
                break;
        }

            case ATC_CALL_EXP_IND:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR, g_call_exp_link_id);
                ATC_ProcessCallExpInd(S_CONFIG_INFO_PTR);
                break;
            }

#ifndef _ULTRA_LOW_CODE_
            case ATC_POWEROFF_EXP_IND:
                {
                    ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,g_poweroff_exp_link_id);
                    ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR,S_ATC_RESULT_CODE_OK,0);
                    POWER_PowerOff();
                    break;
                }
#endif

#ifdef _MUX_ENABLE_
            case APP_MN_SETUP_COMPLETE_IND:
            {
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,((APP_MN_SETUP_COMPLETE_IND_T*)sig_ptr)->call_id,ATC_DOMAIN_CS);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_SETUP_COMPLETE_IND_T *)sig_ptr)->dual_sys));
                ATC_ProcessSetupCompleteSignal(S_CONFIG_INFO_PTR,sig_ptr);
                break;
            }

            case APP_MN_SETUP_COMPLETE_CNF:
            {
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,((APP_MN_SETUP_COMPLETE_CNF_T*)sig_ptr)->call_id,ATC_DOMAIN_CS);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_SETUP_COMPLETE_CNF_T *)sig_ptr)->dual_sys));
                if(recv_event_result)
                    ATC_ProcessSetupCompleteSignal(S_CONFIG_INFO_PTR,sig_ptr);
                break;
            }
#else
            case APP_MN_SETUP_COMPLETE_IND:
            {
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,((APP_MN_SETUP_COMPLETE_IND_T*)sig_ptr)->call_id,ATC_DOMAIN_CS);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_SETUP_COMPLETE_IND_T *)sig_ptr)->dual_sys));
                if (recv_event_result)
                {
                    ATC_ProcessSetupCompleteSignal(S_CONFIG_INFO_PTR,sig_ptr);
                }
                break;
            }
            case APP_MN_SETUP_COMPLETE_CNF:
            {
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,((APP_MN_SETUP_COMPLETE_CNF_T*)sig_ptr)->call_id,ATC_DOMAIN_CS);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_SETUP_COMPLETE_CNF_T *)sig_ptr)->dual_sys));
				
                ATC_ProcessSetupCompleteSignal(S_CONFIG_INFO_PTR,sig_ptr);
				
                if(recv_event_result)
                    ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR, S_ATC_RESULT_CODE_OK,0);
                break;
            }
#endif

#ifdef _MUX_ENABLE_
            case APP_MN_CALL_DISCONNECTED_IND:
            {
                //SCI_TRACE_LOW:"ATC: ATC_Task, Signal Code: APP_MN_CALL_DISCONNECTED_IND, set atc_err_protected_flag"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_2096_112_2_17_22_16_22_63,(uint8*)"");
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,((APP_MN_CALL_DISCONNECTED_IND_T*)sig_ptr)->call_id,ATC_DOMAIN_CS);
                ATC_CLEAR_ALL_EXP_RESULT(((APP_MN_CALL_DISCONNECTED_IND_T*)sig_ptr)->call_id, ATC_DOMAIN_CS);

                ATC_ProcessCallDiscInd(S_CONFIG_INFO_PTR,sig_ptr);
                break;
            }

            case APP_MN_CALL_ERR_IND:
            {
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,((APP_MN_CALL_ERR_IND_T*)sig_ptr)->call_id,ATC_DOMAIN_CS);
                ATC_CLEAR_ALL_EXP_RESULT(((APP_MN_CALL_ERR_IND_T*)sig_ptr)->call_id, ATC_DOMAIN_CS);
                ATC_ProcessCallDiscInd(S_CONFIG_INFO_PTR,sig_ptr);
                break;
            }
#else
            case APP_MN_CALL_DISCONNECTED_IND:
            {
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,((APP_MN_CALL_DISCONNECTED_IND_T*)sig_ptr)->call_id,ATC_DOMAIN_CS);
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_CALL_DISCONNECTED_IND_T *)sig_ptr)->dual_sys));
                ATC_ProcessCallDiscInd(S_CONFIG_INFO_PTR,sig_ptr);
                break;
            }
            
            case APP_MN_CALL_ERR_IND:
            {
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,((APP_MN_CALL_ERR_IND_T*)sig_ptr)->call_id,ATC_DOMAIN_CS);
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_CALL_ERR_IND_T *)sig_ptr)->dual_sys));
                ATC_ProcessCallDiscInd(S_CONFIG_INFO_PTR,sig_ptr);
                break;
            }
#endif

            case APP_MN_SYNC_IND:   //Can send DTMF tone when the signal received.Tch is already assigned by network.
            {
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode, 0xff,ATC_DOMAIN_CS);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_SYNC_IND_T *)sig_ptr)->dual_sys));
                ATC_ProcessMnSyncInd(S_CONFIG_INFO_PTR, (APP_MN_SYNC_IND_T*)sig_ptr);
                break;
            }

#ifndef _ULTRA_LOW_CODE_
            case APP_MN_READ_DSP_MEM_CNF:
            {
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,atc_urc_link_id);
                if(recv_event_result)
                    ATC_ProcessReadDSPCnf(S_CONFIG_INFO_PTR, (APP_MN_READ_DSP_MEM_CNF_T*)sig_ptr);
                break;
            }
#endif

#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)

            case APP_MN_DEACTIVE_PS_CNF:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_DEACTIVE_PS_CNF_T);
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_DEACTIVE_PS_CNF_T *)sig_ptr)->dual_sys));
                ATC_ProcessPowerOffCnf(S_CONFIG_INFO_PTR, (APP_MN_DEACTIVE_PS_CNF_T*)sig_ptr);
                if(recv_event_result)
                    ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR,S_ATC_RESULT_CODE_OK,0);
                break;
            }

            case APP_MN_NETWORK_STATUS_IND:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_NETWORK_STATUS_IND_T *)sig_ptr)->dual_sys));
                ATC_ProcessNetworkStatusInd(S_CONFIG_INFO_PTR, (APP_MN_NETWORK_STATUS_IND_T*)sig_ptr);
                break;
            }
#endif
#if  !defined(_ULTRA_LOW_CODE_) ||defined(_ATC_UIX8910_ENABLE_)
            case APP_MN_CALL_FORWARD_IND:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_CALL_FORWARD_IND_T *)sig_ptr)->dual_sys));
                ATC_ProcessForwardInd(S_CONFIG_INFO_PTR, (APP_MN_CALL_FORWARD_IND_T*)sig_ptr);
                break;
            }

            case APP_MN_CALL_BARRED_IND:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_CALL_BARRED_IND_T *)sig_ptr)->dual_sys));
                ATC_ProcessBarredInd(S_CONFIG_INFO_PTR, (APP_MN_CALL_BARRED_IND_T*)sig_ptr);
                break;
            }

            case APP_MN_FORWARD_STATUS_IND:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_FORWARD_STATUS_IND_T *)sig_ptr)->dual_sys));
                ATC_ProcessForwardStatusInd(S_CONFIG_INFO_PTR, (APP_MN_FORWARD_STATUS_IND_T*)sig_ptr);
                break;
            }

            case APP_MN_BUILD_MPTY_IND:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_BUILD_MPTY_IND_T *)sig_ptr)->dual_sys));
                ATC_ProcessBuildMptyInd(S_CONFIG_INFO_PTR, (APP_MN_BUILD_MPTY_IND_T*)sig_ptr);
                break;
            }
#endif
#if !defined( _ULTRA_LOW_CODE_) || defined(_ATC_UIX8910_ENABLE_)
            case APP_MN_BUILD_MPTY_CNF:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_BUILD_MPTY_CNF_T);
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_BUILD_MPTY_CNF_T *)sig_ptr)->dual_sys));
                if(recv_event_result)
                    ATC_ProcessBuildMptyCnf(S_CONFIG_INFO_PTR, (APP_MN_BUILD_MPTY_CNF_T*)sig_ptr);
                break;
            }

            case APP_MN_SPLIT_MPTY_CNF:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_SPLIT_MPTY_CNF_T);
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_SPLIT_MPTY_CNF_T *)sig_ptr)->dual_sys));
                if(recv_event_result)
                    ATC_ProcessSplitMptyCnf(S_CONFIG_INFO_PTR, (APP_MN_SPLIT_MPTY_CNF_T*)sig_ptr);
                break;
            }

            case APP_MN_CALL_HELD_CNF:
            {
                SCI_TraceLow("ATC_Task, received APP_MN_CALL_HELD_CNF\n");
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_CALL_HELD_CNF_T);
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,((APP_MN_CALL_HELD_CNF_T*)sig_ptr)->call_id,ATC_DOMAIN_CS);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_CALL_HELD_CNF_T *)sig_ptr)->dual_sys));
                if(recv_event_result)
                    ATC_ProcessHeldCnf(S_CONFIG_INFO_PTR, (APP_MN_CALL_HELD_CNF_T*)sig_ptr);
                break;
            }

            case APP_MN_CALL_HELD_IND:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_CALL_HELD_IND_T *)sig_ptr)->dual_sys));
                ATC_ProcessHeldInd(S_CONFIG_INFO_PTR, (APP_MN_CALL_HELD_IND_T*)sig_ptr);
                break;
            }

            case APP_MN_CALL_RETRIEVED_IND:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_CALL_RETRIEVED_IND_T *)sig_ptr)->dual_sys));
                ATC_ProcessRetrievedInd(S_CONFIG_INFO_PTR, (APP_MN_CALL_RETRIEVED_IND_T*)sig_ptr);
                break;
            }

            case APP_MN_CALL_RETRIEVED_CNF:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_CALL_RETRIEVED_CNF_T);
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_CALL_RETRIEVED_CNF_T *)sig_ptr)->dual_sys));
                if(recv_event_result)
                    ATC_ProcessCallRetrievCnf(S_CONFIG_INFO_PTR, (APP_MN_CALL_RETRIEVED_CNF_T*)sig_ptr);
                break;
            }
#endif
#if !defined( _ULTRA_LOW_CODE_) || defined(_ATC_SYNC_TOOL_) || defined(_ATC_UIX8910_ENABLE_)
            // The following signals for SMS service
            case APP_MN_SMS_READY_IND:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_SMS_READY_IND_T *)sig_ptr)->dual_sys));
                ATC_ProcessSmsReadyInd(S_CONFIG_INFO_PTR, (APP_MN_SMS_READY_IND_T*)sig_ptr);
                break;
            }

            case APP_MN_READ_SMS_CNF:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_READ_SMS_CNF_T);
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                if(recv_event_result)
                    ATC_ProcessReadSmsCnf(S_CONFIG_INFO_PTR, (APP_MN_READ_SMS_CNF_T*)sig_ptr);
                break;
            }

            case APP_MN_SEND_SMS_CNF:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_SEND_SMS_CNF_T);
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                if(recv_event_result)
                {
                    #if 0
                    if(TRUE == g_is_stk_sendsms[((APP_MN_SEND_SMS_CNF_T *)sig_ptr)->dual_sys])
                    {
                        ATC_STKProcessSendSmsCnf(S_CONFIG_INFO_PTR, (APP_MN_SEND_SMS_CNF_T*)sig_ptr);
                    }
                    else
                    #endif
                    {
                        ATC_ProcessSendSmsCnf(S_CONFIG_INFO_PTR, (APP_MN_SEND_SMS_CNF_T*)sig_ptr);
                    }
                }
                break;
            }
#endif
            

#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)
            case APP_MN_SMS_STATUS_REPORT_IND:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_SMS_STATUS_REPORT_IND_T *)sig_ptr)->dual_sys));
                ATC_IndicateSysId(((APP_MN_SMS_STATUS_REPORT_IND_T*)sig_ptr)->dual_sys);
                ATC_ProcessSmsStatusReportInd(S_CONFIG_INFO_PTR,(APP_MN_SMS_STATUS_REPORT_IND_T*)sig_ptr);
                break; 
            }

            case APP_MN_SMS_MEM_FULL_IND:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_SMS_MEM_FULL_IND_T *)sig_ptr)->dual_sys));
                ATC_IndicateSysId(((APP_MN_SMS_MEM_FULL_IND_T*)sig_ptr)->dual_sys);
                ATC_ProcessSmsFullInd(S_CONFIG_INFO_PTR, (APP_MN_SMS_MEM_FULL_IND_T *)sig_ptr);
                break;
            }

            case APP_MN_UPDATE_SMS_STATE_CNF:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_UPDATE_SMS_STATUS_CNF_T);
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_UPDATE_SMS_STATUS_CNF_T *)sig_ptr)->dual_sys));
                #ifdef _ATC_SYNC_TOOL_
                if(recv_event_result)
                    ATC_ProcessUpdateSmsCnf(S_CONFIG_INFO_PTR, (APP_MN_UPDATE_SMS_STATUS_CNF_T*)sig_ptr);
                #endif
                break;
            }
    #endif
#if !defined( _ULTRA_LOW_CODE_ ) ||defined(_ATC_UIX8910_ENABLE_)
            case APP_MN_WRITE_SMS_CNF:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_WRITE_SMS_CNF_T);
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_WRITE_SMS_CNF_T *)sig_ptr)->dual_sys));
                if(recv_event_result)
                    ATC_ProcessWriteSmsCnf(S_CONFIG_INFO_PTR, (APP_MN_WRITE_SMS_CNF_T *)sig_ptr);
                break;
            }

            case APP_MN_SMS_IND:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_SMS_IND_T *)sig_ptr)->dual_sys));
                ATC_IndicateSysId(((APP_MN_SMS_IND_T*)sig_ptr)->dual_sys);
                ATC_ProcessSmsInd(S_CONFIG_INFO_PTR, (APP_MN_SMS_IND_T*)sig_ptr);
                break;
            }

            case APP_MN_AOC_MODIFIED_IND:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_AOC_MODIFIED_IND_T *)sig_ptr)->dual_sys));
                ATC_ProcessIncreaseAcmInd(S_CONFIG_INFO_PTR, (APP_MN_AOC_MODIFIED_IND_T*)sig_ptr);
                break;
            }

            // the following signals for cell broadcast service
            case APP_MN_SMSCB_MSG_IND:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_SMSCB_MSG_IND_T *)sig_ptr)->dual_sys));
                ATC_ProcessSmscbMsgInd(S_CONFIG_INFO_PTR, (APP_MN_SMSCB_MSG_IND_T*)sig_ptr);
                break;
            }

            // The following are signals for SS.
            case APP_MN_INTERROGATE_CNF:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_INTERROGATE_CNF_T);
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_INTERROGATE_CNF_T *)sig_ptr)->dual_sys));
                if(recv_event_result)
                    ATC_ProcessInterCnf(S_CONFIG_INFO_PTR, (APP_MN_INTERROGATE_CNF_T*)sig_ptr);
                break;
            }

            case APP_MN_DEACTIVATE_SS_CNF:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_DEACTIVATE_SS_CNF_T);
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_DEACTIVATE_SS_CNF_T *)sig_ptr)->dual_sys));
                if(recv_event_result)
                    ATC_ProcessDeactCnf(S_CONFIG_INFO_PTR, (APP_MN_DEACTIVATE_SS_CNF_T*)sig_ptr);
                break;
            }

            case APP_MN_ACTIVATE_SS_CNF:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_ACTIVATE_SS_CNF_T);
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_ACTIVATE_SS_CNF_T *)sig_ptr)->dual_sys));
                if(recv_event_result)
                    ATC_ProcessActCnf(S_CONFIG_INFO_PTR, (APP_MN_ACTIVATE_SS_CNF_T*)sig_ptr);
                break;
            }

            case APP_MN_ERASE_SS_CNF:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_ERASE_SS_CNF_T);
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_ERASE_SS_CNF_T *)sig_ptr)->dual_sys));
                if(recv_event_result)
                    ATC_ProcessEraseCnf(S_CONFIG_INFO_PTR, (APP_MN_ERASE_SS_CNF_T*)sig_ptr);
                break;
            }
                
            case APP_MN_REGISTER_SS_CNF:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_REGISTER_SS_CNF_T);
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_REGISTER_SS_CNF_T *)sig_ptr)->dual_sys));
                if(recv_event_result)
                    ATC_ProcessRegisterCnf(S_CONFIG_INFO_PTR, (APP_MN_REGISTER_SS_CNF_T*)sig_ptr);
                break;
            }
            
            case APP_MN_PUCT_INFO_CNF:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_PUCT_INFO_CNF_T);
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_PUCT_INFO_CNF_T *)sig_ptr)->dual_sys));
                if(recv_event_result)
                    ATC_ProcessPuctCnf(S_CONFIG_INFO_PTR,(APP_MN_PUCT_INFO_CNF_T*)sig_ptr);
                break;
            }

            //The following three is about CUSD.
            case APP_MN_USSD_IND:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_USSD_IND_T *)sig_ptr)->dual_sys));
                ATC_ProcessUssdInd(S_CONFIG_INFO_PTR,(APP_MN_USSD_IND_T *)sig_ptr);
                break;
            }

            case APP_MN_USSD_NOTIFY_IND:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_USSD_NOTIFY_IND_T *)sig_ptr)->dual_sys));
                ATC_ProcessUssdNotifyInd(S_CONFIG_INFO_PTR,(APP_MN_USSD_NOTIFY_IND_T *)sig_ptr);
                break;
            }

            case APP_MN_USSD_SERVICE_CNF:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_USSD_SERVICE_CNF_T);
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                if(recv_event_result)
                    ATC_ProcessUssdServiceCnf(S_CONFIG_INFO_PTR,(APP_MN_USSD_SERVICE_CNF_T *)sig_ptr);
                break;
            }

            case APP_MN_RELEASE_SS_IND:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_RELEASE_SS_IND_T);
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_RELEASE_SS_IND_T *)sig_ptr)->dual_sys));
                ATC_ProcessRelCnf(S_CONFIG_INFO_PTR, (APP_MN_RELEASE_SS_IND_T*)sig_ptr);
                break;
            }

            case APP_MN_GET_PASSWORD_IND:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_GET_PASSWORD_IND_T);
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_GET_PASSWORD_IND_T *)sig_ptr)->dual_sys));
                ATC_ProcessGetPswInd(S_CONFIG_INFO_PTR, (APP_MN_GET_PASSWORD_IND_T*)sig_ptr);
                break;
            }

            case APP_MN_REGISTER_PASSWORD_CNF:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_REGISTER_PASSWORD_CNF_T);
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_REGISTER_PASSWORD_CNF_T *)sig_ptr)->dual_sys));
                if(recv_event_result)
                    ATC_ProcessPwdCnf(S_CONFIG_INFO_PTR, (APP_MN_REGISTER_PASSWORD_CNF_T*)sig_ptr);
                break;
            }
#endif
            // the following is about the SIM signals 
            case APP_MN_SIM_NOT_READY_IND:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_SIM_NOT_READY_IND_T *)sig_ptr)->dual_sys));
                ATC_ProcessNotReadyInd(S_CONFIG_INFO_PTR, (APP_MN_SIM_NOT_READY_IND_T*)sig_ptr);
                break;
            }

            case APP_MN_SIM_READY_IND:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_SIM_READY_IND_T *)sig_ptr)->dual_sys));
                ATC_ProcessReadyInd(S_CONFIG_INFO_PTR, (APP_MN_SIM_READY_IND_T*)sig_ptr);
                break;
            }
 
#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)
            case APP_MN_SIM_PIN_FUNC_CNF:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_SIM_PIN_FUNC_CNF_T);
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_SIM_PIN_FUNC_CNF_T *)sig_ptr)->dual_sys));
                if(recv_event_result)
                    ATC_ProcessChvCnf(S_CONFIG_INFO_PTR, (APP_MN_SIM_PIN_FUNC_CNF_T*)sig_ptr);
                break;
            }

            case APP_MN_SIM_GET_PIN_IND:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_SIM_GET_PIN_IND_T *)sig_ptr)->dual_sys));
                ATC_ProcessPinInd(S_CONFIG_INFO_PTR, (APP_MN_SIM_GET_PIN_IND_T*)sig_ptr);
                break;
            }

            case APP_MN_PLMN_SELECT_CNF:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_PLMN_SELECT_CNF_T);
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_PLMN_SELECT_CNF_T *)sig_ptr)->dual_sys));
                if(recv_event_result)
                    ATC_ProcessPlmnSelCnf(S_CONFIG_INFO_PTR, (APP_MN_PLMN_SELECT_CNF_T*)sig_ptr);
                break;
            }
        #endif
#ifndef _ULTRA_LOW_CODE_
            case APP_MN_SCELL_RSSI_IND:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_SCELL_RSSI_IND_T);
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_SCELL_RSSI_IND_T *)sig_ptr)->dual_sys));
                ATC_ProcessRssiInd(S_CONFIG_INFO_PTR, (APP_MN_SCELL_RSSI_IND_T*)sig_ptr);
                break;
            }

            case APP_MN_CALL_READY_IND:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_CALL_READY_IND_T *)sig_ptr)->dual_sys));
                s_config_info.call_ready_flag[((APP_MN_CALL_READY_IND_T*)sig_ptr)->dual_sys] = TRUE;
                break;
            }
#endif
#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)
            case APP_MN_PLMN_LIST_CNF:
            {
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_PLMN_LIST_CNF_T *)sig_ptr)->dual_sys));
                if(recv_event_result)
                    ATC_ProcessPlmnListCnf(S_CONFIG_INFO_PTR, (APP_MN_PLMN_LIST_CNF_T*)sig_ptr);
                break;
            }
#endif
#ifndef _ULTRA_LOW_CODE_
#if 0
            //The following is about STK
            case APP_MN_SIMAT_DISPLAY_TEXT_IND:
            case APP_MN_SIMAT_GET_INKEY_IND:
            case APP_MN_SIMAT_GET_INPUT_IND:
            case APP_MN_SIMAT_SETUP_CALL_IND:
            case APP_MN_SIMAT_SETUP_MENU_IND:
            case APP_MN_SIMAT_SELECT_ITEM_IND:
            case APP_MN_SIMAT_SEND_SMS_IND:
            case APP_MN_SIMAT_SEND_SS_IND:
            case APP_MN_SIMAT_SEND_USSD_IND:
            case APP_MN_SIMAT_PLAY_TONE_IND:
            case APP_MN_SIMAT_PRO_LOCAL_INFO_IND:
            //signals which doesnt support
            case EV_MN_APP_SIMAT_REFRESH_IND_F:
            case SIMAT_EVENT_SETUP_EVENT_LIST: 
            case EV_MN_APP_SIMAT_SETUP_IDLE_TEXT_IND_F:  
            case EV_MN_APP_SIMAT_SEND_DTMF_IND_F: 
            case EV_MN_APP_SIMAT_LANG_NOTIFY_IND_F:   
            case SIMAT_MO_CC_RSP:
            case SIMAT_EVENT_SS_CON_RSP:   
            case SIMAT_EVENT_USSD_CON_RSP: 
            case EV_MN_APP_SIMAT_SMS_CTL_TO_MMI_IND_F:   
            case SIMAT_EVENT_SMS_PP_RSP:
            case SIMAT_EVENT_SMS_CB_RSP:
            {
#ifndef _MUX_REDUCED_MODE_ 
                ATC_ProcessSTKSignal(S_CONFIG_INFO_PTR,sig_ptr);
#endif
                break;
            }

            case SIMAT_SESSION_TERMINATED_IND:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_SIMAT_SESSION_TERMINATED_IND_T *)sig_ptr)->dual_sys));
#ifdef _MUX_REDUCED_MODE_
                SCI_MEMSET((uint8 *)g_rsp_str, 0, MAX_ATC_RSP_LEN);

                sprintf((char*)g_rsp_str, "%%SATI: \"\"");
                ATC_BuildInfoRsp(S_CONFIG_INFO_PTR, g_rsp_str);
#else
                ATC_ProcessSTKSignal(S_CONFIG_INFO_PTR,sig_ptr);
#endif
                break;
            }

            case ATC_STSF_EXP_IND:
            case ATC_STSF_EXP_IND1:
            case ATC_STSF_EXP_IND2:
            case ATC_STSF_EXP_IND3:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(sig_ptr->SignalCode - ATC_STSF_EXP_IND));
                ATC_ProcessTimerEXp(S_CONFIG_INFO_PTR);
                break;
            }
#endif
            // The following signals for battery charge service
            case ATC_CHR_CAP_IND:
            case ATC_CHR_WARNING_IND:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,atc_urc_link_id);
                ATC_ProcessChrCapInd(S_CONFIG_INFO_PTR,sig_ptr);
                break;
            }

            case ATC_CHR_CHARGE_START_IND:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,atc_urc_link_id);
                ATC_ProcessChrStartInd(S_CONFIG_INFO_PTR,sig_ptr);
                break;
            }

            case ATC_CHR_CHARGE_END_IND:
            case ATC_CHR_FINISH_IND:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,atc_urc_link_id);
                ATC_ProcessChrEndInd(S_CONFIG_INFO_PTR,sig_ptr);
                break;
            }
#endif/*_ULTRA_LOW_CODE_*/
#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)
            //The following is about phonebook.
            case APP_MN_ADN_UPDATE_CNF:
            case APP_MN_FDN_UPDATE_CNF:
            {
                //SCI_TRACE_LOW:"ATC:APP_MN_ADN_UPDATE_CNF"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_2577_112_2_17_22_16_23_64,(uint8*)"");
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_ADN_UPDATE_CNF_T);
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_ADN_UPDATE_CNF_T *)sig_ptr)->dual_sys));
                if(recv_event_result)
                    ATC_ProcessDelItemCnf(S_CONFIG_INFO_PTR, (APP_MN_ADN_UPDATE_CNF_T*)sig_ptr);
                break;
            }
#endif/*_ULTRA_LOW_CODE_*/

            case APP_MN_SET_BAND_CNF:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_SET_BAND_CNF_T);
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_SET_BAND_CNF_T *)sig_ptr)->dual_sys));
                if(recv_event_result)
                    ATC_ProcessSetBandCnf(S_CONFIG_INFO_PTR,(APP_MN_SET_BAND_CNF_T *)sig_ptr);
                break;
            }

            case APP_MN_PREFER_PLMN_CHANGE_CNF:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_PREFER_PLMN_CHANGE_CNF_T);
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_PREFER_PLMN_CHANGE_CNF_T *)sig_ptr)->dual_sys)); 
                if(recv_event_result)
                    ATC_ProcessPreferPlmnChangCnf(S_CONFIG_INFO_PTR, (APP_MN_PREFER_PLMN_CHANGE_CNF_T *)sig_ptr);
                break;
            }

            case APP_MN_CRSM_CNF:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_CRSM_CNF_T);
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_CRSM_CNF_T *)sig_ptr)->dual_sys)); 
                if(recv_event_result)
                    ATC_ProcessCrsmCnf(S_CONFIG_INFO_PTR, (APP_MN_CRSM_CNF_T *)sig_ptr);
                break;
            }

            case ATC_WRITE_RSP_REQ:
            {
                SCI_TRACE_LOW("ATC_WRITE_RSP_REQ, at_Rsp:%s",(uint8 *)((AT_WRITE_RSP_T*)sig_ptr)->str);
#ifdef _MUX_ENABLE_
                if(s_config_info.current_link_id == ATC_DEBUG_LINK)
                {
                SIO_ATC_WriteCmdRes((uint8 *)((AT_WRITE_RSP_T*)sig_ptr)->str,((AT_WRITE_RSP_T*)sig_ptr)->len);
                }
                else
                {
                MUX_Write((uint8 *)((AT_WRITE_RSP_T*)sig_ptr)->str,((AT_WRITE_RSP_T*)sig_ptr)->len,s_config_info.current_link_id);
                }
#else
#ifdef PPP_USB
                SIO_ATC_WriteCmdRes((uint8 *)((AT_WRITE_RSP_T*)sig_ptr)->str,((AT_WRITE_RSP_T*)sig_ptr)->len,ATC_GetSioPort());

#else
                SIO_ATC_WriteCmdRes((uint8 *)((AT_WRITE_RSP_T*)sig_ptr)->str,((AT_WRITE_RSP_T*)sig_ptr)->len);
#endif
#endif
                SCI_FREE(((AT_WRITE_RSP_T*)sig_ptr)->str);
                break;
            }

            case APP_MN_PS_POWER_OFF_CNF:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_PS_POWER_OFF_CNF_T);
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_PS_POWER_OFF_CNF_T *)sig_ptr)->dual_sys));

                ATC_ProcessPsPowerOffCnf(S_CONFIG_INFO_PTR, (xSignalHeaderRec*)sig_ptr,recv_event_result);

                break;
            }

            case APP_MN_PS_POWER_ON_CNF:
            {
                CHECK_CURRENT_CARD_ID_BRK(APP_MN_PS_POWER_ON_CNF_T);
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                ATC_SET_CURRENT_LINK_ID(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_PS_POWER_ON_CNF_T *)sig_ptr)->dual_sys));

                ATC_ProcessPsPowerOnCnf(S_CONFIG_INFO_PTR, (xSignalHeaderRec*)sig_ptr, recv_event_result);
                
                break;
            }
#if 0
            case APP_MN_SIMAT_FWD_PC_IND:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_SIMAT_FWD_PC_IND_T *)sig_ptr)->dual_sys));
                ATC_ProcessSIMATFwdInd(S_CONFIG_INFO_PTR, (xSignalHeaderRec*)sig_ptr);
                break;
            }

#ifndef _ULTRA_LOW_CODE_
            case APP_MN_SIMAT_REFRESH_CNF:
            {
                ATC_SET_CURRENT_LINK_ID_NO_EVENT(S_CONFIG_INFO_PTR,ATC_GetDualSimMuxLinkId(((APP_MN_SIMAT_REFRESH_CNF_T *)sig_ptr)->dual_sys));
                ATC_ProcessSIMRefreshCnf(S_CONFIG_INFO_PTR, (APP_MN_SIMAT_REFRESH_CNF_T *)sig_ptr);
                break;
            }
#endif/*_ULTRA_LOW_CODE_*/
#endif

#ifdef ATC_SAT_ENABLE            
#ifdef TCPIP_SUPPORT
            //handle asynchronous socket event
            case SOCKET_ASYNC_GETHOSTBYNAME_CNF:
            case SOCKET_CONNECT_EVENT_IND:
            case SOCKET_READ_EVENT_IND:
            case SOCKET_WRITE_EVENT_IND:
            case SOCKET_CONNECTION_CLOSE_EVENT_IND:
            {
            #ifndef _GSM_ONLY_
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_DOMAIN_PS);
                ATC_ProcessSocketEvent(S_CONFIG_INFO_PTR,sig_ptr);
            #endif
                break;
            }
#endif
#endif /*ATC_SAT_ENABLE*/

#ifdef _SUPPORT_GPRS_
            case MN_ATC_GPRS_RESUME_IND:
            {
                ATC_HandleDataReqPendingList(((APP_MN_GPRS_RESUME_IND_SIG*)sig_ptr)->nsapi);
                break;
            }
#endif
#if 0
#ifdef BT_PS_BQB_TEST
            //handle bluetooth L2CPA  event
            case ID_STATUS_TEST_L2CAP_CREAT_ACL_CNF:
            case ID_STATUS_TEST_L2CAP_DIS_ACL_CNF:
            case ID_STATUS_TEST_L2CAP_REG_CNF:
            case ID_STATUS_TEST_L2CAP_CONN_CNF:
            case ID_STATUS_TEST_L2CAP_DIS_CNF:
            case ID_STATUS_TEST_L2CAP_SENDDATA_CNF:
            case ID_STATUS_TEST_L2CAP_ECHO_CNF:
            case ID_STATUS_TEST_L2CAP_DATA_IND:
            {
                SCI_TRACE_LOW("ATC:process bt l2cap msg.");
                ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                ATC_ProcessL2capTestMsg(S_CONFIG_INFO_PTR,sig_ptr);
                break;
            }
#endif
#endif

#if defined (BLUETOOTH_SUPPORT_SPRD_BT)
        case APP_BT_ME_ON_CNF:   
        case APP_BT_ME_OFF_CNF:	
        case APP_BT_INQ_DEV_NAME:	
        case APP_BT_INQ_COMP_CNF:	
        case APP_BT_INQUIRY_CANCEL:	
        case APP_BT_VISIBILE_CNF:	
        case APP_BT_HIDDEN_CNF:	
        case APP_BT_DEV_PIN_REQ: 
        case APP_BT_SSP_NUM_IND:	
        case APP_BT_DEV_PAIR_COMPLETE:
        case APP_BT_DEV_PAIR_COMPLETE_IND:
        case APP_BT_DELETE_DEVICE_RES:
        case APP_BT_SPP_DATA_RECIEVE_IND:
        case APP_BT_SPP_DATA_RECIEVE_CNF:
        case APP_BT_SPP_CONNECT_IND:
        case APP_BT_SPP_DISCONNECT_IND:
            
#ifdef CHIP_VER_EX_SL6210C
#ifdef BT_DIALER_SUPPORT            
        case APP_BT_HFU_CONNECT_CNF:
        case APP_BT_HFU_DISCONNECT_CNF:
        case APP_BT_HFU_CLIP_IND:
        case APP_BT_HFU_CLCC_RECORD_IND:
        case APP_BT_HFU_AUDIO_DISCONNECTED:
        case APP_BT_HFU_AUDIO_CONNECTED:
        case APP_BT_HFU_HANGUP_RESULT_IND:
        case APP_BT_HFU_REDIAL_RESULT_IND:
        case APP_BT_HFU_ATA_RESULT_IND:
#endif      

#ifdef BT_HID_SUPPORT
        case APP_BT_HID_CONNECT_IND:
        case APP_BT_HID_DISCONNECT_IND:
#endif 

#ifdef BT_AV_SNK_SUPPORT
        case APP_BT_A2DPSNK_CONNECT_CNF:
        case APP_BT_A2DPSNK_DISCONNECT_CNF:
#endif 

#ifdef BT_AV_CT_SUPPORT
        case APP_BT_AVRCP_CT_MEDIA_ATTRIBUTES:
        case APP_BT_AVRCP_SEND_REWIND_OR_FASTFWD_SINGNAL:
#endif 
#endif
        case APP_BT_HFAG_SLC_CONNECTED:
        case APP_BT_HFAG_SLC_CONNECTED_IND:
        case APP_BT_HFAG_SLC_DISCONNECTED:
        case APP_BT_HFAG_PLAY_CFM:
        case APP_BT_HFAG_STOP_CFM:
        case APP_BT_HFAG_HFU_CALL_ANSWER:
        case APP_BT_HFAG_HFU_CALL_HANGUP:
        case APP_BT_HFAG_HFU_DIAL_NUMBER:
        case APP_BT_HFAG_HFU_DIAL_MEMORY:
        case APP_BT_HFAG_HFU_DIAL_REDIAL:
        case APP_BT_HFAG_HFU_HOLD:
        case APP_BT_HFAG_HFU_DTMF_NUMBER:
        case APP_BT_HFAG_AUDIO_CONNECTED:
        case APP_BT_HFAG_AUDIO_DISCONNECTED:
        case APP_BT_HFAG_HFU_STATUS_REQ:
        case APP_BT_HFAG_HS_MIC_VOL_CHANGE:
        case APP_BT_HFAG_HS_SPK_VOL_CHANGE:

        case AT_BT_GATT_SERVER_REG_READ_RES:       
        case AT_BT_GATT_SERVER_REG_WRITE_RES: 
        case AT_BT_GATT_SERVER_DEREG_WRITE_RES:
        case AT_BT_GATT_SERVER_SERVICE_READ_RES:
        case AT_BT_GATT_SERVER_SERVICE_ADD_WRITE_RES:
        case AT_BT_GATT_SERVER_SERVICE_DEL_WRITE_RES:
        case AT_BT_GATT_SERVER_CHAR_WRITE_RES:
        case AT_BT_GATT_SERVER_DESC_WRITE_RES:
        case AT_BT_GATT_SERVER_SERVICE_START_WRITE_RES:
        case AT_BT_GATT_SERVER_SERVICE_STOP_WRITE_RES:
        case AT_BT_GATT_SERVER_INDICAT_NOTIFY_WRITE_RES:
        case AT_BT_GATT_SERVER_REQ_RECIEVE_IND:
        case AT_BT_GATT_SERVER_RSP_WRITE_RES:
        case AT_BT_GATT_SERVER_CONN_READ_RES:
        case AT_BT_GATT_SERVER_DISCONN_WRITE_RES:

            ATC_ProcessBtSignal(S_CONFIG_INFO_PTR, sig_ptr);
            break;
#endif

#ifdef _MNGPS_SUPPORT_
            case APP_MN_GPS_DOWNLOAD_CNF:
            case APP_MN_GPS_INIT_CNF:
            case APP_MN_GPS_START_CNF:
            case APP_MN_GPS_STOP_CNF:
            case APP_MN_GPS_RESET_CNF:
            case APP_MN_GPS_READ_STATUS_CNF:
            case APP_MN_GPS_READ_INFO_CNF:
            case APP_MN_GPS_OUTPUT_CNF:
            case APP_MN_GPS_POWEROFF_CNF:
            case APP_MN_GPS_OUTPUT_INFO_IND:
            case APP_MN_GPS_ERR_IND:
            case APP_MN_GPS_FIX_IND:
            {
                SCI_TRACE_LOW("ATC: mngps sig_ptr->SignalCode=%d", sig_ptr->SignalCode);
                if(APP_MN_GPS_OUTPUT_INFO_IND != sig_ptr->SignalCode)
                {
                	recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                
                }
                else
                {
                    recv_event_result = TRUE;
                }
                if(recv_event_result)
                    ATC_ProcessGpsEvent(S_CONFIG_INFO_PTR,sig_ptr);
                
            }
            
            break;
#endif

            case APP_MN_SIM_POWER_ON_CNF:
            case APP_MN_SIM_POWER_OFF_CNF:
                recv_event_result = ATC_RECEIVE_EXPECTED_EVENT(S_CONFIG_INFO_PTR,sig_ptr->SignalCode,0xff,ATC_NO_DOMAIN);
                if(recv_event_result)
                    ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR, S_ATC_RESULT_CODE_OK, 0);
                break;

            case APP_MN_CALL_AUDIO_RESTART_IND:
                if (FilterMsgFromATCmdCallEX(sig_ptr->SignalCode))
                {
                    ATC_ProcessCallAudioRestart(sig_ptr);
                }
                break;

            default:
                break;
        }  
        SCI_FREE_SIGNAL(sig_ptr);
    }
}

/******************************************************************************/
// Description : This function do some initialization work for ATC
// Global resource dependence : g_atc_info_table
//                              g_atc_handle_table
//                              g_atc_result_code_text
// Author : Steven
// Note : This function should be called first on starting ATC task
/******************************************************************************/
LOCAL ATC_STATUS ATC_Initialize( void // Retur S_ATC_SUCCESS if success,
                                                             ) // otherwise return error code.
        
{
    ATC_STATUS    status = S_ATC_SUCCESS;
    POWER_RESTART_CONDITION_E      start_condition = POWER_GetRestartCondition();
    int32    i  = 0;
    int32    j  = 0;

    status = ATCM_CreateHeap();
    if ( S_ATC_SUCCESS != status )
    {
        //ATC_TRACE:"ATC initialize failed."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_2704_112_2_17_22_16_23_65,(uint8*)"" );
        return status;
    }

    // reset the static variable
    SCI_MEMSET(&s_atc_global_info, 0, sizeof(s_atc_global_info));
#ifdef _MUX_ENABLE_
    SCI_MEMSET((void *)s_setup_menu_len,0,sizeof(s_setup_menu_len));
#endif
    
#ifdef _MUX_ENABLE_
   for(i=0; i<ATC_MAX_LINK_ID; i++)
   {
       s_atc_global_info.atc_link_info[i].cur_cmd_line_buf_ptr = NULL;
       s_atc_global_info.atc_link_info[i].last_cmd_line_buf_ptr = NULL;
   }
#else
    s_atc_global_info.cur_cmd_line_buf_ptr  = NULL;
    s_atc_global_info.last_cmd_line_buf_ptr = NULL;
#endif
    s_atc_global_info.buffered_at_cmd_count = 0;
    s_atc_global_info.atc_info_table        = g_atc_info_table;
    s_atc_global_info.atc_handle_table      = g_atc_handle_table;
    s_atc_global_info.atc_config_ptr        = &s_config_info;

    for (i = MN_DUAL_SYS_1; i < MN_SYS_NUMBER; i++)
    {
        SCI_MEMSET(&g_calls_state[i], 0, sizeof(g_calls_state[i]));
        for (j = 0; j<MN_CALL_MAX_CALL_NUM; j++)
        {
            g_calls_state[i].call_context[j].call_id = ATC_INVALID_CALLID;
        }
    }

    ATC_InitConfig(s_atc_global_info.atc_config_ptr);
    
#ifdef _MUX_ENABLE_
    //default atc buffer
    atc_buffer[0].atc_buf_ptr  = atc_rec_buffer0;
    atc_buffer[0].size            = ATC_REC_BUF_SIZE;
    atc_buffer[0].start_point  = 0;
    atc_buffer[0].end_point    = 0;
    atc_buffer[ATC_DEBUG_LINK].atc_buf_ptr = atc_rec_bufferdebug;
    atc_buffer[ATC_DEBUG_LINK].size            = ATC_REC_BUF_SIZE;
    atc_buffer[ATC_DEBUG_LINK].start_point  = 0;
    atc_buffer[ATC_DEBUG_LINK].end_point   = 0;

    for(i=1; i<ATC_DEBUG_LINK; i++)
    {
       atc_buffer[i].atc_buf_ptr = NULL;
       atc_buffer[i].size            = 0;
       atc_buffer[i].start_point  = 0;
       atc_buffer[i].end_point   = 0;
    }
    
    for(i=0; i<ATC_MAX_LINK_ID; i++)
    {
        cur_atc_cmd_size[i] = 0;
    }
    
    for(i=1; i<ATC_DEBUG_LINK; i++)
    {
        ATC_Mux_Setup_Link(i);
    }

    for(i=0;i<ATC_MAX_LINK_ID;i++)
    {
        atc_bs_ch[i] = ATC_BACKSPACE_CHAR;
    }
#endif

    
    //Register event 
    ATC_RegisterEvent(); 

    if ((RESTART_BY_ALARM != start_condition &&RESTART_BY_CHARGE != start_condition)
        || 1 == CLASSMARK_GetModelType() )
    {
        /*at worked as a pipo, initial is not a must. */	
        //set the audio mode
        // AUDIO_SetDevMode(s_atc_global_info.atc_config_ptr->audio_mode);
        //set the volume.
        // AUDIO_SetVolume(AUDIO_DEVICE_MODE_HANDHOLD,AUD_TYPE_VOICE,s_atc_global_info.atc_config_ptr->speaker_volume);
        /*at worked as a pipo, initial is not a must.  */
        
#ifdef _ATC_ONLY 
        MNPHONE_StartupPs(MN_GMMREG_RAT_GPRS);
#endif
    }

#ifndef _MUX_ENABLE_
    if (s_atc_global_info.atc_config_ptr->atc_config_nv.mux_mode)
    {
        SIO_SetUserPortMode(MIX_MODE);
    }
    else
    {
        SIO_SetUserPortMode(ATC_MODE);
    }
#endif

if ((RESTART_BY_ALARM != start_condition &&RESTART_BY_CHARGE != start_condition)
    || 1 == CLASSMARK_GetModelType() )
{
    /*at worked as a pipo, initial is not a must. */	
    //set the audio mode
    //AUDIO_SetDevMode(s_atc_global_info.atc_config_ptr->audio_mode);
    //set the volume.
    //AUDIO_SetVolume(AUDIO_DEVICE_MODE_HANDHOLD,AUD_TYPE_VOICE,s_atc_global_info.atc_config_ptr->speaker_volume);
    /*at worked as a pipo, initial is not a must.  */

#ifdef _MUX_ENABLE_
#ifdef _ATC_ONLY 
    for (i = MN_DUAL_SYS_1; i < MN_DUAL_SYS_MAX; i++)
    {
        MNPHONE_PowerOnSimEx(i, TRUE);
    }

#ifdef _MUX_REDUCED_MODE_
    //MUX_IsNeedToActivePS return TRUE if MS runs in cal_pos mode.
    if(RM_GetCalibrationPostMode())
    {
        MNPHONE_PowerOnPsEx(MN_DUAL_SYS_1);
    }
    
#else

    for (i = MN_DUAL_SYS_1; i < MN_DUAL_SYS_MAX; i++)
    {
        MNPHONE_PowerOnPsEx(i);
    }
#endif
#endif

#else
#ifdef _ATC_ONLY
    MNPHONE_StartupPs(MN_GMMREG_RAT_GPRS);
#endif
#endif
}

#ifdef _MUX_ENABLE_
    is_first_sync = TRUE;
#endif
    
    //Send the unsolicited +SIND: 3
    ATC_UpSINDInfo(MN_DUAL_SYS_1, s_atc_global_info.atc_config_ptr, PNULL, ATC_WIND_BIT2, 3);

    //Send the unsolicited +SIND: 4
#ifndef _MUX_REDUCED_MODE_
    ATC_UpSINDInfo(MN_DUAL_SYS_1, s_atc_global_info.atc_config_ptr, PNULL, ATC_WIND_BIT3, 4);
#endif

#ifdef _MUX_ENABLE_
    is_first_sync = FALSE;
#endif

    return status;
}

#ifdef _MUX_ENABLE_
/******************************************************************************/
// Description : This function reads a new AT command line from cmux unit and 
//               parses it
// Global resource dependence : 
// Author : Wang Hui
// Note :  
/******************************************************************************/

LOCAL ATC_STATUS ATC_RecNewLineSig(      // Return S_ATC_SUCCESS if success, 
                                                                           // otherwise return error code.
        ATC_GLOBAL_INFO_T      *atc_global_info_ptr, // point to the ATC_GLOBAL_INFO_T struct.
        const xSignalHeaderRec *sig_ptr                      // point to signal struct 
        )
{
    ATC_STATUS                       status    = S_ATC_SUCCESS;
    uint                      atc_line_length    = 0;
    uint                    atc_read_length    = 0;
    uint8                           atc_link_id    =0;
    uint                                para_len    = 0;
    uint8                              para_str[ATC_SHARP_LEN] = {0};
    ATC_CUR_INFO_LIST_T     *atc_cur_info_ptr             = NULL;
    ATC_CUR_INFO_LIST_T     *atc_last_info_ptr             = NULL;

    SCI_ASSERT( NULL != atc_global_info_ptr ); /*assert verified check NULL pointer*/
    SCI_ASSERT( NULL != sig_ptr );   /*assert verified check NULL pointer*/
    SCI_ASSERT( ATC_MUX_RECV_NEW_AT == sig_ptr->SignalCode ); /*assert verified check signal code*/

    /* Get new AT command line length */
    atc_line_length =(uint)(((ATC_MUX_RECV_NEW_AT_T *)sig_ptr)->len);
    atc_link_id = ((ATC_MUX_RECV_NEW_AT_T *)sig_ptr)->link_id;
    atc_global_info_ptr->atc_config_ptr->current_link_id = atc_link_id;	

     //SCI_TRACE_LOW:"ATC: ATC_RecNewLineSig,link_id:%d,len:%d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_2923_112_2_17_22_16_24_66,(uint8*)"dd",atc_link_id,atc_line_length);

    if (0 == atc_line_length)    
    {
        return S_ATC_FAIL;
    }
   
    /* Too long atc command,just clear the atc buffer
     */
    if(atc_line_length > ATC_MAX_CMD_LINE_LENGTH - 2)
    {
        ATC_ClrCmdLine(atc_link_id);
        status = ERR_TEXT_STRING_TOO_LONG;

        //restore AT command parase mode
        if(atc_link_id == ATC_DEBUG_LINK)
        {
            SIO_ATC_SetCmdLineTerminateChar(atc_global_info_ptr->atc_config_ptr->s3_reg, '\0');
        }
        else
        {
            ATC_SetCmdLineTerminateChar(atc_link_id,atc_global_info_ptr->atc_config_ptr->s3_reg, '\0');
        }

        ATC_BuildResultCodeRsp(atc_global_info_ptr->atc_config_ptr, S_ATC_RESULT_CODE_ERROR, status);

        return status;
    } 
     /* 
     ** If the new cmd line two short, it could be an "A/" command.
     ** So we can use the short buf to store it.
     */
    else if( atc_line_length <= ATC_MIN_CMD_LINE_LENGTH )
    {
        if ( atc_global_info_ptr->atc_link_info[atc_link_id].last_cmd_line_buf_ptr ==
                atc_global_info_ptr->atc_link_info[atc_link_id].cmd_line_buf_short )
        {
            SCI_MEMCPY(atc_global_info_ptr->atc_link_info[atc_link_id].cmd_line_buf_long,atc_global_info_ptr->atc_link_info[atc_link_id].cmd_line_buf_short,
                      ATC_MIN_CMD_LINE_LENGTH);

            atc_global_info_ptr->atc_link_info[atc_link_id].last_cmd_line_buf_ptr = 
                atc_global_info_ptr->atc_link_info[atc_link_id].cmd_line_buf_long;
        }

        atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr = 
                atc_global_info_ptr->atc_link_info[atc_link_id].cmd_line_buf_short;
    }
    else 
    {
        atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr = 
            atc_global_info_ptr->atc_link_info[atc_link_id].cmd_line_buf_long;
    }

    /*
     ** Read AT command line from ATC buffer
     */
    ATC_ReadCmdLine(atc_link_id,atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr,
                                    atc_line_length,&atc_read_length);

    if ( atc_read_length != atc_line_length )
    {
        //SCI_TRACE_LOW:"ATC: backspace? len: %d->%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_2984_112_2_17_22_16_24_67,(uint8*)"dd", atc_line_length, atc_read_length);
        atc_line_length = atc_read_length;
    }
   
    /* Add two NULL terminater for atcparse */
    atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr[atc_line_length] = '\0';
    atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr[atc_line_length + 1] = '\0';

    if((1 == atc_read_length) || (2 == atc_read_length && 0x0A==atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr[0]))
    {
        return S_ATC_DISCARD;

    }
    
    /* trace out the at command line */
    if(atc_line_length < 200)//please make sure all the context length is less than 250 in the quotation mark including prefixed characters
    {
        uint8 tchar[42] = {0};
        
        ConvertBinToHex(atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr, (atc_line_length>20)? 20:atc_line_length, tchar);
        //SCI_TRACE_LOW:"ATC: ATC_RecNewLineSig, len:%d,hex:%s,line:%s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_3004_112_2_17_22_16_24_68,(uint8*)"dss",atc_line_length, tchar, atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr);
    }
    else
    {
        //SCI_TRACE_LOW:"ATC: ATC_RecNewLineSig, len:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_3008_112_2_17_22_16_24_69,(uint8*)"d",atc_line_length);
    }

    if(atc_global_info_ptr->atc_config_ptr->s3_reg  == *atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr
        ||atc_global_info_ptr->atc_config_ptr->s4_reg == *atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr)
    {
        atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr ++;
        atc_line_length --;
    }

    // remove ; from atc line for atcparase
    if((2 < atc_line_length)
        &&  (atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr[atc_line_length - 1] == atc_global_info_ptr->atc_config_ptr->s3_reg) 
        &&  (atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr[atc_line_length - 2] == ';'))
    {
        atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr[atc_line_length - 2] = atc_global_info_ptr->atc_config_ptr->s3_reg;
        atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr[atc_line_length - 1] = '\0'; //add null terminater for atcparse
        atc_line_length --;
    }

    if (atc_global_info_ptr->atc_config_ptr->atc_link_config[atc_link_id].is_ctrlz_end)
    {
        atc_global_info_ptr->atc_config_ptr->atc_link_config[atc_link_id].is_ctrlz_end = FALSE;

        if(ATC_DEBUG_LINK == atc_link_id)
        {
            SIO_ATC_SetCmdLineTerminateChar(atc_global_info_ptr->atc_config_ptr->s3_reg, '\0');	
        }
        else
        {
            ATC_SetCmdLineTerminateChar(atc_link_id,atc_global_info_ptr->atc_config_ptr->s3_reg, '\0');
        }

        switch(atc_global_info_ptr->atc_config_ptr->atc_link_config[atc_link_id].ctrlz_end_status)
        {
            case ATC_CTRLZ_SMS:
            {
                status = ATC_ProcessSmsPdu(atc_global_info_ptr->atc_config_ptr, 
                atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr);
                break;
            }

#if 0
            case ATC_CTRLZ_STK:
            {
            #ifndef _ULTRA_LOW_CODE_
                status = ATC_ProcessSioStkData(atc_global_info_ptr->atc_config_ptr, 
                atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr);
            #endif
                break;
            }
#endif
            
            case ATC_CTRLZ_SRWD:
            {
            #ifndef _ULTRA_LOW_CODE_
                status = ATC_ProcessSioSRWDData(atc_global_info_ptr->atc_config_ptr, 
                atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr);
            #endif
                break;
            }

            case ATC_CTRLZ_SSEA:
            {
                status = ATC_ProcessSioSSEAData(atc_global_info_ptr->atc_config_ptr, 
                atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr,atc_line_length); 
                break;
            }

            case ATC_CTRLZ_SADM:
            {
            #ifdef _AUDIO_ENHANCE_
                status = ATC_ProcessSioSADMData(atc_global_info_ptr->atc_config_ptr, 
                atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr,atc_line_length);
            #endif 
                break;
            }

#ifdef ATC_SAT_ENABLE
            case ATC_CTRLZ_GPRS:
            {
            #ifndef _GSM_ONLY_
                status = ATC_ProcessSioData(atc_global_info_ptr->atc_config_ptr, 
                atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr, atc_line_length);
            #endif
                break;
            }
#endif /*ATC_SAT_ENABLE*/

            case ATC_CTRLZ_SNVM:
            {
            #ifdef _ATC_ONLY
                status = ATC_ProcessSioSNVMData(atc_global_info_ptr->atc_config_ptr, 
                atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr);
            #endif
                break;
            }

            case ATC_CTRLZ_SADM_ARM:
            {
            #ifdef AUDIO_PARAM_ARM
                status = ATC_ProcessSioARMSADMData(atc_global_info_ptr->atc_config_ptr,
                atc_global_info_ptr->cur_cmd_line_buf_ptr, atc_line_length);
            #endif
                break;
            }

            case ATC_CTRLZ_SADMNV_ARM:
            {
            #ifdef AUDIO_PARAM_ARM
                status = ATC_ProcessSioARMSADMNVData(atc_global_info_ptr->atc_config_ptr,
                atc_global_info_ptr->cur_cmd_line_buf_ptr, atc_line_length);
            #endif
                break;
            }

            case ATC_CTRLZ_CVS:
            {
                status = ATC_ProcessSioSADCVSData(atc_global_info_ptr->atc_config_ptr,
                    atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr,atc_line_length);
                break;
            }
            case ATC_CTRLZ_CVSNV:
            {
                status = ATC_ProcessSioSADCVSNVData(atc_global_info_ptr->atc_config_ptr,
                    atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr,atc_line_length);
                break;
            }
            case ATC_CTRLZ_ENHA:
            {
                status = ATC_ProcessSioENHA_Data(atc_global_info_ptr->atc_config_ptr,
                                                atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr,atc_line_length);
                break;
            }
        
            case ATC_CTRLZ_ENHA_TUN:
            {
                status = ATC_ProcessSioENHA_Tun_Data(atc_global_info_ptr->atc_config_ptr,
                                                atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr,atc_line_length);
                break;
            }
        
            case ATC_CTRLZ_ENHANV:
            {
                status = ATC_ProcessSioENHA_NVData(atc_global_info_ptr->atc_config_ptr,
                                                atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr,atc_line_length);
                break;
            }
        
            case ATC_CTRLZ_ENHANV_TUN:
            {
                status = ATC_ProcessSioENHA_Tun_NVData(atc_global_info_ptr->atc_config_ptr,
                                                atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr,atc_line_length);
                break;
            }
           case ATC_CTRLZ_SIMLOCKDATA:
           {
                SCI_TRACE_LOW("ATC_CTRLZ_SIMLOCKDATA ATC_ProcessSimlockData");
                status = ATC_ProcessSimlockData(atc_global_info_ptr->atc_config_ptr, 
                                                  atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr);
                break;	
            }
           
            default:
            {
                SCI_ASSERT(FALSE); /*assert verified check default case*/
                break;
            }
        }
    }
    else
    {
        /* Parse the AT command line */
        zerolex(); //add for reset lex state

        if(atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr)
        {
            const uint8 *buf_ptr = NULL;
            uint             buf_len = 0;

            buf_ptr = atcparse_at(atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr, atc_line_length);
            buf_len = atc_line_length - ((uint32)buf_ptr - (uint32)atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr);

            //SCI_TRACE_LOW:"ATC: ATC_RecNewLineSig, ignore ip data, len:%d->%d, new atcmd:%s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_3236_112_2_17_22_16_25_70,(uint8*)"dds", atc_line_length, buf_len, (const char *)buf_ptr);
            atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr = (uint8 *)buf_ptr;
            atc_line_length = buf_len;
        }

        atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr[atc_line_length-1] = 0xD;
        status = atcparse(atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr,&atc_cur_info_ptr);

        if(((0x41 == *atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr)
            ||  (0x61 == *atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr))
            && ((0x54 == *(atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr+1))
            ||  (0x74 == *(atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr+1)))
            && (0x23 == *(atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr+2))
            && (0x23 == *(atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr+3)))
        {
            SCI_MEMSET(para_str,0,ATC_SHARP_LEN);
            para_len = atc_line_length-4;

            if(para_len > ATC_SHARP_LEN)
            {
                ATCM_FreeAll();
                //SCI_TRACE_LOW:"ATC: ATC_RecNewLineSig, Assert Failure sharp len too long."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_3257_112_2_17_22_16_25_71,(uint8*)"");
                return ERR_OPERATION_NOT_SUPPORTED;
            }

            SCI_MEMCPY(para_str,(atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr+4),para_len);
            para_len = para_len - 1;
            //call the extern function
            //SCI_TRACE_LOW:"ATC: ATC_RecNewLineSig, Call the external function"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_3264_112_2_17_22_16_25_72,(uint8*)"");

#ifndef _ATC_ONLY
#ifdef SYNCTOOL_SUPPORT
            ATC_ExtCmd(para_str, para_len);
#endif
#endif

            ATCM_FreeAll();

            //need do nothing while received AT## command
            return S_ATC_DEFAULT_HANDLE;
        }

        if(((0x41 == *atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr)
            || (0x61 == *atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr))
            &&((0x54 == *(atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr+1))
            || (0x74 == *(atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr+1)))
            &&((0x44 == *(atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr+2))
            || (0x64 == *(atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr+2)))
            &&(0x2b == *(atc_global_info_ptr->atc_link_info[atc_link_id].cur_cmd_line_buf_ptr+3)))
        {
            g_atc_international_call = TRUE;
        }
        else
        {
            g_atc_international_call = FALSE;
        }

    }
    
    switch ( status ) 
    {
        case S_ATC_SUCCESS:
        {
            SCI_ASSERT( NULL != atc_cur_info_ptr ); /*assert verified check NULL pointer*/
            //SCI_TRACE_LOW:"ATC: ATC_RecNewLineSig, Parse AT cmd line success."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_3300_112_2_17_22_16_25_73,(uint8*)"" );

            if(atc_cur_info_ptr->next) 
            {
                //SCI_TRACE_LOW:"ATC: cmd next %x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_3304_112_2_17_22_16_25_74,(uint8*)"d",atc_cur_info_ptr->next->cmd_index);
            }
            
            /*
            ** we link the infos of AT cmds to the end of the list which 
            ** store in atc_config 
            */
            if ( NULL == atc_global_info_ptr->atc_link_info[atc_link_id].atc_info_ptr )
            {
                atc_global_info_ptr->atc_link_info[atc_link_id].atc_info_ptr = atc_cur_info_ptr;
            }
            else
            {
                atc_last_info_ptr = atc_global_info_ptr->atc_link_info[atc_link_id].atc_info_ptr;

                while ( NULL != atc_last_info_ptr->next )
                {
                    atc_last_info_ptr = atc_last_info_ptr->next;
                }

                atc_last_info_ptr->next = atc_cur_info_ptr;
            }
            break;
        }
            
        case S_ATC_DEFAULT_HANDLE:
        {
            //SCI_TRACE_LOW:"ATC: ATC_RecNewLineSig, Parse SMS PDU successful"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_3331_112_2_17_22_16_25_75,(uint8*)"");
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC: ATC_RecNewLineSig, Parse AT command line failed."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_3337_112_2_17_22_16_25_76,(uint8*)"" );
            //response the result code
            ATC_BuildResultCodeRsp(atc_global_info_ptr->atc_config_ptr, S_ATC_RESULT_CODE_ERROR, status);
            break;
        }
    }
    
     return status;
}

/******************************************************************************/
// Description : This function processes the AT commands stored in the AT 
//               config struct one by one.
// Global resource dependence : None
// Author : Steven
// Note : None
/******************************************************************************/
ATC_STATUS ATC_ProcessCmds( // Return S_ATC_SUCCESS if success, 
                                                         // otherwise return error code.
        ATC_GLOBAL_INFO_T     * atc_global_info_ptr // point to the ATC_GLOBAL_INFO_T struct.
        )
{
    ATC_STATUS  status      = S_ATC_SUCCESS;
    uint8       atc_link_id     = 0;

    SCI_ASSERT( NULL != atc_global_info_ptr ); /*assert verified check NULL pointer*/

    atc_link_id = atc_global_info_ptr->atc_config_ptr->current_link_id;
    //SCI_TRACE_LOW:"ATC: prcesscmds link %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_3363_112_2_17_22_16_25_77,(uint8*)"d", s_atc_global_info.atc_config_ptr->current_link_id);

    while ( NULL != atc_global_info_ptr->atc_link_info[atc_link_id].atc_info_ptr )
    {
        status = ATC_DespatchCmd( atc_global_info_ptr, atc_global_info_ptr->atc_link_info[atc_link_id].atc_info_ptr );
        if ( S_ATC_SUCCESS != status )
        {
            atc_global_info_ptr->atc_link_info[atc_link_id].atc_info_ptr = PNULL;
            break;
        }
        atc_global_info_ptr->atc_link_info[atc_link_id].atc_info_ptr = atc_global_info_ptr->atc_link_info[atc_link_id].atc_info_ptr->next;     
    }
    
    switch ( status )
    {
        case S_ATC_SUCCESS:
        {
            SCI_ASSERT ( NULL == atc_global_info_ptr->atc_link_info[atc_link_id].atc_info_ptr ); /*assert verified check NULL pointer*/

            if( is_cmux_recv )
            {
                uint32    i = 0;

                MUX_Startup(MUX_SIDE_UE, cmux_sys_para);
                is_cmux_recv=FALSE;
                
                //SHIFENG add 
                for (i = MN_DUAL_SYS_1; i < MN_SYS_NUMBER; i++)
                {
                    g_mux_dlc_info[i + 1].dlci           = i + 1;
                    g_mux_dlc_info[i + 1].from          = MUX_1;
                    g_mux_dlc_info[i + 1].used_state = MUX_DLC_STATE_CONNECT;
                }

                for (i = 1; i <= ATC_PDP_NUM_MAX; i++)
                {
                    g_mux_dlc_info[i + MN_SYS_NUMBER].dlci           = i + MN_SYS_NUMBER;
                    g_mux_dlc_info[i + MN_SYS_NUMBER].from          = MUX_1;
                    g_mux_dlc_info[i + MN_SYS_NUMBER].used_state = MUX_DLC_STATE_CONNECT;
                }
            }

            //SCI_TRACE_LOW:"ATC: 5 atc_global_info config link %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_3405_112_2_17_22_16_25_78,(uint8*)"d", s_atc_global_info.atc_config_ptr->current_link_id);
            ATC_BuildResultCodeRsp(atc_global_info_ptr->atc_config_ptr, S_ATC_RESULT_CODE_OK ,0);
            break;
        }

        case S_ATC_LAST_ERROR:
        {
            ATC_BuildResultCodeRsp(atc_global_info_ptr->atc_config_ptr,
            S_ATC_RESULT_CODE_OK ,0);
            ATC_BuildResultCodeRsp(atc_global_info_ptr->atc_config_ptr,
            S_ATC_RESULT_CODE_ERROR ,status);
            status = S_ATC_SUCCESS;
            break;
        }

        case S_ATC_DEFAULT_HANDLE:
        {
            break;
        }

        default:
        {
            ATC_BuildResultCodeRsp(atc_global_info_ptr->atc_config_ptr,
            S_ATC_RESULT_CODE_ERROR ,status);
            break;
        }
    }

    // If the AT commands are all handled, we free our private heap mem 
    ATCM_FreeAll();
    return status;
}

#else/*_MUX_ENABLE_*/
/******************************************************************************/
// Description : This function reads a new AT command line from sio unit and 
//               parses it
// Global resource dependence : 
// Author : Steven
// Note :   Modify: Xueli
/******************************************************************************/
LOCAL ATC_STATUS ATC_RecNewLineSig( // Return S_ATC_SUCCESS if success, 
                                                                      // otherwise return error code.
        ATC_GLOBAL_INFO_T      *atc_global_info_ptr, // point to the ATC_GLOBAL_INFO_T struct.
        const xSignalHeaderRec *sig_ptr                      // point to signal struct 
        )
{
    ATC_STATUS                                status    = S_ATC_SUCCESS;
    uint                               atc_line_length    = 0;
    uint                             atc_read_length    = 0;
    uint8              para_str[ATC_SHARP_LEN]    = {0};
    uint                                         para_len    = 0;
    ATC_CUR_INFO_LIST_T *atc_cur_info_ptr    = NULL;
    ATC_CUR_INFO_LIST_T *atc_last_info_ptr   = NULL;

    SCI_ASSERT( NULL != atc_global_info_ptr ); /*assert verified*/

    if( SIO_RECV_NEW_AT != sig_ptr->SignalCode )
    {
        return S_ATC_FAIL;
    }

    /* Get new AT command line length */
    atc_line_length =(uint)(((SIO_RECV_NEW_AT_T *)sig_ptr)->len);
        
    if (0 == atc_line_length)    
    {
        return S_ATC_FAIL;
    }

#ifdef PPP_USB
    if(ATC_SIO_PPP_PORT == (uint8)(((SIO_RECV_NEW_AT_T *)sig_ptr)->sio_port))
	ATC_SetSioPort(ATC_SIO_PPP_PORT);
    else
	ATC_SetSioPort(ATC_SIO_DEFAULT_PORT);
	
#endif
   
    /* Too long atc command,just clear the sio buffer and 
     * note the sio buffer size is different from atc buffer size
     */
    if(atc_line_length > ATC_MAX_CMD_LINE_LENGTH - 2)
    {
#ifdef PPP_USB
	   SIO_ATC_ClrCmdLine(ATC_GetSioPort());
#else
        SIO_ATC_ClrCmdLine();
#endif
        status = ERR_TEXT_STRING_TOO_LONG;

        //restore AT command parase mode
  #ifdef PPP_USB
      SIO_ATC_SetCmdLineTerminateChar(atc_global_info_ptr->atc_config_ptr->s3_reg, '\0',ATC_GetSioPort());
   #else
      SIO_ATC_SetCmdLineTerminateChar(atc_global_info_ptr->atc_config_ptr->s3_reg, '\0');
   #endif
        ATC_BuildResultCodeRsp(atc_global_info_ptr->atc_config_ptr, 
        S_ATC_RESULT_CODE_ERROR, status);

        return status;
    } 
     /* 
     ** If the new cmd line two short, it could be an "A/" command.
     ** So we can use the short buf to store it.
     */
    else if( atc_line_length <= ATC_MIN_CMD_LINE_LENGTH )
    {
        if ( atc_global_info_ptr->last_cmd_line_buf_ptr == atc_global_info_ptr->cmd_line_buf_short )
        {
            SCI_MEMCPY(atc_global_info_ptr->cmd_line_buf_long,atc_global_info_ptr->cmd_line_buf_short, ATC_MIN_CMD_LINE_LENGTH);

            atc_global_info_ptr->last_cmd_line_buf_ptr =  atc_global_info_ptr->cmd_line_buf_long;
        }

        atc_global_info_ptr->cur_cmd_line_buf_ptr = atc_global_info_ptr->cmd_line_buf_short;
    }
    else 
    {
        atc_global_info_ptr->cur_cmd_line_buf_ptr =  atc_global_info_ptr->cmd_line_buf_long;
    }


    if (g_is_diag_cmd)
    {
        ATC_ReadDiagCmdLine(atc_global_info_ptr->cur_cmd_line_buf_ptr,
                            atc_line_length, &atc_read_length);
    }
    else
    {
        /*
         ** Read AT command line from SIO
         */
#ifdef PPP_USB
        SIO_ATC_ReadCmdLine(atc_global_info_ptr->cur_cmd_line_buf_ptr, atc_line_length,&atc_read_length,ATC_GetSioPort());
#else
        SIO_ATC_ReadCmdLine(atc_global_info_ptr->cur_cmd_line_buf_ptr, atc_line_length,&atc_read_length);
#endif		
    }

    if ( atc_read_length < atc_line_length ) //CID:11698
    {
        atc_line_length = atc_read_length;
    }

#if defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)
    // process +++ATH case
    if((0x2B == *atc_global_info_ptr->cur_cmd_line_buf_ptr)
    &&(0x2B == *(atc_global_info_ptr->cur_cmd_line_buf_ptr+1))
    &&(0x2B == *(atc_global_info_ptr->cur_cmd_line_buf_ptr+2)))
    {
        if (((0x41 == *(atc_global_info_ptr->cur_cmd_line_buf_ptr+3))
          || (0x61 == *(atc_global_info_ptr->cur_cmd_line_buf_ptr+3)))
          &&((0x54 == *(atc_global_info_ptr->cur_cmd_line_buf_ptr+4))
          || (0x74 == *(atc_global_info_ptr->cur_cmd_line_buf_ptr+4)))
          &&((0x48 == *(atc_global_info_ptr->cur_cmd_line_buf_ptr+5))
          || (0x68 == *(atc_global_info_ptr->cur_cmd_line_buf_ptr+5))))
        {
            uint8 ath[5] = {'A','T','H','\n', 0};
            atc_line_length -=3;

            g_is_hang_up_data_call = TRUE;

            SCI_MEMCPY(atc_global_info_ptr->cur_cmd_line_buf_ptr, ath, sizeof(ath));
        }
    }
#endif
  
    /* Add two NULL terminater for atcparse */
    atc_global_info_ptr->cur_cmd_line_buf_ptr[atc_line_length] = '\0';
    atc_global_info_ptr->cur_cmd_line_buf_ptr[atc_line_length + 1] = '\0';
    
    
    /* trace out the at command line */
    if(atc_line_length < 200)//please make sure all the context length is less than 250 in the quotation mark including prefixed characters
    {
        uint8 tchar[42] = {0};
        
        //SCI_TRACE_LOW:"ATC:RECV AT len = %d,Content = %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_3530_112_2_17_22_16_25_79,(uint8*)"ds",atc_line_length,atc_global_info_ptr->cur_cmd_line_buf_ptr);
        ConvertBinToHex(atc_global_info_ptr->cur_cmd_line_buf_ptr, (atc_line_length>20)? 20:atc_line_length, tchar);
    }
    else
    {
        //SCI_TRACE_LOW:"ATC:Recv AT len = %d,too long!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_3535_112_2_17_22_16_25_80,(uint8*)"d",atc_line_length);
    }

    if((atc_global_info_ptr->atc_config_ptr->s3_reg  == *atc_global_info_ptr->cur_cmd_line_buf_ptr
        || atc_global_info_ptr->atc_config_ptr->s4_reg == *atc_global_info_ptr->cur_cmd_line_buf_ptr)
        &&(atc_line_length > 0))
    {
        atc_global_info_ptr->cur_cmd_line_buf_ptr ++;
        atc_line_length --;
    }

    // remove ; from atc line for atcparase
    if((atc_line_length > 2) 
        && (atc_global_info_ptr->cur_cmd_line_buf_ptr[atc_line_length - 1] == atc_global_info_ptr->atc_config_ptr->s3_reg)
        && (atc_global_info_ptr->cur_cmd_line_buf_ptr[atc_line_length - 2] == ';'))
    {
        atc_global_info_ptr->cur_cmd_line_buf_ptr[atc_line_length - 2] = atc_global_info_ptr->atc_config_ptr->s3_reg;
        atc_global_info_ptr->cur_cmd_line_buf_ptr[atc_line_length - 1] = '\0'; //add null terminater for atcparse
        atc_line_length --;
    }

    if (atc_global_info_ptr->atc_config_ptr->is_ctrlz_end)
    {
        atc_global_info_ptr->atc_config_ptr->is_ctrlz_end = FALSE;
        
  #ifdef PPP_USB
         SIO_ATC_SetCmdLineTerminateChar(atc_global_info_ptr->atc_config_ptr->s3_reg, '\0',ATC_GetSioPort());
  #else
        SIO_ATC_SetCmdLineTerminateChar(atc_global_info_ptr->atc_config_ptr->s3_reg, '\0');
   #endif
        switch(atc_global_info_ptr->atc_config_ptr->ctrlz_end_status)
        {
            case ATC_CTRLZ_SMS:
            {
            #if !defined( _ULTRA_LOW_CODE_) || defined(_ATC_SYNC_TOOL_)
                status = ATC_ProcessSmsPdu(atc_global_info_ptr->atc_config_ptr, 
                              atc_global_info_ptr->cur_cmd_line_buf_ptr);
            #endif
                break;
            }

#if 0
            case ATC_CTRLZ_STK:
            {
            #ifndef _ULTRA_LOW_CODE_
                status = ATC_ProcessSioStkData(atc_global_info_ptr->atc_config_ptr, 
                atc_global_info_ptr->cur_cmd_line_buf_ptr);
            #endif
                break;
            }
#endif
            
            case ATC_CTRLZ_SRWD:
            {
            #ifndef _ULTRA_LOW_CODE_
                status = ATC_ProcessSioSRWDData(atc_global_info_ptr->atc_config_ptr, 
                atc_global_info_ptr->cur_cmd_line_buf_ptr);
            #endif
                break;
            }

            case ATC_CTRLZ_SSEA:
            {
                status = ATC_ProcessSioSSEAData(atc_global_info_ptr->atc_config_ptr, 
                atc_global_info_ptr->cur_cmd_line_buf_ptr,atc_line_length); 
                break;
            }

            case ATC_CTRLZ_SADM:
            {
                status = ATC_ProcessSioSADMData(atc_global_info_ptr->atc_config_ptr, 
                atc_global_info_ptr->cur_cmd_line_buf_ptr,atc_line_length);
                break;
            }

            case ATC_CTRLZ_SADMNV:  
            {
                status = ATC_ProcessSioSADMNVData(atc_global_info_ptr->atc_config_ptr, 
                atc_global_info_ptr->cur_cmd_line_buf_ptr,atc_line_length);
                break;
            }

#ifdef ATC_SAT_ENABLE
            case ATC_CTRLZ_GPRS:
            {
            #ifndef _GSM_ONLY_
                status = ATC_ProcessSioData(atc_global_info_ptr->atc_config_ptr, 
                atc_global_info_ptr->cur_cmd_line_buf_ptr, atc_line_length);
                //SCI_TRACE_LOW:"atc: ATC_CTRLZ_GPRS, status=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_3616_112_2_17_22_16_25_81,(uint8*)"d",status);
            #endif
                break;
            }
#endif /*ATC_SAT_ENABLE*/

            case ATC_CTRLZ_SNVM:
            {
            #ifdef _ATC_ONLY
                status = ATC_ProcessSioSNVMData(atc_global_info_ptr->atc_config_ptr, 
                atc_global_info_ptr->cur_cmd_line_buf_ptr);
            #endif
                break;
            }

            case ATC_CTRLZ_SADM_ARM:
            {
            #ifdef AUDIO_PARAM_ARM
                status = ATC_ProcessSioARMSADMData(atc_global_info_ptr->atc_config_ptr,
                atc_global_info_ptr->cur_cmd_line_buf_ptr, atc_line_length);
            #endif
                break;
            }

            case ATC_CTRLZ_SADMNV_ARM:
            {
            #ifdef AUDIO_PARAM_ARM
                status = ATC_ProcessSioARMSADMNVData(atc_global_info_ptr->atc_config_ptr,
                atc_global_info_ptr->cur_cmd_line_buf_ptr, atc_line_length);
            #endif
                break;
            }

            case ATC_CTRLZ_CVS:
            {
                status = ATC_ProcessSioSADCVSData(atc_global_info_ptr->atc_config_ptr,
                    atc_global_info_ptr->cur_cmd_line_buf_ptr,atc_line_length);
                break;
            }
            case ATC_CTRLZ_CVSNV:
            {
                status = ATC_ProcessSioSADCVSNVData(atc_global_info_ptr->atc_config_ptr,
                    atc_global_info_ptr->cur_cmd_line_buf_ptr,atc_line_length);
                break;
            }
            case ATC_CTRLZ_ENHA:
            {
                status = ATC_ProcessSioENHA_Data(atc_global_info_ptr->atc_config_ptr,
                                                atc_global_info_ptr->cur_cmd_line_buf_ptr,atc_line_length);
                break;
            }
        
            case ATC_CTRLZ_ENHA_TUN:
            {
                status = ATC_ProcessSioENHA_Tun_Data(atc_global_info_ptr->atc_config_ptr,
                                                atc_global_info_ptr->cur_cmd_line_buf_ptr,atc_line_length);
                break;
            }
        
            case ATC_CTRLZ_ENHANV:
            {
                status = ATC_ProcessSioENHA_NVData(atc_global_info_ptr->atc_config_ptr,
                                                atc_global_info_ptr->cur_cmd_line_buf_ptr,atc_line_length);
                break;
            }

            case ATC_CTRLZ_ENHANV_TUN:
            {
                status = ATC_ProcessSioENHA_Tun_NVData(atc_global_info_ptr->atc_config_ptr,
                                                atc_global_info_ptr->cur_cmd_line_buf_ptr,atc_line_length);
                break;
            }

           case ATC_CTRLZ_SIMLOCKDATA:
           {
                SCI_TRACE_LOW("ATC_CTRLZ_SIMLOCKDATA ATC_ProcessSimlockData");
                status = ATC_ProcessSimlockData(atc_global_info_ptr->atc_config_ptr, atc_global_info_ptr->cur_cmd_line_buf_ptr);
	        break;	
            }

            default:
            {
                //SCI_TRACE_LOW:"ATC:ASSERT invlaid ctrlz=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_3723_112_2_17_22_16_26_82,(uint8*)"d",atc_global_info_ptr->atc_config_ptr->ctrlz_end_status);
                break;
            }
        }
    }
    else
    {
        /* Parse the AT command line */
        zerolex(); //add for reset lex state

        if(atc_global_info_ptr->cur_cmd_line_buf_ptr)
        {
            const uint8 *buf_ptr  = NULL;
            uint            buf_len   = 0;

            buf_ptr = atcparse_at(atc_global_info_ptr->cur_cmd_line_buf_ptr, atc_line_length);
            buf_len = atc_line_length - ((uint32)buf_ptr - (uint32)atc_global_info_ptr->cur_cmd_line_buf_ptr);

            //SCI_TRACE_LOW:"ATC: ATC_RecNewLineSig, ignore ip data, len:%d->%d, new atcmd:%s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_3744_112_2_17_22_16_26_83,(uint8*)"dds", atc_line_length, buf_len, (const char *)buf_ptr);
            atc_global_info_ptr->cur_cmd_line_buf_ptr = (uint8 *)buf_ptr;
            atc_line_length = buf_len;
        }

        atc_global_info_ptr->cur_cmd_line_buf_ptr[atc_line_length-1] = 0xD;
        status = atcparse(atc_global_info_ptr->cur_cmd_line_buf_ptr,&atc_cur_info_ptr);

        /* Added by king.li and merged by Eddie.Li for supporting DUN. */
        if(0x41 == atc_global_info_ptr->cur_cmd_line_buf_ptr[0]
           && 0x54 == atc_global_info_ptr->cur_cmd_line_buf_ptr[1]
           && 0x53 == atc_global_info_ptr->cur_cmd_line_buf_ptr[2]
           && 0x37 == atc_global_info_ptr->cur_cmd_line_buf_ptr[3])
        {
            ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR,S_ATC_RESULT_CODE_OK,0);
            return S_ATC_DEFAULT_HANDLE;
        }

        if(((0x41 == *atc_global_info_ptr->cur_cmd_line_buf_ptr)
           || (0x61 == *atc_global_info_ptr->cur_cmd_line_buf_ptr))
           &&((0x54 == *(atc_global_info_ptr->cur_cmd_line_buf_ptr+1))
           || (0x74 == *(atc_global_info_ptr->cur_cmd_line_buf_ptr+1)))
           &&(0x23 == *(atc_global_info_ptr->cur_cmd_line_buf_ptr+2))
           &&(0x23 == *(atc_global_info_ptr->cur_cmd_line_buf_ptr+3)))
        {
            SCI_MEMSET(para_str,0,sizeof(para_str));
            para_len = atc_line_length-4;

            if(para_len > ATC_SHARP_LEN)
            {
                ATCM_FreeAll();
                //SCI_TRACE_LOW:"ATC:Assert Failure sharp too long."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_3775_112_2_17_22_16_26_84,(uint8*)"");
                return ERR_OPERATION_NOT_SUPPORTED;
            }

            SCI_MEMCPY(para_str,(atc_global_info_ptr->cur_cmd_line_buf_ptr+4),para_len);
            para_len = para_len - 1;
            //call the extern function
            //SCI_TRACE_LOW("ATC: Call the external function");
#ifndef _ATC_ONLY
#ifdef SYNCTOOL_SUPPORT
            ATC_ExtCmd(para_str, para_len);
#endif
#endif

            ATCM_FreeAll();

            //need do nothing while received AT## command
            return S_ATC_DEFAULT_HANDLE;
        }
    }
    switch ( status ) 
    {
        case S_ATC_SUCCESS:
        {
            SCI_ASSERT(NULL != atc_cur_info_ptr); /*assert verified*/
            //ATC_TRACE:"ATC:Parse AT cmd OK"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_3800_112_2_17_22_16_26_85,(uint8*)"" );

            /*
            ** we link the infos of AT cmds to the end of the list which 
            ** store in atc_config 
            */
            if ( NULL == atc_global_info_ptr->atc_info_ptr )
            {
                atc_global_info_ptr->atc_info_ptr = atc_cur_info_ptr;
            }
            else
            {
                atc_last_info_ptr = atc_global_info_ptr->atc_info_ptr;

                while ( NULL != atc_last_info_ptr->next )
                {
                    atc_last_info_ptr = atc_last_info_ptr->next;
                }

                atc_last_info_ptr->next = atc_cur_info_ptr;
            }
            break;
        }
            
        case S_ATC_DEFAULT_HANDLE:
        {
            //ATC_TRACE:"ATC:Parse SMS PDU successful"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_3826_112_2_17_22_16_26_86,(uint8*)"");
            break;
        }

        default:
        {
            //ATC_TRACE:"ATC:Parse AT command line failed.\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_3832_112_2_17_22_16_26_87,(uint8*)"" );
            ATCS_TRACE_STATUS( status );
            //response the result code
            ATC_BuildResultCodeRsp(atc_global_info_ptr->atc_config_ptr, S_ATC_RESULT_CODE_ERROR, status);
            break;
        }
    }
    
     return status;
}


/******************************************************************************/
// Description : This function processes the AT commands stored in the AT 
//               config struct one by one.
// Global resource dependence : None
// Author : Steven
// Note : None
/******************************************************************************/
LOCAL ATC_STATUS ATC_ProcessCmds( // Return S_ATC_SUCCESS if success,
                                                                  // otherwise return error code.
        ATC_GLOBAL_INFO_T     * atc_global_info_ptr // point to the ATC_GLOBAL_INFO_T struct.
        )
{
    ATC_STATUS      status = S_ATC_SUCCESS;

    SCI_ASSERT( NULL != atc_global_info_ptr ); /*assert verified*/

    while ( NULL != atc_global_info_ptr->atc_info_ptr )
    {
        status = ATC_DespatchCmd( atc_global_info_ptr, atc_global_info_ptr->atc_info_ptr );
        if ( S_ATC_SUCCESS != status )
        {
            atc_global_info_ptr->atc_info_ptr = PNULL;
            break;
        }
        atc_global_info_ptr->atc_info_ptr = atc_global_info_ptr->atc_info_ptr->next;     
    }
    
    switch ( status )
    {
        case S_ATC_SUCCESS:
        {
            if(NULL == atc_global_info_ptr->atc_info_ptr)
            {
                ATC_BuildResultCodeRsp(atc_global_info_ptr->atc_config_ptr,	S_ATC_RESULT_CODE_OK, 0);
            }
            else
            {
                //SCI_TRACE_LOW:"ATC:Assert issue for processCMDs!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_3879_112_2_17_22_16_26_88,(uint8*)"");
            }
            break;
        }

        case S_ATC_LAST_ERROR:
        {
            ATC_BuildResultCodeRsp(atc_global_info_ptr->atc_config_ptr,
            S_ATC_RESULT_CODE_OK ,0);
            ATC_BuildResultCodeRsp(atc_global_info_ptr->atc_config_ptr,
            S_ATC_RESULT_CODE_ERROR ,status);
            status = S_ATC_SUCCESS;
            break;
        }

        case S_ATC_DEFAULT_HANDLE:
        {
            break;
        }

        default:
        {
            ATC_BuildResultCodeRsp(atc_global_info_ptr->atc_config_ptr,
            S_ATC_RESULT_CODE_ERROR ,status);
            break;
        }
    }

    // If the AT commands are all handled, we free our private heap mem 
    ATCM_FreeAll();
    return status;
}

#endif

/******************************************************************************/
// Description : This function despatches AT command to its handle.
// Global resource dependence : 
// Author : Steven
// Note : None
/******************************************************************************/
LOCAL ATC_STATUS ATC_DespatchCmd(  // Return S_ATC_STATUS if success
                                                                   // otherwise return error code.
        ATC_GLOBAL_INFO_T  * atc_global_info_ptr, // point to the ATC_GLOBAL_INFO_T struct.
        ATC_CUR_INFO_LIST_T * atc_cur_info_ptr // Point to the info struct of AT command
        )
{
    ATC_STATUS  status         = S_ATC_SUCCESS;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_ASSERT( NULL != atc_global_info_ptr ); /*assert verified*/
    SCI_ASSERT(NULL != atc_cur_info_ptr); /*assert verified*/

    status = ATC_CheckCmd( atc_global_info_ptr, atc_cur_info_ptr );

    if ( S_ATC_SUCCESS == status )
    { 
    
        //check the SIM card
        if(((S_CONFIG_INFO_PTR)->sim_flag[dual_sys] != TRUE) ||
                ((S_CONFIG_INFO_PTR)->cpin[dual_sys] != ATC_CPIN_CODE_READY))
        {
            SCI_TRACE_LOW("ATC: ATC_DespatchCmd, sim_flag:%d, cpin:%d",
                          (S_CONFIG_INFO_PTR)->sim_flag[dual_sys], (S_CONFIG_INFO_PTR)->cpin[dual_sys]);

         #ifdef ATC_ORANGE_ENABLE
         if((S_CONFIG_INFO_PTR)->cpin[dual_sys] != ATC_CPIN_CODE_READY)
         {
            switch (atc_cur_info_ptr->cmd_index)
            {
                case AT_CMD_CGMI:
                case AT_CMD_GMI:
                case AT_CMD_CGMM:
                case AT_CMD_GMM:
                case AT_CMD_CGMR:
                case AT_CMD_GMR:
                case AT_CMD_CGSN:
                case AT_CMD_GSN:
                case AT_CMD_CPAS:
                case AT_CMD_CFUN:
                case AT_CMD_CPIN:
                //case AT_CMD_CDIS:
                case AT_CMD_CIND:
                case AT_CMD_D:
                    
                    break;
                default:
                    return ERR_SIM_PIN_REQUIRED;
            }
        }
        #endif
        }
    
        if ( ATC_CMD_TYPE_TEST == (atc_cur_info_ptr->cmd_type & 0x000F) )
        {
            // add for handle the specific test command
            switch (atc_cur_info_ptr->cmd_index)
            {
 
#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)
                case AT_CMD_CPBR:
                case AT_CMD_CPBS:
                case AT_CMD_CPBF:
                case AT_CMD_CPBW:
                case AT_CMD_CPBP:
                case AT_CMD_CPBN:
                case AT_CMD_CCWA:
                case AT_CMD_COPS:
                #if 0
                case AT_CMD_CNUM:
                case AT_CMD_CSVM:
                case AT_CMD_STSF:
                case AT_CMD_SAC:
                #endif
                case AT_CMD_CSCS:
                case AT_CMD_FCLASS:
                case AT_CMD_GCI:
                case AT_CMD_SHARP_CLS:
#elif defined(_ATC_UIX8910_ENABLE_)
                case AT_CMD_CPBR:
                case AT_CMD_CPBS:
                case AT_CMD_CPBF:
                case AT_CMD_CPBW:
                case AT_CMD_CPBP:
                case AT_CMD_CPBN:
                case AT_CMD_CNUM:
                case AT_CMD_COPS:
                case AT_CMD_CEMODE:
                case AT_CMD_CGDSCONT:
                case AT_CMD_xor_SYSCONFIG:
#endif
                case AT_CMD_CPMS:
                #ifdef ATC_ORANGE_ENABLE
                case AT_CMD_CMGL:
                #endif
                {
                    status = atc_global_info_ptr->atc_handle_table[atc_cur_info_ptr->cmd_index]
                    (atc_global_info_ptr->atc_config_ptr,atc_cur_info_ptr );
                    break;
                }
                default:
                {
                    status = ATC_BuildTestCmdResponse(atc_global_info_ptr, atc_cur_info_ptr);
                    break;
                }        
            }
        }
        else
        {
            status = atc_global_info_ptr->atc_handle_table[atc_cur_info_ptr->cmd_index](atc_global_info_ptr->atc_config_ptr,atc_cur_info_ptr );
        }
    }
    else if ( S_ATC_LAST_ERROR == status )
    {
        status = S_ATC_LAST_ERROR;
    }
    else
    {
        status = S_ATC_RESULT_CODE_ERROR;
    }

    return status;
}

/******************************************************************************/
// Description : This function checks the current received AT command.
// Global resource dependence : None
// Author : Steven
// Note : None
/******************************************************************************/
LOCAL ATC_STATUS ATC_CheckCmd(  // Return S_ATC_SUCCESS if command is valid,
                                                              // otherwise return error code.
        ATC_GLOBAL_INFO_T   *atc_global_info_ptr, // point to the ATC_GLOBAL_INFO_T struct.
        ATC_CUR_INFO_LIST_T *atc_cur_info_ptr  // Point to the info struct of  AT command.
        )
{
    ATC_STATUS                                                   status  = S_ATC_SUCCESS;
    const ATC_INFO_T                                 *atc_info_ptr  = NULL;
    ATC_CUR_PARAM_INFO_LIST_T    *cur_param_info_ptr  = NULL;
    const ATC_PARAM_INFO_T                 *param_info_ptr  = NULL;
    const ATC_PARAM_NUMERIC_T           *num_param_ptr  = NULL;
    const ATC_PARAM_STRING_T                *str_param_ptr  = NULL;
    uint16                                                                     i   = 0;

    SCI_ASSERT( NULL != atc_global_info_ptr ); /*assert verified*/
    SCI_ASSERT(NULL != atc_cur_info_ptr);       /*assert verified*/

    atc_info_ptr = &atc_global_info_ptr->atc_info_table[atc_cur_info_ptr->cmd_index];
    SCI_ASSERT( atc_info_ptr->cmd_index == atc_cur_info_ptr->cmd_index );/*assert verified*/

    if ( atc_cur_info_ptr->cmd_type & ATC_CMD_TYPE_ERROR )
    {
        status = S_ATC_LAST_ERROR;
        goto CheckCmdDone;
    }

    /* Check AT Command Type */
    if ( !(atc_cur_info_ptr->cmd_type & atc_info_ptr->cmd_type & 0x000F) )
    {
        status = ERR_OPERATION_NOT_SUPPORTED;
        goto CheckCmdDone;
    }

    /* If Command action type is not SET, then we need not to check param */
    if ( ATC_CMD_TYPE_SET != (atc_cur_info_ptr->cmd_type & 0x000F) )
    {
        status = S_ATC_SUCCESS;
        goto CheckCmdDone;
    }
    
    /* Check AT Command Parameter Number */
    //the num of params should equal or less than the max param numbers
    if ( atc_cur_info_ptr->cur_param_num > atc_info_ptr->param_num )
    {
        status = ERR_INVALID_INDEX;
        goto CheckCmdDone;
    }

    if ( 0 == atc_info_ptr->param_num )
    {
        status = S_ATC_SUCCESS;
        goto CheckCmdDone;
    }
    
    param_info_ptr = atc_info_ptr->param_ptr;
    cur_param_info_ptr = atc_cur_info_ptr->cur_param_list_ptr;

    while ( NULL != cur_param_info_ptr )
    {
        if ( (ATC_CMD_PARAM_TYPE_DEFAULT != param_info_ptr->param_type) 
            && (ATC_CMD_PARAM_TYPE_DEFAULT != cur_param_info_ptr->param_type) )
        {
            if ( param_info_ptr->param_type != cur_param_info_ptr->param_type )
            {
                status = ERR_OPERATION_NOT_ALLOWED;
                goto CheckCmdDone;
            }
        }
        
        switch ( param_info_ptr->param_type )
        {
            case ATC_CMD_PARAM_TYPE_NUMERIC:
            {
                num_param_ptr = param_info_ptr->param_info.num_array_ptr;
                for ( i = 0; i < param_info_ptr->param_range_num; i++ )
                {
                    if ( cur_param_info_ptr->param_info.num < num_param_ptr->val_min )
                    {
                        status = ERR_OPERATION_NOT_ALLOWED;
                        goto CheckCmdDone;
                    }
                    
                    if ( (cur_param_info_ptr->param_info.num > num_param_ptr->val_max)
                         && (i == (param_info_ptr->param_range_num - 1)) )
                    {
                        status = ERR_OPERATION_NOT_ALLOWED;
                        goto CheckCmdDone;
                    }

                    if ( (cur_param_info_ptr->param_info.num >= num_param_ptr->val_min) &&
                         (cur_param_info_ptr->param_info.num <= num_param_ptr->val_max) )
                    {
                        break;
                    }
                    
                    num_param_ptr++;
                } /* End of loop for(i) */
                break;
            }

            case ATC_CMD_PARAM_TYPE_STRING:
            {
                str_param_ptr = param_info_ptr->param_info.str_array_ptr;
                for ( i = 0; i < param_info_ptr->param_range_num; i++ )
                {
                    //add check valid the pointer of string type param
                    if ( cur_param_info_ptr->param_info.str_ptr != NULL &&
                    	0 == strcmp((char *)cur_param_info_ptr->param_info.str_ptr->str_ptr,(char *)str_param_ptr->str_ptr) )
                    {
                        break;
                    }
                    else
                    {
                        if ( (param_info_ptr->param_range_num - 1) == i )
                        {
                            status = ERR_OPERATION_NOT_ALLOWED;
                            goto CheckCmdDone;
                        }
                    }

                    str_param_ptr++;
                } /* End of loop for(i) */
                break;
            }

            default:
                break;
        } /* End of switch ( cur_param_info_ptr->param_type ) */

        param_info_ptr++;
        cur_param_info_ptr = cur_param_info_ptr->next;
    } /* End of loop while ( NULL != cur_param_info_ptr ) */

CheckCmdDone:

    if ( S_ATC_SUCCESS == status )
    {
        //ATC_TRACE:"ATC:Check AT Command valid.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_4126_112_2_17_22_16_26_89,(uint8*)"" );
    }
    else
    {
        //ATC_TRACE:"ATC:Check AT Command invalid.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_4130_112_2_17_22_16_26_90,(uint8*)"" );
        ATCS_TRACE_STATUS( status );
    }
    return status;
}

/******************************************************************************/
// Description : This function contructs the result code response 
// Global resource dependence : 
// Author : Ivan
// Note : 
// History:
/******************************************************************************/
void ATC_BuildResultCodeRsp( 
        ATC_CONFIG_T       *atc_config_ptr,  // point to the ATC_CONFIG_T struct.
        int32              result_code_idx,        // result code index
        int32              err_code_idx             // error code index    
        )
{
    uint8       *rsp_buf_ptr  = g_rsp_str;
    uint8       cr_val            = 0;
    uint8       lf_val             = 0;
#ifdef _MUX_ENABLE_
    uint8       atc_link_id     = 0;
#endif

    SCI_ASSERT( NULL != atc_config_ptr ); /*assert verified check NULL pointer*/
    
#ifdef _MUX_ENABLE_
    atc_link_id = atc_config_ptr->current_link_id;
    //SCI_TRACE_LOW:"atc: mux enable link id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_4158_112_2_17_22_16_27_91,(uint8*)"d", atc_link_id);
#endif

    cr_val = atc_config_ptr->s3_reg;
    lf_val = atc_config_ptr->s4_reg;

    if(err_code_idx == ERR_TEXT_STRING_TOO_LONG)
    {
        g_rsp_str[0] = cr_val;
        g_rsp_str[1] = lf_val;
        rsp_buf_ptr = &g_rsp_str[2];

        sprintf((char*)rsp_buf_ptr, "%s%ld%c%c", "+CME ERROR: ", err_code_idx, cr_val, lf_val);

        SCI_TRACE_LOW("ATC_BuildResultCodeRsp, at_Rsp:%s",g_rsp_str);
#ifdef _MUX_ENABLE_
        if(ATC_DEBUG_LINK == atc_link_id)
        {
            SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
        }
        else
        {
            MUX_Write(g_rsp_str, strlen((char*)g_rsp_str),atc_link_id);
        }
#else

#ifdef PPP_USB
        SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str),ATC_GetSioPort());
#else
        SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
#endif
#endif
        return;
    }

    if (atc_config_ptr->q_value)
    {
        return ;
    }

    if (0 == atc_config_ptr->v_value)
    {
        sprintf((char *)rsp_buf_ptr, "%ld%c", (result_code_idx - S_ATC_RESULT_CODE_BASE), cr_val);
    }
    else 
    {
        g_rsp_str[0] = cr_val;
        g_rsp_str[1] = lf_val;
        rsp_buf_ptr    = &g_rsp_str[2];

        if (S_ATC_RESULT_CODE_ERROR == result_code_idx)
        {
            switch (atc_config_ptr->atc_config_nv.cmee)
            {
                case NO_RESULT_CODE_MODE:
                {
                    sprintf((char*)rsp_buf_ptr, "%s%c%c", (char*)g_atc_result_code_text[result_code_idx - S_ATC_RESULT_CODE_BASE], cr_val, lf_val);
                    break;
                }

                case NUMERIC_ERR_VALUE_MODE:
                {
                    sprintf((char*)rsp_buf_ptr, "%s%ld%c%c", "+CME ERROR: ", err_code_idx, cr_val, lf_val);
                    break;
                }

                case VERBOSE_ERR_VALUE_MODE:
                {
                    sprintf((char*)rsp_buf_ptr, "+CME ERROR: ");//, err_verbose, cr_val, lf_val);
                    rsp_buf_ptr += strlen("+CME ERROR: ");
                    ATC_GetErrVerbose(err_code_idx, (char *)rsp_buf_ptr);
                    rsp_buf_ptr += strlen((char*)rsp_buf_ptr);
                    sprintf((char*)rsp_buf_ptr, "%c%c", cr_val, lf_val);
                    break;
                }

                default:
                {
                    //SCI_TRACE_LOW:"ATC:assert invlaid cmee type!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_4230_112_2_17_22_16_27_92,(uint8*)"");
                    break;
                }
            }
        }
        else
        {
            /* check CR flag in global at config          */
            if( atc_config_ptr->cr && (result_code_idx == S_ATC_RESULT_CODE_CONNECT) )
            {
                sprintf((char*)rsp_buf_ptr,"+CR: GPRS %s%c%c", (char*)g_atc_result_code_text[result_code_idx - S_ATC_RESULT_CODE_BASE], cr_val, lf_val);
            }
            else
            {
                sprintf((char*)rsp_buf_ptr, "%s%c%c", (char*)g_atc_result_code_text[result_code_idx - S_ATC_RESULT_CODE_BASE], cr_val, lf_val);
            }
        }
    }
    
    //SCI_TRACE_LOW:"ATC: BuildResultCodeRsp,The string:%s"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_4249_112_2_17_22_16_27_93,(uint8*)"s", g_rsp_str);
    SCI_TRACE_LOW("ATC_BuildResultCodeRsp,2, at_Rsp:%s",g_rsp_str);

#ifdef _MUX_ENABLE_
    if(ATC_DEBUG_LINK == atc_link_id)
    {
        SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
    }
    else
    {
        MUX_Write(g_rsp_str, strlen((char*)g_rsp_str),atc_link_id);
    }
#else
#ifdef PPP_USB
    SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str),ATC_GetSioPort());
#else
    SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
#endif

#endif
}
 
#if !defined( _ULTRA_LOW_CODE_) || defined(_ATC_SYNC_TOOL_)
/******************************************************************************/
// Description : This function contructs the result code response for SMS commands
// Global resource dependence : 
// Author : Ivan
// Note : 
/******************************************************************************/
void ATC_BuildSmsCodeRsp( 
        ATC_CONFIG_T       *atc_config_ptr,  // point to the ATC_CONFIG_T struct.
        int32              err_code_idx             // error code index    
        )
{
    uint8       *rsp_buf_ptr  = g_rsp_str;
    uint8       cr_val            = 0;
    uint8       lf_val             = 0;

    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(ATC_SMS_ERR_300 <= err_code_idx && err_code_idx <= ATC_SMS_ERR_500); /*assert verified*/

    cr_val = atc_config_ptr->s3_reg;
    lf_val = atc_config_ptr->s4_reg;

    g_rsp_str[0] = cr_val;
    g_rsp_str[1] = lf_val;
    rsp_buf_ptr = &g_rsp_str[2];

    switch (atc_config_ptr->atc_config_nv.cmee)
    {
        case NO_RESULT_CODE_MODE:
        {
            sprintf((char*)rsp_buf_ptr, "%s%c%c", (char*)g_atc_result_code_text[S_ATC_RESULT_CODE_ERROR - S_ATC_RESULT_CODE_BASE], cr_val, lf_val);
            break;
        }

        case NUMERIC_ERR_VALUE_MODE:
        {
            sprintf((char*)rsp_buf_ptr, "%s%d%c%c", "+CMS ERROR: ", s_sms_verbos_info[err_code_idx].error_code, cr_val, lf_val);
            break;
        }

        case VERBOSE_ERR_VALUE_MODE:
        {
            sprintf((char*)rsp_buf_ptr, "%s%s%c%c", "+CMS ERROR: ", s_sms_verbos_info[err_code_idx].error_str, cr_val, lf_val);
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC:assert invalid cmee type!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_4311_112_2_17_22_16_27_94,(uint8*)"");
            break;
        }
    }

    //SCI_TRACE_LOW:"ATC:BuildSmsCodeRsp,The string=%s"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_4316_112_2_17_22_16_27_95,(uint8*)"s", g_rsp_str);
    SCI_TRACE_LOW("ATC_BuildSmsCodeRsp,at_Rsp:%s",g_rsp_str);
#ifdef _MUX_ENABLE_
    if(ATC_DEBUG_LINK == atc_config_ptr->current_link_id)
    {
        SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
    }
    else
    {
        MUX_Write(g_rsp_str, strlen((char*)g_rsp_str), atc_config_ptr->current_link_id);
    }
#else

#ifdef PPP_USB
    SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str),ATC_GetSioPort());
#else
    SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
#endif

#endif
}
#endif

/******************************************************************************/
// Description : This function constructs the AT command response.
// Global resource dependence : None
// Author : Ivan
// Note : 
/******************************************************************************/
void ATC_BuildInfoRsp(
        ATC_CONFIG_T   *atc_config_ptr, // point to the ATC_CONFIG_T struct.
        uint8          *rsp_str_ptr     // Point the response string
        )
{
    uint8      cr_val    = 0;                 // holds the atc_config_ptr->S3_reg value
    uint8      lf_val     = 0;                 // holds the atc_config_ptr->S4_reg value
    uint8      v_val     = 0;                  // holds the atc_config_ptr->V_option value

    uint8      rsp_buf[10];
#ifdef _MUX_ENABLE_
    uint8      atc_link_id;
#endif

    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT( NULL != rsp_str_ptr ); /*assert verified*/
    
#ifdef _MUX_ENABLE_
    if(strlen((char *)rsp_str_ptr)<=20)
    {
        //SCI_TRACE_LOW:"ATC: ATC_BuildInfoRsp \"%s\""
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_4356_112_2_17_22_16_27_96,(uint8*)"s", rsp_str_ptr);
    }
    else
    {
        //SCI_TRACE_LOW:"ATC: ATC_BuildInfoRsp \"%c%c%c%c%c%c%c%c%c%c%c%c\""
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_4360_112_2_17_22_16_27_97,(uint8*)"cccccccccccc", rsp_str_ptr[0], rsp_str_ptr[1], rsp_str_ptr[2], rsp_str_ptr[3], rsp_str_ptr[4],rsp_str_ptr[5], rsp_str_ptr[6], rsp_str_ptr[7], rsp_str_ptr[8], rsp_str_ptr[9], rsp_str_ptr[10], rsp_str_ptr[11]);
    }
    
    atc_link_id = atc_config_ptr->current_link_id;
    if(ATC_DEBUG_LINK == atc_link_id)
    {
        //SCI_TRACE_LOW:"ATC: ATC_BuildInfoRsp debug link %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_4366_112_2_17_22_16_27_98,(uint8*)"d", ATC_DEBUG_LINK);
    }
    else
    {
        //SCI_TRACE_LOW:"ATC: ATC_BuildInfoRsp link_id %d, work mode %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_4370_112_2_17_22_16_27_99,(uint8*)"dd", atc_config_ptr->current_link_id, g_mux_dlc_info[atc_link_id].work_mode);
        if(MUX_DLC_DATA_MODE == g_mux_dlc_info[atc_link_id].work_mode)
        {
            return;
        }
    }
#endif

    // Set the config values
    cr_val = atc_config_ptr->s3_reg;
    lf_val = atc_config_ptr->s4_reg;
    v_val  = atc_config_ptr->v_value;

    if (atc_config_ptr->q_value)
    {
        return;
    }
    
    if (atc_config_ptr->v_value)
    {
        sprintf((char*)rsp_buf, "%c%c",cr_val,lf_val);
        SCI_TRACE_LOW("ATC_BuildInfoRsp,at_Rsp:%s",rsp_buf);
#ifdef _MUX_ENABLE_
        if(ATC_DEBUG_LINK == atc_link_id )
        {
            SIO_ATC_WriteCmdRes(rsp_buf, strlen((char*)rsp_buf));
        }
        else
        {
            MUX_Write(rsp_buf, strlen((char*)rsp_buf),atc_link_id);
        }
#else
#ifdef PPP_USB
        SIO_ATC_WriteCmdRes(rsp_buf, strlen((char*)rsp_buf),ATC_GetSioPort());
#else
        SIO_ATC_WriteCmdRes(rsp_buf, strlen((char*)rsp_buf));
#endif
#endif
    }
    SCI_TRACE_LOW("ATC_BuildInfoRsp,2,at_Rsp:%s",rsp_str_ptr);
#ifdef _MUX_ENABLE_
    if(ATC_DEBUG_LINK == atc_link_id)
    {
        SIO_ATC_WriteCmdRes(rsp_str_ptr, strlen((char*)rsp_str_ptr));
    }
    else
    {
        MUX_Write(rsp_str_ptr, strlen((char*)rsp_str_ptr),atc_link_id);
    }
#else

#ifdef PPP_USB
    SIO_ATC_WriteCmdRes(rsp_str_ptr, strlen((char*)rsp_str_ptr),ATC_GetSioPort());
#else
    SIO_ATC_WriteCmdRes(rsp_str_ptr, strlen((char*)rsp_str_ptr));
#endif
#endif
    sprintf((char*)rsp_buf, "%c%c",cr_val,lf_val);
    SCI_TRACE_LOW("ATC_BuildInfoRsp,3,at_Rsp:%s",rsp_buf);
#ifdef _MUX_ENABLE_
    if(ATC_DEBUG_LINK ==atc_link_id)
    {
        SIO_ATC_WriteCmdRes(rsp_buf, strlen((char*)rsp_buf));
    }
    else
    {
        MUX_Write(rsp_buf, strlen((char*)rsp_buf),atc_link_id);
    }
#else
#ifdef PPP_USB
    SIO_ATC_WriteCmdRes(rsp_buf, strlen((char*)rsp_buf),ATC_GetSioPort());
#else
    SIO_ATC_WriteCmdRes(rsp_buf, strlen((char*)rsp_buf));
#endif

#endif

}

/******************************************************************************/
// Description : This function builds test command response.
// Global resource dependence : None
// Author : Steven
// Note : None
/******************************************************************************/
LOCAL ATC_STATUS ATC_BuildTestCmdResponse(  // Return S_ATC_SUCCESS if success,
                                                                                       // otherwise return error code.
        ATC_GLOBAL_INFO_T   *atc_global_info_ptr, // point to the ATC_GLOBAL_INFO_T struct.
        ATC_CUR_INFO_LIST_T *atc_cur_info_ptr // Point the info struct of AT cmd
        )
{
    ATC_STATUS                                         status    = S_ATC_SUCCESS;
    const ATC_INFO_T                       *atc_info_ptr   = NULL;
    const ATC_PARAM_INFO_T       *param_info_ptr   = NULL;
    const ATC_PARAM_STRING_T     *str_param_ptr   = NULL;
    const ATC_PARAM_NUMERIC_T *num_param_ptr  = NULL;

    uint8                                          *res_buf_ptr   = NULL;
    uint16                                                          i    = 0;
    uint16                                                          j    = 0;
    
#ifdef _MUX_ENABLE_	
    uint8                         atc_link_id = 0;
#endif

    SCI_ASSERT( NULL != atc_global_info_ptr ); /*assert verified*/
    SCI_ASSERT( NULL != atc_cur_info_ptr ); /*assert verified*/
    SCI_ASSERT( ATC_CMD_TYPE_TEST == (atc_cur_info_ptr->cmd_type & 0x0F) );/*assert verified*/
#ifdef _MUX_ENABLE_	
    atc_link_id = atc_global_info_ptr->atc_config_ptr->current_link_id;
#endif

    atc_info_ptr = &atc_global_info_ptr->atc_info_table[atc_cur_info_ptr->cmd_index];
    SCI_ASSERT( atc_info_ptr->cmd_index == atc_cur_info_ptr->cmd_index );/*assert verified*/

    res_buf_ptr = g_rsp_str;

    if (atc_global_info_ptr->atc_config_ptr->q_value)
    {
        return status;
    }

    if ( atc_global_info_ptr->atc_config_ptr->v_value )
    {
        *res_buf_ptr++ = atc_global_info_ptr->atc_config_ptr->s3_reg;
        *res_buf_ptr++ = atc_global_info_ptr->atc_config_ptr->s4_reg;
    }

    /* Put AT command name in response buf */
    SCI_ASSERT(atc_info_ptr->cmd_name->str_len <= MAX_ATC_RSP_LEN);/*assert verified*/
    SCI_MEMCPY(res_buf_ptr, atc_info_ptr->cmd_name->str_ptr, atc_info_ptr->cmd_name->str_len);

    res_buf_ptr += atc_info_ptr->cmd_name->str_len;
    *res_buf_ptr++ = ':';
    *res_buf_ptr++ = ' ';

    param_info_ptr = atc_info_ptr->param_ptr;
    for ( i = 0; i < atc_info_ptr->param_num; i++ )
    {
        switch ( param_info_ptr->param_type )
        {
            case ATC_CMD_PARAM_TYPE_NUMERIC:
            {
                *res_buf_ptr++ = '(';
                num_param_ptr = param_info_ptr->param_info.num_array_ptr;
                for ( j = 0; j < param_info_ptr->param_range_num; j++ )
                {
                    sprintf((char *)res_buf_ptr, "%ld", num_param_ptr->val_min);
                    res_buf_ptr += strlen( (const char *)res_buf_ptr );

                    if ( num_param_ptr->val_min != num_param_ptr->val_max )
                    {
                        *res_buf_ptr++ = '-';
                        sprintf((char *)res_buf_ptr, "%ld", num_param_ptr->val_max);                    
                        res_buf_ptr += strlen( (const char *)res_buf_ptr );
                    }

                    if ( (param_info_ptr->param_range_num - 1) != j )
                    {
                        *res_buf_ptr++ = ',';
                    }
                    
                    num_param_ptr++;
                } /* End of loop for(i) */

                *res_buf_ptr++ = ')';
                break;
            }

            case ATC_CMD_PARAM_TYPE_STRING:
            {
                *res_buf_ptr++ = '(';
                str_param_ptr = param_info_ptr->param_info.str_array_ptr;
                for ( j = 0; j < param_info_ptr->param_range_num; j++ )
                {
                    *res_buf_ptr++ = '"';
                    memcpy(res_buf_ptr, str_param_ptr->str_ptr, str_param_ptr->str_len);
                    res_buf_ptr += str_param_ptr->str_len;
                    *res_buf_ptr++ = '"';

                    if ( (param_info_ptr->param_range_num - 1) != j )
                    {
                        *res_buf_ptr++ = ',';
                    }

                    str_param_ptr++;
                }

                *res_buf_ptr++ = ')';
                break;
            }

            default:
                break;
        } /* End of switch ( param_info_ptr->param_type ) */
     
        if ( (atc_info_ptr->param_num - 1) != i )
        {
            *res_buf_ptr++ = ',';
        }   
        param_info_ptr++;
    } /* End of loop for(i) */

    *res_buf_ptr++ = atc_global_info_ptr->atc_config_ptr->s3_reg;
    *res_buf_ptr++ = atc_global_info_ptr->atc_config_ptr->s4_reg;
    *res_buf_ptr   = '\0';

    //SCI_TRACE_LOW:"ATC:BuildTestCmdResponse,The string=%s"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_4560_112_2_17_22_16_27_100,(uint8*)"s", g_rsp_str);
    SCI_TRACE_LOW("ATC_BuildTestCmdResponse,at_Rsp:%s",g_rsp_str);
#ifdef _MUX_ENABLE_
    if(ATC_DEBUG_LINK ==atc_link_id)
    {
        SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
    }
    else
    {
        MUX_Write(g_rsp_str, strlen((char*)g_rsp_str),atc_link_id);
    }
#else
#ifdef PPP_USB
    SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str),ATC_GetSioPort());
#else
    SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
#endif
#endif 
    
    return status;
}

/******************************************************************************/
// Description : This function send the asynchronous information.
// Global resource dependence : None
// Author : Elvis.xu
// Note :
/******************************************************************************/
void ATC_SendAsyncInfo(
                 ATC_CONFIG_T   *atc_config_ptr,  // point to the ATC_CONFIG_T struct.
                 uint8 ctlz_end_status
                 )
{
    uint8      data_ptr[20]={0};
	uint8      data_length = 0;
#ifdef _MUX_ENABLE_
    uint8  atc_link_id = atc_config_ptr->current_link_id;

    atc_config_ptr->atc_link_config[atc_link_id].is_ctrlz_end = TRUE;
    atc_config_ptr->atc_link_config[atc_link_id].ctrlz_end_status = ctlz_end_status;
    sprintf((char*)data_ptr, "%c%c> ", atc_config_ptr->s3_reg, atc_config_ptr->s4_reg);

    data_length = strlen((char*)data_ptr);
    if(atc_link_id == ATC_DEBUG_LINK)
    {
        SCI_TRACE_LOW("ATC: send > Async Info to SIO");
        SCI_TRACE_LOW("ATC_SendAsyncInfo,at_Rsp:%s",data_ptr);
        SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);      
        SIO_ATC_WriteCmdRes(data_ptr, data_length);
    }
    else
    {
        SCI_TRACE_LOW("ATC: send Async Info with current_link_id is %d", atc_link_id);
        // set the terminate char to receive PDU from the SIO
        ATC_SetCmdLineTerminateChar(atc_link_id, CTRL_Z, ESC);
        MUX_Write(data_ptr, data_length, atc_link_id);
    }
    
#else
    SCI_TRACE_LOW("ATC: send > Async Info to SIO");
    sprintf((char*)data_ptr, "%c%c> %c", atc_config_ptr->s3_reg, atc_config_ptr->s4_reg, atc_config_ptr->s3_reg);

    data_length = strlen((char*)data_ptr);
    // set the terminate char to receive PDU from the SIO
    #ifdef PPP_USB
    SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC,ATC_GetSioPort());
  #else
    SIO_ATC_SetCmdLineTerminateChar(CTRL_Z, ESC);
   #endif
    atc_config_ptr->is_ctrlz_end = TRUE;
    atc_config_ptr->ctrlz_end_status = ctlz_end_status;
    SCI_TRACE_LOW("ATC_SendAsyncInfo,2, at_Rsp:%s",data_ptr);
#ifdef PPP_USB
    SIO_ATC_WriteCmdRes(data_ptr, data_length,ATC_GetSioPort());
#else
    SIO_ATC_WriteCmdRes(data_ptr, data_length);
#endif
	
#endif
}
#ifndef _ULTRA_LOW_CODE_ //reduce atc code Nemge
/******************************************************************************/
// Description : This function contructs the result code response
// Global resource dependence :
// Author : wanghui
// Note :
// History:
/******************************************************************************/
void ATC_BuildResultErrCodeRsp(
    ATC_CONFIG_T       *atc_config_ptr,  // point to the ATC_CONFIG_T struct.
    int32              result_code_idx,  // result code index
    int32              err_code_idx      //error code index
)
{
    uint8       *rsp_buf_ptr  = g_rsp_str;
    uint8       cr_val;
    uint8       lf_val;
#ifdef _MUX_ENABLE_
    uint8       atc_link_id;
#endif
    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/

#ifdef _MUX_ENABLE_
    atc_link_id = atc_config_ptr->current_link_id;
#endif
    cr_val = atc_config_ptr->s3_reg;
    lf_val = atc_config_ptr->s4_reg;

    if(atc_config_ptr->q_value)
    {
        SCI_TRACE_LOW("ATC: ATC_BuildResultErrCodeRsp, q_value is TRUE");
        return ;
    }

    if(0 == atc_config_ptr->v_value)
    {
        sprintf((char *)rsp_buf_ptr, "%ld%c",
                (result_code_idx - S_ATC_RESULT_CODE_BASE), cr_val);
    }
    else
    {
        g_rsp_str[0] = cr_val;
        g_rsp_str[1] = lf_val;
        rsp_buf_ptr = &g_rsp_str[2];

        sprintf((char *)rsp_buf_ptr, "%s%c%c",
                (char *)g_atc_result_code_text[result_code_idx - S_ATC_RESULT_CODE_BASE], cr_val, lf_val);
    }

    //SCI_TRACE_LOW("ATC: ATC_BuildResultErrCodeRsp %s", g_rsp_str);
    SCI_TRACE_LOW("ATC_BuildResultErrCodeRsp,at_Rsp:%s",data_ptr);
#ifdef _MUX_ENABLE_
    if(ATC_DEBUG_LINK ==atc_link_id)
    {
        SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
    }
    else
    {
        MUX_Write(g_rsp_str, strlen((char*)g_rsp_str),atc_link_id);
    }
#else
#ifdef PPP_USB
    SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str),ATC_GetSioPort());
#else
    SIO_ATC_WriteCmdRes(g_rsp_str, strlen((char*)g_rsp_str));
#endif

#endif 
}
#endif
/******************************************************************************/
// Description : This function get the error string
// Global resource dependence : None
// Author : Ivan
// Note : None
// History:
/******************************************************************************/
LOCAL void ATC_GetErrVerbose(
                       int32 err_idx,
                       char *err_verbose
                       )
{
    switch( err_idx )
    {
        case ERR_PHONE_FAILURE:
        default:
        {
            strcpy(err_verbose,"phone failure");//0
            break;
        }
        
        case ERR_NO_CONNECT_TO_PHONE:
        {
            strcpy(err_verbose,"no connection to phone");//
            break;
        }
        
        case ERR_PHONE_ADAPTOR_LINK_RESERVED:
        {
            strcpy(err_verbose,"phone-adaptor link reserved");//2
            break;
        }
        
        case ERR_OPERATION_NOT_ALLOWED:
        {
            strcpy(err_verbose,"operation not allowed");//3
            break;
        }
        
        case ERR_OPERATION_NOT_SUPPORTED:
        {
            strcpy(err_verbose,"operation not supported");//4
            break;
        }
        
        case ERR_PH_SIM_PIN_REQUIRED:
        {
            strcpy(err_verbose,"PH-SIM PIN required");//5
            break;
        }
        
        case ERR_PH_FSIM_PIN_REQUIRED:
        {
            strcpy(err_verbose,"PH-FSIM PIN required");//6
            break;
        }
        
        case ERR_PH_FSIM_PUK_REQUIRED:
        {
            strcpy(err_verbose,"PH-FSIM PUK required");//7
            break;
        }
        
        case ERR_SIM_NOT_INSERTED:
        {
            strcpy(err_verbose,"SIM not inserted");//10
            break;
        }
        
        case ERR_SIM_PIN_REQUIRED:
        {
            strcpy(err_verbose,"SIM PIN required");//11
            break;
        }
        
        case ERR_SIM_PUK_REQUIRED:
        {
            strcpy(err_verbose,"SIM PUK required");//12
            break;
        }
        
        case ERR_SIM_FAILURE:
        {
            strcpy(err_verbose,"SIM failure");//13
            break;
        }
        
        case ERR_SIM_BUSY:
        {
            strcpy(err_verbose,"SIM busy");//14
            break;
        }
        
        case ERR_SIM_WRONG:
        {
            strcpy(err_verbose,"SIM wrong");//15
            break;
        }
        
        case ERR_INCORRECT_PASSWORD:
        {
            strcpy(err_verbose,"incorrect password");//16
            break;
        }
        
        case ERR_SIM_PIN2_REQUIRED:
        {
            strcpy(err_verbose,"SIM PIN2 required");//17
            break;
        }
        
        case ERR_SIM_PUK2_REQUIRED:
        {
            strcpy(err_verbose,"SIM PUK2 required");//18
            break;
        }
        
        case ERR_MEMORY_FULL:
        {
            strcpy(err_verbose,"memory full");//20
            break;
        }
        
        case ERR_INVALID_INDEX:
        {
            strcpy(err_verbose,"invalid index");//21
            break;
        }
        
        case ERR_NOT_FOUND:
        {
            strcpy(err_verbose,"not found");//22
            break;
        }
     
        case ERR_MEMORY_FAILURE:
        {
            strcpy(err_verbose,"memory failure");//23
            break;
        }
        
        case ERR_TEXT_STRING_TOO_LONG:
        {
            strcpy(err_verbose,"text string too long");//24
            break;
        }
        
        case ERR_INVALID_CHARACTERS_IN_TEXT_STRING:
        {
            strcpy(err_verbose,"invalid characters in text string");//25
            break;
        }
        
        case ERR_DIAL_STRING_TOO_LONG:
        {
            strcpy(err_verbose,"dial string too long");//26
            break;
        }
        
        case ERR_INVALID_CHARACTERS_IN_DIAL_STRING:
        {
            strcpy(err_verbose,"invalid characters in dial string");//27
            break;
        }
        
        case ERR_GPRS_OPERATION_FAILURE:
        {
            strcpy(err_verbose,"GPRS operation failure");//28
            break;
        }
        
        case ERR_GPRS_SEND_DATA_FAILURE:
        {
            strcpy(err_verbose,"GPRS send data failure");//29
            break;
        }
        
        case ERR_NO_NETWORK_SERVICE:
        {
            strcpy(err_verbose,"no network service");//30
            break;
        }
        
        case ERR_NETWORK_TIMEOUT:
        {
            strcpy(err_verbose,"network timeout");//31
            break;
        }
        
        case ERR_NETWORK_NOT_ALLOWED_EMERGENCY_CALLS_ONLY:
        {
            strcpy(err_verbose,"network not allowed - emergency calls only");//32
            break;
        }
        
        case ERR_NETWORK_PERSONALISATION_PIN_REQUIRED:
        {
            strcpy(err_verbose,"network personalisation PIN required");//40
            break;
        }
        
        case ERR_NETWORK_PERSONALISATION_PUK_REQUIRED:
        {
            strcpy(err_verbose,"network personalisation PUK required");//41
            break;
        }
        
        case ERR_NETWORK_SUBSET_PERSONALISATION_PIN_REQUIRED:
        {
            strcpy(err_verbose,"network subset personalisation PIN required");//42
            break;
        }
        
        case ERR_NETWORK_SUBSET_PERSONALISATION_PUK_REQUIRED:
        {
            strcpy(err_verbose,"network subset personalisation PUK required");//43
            break;
        }
        
        case ERR_SERVICE_PROVIDER_PERSONALISATION_PIN_REQUIRED:
        {
            strcpy(err_verbose,"service provider personalisation PIN required");//44
            break;
        }
        
        case ERR_SERVICE_PROVIDER_PERSONALISATION_PUK_REQUIRED:
        {
            strcpy(err_verbose,"service provider personalisation PUK required");//45
            break;
        }
        
        case ERR_CORPORATE_PERSONALISATION_PIN_REQUIRED:
        {
            strcpy(err_verbose,"corporate personalisation PIN required");//46
            break;
        }
        
        case ERR_CORPORATE_PERSONALISATION_PUK_REQUIRED:
        {
            strcpy(err_verbose,"corporate personalisation PUK required");//47
            break;
        }
        
        case ERR_AT_COMMAND_DISCARDED:
        {
            strcpy(err_verbose, "AT command discarded");//60
            break;
        }
        
        case ERR_SIM_CARD_REJ_BY_NET:
        {
            strcpy(err_verbose, "SIM card reject by network");//62
            break;
        }
        
        case ERR_SIM_CARD_SERVICE_NOT_AVAILABLE:
        {
            strcpy(err_verbose, "SIM card service not available");//63
            break;
        }
        
        case ERR_SIM_CARD_PIN_UNINITIALIZED:
        {
            strcpy(err_verbose, "SIM card PIN uninitialized");//64
            break;
        }
        
        case ERR_SIM_CARD_PIN_BLOCKED:
        {
            strcpy(err_verbose, "SIM card PIN blocked");//65
            break;
        }
        
        case ERR_SIM_CARD_PUK_BLOCKED:
        {
            strcpy(err_verbose, "SIM card PUK blocked");//66
            break;
        }
        
        case ERR_UNKNOWN:
        {
            strcpy(err_verbose,"unknown");//numeric 100
            break;
        }
        
        case ERR_ILLEGAL_MS:
        {
            strcpy(err_verbose,"Illegal MS (#3)");//numeric 103
            break;
        }
        
        case ERR_ILLEGAL_ME:
        {
            strcpy(err_verbose,"Illegal ME (#6)");//numeric 106
            break;
        }
        
        case ERR_GPRS_SERVICES_NOT_ALLOWED:
        {
            strcpy(err_verbose,"GPRS services not allowed (#7)");//numeric 107
            break;
        }
        
        case ERR_PLMN_NOT_ALLOWED:
        {
            strcpy(err_verbose,"PLMN not allowed (#11)");//numeric 111
            break;
        }
        
        case ERR_LOCATION_AREA_NOT_ALLOWED:
        {
            strcpy(err_verbose,"Location area not allowed (#12)");//numeric 112
            break;
        }
        
        case ERR_ROAMING_NOT_ALLOWED_IN_THIS_LOCATION_AREA:
        {
            strcpy(err_verbose,"Roaming not allowed in this location area (#13)");//numeric 113
            break;
        }
        
        case ERR_SERVICE_OPTION_NOT_SUPPORTED  :
        {
            strcpy(err_verbose,"Service option not supported (#32)");//132
            break;
        }
        
        case ERR_REQUEST_SERVICE_OPTION_NOT_SUBSCRIBED :     
        {
            strcpy(err_verbose,"Request service option not subscribed (#33)");//133
            break;
        }
        
        case ERR_SERVICE_OPTION_OUTOF_ORDER:
        {
            strcpy(err_verbose,"Service option temprarily out of order(#34)");//134
            break;
        }
        
        case ERR_PDP_AUTHENTICATION_FAILED:                     
        {
            strcpy(err_verbose,"PDP authentication failure");//149
            break;
        }
        case ERR_MISSING_OR_UNKOWN_APN:
        {
            strcpy(err_verbose,"Missing or unkown apn");//533
            break;
        }
    }
}

/******************************************************************************/
// Description : This function get the error string
// Global resource dependence : None
// Author : Ivan
// Note : None
/******************************************************************************/
void ATC_InitConfig(
                    ATC_CONFIG_T        *atc_config_ptr 
                    )
{
    uint  i    = 0;
 
#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
#endif

    SCI_ASSERT(NULL != atc_config_ptr); /*assert verified*/

    SCI_MEMSET(atc_config_ptr, 0, sizeof(ATC_CONFIG_T));

    atc_config_ptr->s0_reg = DEF_S0;
    atc_config_ptr->s2_reg = DEF_S2;
    atc_config_ptr->s3_reg = DEF_S3;
    atc_config_ptr->s4_reg = DEF_S4;
    atc_config_ptr->s5_reg = DEF_S5;
    atc_config_ptr->s6_reg = DEF_S6;
    atc_config_ptr->s7_reg = DEF_S7;
    atc_config_ptr->s8_reg = DEF_S8;
    atc_config_ptr->s10_reg = DEF_S10;

    atc_config_ptr->cr      = DEF_CR;
    atc_config_ptr->crc     = DEF_CRC;

    atc_config_ptr->audio_flag = FALSE;
    atc_config_ptr->cgreg = DEF_CGREG;
    atc_config_ptr->csq_flag  = FALSE;
    atc_config_ptr->cced_type = ATC_CCED_NCELL;
    atc_config_ptr->ccwa_flag = TRUE;

    atc_config_ptr->l_value  = DEF_L;
    atc_config_ptr->m_value  = DEF_M;
    atc_config_ptr->p_value  = DEF_P;
    atc_config_ptr->q_value  = DEF_Q;
    atc_config_ptr->v_value  = DEF_V;
    atc_config_ptr->x_value  = DEF_X;
    atc_config_ptr->ath_flag = FALSE;
    atc_config_ptr->cmut_flag  = FALSE;
    atc_config_ptr->sdmut_flag = FALSE;
    //atc_config_ptr->crsl_value = DEF_CRSL_VALUE;
    atc_config_ptr->vtd_value  = 0;
    //atc_config_ptr->cicb_val   = CICB_SPEECH_CALL; 
    atc_config_ptr->cmod       = DEF_CMOD;

    atc_config_ptr->cbst.speed = DEF_CBST_SPEED;
    atc_config_ptr->cbst.name  = DEF_CBST_NAME;
    atc_config_ptr->cbst.ce       = DEF_CBST_CE;

    atc_config_ptr->cops.mode     = DEF_COPS_MODE;
    atc_config_ptr->cops.format   = DEF_COPS_FORMAT;

    atc_config_ptr->cfun_flag       = TRUE;

    sprintf((char*)atc_config_ptr->cscs, "GSM");
    atc_config_ptr->cpas              = ATC_CPAS_STATUS_READY;
    
    for(i=0; i<MN_SYS_NUMBER; i++)
    {
        atc_config_ptr->sim_flag[i]         = FALSE;
        atc_config_ptr->cpin[i]             = ATC_CPIN_CODE_READY;
        atc_config_ptr->cpin_flag[i]        = FALSE;
        atc_config_ptr->pin1_enable_flag[i] = FALSE;
        atc_config_ptr->smscb_flag[i]       = FALSE;
    }

#ifdef _MUX_ENABLE_
    for(i = 0; i<ATC_MAX_LINK_ID; i++)
    {
        atc_config_ptr->atc_link_config[i].is_ctrlz_end  = FALSE;
    }
#else
    atc_config_ptr->is_ctrlz_end  = FALSE;
#endif
    atc_config_ptr->aoc_mode         = 0;
    atc_config_ptr->ss_operate       = ATC_INVALID_OPERATE;
    atc_config_ptr->audio_mode         = AUDIO_DEVICE_MODE_HANDHOLD;
    atc_config_ptr->speaker_volume   = AT_AUD_VOL_DEFAULT_VAL;
    atc_config_ptr->ssea_vol_up        = AT_AUD_VOL_DEFAULT_VAL;
    atc_config_ptr->ssea_vol_down    = AT_AUD_VOL_DEFAULT_VAL;

#ifdef _MUX_ENABLE_
    atc_config_ptr->current_link_id = ATC_INVALID_LINK_ID;
    atc_config_ptr->atc_exp_link_info.start_point = 0;
    atc_config_ptr->atc_exp_link_info.end_point = 0;
    
    for(i=0;i<ATC_MAX_EXP_RESULT;i++)
    {
        ATC_CLEAR_EXP_RESULT(&atc_config_ptr->atc_exp_link_info.atc_exp_result_info[i]);
    }
    
    for(i=0; i<ATC_MAX_LINK_ID; i++)
    {
        atc_config_ptr->atc_link_config[i].is_ctrlz_end = FALSE;
    }
#endif

    atc_config_ptr->connect_result_code_mode = ATC_PLAIN_CONNECT;

    T_ATC_CALL_EXP_IND     = PNULL;
#ifndef _ULTRA_LOW_CODE_
    T_ATC_POWEROFF_EXP_IND = PNULL;
#endif

    //Read all the item which store in the NV.
    ATC_ReadNVItem(atc_config_ptr);
#ifdef _MUX_ENABLE_
    ATC_SetEcho(atc_config_ptr->atc_config_nv.e_value);
    for(i=0;i<ATC_MAX_LINK_ID;i++)
    {
        ATC_SetCmdLineTerminateChar(i,DEF_S3, 0);  
    }
    
    SIO_ATC_SetEcho(atc_config_ptr->atc_config_nv.e_value);
    SIO_ATC_SetCmdLineTerminateChar(DEF_S3, 0); 
#else
    #ifdef PPP_USB
    SIO_ATC_SetEcho(atc_config_ptr->atc_config_nv.e_value,ATC_GetSioPort());
    SIO_ATC_SetCmdLineTerminateChar(DEF_S3, 0,ATC_GetSioPort());
  #else
    SIO_ATC_SetEcho(atc_config_ptr->atc_config_nv.e_value);
    SIO_ATC_SetCmdLineTerminateChar(DEF_S3, 0);
   #endif	
#endif

 
#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)
    #if 0
    //Initialise the variable of STK
    ATC_InitStkVar(atc_config_ptr);
    #endif
    #ifdef _ATC_SYNC_TOOL_
    //Initialise the variable of SMS
    ATC_InitSmsConfig();
    #endif

    //Initialise the variable of Phonebook
    ATC_InitPhonebookStaticVar(dual_sys);

    //Initialise the variable of Basic AT command
    ATC_InitBasicVar();
#elif defined(_ATC_UIX8910_ENABLE_)
    ATC_InitSmsConfig();
    ATC_InitPhonebookStaticVar(dual_sys);
#endif

#ifdef AUTO_TEST_SUPPORT
   ATC_InitEngVar();
#endif
#ifndef _ULTRA_LOW_CODE_
    //Initialise the variable of SS
    ATC_InitSsVar();
#endif
}

/******************************************************************************/
// Description : register the event to MN
// Global resource dependence : None
// Author : Ivan
// Note : None
/******************************************************************************/
LOCAL void ATC_RegisterEvent( void )
{
    POWER_RESTART_CONDITION_E      start_condition = POWER_GetRestartCondition();

    if ((RESTART_BY_ALARM != start_condition &&RESTART_BY_CHARGE != start_condition)
        || 1 == CLASSMARK_GetModelType() )
    {
        //Register phone event
        SCI_TraceLow("ATC_RegisterEvent");
        SCI_RegisterMsg( MN_APP_PHONE_SERVICE, 
                                (uint8)EV_MN_APP_NETWORK_STATUS_IND_F, 
                                (uint8)(MAX_MN_APP_PHONE_EVENTS_NUM -1), 
                                SCI_NULL );

        //Register STK evet
#ifdef  _ATC_ONLY
        SCI_RegisterMsg(MN_APP_SIMAT_SERVICE, 
                                (uint8)EV_MN_APP_SIMAT_DISPLAY_TEXT_IND_F, 
                                (uint8)(SIMAT_EVENT_NUM - 1), 
                                SIMAT_CallBack );
                                //SCI_TRACE_LOW:"ATC:RegisterMsg STK"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_5091_112_2_17_22_16_28_101,(uint8*)"");
#endif

#ifdef _SUPPORT_GPRS_
        SCI_RegisterMsg(MN_APP_GPRS_SERVICE,
                                (uint8)EV_MN_APP_SET_PDP_CONTEXT_CNF_F,
                                (uint8)(MAX_MN_APP_GPRS_EVENTS_NUM - 1),
                                SCI_NULL); 
#endif

#if !defined( _ULTRA_LOW_CODE_) || defined(_ATC_SYNC_TOOL_)
        SCI_RegisterMsg( MN_APP_SMS_SERVICE, 
                                (uint8)EV_MN_APP_SMS_READY_IND_F, 
                                (uint8)(MAX_MN_APP_SMS_EVENTS_NUM - 1), 
                                SCI_NULL ); 
#endif
        SCI_RegisterMsg(MN_APP_CALL_SERVICE, 
        (uint8)EV_MN_APP_CALL_START_IND_F, 
        (uint8)(MAX_MN_APP_CALL_EVENTS_NUM - 1), 
        SCI_NULL);

#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)
        SCI_RegisterMsg( MN_APP_SS_SERVICE, 
                                (uint8)EV_MN_APP_BEGIN_REGISTER_SS_F, 
                                (uint8)(MAX_MN_APP_SS_EVENTS_NUM - 1), 
                                SCI_NULL); 

        SCI_RegisterMsg( MN_APP_SMSCB_SERVICE, 
                                (uint8)EV_MN_APP_SMSCB_MSG_IND_F, 
                                (uint8)(MAX_MN_APP_SMSCB_EVENTS_NUM - 1), 
                                SCI_NULL );
        //register SIM server.
        SCI_RegisterMsg( MN_APP_SIM_SERVICE, 
                                (uint8)EV_MN_APP_MSISDN_UPDATE_CNF_F, 
                                (uint8)(MAX_MN_APP_SIM_EVENTS_NUM - 1), 
                                SCI_NULL );
#endif

}
#ifndef _ULTRA_LOW_CODE_
#ifndef _ATC_ONLY
        SCI_RegisterMsg(CHR_SERVICE, 
                            (uint8)CHR_CAP_IND, 
                            (uint8)(CHR_MSG_MAX_NUM - 1), 
                            ATC_ChrCallBack );
#endif
#endif


#ifdef _MNGPS_SUPPORT_
        SCI_RegisterMsg(MN_APP_GPS_SERVICE,
                                (uint8)EV_MN_APP_GPS_DOWNLOAD_CNF_F,
                                (uint8)(GPS_EVENT_NUM - 1),
                                SCI_NULL); 
#endif

}

#ifdef _MUX_ENABLE_
/******************************************************************************/
// Description : clear atc state after ppp modem disconnect
// Global resource dependence : None
// Author : hyman
// Note : None
/******************************************************************************/
void ATC_DisconnectModem( uint8 cid )
{
#ifndef _GSM_ONLY_
    uint32 pdp_id_arr[MN_GPRS_MAX_PDP_CONTEXT_COUNT]={0};

    //SCI_TRACE_LOW:"ATC: ATC_DisconnectModem "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_5180_112_2_17_22_16_29_102,(uint8*)"");
    
    if((cid < 1) || (cid > 11))
    {
        return;
    }
    
    pdp_id_arr[0] = cid;      
   
    if(!(s_atc_global_info.atc_config_ptr->pdp_id_arr[cid - 1]))
    {
        return;
    }    
  
    if(ERR_MNGPRS_NO_ERR == MNGPRS_DeactivatePdpContextEx(g_atc_modem_dial_sys, FALSE, pdp_id_arr))    
    {
        uint8 link_id    = 0;
        
        link_id = ATC_GetDualSimMuxLinkId(g_atc_modem_dial_sys);
        //link_id = ATC_Get_Link_Id(cid, ATC_DOMAIN_PS);
        if(link_id != (uint8)ATC_INVALID_LINK_ID)
        {
            ATC_SET_CURRENT_LINK_ID_NO_EVENT(&s_config_info, link_id);
            ATC_ADD_EXPECTED_EVENT(&s_config_info, APP_MN_DEACTIVATE_PDP_CONTEXT_CNF, pdp_id_arr[0], ATC_DOMAIN_PS);
        }
        else
        {
            //SCI_TRACE_LOW:"ATC: ATC_DisconnectModem, no call(cid:%d) link id! PSD is not existed!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_5207_112_2_17_22_16_29_103,(uint8*)"d", cid);
        }
    }
    else
    {
        //SCI_TRACE_LOW:"ATC: ATC_DisconnectModem, failure in MNGPRS_DeactivatePdpContext"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_5212_112_2_17_22_16_29_104,(uint8*)"");
    }
#endif /* _GSM_ONLY_ */
}
#else
/******************************************************************************/
// Description : clear atc state after ppp modem disconnect
// Global resource dependence : None
// Author : hyman
// Note : None
/******************************************************************************/
void ATC_DisconnectModem( void )
{
#ifdef _SUPPORT_GPRS_
    uint32 pdp_id_arr[MN_GPRS_MAX_PDP_CONTEXT_COUNT]={0};
    
    uint32 i = 0;

    for(i = 0; i < MN_GPRS_MAX_PDP_CONTEXT_COUNT; i++)
    {
        if(FALSE == s_atc_global_info.atc_config_ptr->pdp_id_arr[i])
        {
            continue;
        }
        else 
        {
            //SCI_TRACE_LOW:"pid %d is ATC activated"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_5236_112_2_17_22_16_29_105,(uint8*)"d",i+1);
            pdp_id_arr[0] = i+1;
            if(MN_GPRS_ERR_SUCCESS ==MNGPRS_DeactivatePdpContextEx(g_atc_modem_dial_sys, FALSE,pdp_id_arr))
            {
                SCI_TRACE_LOW("ATC_DisconnectModem success");
            }
            break;
        }
    }

#ifdef PPP_USB
	   SIO_ATC_ClrCmdLine(ATC_GetSioPort());
	SIO_ATC_SetCmdLineTerminateChar(s_atc_global_info.atc_config_ptr->s3_reg, '\0',ATC_GetSioPort());
#else
        SIO_ATC_ClrCmdLine();
	SIO_ATC_SetCmdLineTerminateChar(s_atc_global_info.atc_config_ptr->s3_reg, '\0');
#endif
    //SIO return to AT mode
    SIO_ATC_SetDataMode(FALSE);
   
    //SCI_TRACE_LOW:"ATC:DisconnectModem!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_5248_112_2_17_22_16_29_106,(uint8*)"");
#endif /* _SUPPORT_GPRS_ */
}
#endif

#if !defined(_ULTRA_LOW_CODE_) ||defined(_ATC_UIX8910_ENABLE_)
/******************************************************************************/
// Description : identify if the message is the needed msg on pending state
// Global resource dependence : None
// Author : Ivan.Yin
// Note : None
/******************************************************************************/
LOCAL ATC_STATUS ATC_IdentifyMsg( // the identify result 
                           int msg_type, // received the msg type
                           ATC_GLOBAL_INFO_T * atc_global_info_ptr // global info 
                          )
{
    ATC_STATUS  at_result = S_ATC_SUCCESS;
    
    if(APP_MN_ACTIVATE_PDP_CONTEXT_CNF != msg_type)
    {
        return S_ATC_FAIL;
    }
    
    if (0 == strcmp((const char *)(atc_global_info_ptr->atc_info_table->cmd_name->str_ptr), "+CGACT"))
    {
        at_result = S_ATC_SUCCESS;
    }
    else
    {
        at_result = S_ATC_FAIL;
    }

    return (at_result);
}

/*****************************************************************************/
// Description : This function is the callback of CHR
// Global resource dependence : 
// Author:       Xueli
// Note:
/*****************************************************************************/
LOCAL void ATC_ChrCallBack(uint32 taskID, uint32 argc, void *argv)
{
    uint16                signal_size;  
    uint16                signal_code;
    xSignalHeaderRec      *signal_ptr = PNULL;

    switch (argc)
    {
        case CHR_CAP_IND:
        {
            signal_code = ATC_CHR_CAP_IND;
            signal_size = sizeof(xSignalHeaderRec);
            break;
        }
        
        case CHR_CHARGE_START_IND:
        {
            signal_code = ATC_CHR_CHARGE_START_IND;
            signal_size = sizeof(xSignalHeaderRec);
            break;
        }
        
        case CHR_CHARGE_END_IND:
        {
            signal_code = ATC_CHR_CHARGE_END_IND;
            signal_size = sizeof(xSignalHeaderRec);
            break;
        }
        
        case CHR_WARNING_IND:
        {
            signal_code = ATC_CHR_WARNING_IND;
            signal_size = sizeof(xSignalHeaderRec);
            break;
        }
        
        case CHR_SHUTDOWN_IND:
        {
            signal_code = ATC_CHR_SHUTDOWN_IND;
            signal_size = sizeof(xSignalHeaderRec);
            break;
        }
        
        case CHR_CHARGE_FINISH:
        {
            signal_code = ATC_CHR_FINISH_IND;
            signal_size = sizeof(xSignalHeaderRec);
            break;
        }
        
        case CHR_CHARGE_DISCONNECT:
        {
            signal_code = ATC_CHR_DISC_IND;
            signal_size = sizeof(xSignalHeaderRec);
            break;
        }
        
        case CHR_CHARGE_FAULT:
        {
            signal_code = ATC_CHR_FAULT_IND;
            signal_size = sizeof(xSignalHeaderRec);
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert Failure unknow chr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_5324_112_2_17_22_16_29_107,(uint8*)"");
            return;
        }
    }
    /*lint -e668*/
    // create a signal
    SCI_CREATE_SIGNAL(
                signal_ptr,
                signal_code,
                signal_size,
                SCI_IDENTIFY_THREAD())
    /*lint +e668*/
    // Send the signal to the dedicated task's queue
    SCI_SEND_SIGNAL((xSignalHeaderRec*)signal_ptr, taskID);
}

/*****************************************************************************/
// Description : This function get call state use in CLCC. 
// Global resource dependence : none
// Author:       William
// Note:
/*****************************************************************************/
uint8 ATC_GetCallState(                 // return call state
                    MN_DUAL_SYS_E dual_sys,
                    uint8   call_id     // in call_id
                    )
{
    uint8   call_state = 0xff;
    
    switch(g_calls_state[dual_sys].call_context[call_id].call_entity_state)
    {
        case ATC_CALL_ACTIVE_STATE:
        {
            if(ATC_HOLD_IDLE == g_calls_state[dual_sys].call_context[call_id].hold_state)
            {
                call_state = ATC_CALL_IN_ACTIVE;
            }
            else
            {
                call_state = ATC_CALL_IN_HELD;
            }
            break;
        }

        case ATC_CALL_IDLE_STATE:
        {
            call_state = ATC_CALL_IN_DIALING;
            break;
        }

        case ATC_CALL_OUTGOING_STATE:
        {
            call_state = ATC_CALL_IN_ALERTNG;
            break;
        }

        case ATC_CALL_INCOMING_STATE:
        {
            if(1 == g_calls_state[dual_sys].call_amount)
            {
                call_state = ATC_CALL_IN_INCOMING;
            }
            else
            {
                call_state = ATC_CALL_IN_WAITING;
            }
            break;
        }

        default:
        {
            //SCI_TRACE_LOW:"ATC:call state error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_5395_112_2_17_22_16_29_108,(uint8*)"");
            break;
        }
    }
            
    return call_state;
}
#endif

/*****************************************************************************/
// Description : This function indicate sim id to user
// Global resource dependence : none
// Author:       William
// Note:
/*****************************************************************************/
LOCAL void ATC_IndicateSysId(
                MN_DUAL_SYS_E  dual_sys
                )
{
#ifdef _MUX_ENABLE_
    if(g_mux_sys_info[MUX_1].is_start==MUX_STATUS_OFF)
    {
#endif  
        sprintf((char*)g_rsp_str, "%s: %d","+CSYS", dual_sys);
        ATC_BuildInfoRsp(s_atc_global_info.atc_config_ptr, (uint8*)g_rsp_str);
#ifdef _MUX_ENABLE_
    }
#endif
}

#ifdef _SUPPORT_GPRS_
/*****************************************************************************/
// Description : This function indicate is to send detach request if no pdp is activated
// Global resource dependence : none
// Author:       winnie.zhu
// Note:
/*****************************************************************************/
void ATC_SendDetachReq(MN_DUAL_SYS_E dual_sys)
{
#ifndef WIN32
#ifndef _ATC_ONLY
    MN_GPRS_CON_STATE_T *ret_state_ptr = 0;
    uint32                                               i = 0;
    uint32                     pdp_context_num = 0;
    
    if (ATC_GPRS_DETACH == MMIAPICONNECTION_GetGPRSStatus()) //if manual attach
    {
        MNGPRS_ReadPdpContextStateEx(dual_sys, &ret_state_ptr,&pdp_context_num);

        for (i = 0; i < pdp_context_num; i++)
        {
            if (MN_CONTEXT_ACTIVATED == ret_state_ptr[i].pdp_state)
            {
                //SCI_TRACE_LOW:"pid %d is activated:%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_5444_112_2_17_22_16_29_109,(uint8*)"dd",ret_state_ptr[i].pdp_id);
                break;
            }
        }
        
        if(PNULL !=ret_state_ptr)
        {
            SCI_FREE(ret_state_ptr);
            ret_state_ptr = PNULL;
        }
        
        if (i == pdp_context_num) //no pdp is activated
        {
            MNPHONE_GprsDetachEx(dual_sys, MN_PHONE_GPRS_DETACH);
            //SCI_TRACE_LOW:"ATC_ProcessDeactPdpContextInd,manual attach,send detach"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_5458_112_2_17_22_16_29_110,(uint8*)"");
        }       
    }
#endif
#endif
}
#endif

/******************************************************************************/
// Description : This function up +SIND info
// Global resource dependence : None
// Author : minqian.qian
// Note : None
/******************************************************************************/
void ATC_UpSINDInfo(MN_DUAL_SYS_E  dual_sys,
                    ATC_CONFIG_T *atc_config_ptr,
                    xSignalHeaderRec        *sig_ptr,
                    uint16 atc_wind_bit_flag, //which the wind bit to check with
                    uint16  upValue           //+sind up value
                   )
{
#ifdef _MUX_ENABLE_
    uint8      tmp_link_id = atc_config_ptr->current_link_id;
    atc_config_ptr->current_link_id = ATC_GetDualSimMuxLinkId(dual_sys);
#endif

    BOOLEAN autoUpFlag = (0 != atc_config_ptr->atc_config_nv.creg);
    uint16  rpValue = upValue;

    SCI_ASSERT(PNULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_TRACE_LOW("ATC: +SINDInfo, IndLvl: %d, wb_flag: %d, upValue: %d, UpFlag:%d",
                  atc_config_ptr->atc_config_nv.wind_ind_level, atc_wind_bit_flag, upValue, autoUpFlag);

    if(!autoUpFlag)
    {
        return;
    }


    if(ATC_16BIT1 == ((atc_config_ptr->atc_config_nv.wind_ind_level) | atc_wind_bit_flag))
    {
        switch (atc_wind_bit_flag)
        {
            case ATC_WIND_BIT0:
            case ATC_WIND_BIT1:
            case ATC_WIND_BIT2:
            case ATC_WIND_BIT3:
            case ATC_WIND_BIT6:
            case ATC_WIND_BIT7:
            case ATC_WIND_BIT8:
            {
                sprintf((char*)g_rsp_str, "+SIND: %d",upValue);
                break;
            }

            case ATC_WIND_BIT4:
            case ATC_WIND_BIT5:
            {
                //sind call status
                uint8     callid = 0xff;
    
                SCI_ASSERT(PNULL != sig_ptr);/*assert verified: check null pointer*/
    
                switch(sig_ptr->SignalCode)
                {
                    case APP_MN_CALL_START_IND:
                        callid = ((APP_MN_CALL_START_IND_T *)sig_ptr)->call_id + 1;
                        break;
                    case APP_MN_SETUP_IND:
                        callid = ((APP_MN_SETUP_IND_T *)sig_ptr)->call_id + 1;
                        break;
                    case APP_MN_CALL_DISCONNECTED_IND:
                    case APP_MN_CALL_ERR_IND:
                        callid = ((APP_MN_CALL_DISCONNECTED_IND_T *)sig_ptr)->call_id + 1;
                        break;
                    default:
                        return;
                }

                if(ATC_WIND_BIT4 == atc_wind_bit_flag)
                {
                    sprintf((char *)g_rsp_str, "+SIND: %d,%d",rpValue, callid);
                }
                else
                {
                    sprintf((char *)g_rsp_str, "+SIND: %d,%d,%d",rpValue,callid,
                            ((APP_MN_CALL_DISCONNECTED_IND_T *)sig_ptr)->disconnected_cause);
                }

                break;
            }

            case ATC_WIND_BIT9:
            {
                //sind phonebook status
                SCI_ASSERT(PNULL != sig_ptr);/*assert verified: check null pointer*/
                switch(sig_ptr->SignalCode)
                {
                    case APP_MN_SIM_NOT_READY_IND:
                        sprintf((char *)g_rsp_str, "+SIND: %d,\"SM\",0,\"FD\",0,\"LD\",0,\"MC\",0,\"RC\",0,\"ME\",0",
                                rpValue);
                        break;
                    case APP_MN_SIM_READY_IND:
                        sprintf((char *)g_rsp_str, "+SIND: %d,\"SM\",1,\"FD\",1,\"LD\",1,\"MC\",1,\"RC\",1,\"ME\",1",
                                rpValue);
                        break;
                    default:
                        return;
                }
                break;
            }

            default:
                return;
        }
        
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    #ifdef _MUX_ENABLE_
        atc_config_ptr->current_link_id = tmp_link_id;
    #endif
    }
}

#if defined (BLUETOOTH_SUPPORT_SPRD_BT)
//#ifdef CHIP_VER_EX_SL6210C
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void ATC_SendBtSignal(uint32 sig_id, DPARAM param)
{
    BT_SIGNAL *signal = PNULL;

    if(PNULL == param)
    {
        return;
    }
    SCI_CREATE_SIGNAL(signal,sig_id,sizeof(BT_SIGNAL),SCI_IdentifyThread());
    signal->content = *(BT_MSG_T*)param;
    SCI_SEND_SIGNAL(signal,P_ATC);
}

/*****************************************************************************/
//  Discription: Callback function of blue tooth
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void ATC_BtCallBackFunc(DPARAM param)
{
    uint32 sig_id = 0xFFFFFFFF;
    BT_MSG_T *pMsg = (BT_MSG_T *)param;
    POWER_RESTART_CONDITION_E   restart_condition = POWER_GetRestartCondition();

    //SCI_TRACE_LOW("ATC_BtCallBackFunc:restart_condition=%d",restart_condition);

	if ((RESTART_BY_ALARM == restart_condition) || (RESTART_BY_CHARGE == restart_condition))
	{
		//restart with alarm, need not to handle BT message
		//Free msg space
		if (pMsg->body_size >0)
		{
			SCI_FREE(pMsg->body_ptr);
			pMsg->body_ptr = PNULL;
		}
		return;
	}

	switch(pMsg->msg_id)
	{
	case ID_STATUS_BT_ON_RES:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_BT_ON_RES");
            sig_id = APP_BT_ME_ON_CNF;
            break;

	case ID_STATUS_BT_OFF_RES:
	    SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_BT_OFF_RES");
		sig_id = APP_BT_ME_OFF_CNF;
		break;

        case ID_STATUS_BT_PRIVATE_KEY_FINISH_IND:
             SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_BT_PRIVATE_KEY_FINISH_IND");
        break;

	case ID_STATUS_CM_INQUIRY_RES:
        	SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_CM_INQUIRY_RES");
		sig_id = APP_BT_INQ_DEV_NAME;
	break;

        case ID_STATUS_CM_INQUIRY_REQ: 
             SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_CM_INQUIRY_REQ");
        break;

	case ID_STATUS_CM_INQUIRY_FINISH:
	    SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_CM_INQUIRY_FINISH");
		sig_id = APP_BT_INQ_COMP_CNF;
		break;

	case ID_STATUS_CM_CANCEL_INQUIRY_RES:
	    SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_CM_CANCEL_INQUIRY_RES");
		sig_id = APP_BT_INQUIRY_CANCEL;
		break;

	case ID_STATUS_CM_VISIBLE_RES:
	    SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_CM_VISIBLE_RES");
            sig_id = APP_BT_VISIBILE_CNF;
		break;

	case ID_STATUS_CM_DISVISIBLE_RES:
	    SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_CM_DISVISIBLE_RES");
		sig_id = APP_BT_HIDDEN_CNF;
		break;
		
	case ID_STATUS_SC_PASSKEY_REQ:
	    SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_SC_PASSKEY_REQ");
		sig_id = APP_BT_DEV_PIN_REQ;
		break;

    case ID_STATUS_SC_SSP_NUM_IND:
        SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_SC_SSP_NUM_IND");
        sig_id = APP_BT_SSP_NUM_IND;
        break;		
		
	case ID_STATUS_SC_PAIR_RES:
	    SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_SC_PAIR_RES");
		sig_id = APP_BT_DEV_PAIR_COMPLETE;
		break;

    case ID_STATUS_SC_PAIR_IND:
        SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_SC_PAIR_IND");
		sig_id = APP_BT_DEV_PAIR_COMPLETE_IND;
		break;
		
	case ID_STATUS_SC_DEPAIR_RES:
	    SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_SC_DEPAIR_RES");
            sig_id = APP_BT_DELETE_DEVICE_RES;
        break;

        case ID_STATUS_SD_READ_SERVICES_RES:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_SD_READ_SERVICES_RES");
        break;

//HFG Message define begin	
        case ID_STATUS_HFAG_SLC_CONNECTED:
             SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFAG_SLC_CONNECTED");
             sig_id = APP_BT_HFAG_SLC_CONNECTED;
        break;

        case ID_STATUS_HFAG_SLC_CONNECTED_IND:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFAG_SLC_CONNECTED_IND");
            sig_id = APP_BT_HFAG_SLC_CONNECTED_IND;
        break;

        case ID_STATUS_HFAG_SLC_DISCONNECTED:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFAG_SLC_DISCONNECTED");
            sig_id = APP_BT_HFAG_SLC_DISCONNECTED;
        break;

        case ID_STATUS_HFAG_PLAY_CFM:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFAG_PLAY_CFM");
            sig_id = APP_BT_HFAG_PLAY_CFM;
        break;
        
        case ID_STATUS_HFAG_STOP_CFM:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFAG_STOP_CFM");
        break;

        case ID_STATUS_HFAG_HFU_CALL_ANSWER:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFAG_HFU_CALL_ANSWER");
        break;

        case ID_STATUS_HFAG_HFU_CALL_HANGUP:
           SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFAG_HFU_CALL_HANGUP");
        break;

        case ID_STATUS_HFAG_HFU_DIAL_NUMBER:
           SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFAG_HFU_DIAL_NUMBER");
        break;

        case ID_STATUS_HFAG_HFU_DIAL_MEMORY:
           SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFAG_HFU_DIAL_MEMORY");
        break;

        case ID_STATUS_HFAG_HFU_REDIAL:
           SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFAG_HFU_REDIAL");
        break;

        case ID_STATUS_HFAG_HFU_HOLD:
           SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFAG_HFU_HOLD");
        break;

        case ID_STATUS_HFAG_HFU_DTMF_NUMBER:
           SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFAG_HFU_DTMF_NUMBER");
        break;

        case ID_STATUS_HFAG_AUDIO_CONNECTED:
           SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFAG_AUDIO_CONNECTED");
           sig_id = APP_BT_HFAG_AUDIO_CONNECTED;
        break;

        case ID_STATUS_HFAG_AUDIO_DISCONNECTED:
           SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFAG_AUDIO_DISCONNECTED");
           sig_id = APP_BT_HFAG_AUDIO_DISCONNECTED;
        break;

        case ID_STATUS_HFAG_HFU_STATUS_REQ:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFAG_HFU_STATUS_REQ");
        break;

        case ID_STATUS_HS_MIC_VOL_CHANGE:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HS_MIC_VOL_CHANGE");
        break;

        case ID_STATUS_HS_SPK_VOL_CHANGE:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HS_SPK_VOL_CHANGE");
        break;

        case ID_STATUS_HFAG_HFU_CLCC:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFAG_HFU_CLCC");
            sig_id = APP_BT_HFAG_HFU_CLCC;
        break;
//HFG Message define end


        //OPP Client message begin(sending file)
        case ID_STATUS_OPC_CONNECT_RES:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_OPC_CONNECT_RES");
        break;

        case ID_STATUS_OPC_SERVER_ABORT:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_OPC_SERVER_ABORT");
        break;

        case ID_STATUS_OPC_PUT_ABORT:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_OPC_PUT_ABORT");
        break;

        case ID_STATUS_OPC_PUT_OBJECT:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_OPC_PUT_OBJECT");
        break;

        case ID_STATUS_OPC_PUT_FINAL:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_OPC_PUT_FINAL");
        break;

        case ID_STATUS_OPC_DISCONNECT_IND:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_OPC_DISCONNECT_IND");
        break;

        //OPP Server Message begin (receive file)
        case ID_STATUS_OPS_PUT_REQ:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_OPS_PUT_REQ");
        break;

        case ID_STATUS_OPS_CLIENT_ABORT:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_OPS_CLIENT_ABORT");
        break;
        
        case ID_STATUS_OPS_PUT_ABORT:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_OPS_PUT_ABORT");
        break;

        case ID_STATUS_OPS_PUT_OBJECT:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_OPS_PUT_OBJECT");
        break;

        case ID_STATUS_OPS_PUT_FINAL:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_OPS_PUT_FINAL");
        break;

        case ID_STATUS_OPS_DISCONNECT_IND:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_OPS_DISCONNECT_IND");
        break;        


#ifdef BT_SPP_SUPPORT
//SPP related
        case ID_STATUS_SPP_LINK_STATUS_RES:
           SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_SPP_LINK_STATUS_RES");
        break;

        case ID_STATUS_SPP_DATA_RECIEVE_IND:
           SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_SPP_DATA_RECIEVE_IND");
        break;
        
        case ID_STATUS_SPP_DATA_RECIEVE_CFM:
           SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_SPP_DATA_RECIEVE_CFM");
        break;

	case ID_STATUS_SPP_CONNECT_IND:
	    SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_SPP_CONNECT_IND");
		sig_id = APP_BT_SPP_CONNECT_IND;
		break;

	case ID_STATUS_SPP_DISCONNECT_IND:
	    SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_SPP_DISCONNECT_IND");
		sig_id = APP_BT_SPP_DISCONNECT_IND;
		break;

        case ID_STATUS_SPP_STATUS_IND:
           SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_SPP_STATUS_IND");
        break;

        case ID_STATUS_SPP_SERVICE_NAME_IND:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_SPP_SERVICE_NAME_IND");
        break;

        case ID_STATUS_SPP_DATA_RECEIVE_EX_IND:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_SPP_DATA_RECEIVE_EX_IND");
        break;
//SPP Message end
#endif

#ifdef BT_DIALER_SUPPORT
        //HFU Message begin (BtPhone)
        case ID_STATUS_HFU_SLC_CONNECTED:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFU_SLC_CONNECTED");
            sig_id = APP_BT_HFU_CONNECT_CNF;
        break;

        case ID_STATUS_HFU_SLC_DISCONNECTED:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFU_SLC_DISCONNECTED");
            sig_id = APP_BT_HFU_DISCONNECT_CNF;
        break;

        case ID_STATUS_HFU_AUDIO_CONNECTED:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFU_AUDIO_CONNECTED");
            sig_id = APP_BT_HFU_AUDIO_CONNECTED;
        break;

        case ID_STATUS_HFU_AUDIO_DISCONNECTED:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFU_AUDIO_DISCONNECTED");
            sig_id = APP_BT_HFU_AUDIO_DISCONNECTED;
        break;

        case ID_STATUS_HFU_CIND_IND:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFU_CIND_IND");
        break;

        case ID_STATUS_HFU_RING_IND:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFU_RING_IND");
        break;

        case ID_STATUS_HFU_CLIP_IND:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFU_CLIP_IND");
            sig_id = APP_BT_HFU_CLIP_IND; 
        break;

        case ID_STATUS_HFU_CIEV_IND:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFU_CIEV_IND");
        break;

        case ID_STATUS_HFU_CLCC_RECORD_IND:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFU_CLCC_RECORD_IND");
            sig_id = APP_BT_HFU_CLCC_RECORD_IND;
        break;

        case ID_STATUS_HFU_DIAL_NUMBER_RESULT_IND:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFU_DIAL_NUMBER_RESULT_IND");
        break;

        case ID_STATUS_HFU_ATA_RESULT_IND:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFU_ATA_RESULT_IND");
            sig_id = APP_BT_HFU_ATA_RESULT_IND;
        break;

        case ID_STATUS_HFU_HANGUP_RESULT_IND:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFU_HANGUP_RESULT_IND");
            sig_id = APP_BT_HFU_HANGUP_RESULT_IND;
        break;

        case ID_STATUS_HFU_SEND_DTMF_RESULT_IND:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFU_SEND_DTMF_RESULT_IND");
        break;

        case ID_STATUS_HFU_CALLHOLD_RESULT_IND:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFU_CALLHOLD_RESULT_IND");
        break;

        case ID_STATUS_HFU_CLCC_RESULT_IND:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFU_CLCC_RESULT_IND");
        break;

        case ID_STATUS_HFU_SPK_VOL_IND:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFU_SPK_VOL_IND");
        break;

        case ID_STATUS_HFU_MIC_VOL_IND:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFU_MIC_VOL_IND");
        break;

        case ID_STATUS_HFU_CCWA_IND:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFU_CCWA_IND");
        break;

        case ID_STATUS_HFU_ENABLE_VOICE_RECOGN_RESULT_IND:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFU_ENABLE_VOICE_RECOGN_RESULT_IND");
        break;

        case ID_STATUS_HFU_VOICE_RECOGN_STATE_IND :
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFU_VOICE_RECOGN_STATE_IND");
        break;

        case ID_STATUS_HFU_REDIAL_RESULT_IND:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFU_REDIAL_RESULT_IND");
            sig_id = APP_BT_HFU_REDIAL_RESULT_IND;
        break;

        case ID_STATUS_HFU_MEMORY_DIAL_RESULT_IND:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HFU_MEMORY_DIAL_RESULT_IND");
        break;		
        //HFU Message end
#endif

        //PBAP Message start
#ifdef BT_PBAP_SUPPORT
        case ID_STATUS_PBAP_TP_DISCONNECTED:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_PBAP_TP_DISCONNECTED");
        break;

        case ID_STATUS_PBAP_COMPLETE:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_PBAP_COMPLETE");
        break;

        case ID_STATUS_PBAP_ABORT:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_PBAP_ABORT");
        break;

        case ID_STATUS_PBAP_PARAMS_RX:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_PBAP_PARAMS_RX");
        break;

        case ID_STATUS_PBAP_DATA_IND:
            //Free the pointer
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_PBAP_DATA_IND");
        break;
#endif
        //PBAP Message end

#ifdef BT_MAP_SUPPORT
        case ID_STATUS_MAP_TP_DISCONNECTED:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_MAP_TP_DISCONNECTED");
        break;

        case ID_STATUS_MAP_COMPLETE:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_MAP_COMPLETE");
        break;

        case ID_STATUS_MAP_ABORT:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_MAP_ABORT");
        break;

        case ID_STATUS_MAP_CLIENT_PARAMS_RX:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_MAP_CLIENT_PARAMS_RX");
        break;

        case ID_STATUS_MAP_SERVER_PARAMS_RX:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_MAP_SERVER_PARAMS_RX");
        break;

        case ID_STATUS_MAP_DATA_IND:
            //Should FREE the pointer
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_MAP_DATA_IND");
        break;

        case ID_STATUS_MAP_MNS_SERVER_CONNECTED:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_MAP_MNS_SERVER_CONNECTED");
        break;

        case ID_STATUS_MAP_MNS_SERVER_PARAMS_RX:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_MAP_MNS_SERVER_PARAMS_RX");
        break;

        case ID_STATUS_MAP_MNS_SERVER_DATA_IND:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_MAP_MNS_SERVER_DATA_IND");
        break;
#endif

#ifdef BT_AV_SNK_SUPPORT
        case ID_STATUS_A2DP_SINK_CONNECT_COMPLETED:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_A2DP_SINK_CONNECT_COMPLETED");
            sig_id = APP_BT_A2DPSNK_CONNECT_CNF;
        break;

        case ID_STATUS_A2DP_SINK_DISCONNECT_COMPLETED:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_A2DP_SINK_DISCONNECT_COMPLETED");
            sig_id = APP_BT_A2DPSNK_DISCONNECT_CNF;
        break;

        case ID_STATUS_A2DP_SINK_STREAM_STARTED:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_A2DP_SINK_STREAM_STARTED");
        break;

        case ID_STATUS_A2DP_SINK_STREAM_SUSPENDED:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_A2DP_SINK_STREAM_SUSPENDED");
        break;
#endif

#ifdef BT_AV_CT_SUPPORT
        case ID_STATUS_AVRCP_CT_CONNECT_COMPLETE:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_AVRCP_CT_CONNECT_COMPLETE");
        break;

        case ID_STATUS_AVRCP_CT_DISCONNECT_COMPLETE:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_AVRCP_CT_DISCONNECT_COMPLETE");
        break;

        case ID_STATUS_AVRCP_CT_MEDIA_ATTRIBUTES:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_AVRCP_CT_MEDIA_ATTRIBUTES");
            sig_id = APP_BT_AVRCP_CT_MEDIA_ATTRIBUTES;
        break;

        case ID_STATUS_AVRCP_EVENT_TRACK_CHANGED:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_AVRCP_EVENT_TRACK_CHANGED");
        break;

        case ID_STATUS_AVRCP_CT_PLAY_STATUS:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_AVRCP_CT_PLAY_STATUS");
        break;

        case ID_STATUS_AVRCP_EVENT_PLAYBACK_CHANGED:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_AVRCP_EVENT_PLAYBACK_CHANGED");
        break;

        case ID_STATUS_AVRCP_EVENT_PLAYBACK_POS_CHANGED:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_AVRCP_EVENT_PLAYBACK_POS_CHANGED");
        break;

        case ID_STATUS_AVRCP_EVENT_PLAYER_APPLICATION_CHANGED:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_AVRCP_EVENT_PLAYER_APPLICATION_CHANGED");
        break;

        case ID_STATUS_AVRCP_SUPPORTED_PLAYER_ATTRIBUTES:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_AVRCP_SUPPORTED_PLAYER_ATTRIBUTES");
        break;   

        case ID_STATUS_AVRCP_SEND_REWIND_OR_FASTFWD_SINGNAL:
             SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_AVRCP_SEND_REWIND_OR_FASTFWD_SINGNAL");
             sig_id = APP_BT_AVRCP_SEND_REWIND_OR_FASTFWD_SINGNAL;
        break;

#endif

#ifdef BT_HID_SUPPORT
        case ID_STATUS_HID_CONNECTED_IND:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HID_CONNECTED_IND");
            sig_id = APP_BT_HID_CONNECT_IND;
        break;

        case ID_STATUS_HID_DISCONNECTED_IND:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_HID_DISCONNECTED_IND");
            sig_id = APP_BT_HID_DISCONNECT_IND;
        break;
#endif

//BLE GATT related
#if defined(BLE_INCLUDED) || defined(BT_BLE_SUPPORT)
        case ID_STATUS_GATT_SERVER_REG_READ_RES:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_GATT_SERVER_REG_READ_RES");
			sig_id = AT_BT_GATT_SERVER_REG_READ_RES;
            break;

 	case ID_STATUS_GATT_SERVER_REG_WRITE_RES:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_GATT_SERVER_REG_WRITE_RES");
			sig_id = AT_BT_GATT_SERVER_REG_WRITE_RES;
            break;

 	case ID_STATUS_GATT_SERVER_DEREG_WRITE_RES:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_GATT_SERVER_DEREG_WRITE_RES");
            ATC_SendBtSignal(AT_BT_GATT_SERVER_DEREG_WRITE_RES,param);
			sig_id = AT_BT_GATT_SERVER_DEREG_WRITE_RES;		
            break;            

        case ID_STATUS_GATT_SERVER_SERVICE_READ_RES:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_GATT_SERVER_SERVICE_READ_RES");
			sig_id = AT_BT_GATT_SERVER_SERVICE_READ_RES;	
            break;

        case ID_STATUS_GATT_SERVER_SERVICE_ADD_WRITE_RES:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_GATT_SERVER_SERVICE_ADD_WRITE_RES");
			sig_id = AT_BT_GATT_SERVER_SERVICE_ADD_WRITE_RES;	
            break;
            
        case ID_STATUS_GATT_SERVER_SERVICE_DEL_WRITE_RES:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_GATT_SERVER_SERVICE_DEL_WRITE_RES");
			sig_id = AT_BT_GATT_SERVER_SERVICE_DEL_WRITE_RES;	          
			break;
            
        case ID_STATUS_GATT_SERVER_CHAR_WRITE_RES:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_GATT_SERVER_CHAR_WRITE_RES");
			sig_id = AT_BT_GATT_SERVER_CHAR_WRITE_RES;	          			
            break;

        case ID_STATUS_GATT_SERVER_DESC_WRITE_RES:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_SPP_DATA_RECIEVE_CFM");
			sig_id = AT_BT_GATT_SERVER_DESC_WRITE_RES;	          						
            break;

 	case ID_STATUS_GATT_SERVER_SERVICE_START_WRITE_RES:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_GATT_SERVER_SERVICE_START_WRITE_RES");
			sig_id = AT_BT_GATT_SERVER_SERVICE_START_WRITE_RES;	          						
		    break;

        case ID_STATUS_GATT_SERVER_SERVICE_STOP_WRITE_RES:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_GATT_SERVER_SERVICE_STOP_WRITE_RES");
			sig_id = AT_BT_GATT_SERVER_SERVICE_STOP_WRITE_RES;	          						
            break;

        case ID_STATUS_GATT_SERVER_INDICAT_NOTIFY_WRITE_RES:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_GATT_SERVER_INDICAT_NOTIFY_WRITE_RES");
			sig_id = AT_BT_GATT_SERVER_INDICAT_NOTIFY_WRITE_RES;	          						
			break;
            
        case ID_STATUS_GATT_SERVER_REQ_RECIEVE_IND:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_GATT_SERVER_REQ_RECIEVE_IND");
			sig_id = AT_BT_GATT_SERVER_REQ_RECIEVE_IND;	          						
			break;
            
        case ID_STATUS_GATT_SERVER_RSP_WRITE_RES:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_GATT_SERVER_RSP_WRITE_RES");
			sig_id = AT_BT_GATT_SERVER_RSP_WRITE_RES;	          						
		    break;            

        case ID_STATUS_GATT_SERVER_CONN_READ_RES:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_GATT_SERVER_CONN_READ_RES");
			sig_id = AT_BT_GATT_SERVER_CONN_READ_RES;	          									
            break;
            
        case ID_STATUS_GATT_SERVER_DISCONN_WRITE_RES:
            SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc ID_STATUS_GATT_SERVER_DISCONN_WRITE_RES");
			sig_id = AT_BT_GATT_SERVER_DISCONN_WRITE_RES;	          												
            break;
#endif           
 //BLE GATT related
 

	default:
		SCI_TRACE_LOW("ATC: ATC_BtCallBackFunc, discard msg_id = 0x%x",pMsg->msg_id);
		break;
    }

    if (0xFFFFFFFF != sig_id)
    {
        ATC_SendBtSignal(sig_id,param);
    }
    else
    {
        if ((NULL != pMsg->body_ptr) && (0 != pMsg->body_size))
        {
            SCI_FREE(pMsg->body_ptr);
            pMsg->body_ptr = PNULL;
        }
    }
}
//#endif

ATC_STATUS ATC_ProcessBtSignal(ATC_CONFIG_T *atc_config_ptr, xSignalHeaderRec *sig_ptr)
{
    BT_SIGNAL *signal = (BT_SIGNAL *)sig_ptr;
    BT_MSG_T msg_body = signal->content;

	SCI_TRACE_LOW("ATC: ATC_ProcessBtSignal");
 	
	switch(sig_ptr->SignalCode)
	{
	    case APP_BT_ME_ON_CNF:
		    {
                if(BT_SUCCESS == msg_body.status)
                {
                    ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR, S_ATC_RESULT_CODE_OK, 0);
                }
                else
                {
                    ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR, S_ATC_RESULT_CODE_ERROR, 0);
                }
            }
            break;
            
        case  APP_BT_ME_OFF_CNF:
            {
                if(BT_SUCCESS == msg_body.status)
                {
                    ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR, S_ATC_RESULT_CODE_OK, 0);
                }
                else
                {
                    ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR, S_ATC_RESULT_CODE_ERROR, 0);
                }
            }
            break;

        case APP_BT_INQ_DEV_NAME:
            {
                if(BT_SUCCESS == msg_body.status)
                {
                    BT_DEVICE_INFO      dev_info;
                    uint8 addr_string[ATC_MAX_BT_ADDRESS_STRING_LEN] = {0};
                    uint8 name_string[ATC_MAX_BT_NAME_STRING_LEN] = {0};
                    
                    if (PNULL == msg_body.body_ptr)
                    {
                        SCI_TRACE_LOW("ATC: receive APP_BT_INQ_DEV_CNF, but msg body ptr is NULL.");
                        return S_ATC_SUCCESS;
                    }
					dev_info  = *((BT_DEVICE_INFO*)msg_body.body_ptr); 
                    ConvertBtAddrtoString(dev_info.addr.addr, addr_string);
                    ConvertBtNametoString(dev_info.name,BT_DEVICE_NAME_SIZE, name_string,sizeof(name_string));
                    
                    //+INQ:<address>,<name>,<cod>
                    sprintf((char *)g_rsp_str, "%s%s%s%s%s%08x","+INQ:",addr_string,",",name_string,",",dev_info.dev_class);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
                }
                else
                {
                    ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR, S_ATC_RESULT_CODE_ERROR, 0);
                }
            }
            break;

        case APP_BT_INQ_COMP_CNF:
            {
                if(BT_SUCCESS == msg_body.status)
                {
                    sprintf((char *)g_rsp_str, "%s","+INQE");
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
                }
                else
                {
                    
                }
            }
            break;
            
        case APP_BT_INQUIRY_CANCEL:
            {
                if(BT_SUCCESS == msg_body.status)
                {
                    ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR, S_ATC_RESULT_CODE_OK, 0);
                }
                else
                {
                    ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR, S_ATC_RESULT_CODE_ERROR, 0);
                }
            }
            break;
            
        case APP_BT_VISIBILE_CNF:
            {
                if(BT_SUCCESS == msg_body.status)
                {
                    BT_UpdateNV();
                    ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR, S_ATC_RESULT_CODE_OK, 0);
                }
                else
                {
                    ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR, S_ATC_RESULT_CODE_ERROR, 0);
                }
            }
            break;
            
        case APP_BT_HIDDEN_CNF:
            {
                if(BT_SUCCESS == msg_body.status)
                {
                    BT_UpdateNV();
                    ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR, S_ATC_RESULT_CODE_OK, 0);
                }
                else
                {
                    ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR, S_ATC_RESULT_CODE_ERROR, 0);
                }
            }
            break;
            
        case APP_BT_DEV_PIN_REQ:
            {
                if(BT_SUCCESS == msg_body.status)
                {
                    uint8 addr_string[ATC_MAX_BT_ADDRESS_STRING_LEN] = {0};
                    uint8 name_string[ATC_MAX_BT_NAME_STRING_LEN] = {0};
                    BT_DEVICE_INFO      dev_info;  
                    
                    if (PNULL == msg_body.body_ptr)
                    {
                        SCI_TRACE_LOW("ATC: receive APP_BT_INPUT_PIN_IND, but msg body ptr is NULL.");
                        return S_ATC_SUCCESS;
                    }
					
                    dev_info = *((BT_DEVICE_INFO*)msg_body.body_ptr);
					
                    ConvertBtAddrtoString(dev_info.addr.addr, addr_string);
                    ConvertBtNametoString(dev_info.name,BT_DEVICE_NAME_SIZE, name_string,sizeof(name_string));

                    //+PASSKEYREQ:<address>,<name>
                    sprintf((char *)g_rsp_str, "%s%s%s%s","+PASSKEYREQ:",addr_string,",",name_string);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
                }
                else
                {
                    ATC_BuildResultCodeRsp(S_CONFIG_INFO_PTR, S_ATC_RESULT_CODE_ERROR, 0);
                }
            }
            break;
            
        case APP_BT_SSP_NUM_IND:
            {
                BT_DEVICE_INFO_EXT dev_info = {0};
                uint8 addr_string[ATC_MAX_BT_ADDRESS_STRING_LEN] = {0};
   
                if (PNULL == msg_body.body_ptr)
                {
                    SCI_TRACE_LOW("ATC: receive APP_BT_SSP_NUM_IND, but msg body ptr is NULL.");
                    return S_ATC_SUCCESS;
                }

            	dev_info = *((BT_DEVICE_INFO_EXT *)msg_body.body_ptr);
            	
            	ConvertBtAddrtoString(dev_info.addr.addr, addr_string);

                //+SSPNUMIND:<ssp numeric key>,<address>
            	if(dev_info.ssp_numric_key[0] == 0xff)
                {
                    sprintf((char *)g_rsp_str, "%s%s%s%s","+SSPNUMIND:","FFFFFF",",",addr_string);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
                }
                else
                {
                    sprintf((char *)g_rsp_str, "%s%s%s%s","+SSPNUMIND:",dev_info.ssp_numric_key,",",addr_string);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
                }
            }
            break;
            
        case APP_BT_DEV_PAIR_COMPLETE:
            {
                if(BT_SUCCESS == msg_body.status)
                {
                    if (PNULL == msg_body.body_ptr)
                    {
                        SCI_TRACE_LOW("ATC: receive APP_BT_DEV_PAIR_COMPLETE_CNF, but msg body ptr is NULL.");
                        return S_ATC_SUCCESS;
                    }

                    sprintf((char *)g_rsp_str, "%s","+PAIRCOMPLETE:0");
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
                }
                else
                {
                    sprintf((char *)g_rsp_str, "%s","+PAIRCOMPLETE:1");
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
                }
            }
            break;
            
        case APP_BT_DEV_PAIR_COMPLETE_IND:
            {
                if(BT_SUCCESS == msg_body.status)
                {
                    sprintf((char *)g_rsp_str, "%s","+PAIRCOMPLETE:0");
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
                }
                else
                {
                    sprintf((char *)g_rsp_str, "%s","+PAIRCOMPLETE:1");
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
                }
            }
            break;
            
        case APP_BT_DELETE_DEVICE_RES:
            break;
            
    	case APP_BT_SPP_DATA_RECIEVE_IND:
    	    break;
    	    
    	case APP_BT_SPP_DATA_RECIEVE_CNF:
    	    break;
    	    
    	case APP_BT_SPP_CONNECT_IND:
    	    {
                if(BT_SUCCESS == msg_body.status)
                {
                    sprintf((char *)g_rsp_str, "%s","+CONNECTED");
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
                }
                else
                {
                    sprintf((char *)g_rsp_str, "%s","+DISCONNECTED");
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
                }
            }
    	    break;
    	    
    	case APP_BT_SPP_DISCONNECT_IND:
    	    {
                sprintf((char *)g_rsp_str, "%s","+DISCONNECTED");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
            }
            break;
            
//#ifdef CHIP_VER_EX_SL6210C
#ifdef BT_HID_SUPPORT
        case APP_BT_HID_CONNECT_IND:
    	    {
                sprintf((char *)g_rsp_str, "%s","+CONNECTED HID");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
            }
            break;

        case APP_BT_HID_DISCONNECT_IND:
    	    {
                sprintf((char *)g_rsp_str, "%s","+DISCONNECTED HID");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
            }
            break;
#endif

#ifdef BT_DIALER_SUPPORT
        case APP_BT_HFU_CONNECT_CNF:
    	    {
                sprintf((char *)g_rsp_str, "%s","+CONNECTED HFU");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
            }
            break;

        case APP_BT_HFU_DISCONNECT_CNF:
    	    {
                sprintf((char *)g_rsp_str, "%s","+DISCONNECTED HFU");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
            }
            break;
#endif

#ifdef BT_AV_SNK_SUPPORT
        case APP_BT_A2DPSNK_CONNECT_CNF:
    	    {
                sprintf((char *)g_rsp_str, "%s","+CONNECTED A2DPSNK");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                SCI_MEMSET(&s_atc_bt_avrcp_attr,0,sizeof(AvrcpCtElementAttributes));
            }
            break;
        
        case APP_BT_A2DPSNK_DISCONNECT_CNF:
    	    {
                sprintf((char *)g_rsp_str, "%s","+DISCONNECTED A2DPSNK");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                SCI_MEMSET(&s_atc_bt_avrcp_attr,0,sizeof(AvrcpCtElementAttributes));
            }
            break;
#endif

#ifdef BT_AV_CT_SUPPORT
        case APP_BT_AVRCP_CT_MEDIA_ATTRIBUTES:
            {
                uint8 i = 0;
                char title_str[30] = {0};//the 30 > MAX_TG_PLAYER_STRING_LEN

                if ((BT_SUCCESS == msg_body.status) && (PNULL != msg_body.body_ptr))
                {
                    SCI_MEMCPY(&s_atc_bt_avrcp_attr,(AvrcpCtElementAttributes *)msg_body.body_ptr,sizeof(AvrcpCtElementAttributes));
                    for(i=0;i<7;i++)//AVRCP_NUM_MEDIA_ELEMENTS is 7
                    {
                         if ((1 == s_atc_bt_avrcp_attr.txt[i].attrId) && (0 != s_atc_bt_avrcp_attr.txt[i].length) 
    					 	&& (0x6a == s_atc_bt_avrcp_attr.txt[i].charSet))//Title
                         {
                             SCI_MEMCPY(&title_str[0],&s_atc_bt_avrcp_attr.txt[i].string[0],s_atc_bt_avrcp_attr.txt[i].length);
                             
                             title_str[s_atc_bt_avrcp_attr.txt[i].length] = '\0';
                             sprintf((char *)g_rsp_str, "+SPBTCTRL: Music Title:%s",title_str);//But the Music "Title" must be in English!
                             ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        }
                    }
                }
            }
            break;
#endif

        case APP_BT_HFAG_SLC_CONNECTED:
    	    {
                sprintf((char *)g_rsp_str, "%s","+CONNECTED HFG");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
            }
            break;

        case APP_BT_HFAG_SLC_CONNECTED_IND:
    	    {
                sprintf((char *)g_rsp_str, "%s","+CONNECTED IND HFG");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
            }
            break;

        case APP_BT_HFAG_SLC_DISCONNECTED:
    	    {
                sprintf((char *)g_rsp_str, "%s","+DISCONNECTED HFG");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
            }
            break;

        case APP_BT_HFAG_PLAY_CFM:
        case APP_BT_HFAG_STOP_CFM:
        case APP_BT_HFAG_HFU_CALL_ANSWER:
        case APP_BT_HFAG_HFU_CALL_HANGUP:
        case APP_BT_HFAG_HFU_DIAL_NUMBER:
        case APP_BT_HFAG_HFU_DIAL_MEMORY:
        case APP_BT_HFAG_HFU_DIAL_REDIAL:
        case APP_BT_HFAG_HFU_HOLD:
        case APP_BT_HFAG_HFU_DTMF_NUMBER:
        case APP_BT_HFAG_AUDIO_CONNECTED:
        case APP_BT_HFAG_AUDIO_DISCONNECTED:
        case APP_BT_HFAG_HFU_STATUS_REQ:
        case APP_BT_HFAG_HS_MIC_VOL_CHANGE:
        case APP_BT_HFAG_HS_SPK_VOL_CHANGE:
		break;

        case APP_BT_HFU_CLCC_RECORD_IND:
        {
            BT_HFU_CLCC_T* hfu_clcc = (BT_HFU_CLCC_T*)msg_body.body_ptr;
            SCI_TRACE_LOW("APP_BT_HFU_CLCC_RECORD_IND REPORT TELEPHONE NUMBER" );
            sprintf((char *)g_rsp_str, "%s %s","+HFU REDIAL TELE NUM :",hfu_clcc->number);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
            break;
        }
        
        case APP_BT_HFU_CLIP_IND:
        {
            BT_HFU_CALLERID_PARAMS_T* hfu_clcc = (BT_HFU_CALLERID_PARAMS_T*)msg_body.body_ptr;
   
            SCI_TRACE_LOW("APP_BT_HFU_CLCC_RECORD_IND REPORT TELEPHONE NUMBER" );
            sprintf((char *)g_rsp_str, "%s %s","+HFU TELE NUM :",hfu_clcc->number);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
            break;
        }
            
        case APP_BT_HFU_AUDIO_CONNECTED:
        {
            sprintf((char *)g_rsp_str, "%s","+CONNECTED HFU AUDIO");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
            break;
        }

        case APP_BT_HFU_AUDIO_DISCONNECTED:
        {
            sprintf((char *)g_rsp_str, "%s","+DISCONNECTED HFU AUDIO");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
            break;
        }

#ifdef BT_HFU_SUPPORT
        case APP_BT_HFU_HANGUP_RESULT_IND:
        {
            if(1 == atc_hfu_reject_flag)
            {
                sprintf((char *)g_rsp_str, "%s","+BT CALL REJECT");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
                atc_hfu_reject_flag = 0;
            }
            else
            {
                sprintf((char *)g_rsp_str, "%s","+BT CALL HANGUP");
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
            }
            break;
        }
#endif

#ifdef BT_AV_CT_SUPPORT
        case APP_BT_AVRCP_SEND_REWIND_OR_FASTFWD_SINGNAL:
        {
            AvrcpCtPanelOperation Op_Avrcp = *((AvrcpCtPanelOperation*)(msg_body.body_ptr));
            SCI_TRACE_LOW("yiheng:APP_BT_AVRCP_SEND_REWIND_OR_FASTFWD_SINGNAL");
            AVRCP_CT_SetPanelKey(Op_Avrcp,FALSE); 
            break;
        }
#endif
        case APP_BT_HFU_REDIAL_RESULT_IND:
        {
            sprintf((char *)g_rsp_str, "%s","+BT CALL REDIAL");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
            break;
        }

        case APP_BT_HFU_ATA_RESULT_IND:
        {
            sprintf((char *)g_rsp_str, "%s","+BT CALL ANSWER");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
            break;
        }
//#endif //#ifdef CHIP_VER_EX_SL6210C

//BLE GATT related
#if defined(BLE_INCLUDED) || defined(BT_BLE_SUPPORT)
    	case AT_BT_GATT_SERVER_REG_READ_RES:
    	    {   
                gatt_server_param_t  param =  *((gatt_server_param_t *)msg_body.body_ptr);
                uint8 server_uuid[33] = {0};
                
                ConvertBinToHex(param.server_uuid,16, server_uuid);
                server_uuid[32]='\0';
                
                sprintf((char *)g_rsp_str, "+SPBTCTRL:GATSREG=%d,%s,%d", param.server_handle, server_uuid, param.res_status);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);

            }
            break;

	case AT_BT_GATT_SERVER_REG_WRITE_RES:
    	    {   
                gatt_server_param_t  param =  *((gatt_server_param_t *)msg_body.body_ptr);
                uint8 server_uuid[33] = {0};
                
                ConvertBinToHex(param.server_uuid,16, server_uuid);
                server_uuid[32]='\0';
                
                sprintf((char *)g_rsp_str, "+SPBTCTRL:GATSREG=1,%d,%s,%d", param.server_handle, server_uuid, param.res_status);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);

            }
            break;

    	case AT_BT_GATT_SERVER_DEREG_WRITE_RES:
    	    {
                gatt_server_param_t  param =  *((gatt_server_param_t *)msg_body.body_ptr);
                uint8 server_uuid[33] = {0};
                
                ConvertBinToHex(param.server_uuid, 16, server_uuid);
                server_uuid[32]='\0';
                
                sprintf((char *)g_rsp_str, "+SPBTCTRL:GATSREG=0,%d,%s,%d", param.server_handle, server_uuid, param.res_status);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);

            }
            break;

    	case AT_BT_GATT_SERVER_SERVICE_READ_RES:
    	    {
                gatt_server_param_t  param =  *((gatt_server_param_t *)msg_body.body_ptr);
                uint8 service_uuid[33] = {0};                
                
                ConvertBinToHex(param.service_uuid, 16, service_uuid);
                service_uuid[32]='\0';
                
                sprintf((char *)g_rsp_str, "+SPBTCTRL:GATSS=%d,%s,%d,%d,%d", param.server_handle, service_uuid, param.service_handle, param.service_handle_num, param.res_status);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
            }
            break;
            
     	case AT_BT_GATT_SERVER_SERVICE_ADD_WRITE_RES:
    	    {
                gatt_server_param_t  param =  *((gatt_server_param_t *)msg_body.body_ptr);
                uint8 service_uuid[33] = {0};                
                
                ConvertBinToHex(param.service_uuid, 16, service_uuid);
                service_uuid[32]='\0';
                
                sprintf((char *)g_rsp_str, "+SPBTCTRL:GATSS=1,%d,%s,%d,%d,%d", param.server_handle, service_uuid, param.service_handle, param.service_handle_num,param.res_status);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
            }
            break;
            
     	case AT_BT_GATT_SERVER_SERVICE_DEL_WRITE_RES:
    	    {
                gatt_server_param_t  param =  *((gatt_server_param_t *)msg_body.body_ptr);
                uint8 service_uuid[33] = {0};                
                
                ConvertBinToHex(param.service_uuid, 16, service_uuid);
                service_uuid[32]='\0';
                
                sprintf((char *)g_rsp_str, "+SPBTCTRL:GATSS=0,%d,%s,%d,%d", param.server_handle, service_uuid, param.service_handle, param.res_status);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
            }
            break;  

    	case AT_BT_GATT_SERVER_CHAR_WRITE_RES:
    	    {
                gatt_server_param_t  param =  *((gatt_server_param_t *)msg_body.body_ptr);
                uint8 char_uuid[33] = {0};  
                   
                ConvertBinToHex(param.char_desc_uuid, 16, char_uuid);
                char_uuid[32]='\0';

                sprintf((char *)g_rsp_str, "+SPBTCTRL:GATSC=1,%d,%d,%s,%d,%d", param.server_handle, param.service_handle, char_uuid, param.char_desc_handle,param.res_status);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);

            }
            break;
            
     	case AT_BT_GATT_SERVER_DESC_WRITE_RES:
    	    {
                gatt_server_param_t  param =  *((gatt_server_param_t *)msg_body.body_ptr);
                uint8 char_uuid[33] = {0};  

                ConvertBinToHex(param.char_desc_uuid, 16, char_uuid);
                char_uuid[32]='\0';
                
                sprintf((char *)g_rsp_str, "+SPBTCTRL:GATSD=1,%d,%d,%s,%d,%d", param.server_handle, param.service_handle, char_uuid, param.char_desc_handle,param.res_status);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);

            }
            break;   
            
     	case AT_BT_GATT_SERVER_SERVICE_START_WRITE_RES:
    	    {
                gatt_server_param_t  param =  *((gatt_server_param_t *)msg_body.body_ptr);
                uint8 service_uuid[33] = {0};
                   
                ConvertBinToHex(param.service_uuid, 16, service_uuid);
                service_uuid[32]='\0';
                
                sprintf((char *)g_rsp_str, "+SPBTCTRL:GATSST=1,%d,%s,%d,%d", param.server_handle, service_uuid, param.service_handle, param.res_status);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);

            }
            break;     

  
    	case AT_BT_GATT_SERVER_SERVICE_STOP_WRITE_RES:
    	    {
                gatt_server_param_t  param =  *((gatt_server_param_t *)msg_body.body_ptr);
                uint8 service_uuid[33] = {0};
              
                ConvertBinToHex(param.service_uuid, 16, service_uuid);
                service_uuid[32]='\0';
                
                sprintf((char *)g_rsp_str, "+SPBTCTRL:GATSST=0,%d,%s,%d,%d", param.server_handle, service_uuid, param.service_handle, param.res_status);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);

            }
            break;

     	case AT_BT_GATT_SERVER_INDICAT_NOTIFY_WRITE_RES:
    	    {
                gatt_conn_param_t  param =  *((gatt_conn_param_t *)msg_body.body_ptr);
                
                sprintf((char *)g_rsp_str, "+SPBTCTRL:GATSIND=%d,%d,%d", param.att_handle, param.conn_id, param.res_status);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);

            }
            break;   

     	case AT_BT_GATT_SERVER_REQ_RECIEVE_IND:
    	    {
                gatt_conn_param_t  param =  *((gatt_conn_param_t *)msg_body.body_ptr);
                uint8 req_value[BTGATT_MAX_ATTR_LEN*2+1]={0};

                if(0 == param.read_write_flag) //read
                {
                    sprintf((char *)g_rsp_str, "+SPBTCTRL:GATRREQ=%d,%d,%d,%d,%d", param.conn_id, param.trans_id, param.att_handle, 
                    param.read_write_flag, param.res_status);

                }
                else//write
                {
                    ConvertBinToHex(param.req_value, param.req_len, req_value);
                    req_value[param.req_len*2+1]='\0';

                    sprintf((char *)g_rsp_str, "+SPBTCTRL:GATRREQ=%d,%d,%d,%d,(0x)%s,\"%s\",%d,%d", param.conn_id, param.trans_id, param.att_handle, 
                    param.read_write_flag, req_value, param.req_value, param.req_len, param.res_status);
                }
               
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);

            }
            break;     

    	case AT_BT_GATT_SERVER_RSP_WRITE_RES:
    	    {
                gatt_conn_param_t  param =  *((gatt_conn_param_t *)msg_body.body_ptr);
                
                sprintf((char *)g_rsp_str, "+SPBTCTRL:GATSRSP=%d,%d,%d",param.conn_id,param.att_handle, param.res_status);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);

            }
            break;
            
     	case AT_BT_GATT_SERVER_CONN_READ_RES:
    	    {
                gatt_conn_param_t  param =  *((gatt_conn_param_t *)msg_body.body_ptr);
                uint8 remote_address[13]={0};
                
                ConvertBinToHex(param.remote_address, 6, remote_address);
                remote_address[12]='\0';
                
                sprintf((char *)g_rsp_str, "+SPBTCTRL:GATSCON=%d,%s,%d",param.conn_id, remote_address, param.res_status);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);

            }
            break;   
            
     	case AT_BT_GATT_SERVER_DISCONN_WRITE_RES:
    	    {
                gatt_conn_param_t  param =  *((gatt_conn_param_t *)msg_body.body_ptr);
                
                sprintf((char *)g_rsp_str, "+SPBTCTRL:GATSDISC=0,%d,%d", param.conn_id, param.res_status);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str); 
            }
            break;   
#endif            
//BLE GATT related
        default:
            break;
            
    }

    if (msg_body.body_ptr != PNULL && msg_body.body_size >0)
	{
		SCI_FREE(msg_body.body_ptr);
		msg_body.body_ptr = PNULL;
	}

	return S_ATC_SUCCESS;
}
#endif

