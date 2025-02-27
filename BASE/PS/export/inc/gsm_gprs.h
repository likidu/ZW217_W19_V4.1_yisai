/******************************************************************************
 ** File Name:      gsm_gprs.h                                                *
 ** Author:         Tracy Zhang                                               *
 ** Date:           2010-09-28                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    It contains the structure and enum generated by SDL.
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 2010-09-28     Tracy Zhang      Create.                                   *
 ******************************************************************************/
#ifndef _GSM_GPRS_H
#define _GSM_GPRS_H

#ifdef _LESSEN_TRACE_SUPPORT
#define SCI_TRACE_SLIGHT
#else
#define SCI_TRACE_SLIGHT SCI_TRACE_LOW
#endif

#include "sci_stack_layer1_types.h"
#include "gsm_gprs_types.h"

#include "ps_mn_signals_def.h"
#include "ps_sim_signals_def.h"
#include "ps_layer1_signals_def.h"

#include "mac_ph_if.h"

#endif
