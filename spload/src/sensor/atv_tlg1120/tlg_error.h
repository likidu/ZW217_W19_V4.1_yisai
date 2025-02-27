#ifndef _TLG_ERROR_H_
#define _TLG_ERROR_H_

/****************************************************************************/
/*!@file  error.h
 *
 * @brief defines errors for TLG
 *
 * < Detailed description of subsystem >
 *
 * @sa <optional links to relevant files>
 *
 ****************************************************************************/
/* Revision Information:
 *
 * $Revision: #1 $
 *
 * Copyright (c) 2006-2008 Telegent Systems, Inc.  All rights reserved.
 ****************************************************************************/

/*****************************************************************************
* INCLUDES
*****************************************************************************/

#include "tlg_global.h"           /* for uint32_t definition"  */

/*****************************************************************************
* CONSTANTS
*****************************************************************************/
/** Errors returnd by TLG API.                                              */
enum
{
         TLG_ERR_SUCCESS            = (0x00000000) /*!< No Errors                                  */
        ,TLG_ERR_FAIL               = (0x00000001) /*!< Generic Failure during processing          */
        ,TLG_ERR_PARAM              = (0x00000002) /*!< Bad parameter passed                       */
        ,TLG_ERR_NOT_SUPPORTED      = (0x00000003) /*!< Requested feature is not supported         */
        ,TLG_ERR_BAD_STATE          = (0x00000004) /*!< Called entity was in the wrong state       */
        ,TLG_ERR_NO_MEM             = (0x00000005) /*!< Memory allocation failed                   */
        ,TLG_ERR_BUSY               = (0x00000006) /*!< TLG2300 is busy with previous requests     */
        
        /* These are used by MPEGSW. */         
        ,TLG_ERR_BAD_MODE           = (0x00000007) /*!< Called entity was in the wrong TV mode     */
        ,TLG_ERR_PARAM_ID           = (0x00000009) /*!< Bad parameter id passed                    */
        
        ,TLG_ERR_MAX
};
/*****************************************************************************
* TYPES
*****************************************************************************/
typedef uint32_t tlg_errcode_t, *p_tlg_errcode_t;

/*****************************************************************************
* VARIABLES
*****************************************************************************/

/*****************************************************************************
* PRIVATE FUNCTIONS
*****************************************************************************/

/*****************************************************************************
* PUBLIC FUNCTIONS
*****************************************************************************/


#endif /* _FILENAME_H_ */
