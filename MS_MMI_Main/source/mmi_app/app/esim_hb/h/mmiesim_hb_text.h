#ifndef _MMIESIM_HB_TEXT_H_
#define _MMIESIM_HB_TEXT_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

#define MACRO_MDU_TYPE  MACRO_MDU_TEXT

#include "macro_mdu_def.h"
#ifdef CODE_MATCH_RES_CHECK
#include "esim_hb_mdu_def.h"
#endif


///////////////////////////////////////////////////////////////////////////////
// ������ԴID
///////////////////////////////////////////////////////////////////////////////

typedef enum 
{
    ESIM_TXT_NULL = ( MMI_MODULE_ESIM_HB << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "esim_hb_mdu_def.h"
#endif
    ESIM_TXT_MAX
} ESIM_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif
#endif

