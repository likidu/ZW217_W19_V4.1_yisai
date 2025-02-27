/******************************************************************************
 ** File Name:      uws6121e_i2c_cfg.h                                                 *
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

#ifndef _UIX8910_I2C_CFG_H_
#define _UIX8910_I2C_CFG_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */
#include "../../chip_module/i2c/i2c_phy.h"

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
#define DEBUG_IIC
#ifdef DEBUG_IIC
#define IIC_PRINT( _format_string )   SCI_TRACE_LOW _format_string
#else
#define IIC_PRINT( _format_string )
#endif

/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/
#ifndef I2C_DEV_MAX
#define I2C_DEV_MAX 8
#endif
#define I2C_BUS_MAX 3
#define I2C_ID_MAX 7

#define I2C_PORT_NUM 3


typedef struct
{
    uint32 phy_id;
    uint32 base_addr;
} I2C_BASE_INFO;

typedef struct
{
    uint32 phy_id;
    uint32 sda_pin;
    uint32 scl_pin;
} I2C_GPIO_INFO;

typedef struct
{
    uint32 logic_id;
    uint32 phy_id;
    uint32 port_id;
    I2C_PHY_FUN *phy_fun;
} I2C_PHY_CFG;
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
