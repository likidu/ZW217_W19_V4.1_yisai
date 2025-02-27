/*******************************************************************************
 ** File Name:      uws6121e_timer_cfg.h                                        *
 ** Author:         Ji.Ding                                                    *
 ** Date:           08/20/2010                                                 *
 ** Copyright:      Copyright 2001-xxxx by Spreadtrum Communications,Inc.      *
 *                  All Rights Reserved.                                       *
 **                 This software is supplied under the terms of a license     *
 **                 agreement or non-disclosure agreement with Spreadtrum.     *
 **                 Passing on and copying of this document,and communication  *
 **                 of its contents is not permitted without prior written     *
 **                 authorization.                                             *
 ** Description:    This is the header file of defines signals of the sim      *
 **                 application layer                                          *
 *******************************************************************************

 *******************************************************************************
 **                        Edit History                                        *
 ** ---------------------------------------------------------------------------*
 ** Date           Name             Description                                *
 **----------------------------------------------------------------------------*
 ** 2010.11.1      Ji.Ding          Create                                     *
 ******************************************************************************/


#ifndef __UWS6121E_TIMER_CFG_H__
#define __UWS6121E_TIMER_CFG_H__

/*******************************************************************************
 **                        Dependencies                                        *
 ******************************************************************************/

/*******************************************************************************
 **                        Compiler Flag                                       *
 ******************************************************************************/

#ifdef   __cplusplus
extern   "C"
{
#endif

/*******************************************************************************
 **                        Mcaro Definitions                                   *
 ******************************************************************************/
/* this is depend on the SOC timer resource */
#define TIMER_PHY_NUMBER 4

/*******************************************************************************
 **                        Structures Definitions                              *
 ******************************************************************************/
/* the timer support mode */
typedef enum
{
    TIMER_MODE_FREE,
    TIMER_MODE_PERIOD
} TIMER_PHY_MODE_E;

/* the desciption of timer phy capability */
typedef struct
{
    TIMER_PHY_MODE_E mode; //current work mode

    uint32 clk_freq; //frequency of timer, it is timer's hardware capacity
    uint32 count_bit; //the counter register's bit, it is timer's hardware capacity

    uint32 max_count; //it is defined for the max_unit*clk
    uint32 max_unit; //it is defined for the one loop delay, it' bettter make max_count be integrate

    uint32 current_value; //current count
    uint32 running_status; //current status, running or stop
    uint32 irq_status; //current irq switch, enabled or disabled
} TIMER_PHY_STATUS_T;

/*******************************************************************************
 **                        data prototype                                      *
 ******************************************************************************/

/*******************************************************************************
 **                        Function prototype                                  *
 ******************************************************************************/
/******************************************************************************/
//  Description:    get timer number
//  Global resource dependence:
//  Author:         Ji.Ding
//  Note:
//  Return value:   number of phy timer
/******************************************************************************/
#define TIMER_CFG_get_number() (TIMER_PHY_NUMBER)

#define SYSTEM_CURRENT_CLOCK (0xFF)    /*adapt timer_hal.c TIMER_GetSystemCounterReg()*/

/******************************************************************************/
//  Description:    get appointed timer's capability
//  Global resource dependence:
//  Author:         Ji.Ding
//  Note:
//  Return value:   TIMER_PHY_STATUS_T *
/******************************************************************************/
PUBLIC TIMER_PHY_STATUS_T *TIMER_CFG_get_status (uint32 timer_id);

/*******************************************************************************
 **                        Compiler Flag                                       *
 ******************************************************************************/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif  //__SC6531EFM_TIMER_CFG_H__
// End

