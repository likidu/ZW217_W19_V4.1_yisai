/*
 *	osdep.h
 *	Operating System and Hardware Abstraction Layer
 *	Release $Name: MATRIXSSL-3-4-2-COMM $
 */
/*
 *	Copyright (c) 2013 INSIDE Secure Corporation
 *	All Rights Reserved
 *
 *	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF INSIDE.
 *
 *	Please do not edit this file without first consulting INSIDE support.
 *	Unauthorized changes to this file are not supported by INSIDE.
 *
 *	The copyright notice above does not evidence any actual or intended
 *	publication of such source code.
 *
 *	This Module contains Proprietary Information of INSIDE and should be
 *	treated as Confidential.
 *
 *	The information in this file is provided for the exclusive use of the
 *	licensees of INSIDE. Such users have the right to use, modify,
 *	and incorporate this code into products for purposes authorized by the
 *	license agreement provided they include this notice and the associated
 *	copyright notice with any such product.
 *
 *	The information in this file is provided "AS IS" without warranty.
 */
/******************************************************************************/

#ifndef _h_PS_PLATFORM
#define _h_PS_PLATFORM
#include "sci_types.h"
#include "os_api.h"
/******************************************************************************/
/*
    APIs that must be implemented on every platform
*/
extern int		osdepTraceOpen(void);
extern void		osdepTraceClose(void);
extern int		osdepTimeOpen(void);
extern void		osdepTimeClose(void);
extern int		osdepEntropyOpen(void);
extern void		osdepEntropyClose(void);
#ifdef HALT_ON_PS_ERROR
	extern void  osdepBreak(void);
#endif
#ifndef min
	#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif /* min */

/******************************************************************************/
/*
    If the Makefile specifies that MatrixSSL does not currently have
    a layer for the given OS, or the port is to "bare metal" hardware,
    do basic defines here and include externally provided file "matrixos.h".
    In addition, if building for such a platform, a C file defining the above
    functions must be linked with the final executable.
*/
#ifdef PS_UNSUPPORTED_OS
    #define PSPUBLIC extern
    #include "matrixos.h"
#else
/******************************************************************************/
/*
    Supported Platforms below. The implementations of the apis are in
    platform specific directories, such as core/POSIX and core/ECOS

    POSIX define is used for Linux and Mac OS X
*/
#include <stdio.h>
	
#ifndef POSIX
	#if defined(LINUX) || defined(OSX) 
	#define POSIX
	#endif
#endif

#ifdef POSIX
	#include <stdint.h>
	typedef int32_t int32;
	typedef uint32_t uint32;
	typedef int16_t int16;
	typedef uint16_t uint16;
	#ifdef HAVE_NATIVE_INT64 
	typedef int64_t int64;
	typedef uint64_t uint64;
	#endif
#endif /* POSIX */
#ifdef WIN32
	#include <windows.h>
	typedef signed long int32;
	typedef unsigned long uint32;
	typedef signed short int16;
	typedef unsigned short uint16;
	#ifdef HAVE_NATIVE_INT64 
	typedef DWORD64	uint64;
	typedef INT64	int64;
	#endif
#endif /* WIN32 */

/******************************************************************************/
/*
	Hardware Abstraction Layer
*/
/* Hardware Abstraction Layer - define functions in HAL directory */
//#if defined(POSIX) || defined(WIN32) || defined(ECOS)
#define halOpen() 0
#define halClose()
#define halAlert()
//#else
//extern int32	halOpen(void);
//extern void		halAlert(void);
//extern void		halClose(void);
//#endif /* HAL defines */

/******************************************************************************/
/*
	OS-specific psTime_t types
	
	Make psTime_t an opaque time value.
*/
//#ifdef WIN32
//	typedef LARGE_INTEGER psTime_t;
//#else
	typedef SCI_TICK_TIME_T psTime_t;
// #endif
//#ifdef VXWORKS
//	typedef struct {
//		long sec;
//		long usec;
//	} psTime_t;
//#endif

