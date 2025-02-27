/******************************************************************************
 ** File Name:    UMOD_handler.h                                                  *
 ** Author:       Jim.Lin                                                 *
 ** DATE:         2007-1-30                                                   *
 ** Copyright:    2007 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 2007-01-30     Jim.Lin     Create.                                     *
 ** 2007-02-06    Jim.Lin      modify the comment
 *****************************************************************************/

#ifndef _UMOD_HANDLER_H_
#define _UMOD_HANDLER_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
#include "UMOD_drv.h"
/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: usb global int handler
//	dependence:  none
//  Author:      weihua.wang
//	Note:        called in IRQ handler   
/*****************************************************************************/
PUBLIC void UMOD_usb_irq (uint32 param);

/*****************************************************************************/
//  Description: UMOD dma ep0 in handler
//	dependence:  none
//  Author:      weihua.wang
//	Note:        not used yet   
/*****************************************************************************/
PUBLIC void UMOD_dma_epin (uint32 param);

/*****************************************************************************/
//  Description: UMOD dma ep0 out handler
//	dependence:  none
//  Author:      weihua.wang
//	Note:        not used yet   
/*****************************************************************************/
PUBLIC void UMOD_dma_epout (uint32 param);

/*****************************************************************************/
//  Description: UMOD dma ep1 handler
//	dependence:  none
//  Author:      weihua.wang
//	Note:        not used yet   
/*****************************************************************************/
PUBLIC void UMOD_dma_ep1 (uint32 param);

/*****************************************************************************/
//  Description: UMOD dma ep2 handler
//	dependence:  none
//  Author:      weihua.wang
//	Note:        not used yet   
/*****************************************************************************/
PUBLIC void UMOD_dma_ep2 (uint32 param);

/*****************************************************************************/
//  Description: UMOD dma ep3 handler
//	dependence:  none
//  Author:      weihua.wang
//	Note:        not used yet   
/*****************************************************************************/
PUBLIC void UMOD_dma_ep3 (uint32 param);

/*****************************************************************************/
//  Description: UMOD dma ep4 handler
//	dependence:  none
//  Author:      weihua.wang
//	Note:        not used yet   
/*****************************************************************************/
PUBLIC void UMOD_dma_ep4 (uint32 param);


/*****************************************************************************/
//  Description: got char from usb vcom
//	dependence:  none
//  Author:      weihua.wang
//	Note:        called in assert mode   
/*****************************************************************************/
PUBLIC int32 UMOD_GetChar(uint32 port_num);
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 
