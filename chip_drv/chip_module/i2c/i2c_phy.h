/******************************************************************************
 ** File Name:      I2C_phy.h                                                 *
 ** Author:         liuhao                                                   *
 ** DATE:           06/28/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the basic hw interfaces of I2C device.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/28/2010     liuhao     Create.                                   *
 ******************************************************************************/

#ifndef _I2C_PHY_H_
#define _I2C_PHY_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

#include "sci_types.h"

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
//I2C error code define
typedef enum
{
    ERR_I2C_NONE = 0,               // Success,no error
    ERR_I2C_ACK_TIMEOUT,            // I2C wait ACK timeout
    ERR_I2C_INT_TIMEOUT,            // I2C wait INT timeout
    ERR_I2C_BUSY_TIMEOUT,           // I2C wait BUSY timeout
    ERR_I2C_DEVICE_NOT_FOUND,       // I2C device not found
    ERR_I2C_NO_MUTEX        // No mutex created
} ERR_I2C_E;


typedef ERR_I2C_E (*_init) (uint32 phy_id, uint32 freq, uint32 port);
typedef ERR_I2C_E (*_start) (uint32 phy_id, uint8 addr, BOOLEAN rw, BOOLEAN ack_en);
typedef ERR_I2C_E (*_write) (uint32 phy_id, uint8 *pCmd, uint32 len, BOOLEAN ack_en, BOOLEAN no_stop);
typedef ERR_I2C_E (*_read) (uint32 phy_id, uint8 *pCmd, uint32 len, BOOLEAN ack_en);
typedef ERR_I2C_E (*_stop) (uint32 phy_id);
typedef ERR_I2C_E (*_sendack) (uint32 phy_id);
typedef ERR_I2C_E (*_getack) (uint32 phy_id);

typedef struct
{
    _init init;
    _start start;
    _write write;
    _read read;
    _stop stop;
    _sendack sendack;
    _getack getack;
} I2C_PHY_FUN;



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