/* #define USE_HIGHRES_TIME */
//#ifdef POSIX
//	#include <sys/time.h>
//	#include <time.h>
//	typedef struct timeval psTime_t;
//#endif

/******************************************************************************/
/*
	Export or import functions for Windows DLLs	
*/
#ifdef WIN32
	#ifndef _USRDLL
	//#define PSPUBLIC extern __declspec(dllimport)
    #define PSPUBLIC//change by li.li
	#else
	#define PSPUBLIC extern __declspec(dllexport)
	#endif
#else
	#define PSPUBLIC extern
#endif /* !WIN */

/******************************************************************************/
/*
	Raw trace and error
*/
PSPUBLIC void _psTrace(char *msg);
PSPUBLIC void _psTraceInt(char *msg, int32 val);
PSPUBLIC void _psTraceStr(char *msg, char *val);
PSPUBLIC void _psTracePtr(char *message, void *value);
PSPUBLIC void psTraceBytes(char *tag, unsigned char *p, int l);

PSPUBLIC void _psError(char *msg);
PSPUBLIC void _psErrorInt(char *msg, int32 val);
PSPUBLIC void _psErrorStr(char *msg, char *val);

/******************************************************************************/
/*
	Core trace
*/
#define _psTrace SCI_TRACE_LOW
#define _psTraceStr SCI_TRACE_LOW 
#define _psTraceInt SCI_TRACE_LOW
#define _psTracePtr SCI_TRACE_LOW

#ifndef USE_CORE_TRACE
#define psTraceCore(x) 
#define psTraceStrCore(x, y) 
#define psTraceIntCore(x, y)
#define psTracePtrCore(x, y)

#else
#define psTraceCore(x) _psTrace(x)
#define psTraceStrCore(x, y) _psTraceStr(x, y)
#define psTraceIntCore(x, y) _psTraceInt(x, y)
#define psTracePtrCore(x, y) _psTracePtr(x, y)
#endif /* USE_CORE_TRACE */


/******************************************************************************/
/*
	HALT_ON_PS_ERROR define at compile-time determines whether to halt on
	psAssert and psError calls
*/ 
#define psAssert(C)  if (C) ; else \
{halAlert();_psTraceStr("psAssert %s", __FILE__);_psTraceInt(":%d ", __LINE__);\
_psError(#C);} 

#define psError(a) \
 halAlert();_psTraceStr("psError %s", __FILE__);_psTraceInt(":%d ", __LINE__); \
 _psError(a);
 
#define psErrorStr(a,b) \
 halAlert();_psTraceStr("psError %s", __FILE__);_psTraceInt(":%d ", __LINE__); \
 _psErrorStr(a,b)
 
#define psErrorInt(a,b) \
 halAlert();_psTraceStr("psError %s", __FILE__);_psTraceInt(":%d ", __LINE__); \
 _psErrorInt(a,b)

/******************************************************************************/
/*
	OS specific file system apis
*/
#ifdef MATRIX_USE_FILE_SYSTEM
	#ifdef POSIX
	#include <sys/stat.h>
	#endif /* POSIX */
#endif /* MATRIX_USE_FILE_SYSTEM */

#ifdef USE_MULTITHREADING
/******************************************************************************/
/*
	Defines to make library multithreading safe
*/
extern int32 osdepMutexOpen(void);
extern int32 osdepMutexClose(void);

#ifdef WIN32
	typedef CRITICAL_SECTION psMutex_t;
#elif POSIX
	#include <pthread.h>
	#include <string.h>
	typedef pthread_mutex_t psMutex_t;
#elif VXWORKS
	#include "semLib.h"
	typedef SEM_ID	psMutex_t; 
#else
	#error psMutex_t must be defined
#endif /* OS specific mutex */
#endif /* USE_MULTITHREADING */

/******************************************************************************/

#endif /* !PS_UNSUPPORTED_OS */
#endif /* _h_PS_PLATFORM */

