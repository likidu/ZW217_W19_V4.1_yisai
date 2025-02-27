/******************************************************************************
 ** File Name:      isr_drvapi.h                                              *
 ** Author:         Xuepeng.Zhang                                              *
 ** DATE:           08/08/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the related driver of Interrupt      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
  ** 08/08/2010    Xuepeng.Zhang        Create                           *
******************************************************************************/
#ifndef _ISR_DRVAPI_H_
#define _ISR_DRVAPI_H_

#include "os_api.h"
#include "priority_kernel.h"
/*****************************************************************************/
//  Description:    Circular Buffer Function MACROS..
//  Is used to hold characters that arrive before the application is ready for them
//  Author:         steve.zhan
//  Note:  Use these three MACROS together..
/*****************************************************************************/
#define INPUT_BUFFER_INIT(VALUE_TYPE, _MAX_SIZE)\
    static VALUE_TYPE input_buffer[_MAX_SIZE]; \
    static VALUE_TYPE*  input_write_ptr  = &input_buffer[0];\
    static VALUE_TYPE*  input_read_ptr = &input_buffer[0];\
    static int32 MaxSize = _MAX_SIZE; \
    LOCAL void IsrWriteBuffer(VALUE_TYPE value) \
    { \
        VALUE_TYPE *save_ptr = input_write_ptr; \
        *input_write_ptr++ = value; \
        if (input_write_ptr > &input_buffer[MaxSize-1]) \
            input_write_ptr = &input_buffer[0]; \
        if (input_write_ptr == input_read_ptr) \
            input_write_ptr = save_ptr; \
    }\
    LOCAL BOOLEAN threadReadBuffer(VALUE_TYPE* value) \
    { \
        if (input_read_ptr != input_write_ptr) \
        { \
            *value = *input_read_ptr++; \
            if (input_read_ptr > &input_buffer[MaxSize-1]) \
                input_read_ptr = &input_buffer[0]; \
            return FALSE; \
        } \
        return TRUE; \
    }


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
typedef struct
{
    uint32   message;
    uint32   wparam;
    void     *lparam;
} TB_MSG;

typedef enum
{
    ISR_DONE = 0x0,
    CALL_HISR = 0xe05a05a5//magic number
} ISR_EXE_T;

// ISR function pointer prototype.
typedef ISR_EXE_T (* TB_ISR) (uint32);

// CallBack function pointer prototype.
typedef void (* TB_CALLBACK) (TB_MSG *);


// Return value.
#define TB_NULL                 0x0
#define TB_SUCCESS              0x00
#define TB_ISR_NULL             0x01
#define TB_NO_CALLBACK          0x02
#define TB_CALLBACK_ASSIGNED    0x03
#define TB_ISR_ASSIGNED         0x0E
#define TB_PTR_ERROR            0x0F
#define TB_INT_ERROR            0x10
#define TB_PARAM_ERROR          0x20

//hisr
#if 0
typedef enum
{
    CHIPDRV_HISR_PRIO_0 = PRI_HISR_PRIO_0,//The highest priority
    CHIPDRV_HISR_PRIO_1 = PRI_HISR_PRIO_1,
    CHIPDRV_HISR_PRIO_2 = PRI_HISR_PRIO_2
} CHIPDRV_HISR_PRI_E;
#else
#define   CHIPDRV_HISR_PRIO_0    PRI_HISR_PRIO_0
#define   CHIPDRV_HISR_PRIO_1    PRI_HISR_PRIO_1
#define   CHIPDRV_HISR_PRIO_2    PRI_HISR_PRIO_2

typedef uint32  CHIPDRV_HISR_PRI_E ;

#endif
typedef void hisr_func_t (uint32 count, void *data);

//


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    This function should be called at the startup of the
//                  application. It initializes TBIOS interrupt resources.
//                  This must be called before installing an IRQ handler.
//  Dependency:     irq_isr_array
//  Author:         Richard.Yang
//  Note:
/*****************************************************************************/
void ISR_Init (void);

/*****************************************************************************/
//  Description:    This function is called when an IRQ happens.
//  Dependency:     irq_isr_array
//  Author:         Richard.Yang
//  Note:
/*****************************************************************************/
//void ISR_Handler(void);
void ISR_HandlerAll (void);

