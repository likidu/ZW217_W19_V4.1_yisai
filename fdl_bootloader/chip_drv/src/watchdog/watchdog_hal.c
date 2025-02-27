/******************************************************************************
 ** File Name:      watchdog_hal.c                                                 *
 ** Author:         Jimmy.Jia                                                 *
 ** DATE:           10/28/2004                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 serrial device. It manages create, send, receive          *
 **                 interface of serrial port.                                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/02/2002     Jimmy.Jia        Create.                                   *
**  01/03/2008     Yi.Qiu        For SC6600L                                  *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
//#include "fdl_bootloader_trc.h"
#include "sci_types.h"
#include "sc_reg.h"
#include "os_api.h"
#include "wdg_drvapi.h"
//#include "fiq_drvapi.h"
#include "watchdog_phy.h"
#include "ref_outport.h"

#define  WDG_TRACE  

/**---------------------------------------------------------------------------*
 **                      Function Declaration                                 *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    This function disable WDG timer
//  Author:         Jimmy.Jia
//  Note:           no rtc_clk to drive the counter
//
/*****************************************************************************/
PUBLIC void WDG_TimerStop
(
    void
)
{
    WDG_CONFIG_T config;
    int32        ret;

    //WDG_TRACE:"Watch Dog Trace: WDG_TimerStop"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,WATCHDOG_HAL_44_112_2_18_1_0_27_7,(uint8*)"");
    WDG_PHY_INT_CLR();

    config.state    = WDG_TIMER_STATE_STOP  ;
    config.mode     = WDG_TIMEOUT_MODE_RESET;
    config.val      = WDG_MAX_COUNT;

    ret = WDG_PHY_CONFIG (&config);

    {
        volatile uint32 tick1 = SCI_GetTickCount();
        volatile uint32 tick2 = SCI_GetTickCount();

        while ( (tick2 - tick1) < 1)
        {
            tick2 = SCI_GetTickCount();
        }
    }
    return;
}

/*****************************************************************************/
//  Description:    This function initialize and setup timer with given value
//  Author:         Jimmy.Jia
//  Note:           rtc_clk drive the counter
//                  load regiter is 32 bits wide so that don't check if param's valid
/*****************************************************************************/
PUBLIC void WDG_TimerStart
(
    uint32 init_val
)
{
    WDG_CONFIG_T config;
    int32        ret;

    //WDG_TRACE:"Watch Dog Trace: WDG_TimerStart"
   // SCI_TRACE_ID(TRACE_TOOL_CONVERT,WATCHDOG_HAL_77_112_2_18_1_0_27_8,(uint8*)"");

    config.state    = WDG_TIMER_STATE_START ;
    config.mode     = WDG_TIMEOUT_MODE_KEEP;
    config.val      = init_val;

    ret = WDG_PHY_CONFIG (&config);

    {
        volatile uint32 tick1 = SCI_GetTickCount();
        volatile uint32 tick2 = SCI_GetTickCount();

        while ( (tick2 - tick1) < 2)
        {
            tick2 = SCI_GetTickCount();
        }
    }
}

/*****************************************************************************/
//  Description:    This function handle WatchDog Interrupt
//  Author:         Binggo Zhou
//  Note:
/*****************************************************************************/
PUBLIC void WDG_TimerHandler (uint32 int_num)
{
    WDG_PHY_INT_CLR();
}

/*****************************************************************************/
//  Description:    This function init WatchDog
//  Author:         Binggo Zhou
//  Note:
/*****************************************************************************/
PUBLIC void WDG_TimerInit (void)
{
    WDG_CONFIG_T config;
    //uint32       status;
    int32        ret;

    WDG_TRACE ("Watch Dog Trace: WDG_TimerInit"); /*assert to do*/

    config.state    = WDG_TIMER_STATE_START ;
    config.mode     = WDG_TIMEOUT_MODE_RESET;
    config.val      = WDG_INIT_COUNT;

    ret = WDG_PHY_CONFIG (&config);
#if 0   //added by mingwei
    status = FIQ_RegHandler (TB_WDG_INT, WDG_TimerHandler);
    SCI_PASSERT ( (FIQ_SUCCESS == status || FIQ_ISR_ASSIGNED == status), /*assert to do*/
                  ("%s, %d, fiq regist status: %d", __MODULE__, __LINE__, status));
#endif
}


/*****************************************************************************/
//  Description:    This function Reset MCU
//  Author:         Younger.Yang
//  Note:
/*****************************************************************************/
PUBLIC void WDG_ResetMCU (void)
{
    WDG_CONFIG_T config;
    int32        ret;

    //WDG_TRACE:"Watch Dog Trace: WDG_ResetMCU"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,WATCHDOG_HAL_134_112_2_18_1_0_27_9,(uint8*)"");
    config.state    = WDG_TIMER_STATE_START ;
    config.mode     = WDG_TIMEOUT_MODE_RESET;
    config.val      = 0x50;

    ret = WDG_PHY_CONFIG (&config);
}

/*****************************************************************************/
//  Description:    This function Load WatchDog Timer Value
//  Author:         Binggo Zhou
//  Note:
/*****************************************************************************/
PUBLIC void WDG_TimerLoad (uint32 time_ms)
{
    WDG_CONFIG_T config;
    int32        ret;

    //WDG_TRACE:"Watch Dog Trace: WDG_TimerLoad %d"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,WATCHDOG_HAL_150_112_2_18_1_0_27_10,(uint8*)"d", time_ms);

    config.state    = WDG_TIMER_STATE_KEEP  ;
    config.mode     = WDG_TIMEOUT_MODE_KEEP ;
    config.val      = (time_ms * 1000) / 30;

    ret = WDG_PHY_CONFIG (&config);
}
