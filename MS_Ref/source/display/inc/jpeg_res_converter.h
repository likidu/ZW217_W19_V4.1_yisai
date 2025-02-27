/******************************************************************************
 ** File Name:	jpeg_res_converter.h                                     	  							*
 ** Author:		Shan.He		                                          						*
 ** DATE:		03/17/2008                                              			 				*
 ** Copyright: 	2008 Spreatrum, Incoporated. All Rights Reserved.         				*
 ** Description:   Convert Spreadtrum RES jpeg to  standard jpeg                                                   								*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       								*
 ** -------------------------------------------------------------------------*
 ** DATE              NAME             DESCRIPTION                            						*
 **2009-04-16		  shan.he			Create
 ******************************************************************************/
#ifndef _RES_JPEG_CONVERTER_H_
#define _RES_JPEG_CONVERTER_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

PUBLIC uint32 SJPG_ConvertToStdJPG(uint8 *sjpg_buf, uint32 sjpg_size, 
										uint8 *std_jpg_buf, uint32 std_jpg_buf_size);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif // _JPEG_INTERFACE_H_