/*****************************************************************************/
//  Description:    This function is called to register a new ISR.
//  Dependency:     irq_isr_array
//  Author:         Richard.Yang
//  Note:           If success, the function returns TB_SUCCESS;
//                  if isr_handler is NULL, returns TB_PTR_ERROR;
//                  if int_num is a invalid number, returns TB_INT_ERROR;
//                  if the isr_handler is installed before, returns
//                  TB_ISR_ASSIGNED.
/*****************************************************************************/
uint32 ISR_RegHandler (
    uint32 int_num,      // Interrupt number
    TB_ISR isr_handler   // Interrupt service routine pointer.
);

/*****************************************************************************/
//  Description:    This function is called to register a new ISR and Hisr.
//  Dependency:     irq_isr_array
//  Author:         steve.zhan
//  Note:           If success, the function returns TB_SUCCESS;
//                  if isr_handler is NULL, returns TB_PTR_ERROR;
//                  if int_num is a invalid number, returns TB_INT_ERROR;
//                  if the isr_handler is installed before, returns
//                  TB_ISR_ASSIGNED.
/*****************************************************************************/
uint32 ISR_RegHandler_Ex (
    uint32 logic_num,      // Interrupt number
    TB_ISR isr_handler,   // Interrupt service routine pointer.
    hisr_func_t *pHisrFunc,
    CHIPDRV_HISR_PRI_E hisrPri,
    void              *pHisrData
);

/*****************************************************************************/
//  Description:    This function is called to UnRegister an ISR.
//  Dependency:     irq_isr_array
//  Author:         Richard.Yang
//  Note:           If success, the function returns TB_SUCCESS;
//                  if int_num is a invalid number, returns TB_INT_ERROR;
//                  if the isr_handler is not installed before,
//                  returns TB_ISR_NULL.
/*****************************************************************************/
uint32 ISR_UnRegHandler (
    uint32 int_num   // Interrupt number
);

/*****************************************************************************/
//  Description:    This function is called to do works defined by upper layer.
//  Dependency:     irq_isr_array
//  Author:         Richard.Yang
//  Note:           If OK, Returns TB_SUCCESS;
//                  if int_num is an invalid number, returns TB_INT_ERROR;
//                  if msg is NULL, returns TB_PTR_ERROR ;
//                  if no callback function registered, returns TB_NO_CALLBACK
/*****************************************************************************/
uint32 ISR_Callback (
    uint32 int_num, // Interrupt number
    TB_MSG *msg     // Message pointer
);

/*****************************************************************************/
//  Description:    This function is called to register a callback function
//                  called from lower layer.
//  Dependency:     irq_isr_array
//  Author:         Richard.Yang
//  Note:           If OK, returns TB_SUCCESS;
//                  if int_num is an invalid number, returns TB_INT_ERROR;
//                  if handler pointer is NULL, returns TB_PTR_ERROR;
//                  if callback function is already registered,
//                  returns TB_CALLBACK_ASSIGNED .
/*****************************************************************************/
uint32 ISR_RegCallback (
    uint32      int_num,    // Interrupt number
    TB_CALLBACK handler     // Callback handler
);

/*****************************************************************************/
//  Description:    This function is called to unregister a callback function
//                  called from lower layer.
//  Dependency:     irq_isr_array
//  Author:         Xueliang.Wang
//  Note:           Returns TB_SUCCESS if OK;
//                  returns TB_INT_ERROR if int_num is an invalid number;
//                  returns TB_NO_CALLBACK if no callback function registered.
/*****************************************************************************/
uint32 ISR_UnRegCallback (
    uint32      int_num     // Interrupt number
);

/*****************************************************************************/
//  Description:    Get IRQ IE State
//
//  Dependency:
//  Author:         Yong.Li
//  Note:           ;
/*****************************************************************************/
uint32 INT_HAL_IRQGetIEState(void);

/**---------------------------------------------------------------------------*
 **                         Function Propertype                               *
 **---------------------------------------------------------------------------*/



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif // __cplusplus

#endif // End of _ISR_DRVAPI_H_

