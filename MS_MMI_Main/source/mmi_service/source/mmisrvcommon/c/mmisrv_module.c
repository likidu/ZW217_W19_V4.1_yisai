/*****************************************************************************
** File Name:      mmisrv_module.c                                              *
** Author:                                                                   *
** Date:           08/13/2012                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/13/2012        bin.ji              Creat
******************************************************************************/
#define MMISRV_MODULE_C


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "mn_type.h"
#include "mmisrv_module.h"
#include "mmisrv_nv.h"
#include "mmisrv_nv_internal.h"
#include "mmisrvap_export.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : initial mmisrv modudle
//  Global resource dependence : 
//  Author:bin.ji
//  Modify:
//  Note: 所有新增模块的初始化都需要放在这里
/*****************************************************************************/
PUBLIC void MMISRV_InitMMIModule(void)
{
	MMISRV_RegModuleNvStart();

	MMISRV_RegSampleNv();

	MMISRV_RegModuleNvEnd();

    MMISRVAP_InitModule();
}