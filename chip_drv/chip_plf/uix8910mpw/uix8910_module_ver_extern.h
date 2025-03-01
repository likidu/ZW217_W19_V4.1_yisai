/******************************************************************************
 ** File Name:    sc6531efm_module_ver.h                                            *
 ** Author:       Yuhua.Shi                                                 *
 ** DATE:         08/23/2011                                                  *
 ** Copyright:    2011 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 08/23/2011    Yuhua.Shi      Create.                                     *
 ** 04/11/2012    Yong.Li        Modified.                                    *
 ******************************************************************************/
#ifndef _UIX8910_MODULE_VER_EXTERN_H_
#define _UIX8910_MODULE_VER_EXTERN_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */
typedef volatile unsigned int REG32;

#include "sc6531efm_reg_ahb.h"
#include "sys_ctrl.h"
#include "rda2720m_global.h"
#include "wcn_sys_ctrl.h"
#include "cp_pwrctrl.h"
#include "cp_sysreg.h"

//Chip All module register miscs definitions
#include "../../chip_module/isp/v5/isp_reg_v5.h"
#include "../../chip_module/emc/v5/emc_reg_v5.h"
#include "../../chip_module/int/uix8910/int_reg_uix8910.h"
#include "../../chip_module/timer/uix8910mpw/timer_reg_uix8910.h"
#include "../../chip_module/lcdc/v5/lcdc_reg_v5.h"
#include "../../chip_module/pin/uix8910mpw/iomux_regasm_uix8910.h"
#include "../../chip_module/pin/uix8910mpw/iomux_reg_uix8910.h"
#include "../../chip_module/sys_timer/v5/sys_timer_reg_v5.h"
#include "../../chip_module/usb/uix8910/usb20_reg_uix8910.h"
#include "../../chip_module/sfc/v7/sfc_reg_v7.h"
#include "../../chip_module/uart/uix8910mpw/uart_reg_uix8910.h"
#include "../../chip_module/spinlock/uix8910/spinlock_phy_uix8910.h"
#include "../../chip_module/spinlock/uix8910/spinlock_reg_uix8910.h"

//#include "../../chip_module/watchdog/v5/watchdog_reg_v5.h"


/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/



/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
