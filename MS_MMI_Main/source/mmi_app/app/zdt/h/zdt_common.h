#ifndef __ZDT_COMMON_H__
#define __ZDT_COMMON_H__

#include "std_header.h"
#include "mn_type.h"
#include "mmi_nv.h"
#include "sci_types.h"
#include <stdio.h>
#include "nvitem.h"
#ifdef _RTOS
#include "sci_api.h" /*@tony.yao  replase sci_mem.h with sci_api.h*/
#else
#include "tasks_id.h"
#endif
#include "mn_api.h"
#include "mn_events.h"
#include "mn_type.h"
#include "nv_item_id.h"
#include "sio.h"
#include "sig_code.h"
#include "os_api.h"
#include "IN_message.h"
#include "socket_types.h"

#include "mmi_signal_ext.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "mmi_text.h"
#include "mmi_common.h"
#include "window_parse.h"
#include "guitext.h"
#include "guilcd.h"
#include "mmi_menutable.h"
#include "guilistbox.h"
#include "mmi_image.h"
#include "guiedit.h"
#include "mmipub.h"
#include "mmi_appmsg.h"
#include "mmiset_export.h"
#include "mmienvset_export.h"
#include "mmisms_export.h"
#include "mmipdp_export.h"
#include "mmiconnection_export.h"
#include "mmiphone_export.h"
#include "mmifmm_export.h"


#define ZDT_LOG            ZDT_Trace


#define  AEE_SUCCESS              0  // No error
#define  AEE_EFAILED              1  // General failure
#define  AEE_ENOMEMORY            2  // Insufficient RAM
#define  AEE_ECLASSNOTSUPPORT     3  // Specified class unsupported
#define  AEE_EVERSIONNOTSUPPORT   4  // Version not supported
#define  AEE_EALREADYLOADED       5  // Object already loaded
#define  AEE_EUNABLETOLOAD        6  // Unable to load object/applet
#define  AEE_EUNABLETOUNLOAD      7  // Unable to unload object/applet
#define  AEE_EALARMPENDING        8  // Alarm is pending
#define  AEE_EINVALIDTIME         9  // Invalid time
#define  AEE_EBADCLASS            10 // NULL class object
#define  AEE_EBADMETRIC           11 // Invalid metric specified
#define  AEE_EEXPIRED             12 // App/Component Expired
#define  AEE_EBADSTATE            13 // Invalid state
#define  AEE_EBADPARM             14 // Invalid parameter
#define  AEE_ESCHEMENOTSUPPORTED  15 // Invalid URL scheme
#define  AEE_EBADITEM             16 // Invalid item
#define  AEE_EINVALIDFORMAT       17 // Invalid format
#define  AEE_EINCOMPLETEITEM      18 // Incomplete item
#define  AEE_ENOPERSISTMEMORY     19 // Insufficient flash
#define  AEE_EUNSUPPORTED         20 // API is not supported
#define  AEE_EPRIVLEVEL           21 // Privileges are insufficient for this operation
#define  AEE_ERESOURCENOTFOUND    22
#define  AEE_EREENTERED           23
#define  AEE_EBADTASK             24
#define  AEE_EALLOCATED           25 // App/Module left memory allocated when released.
#define  AEE_EALREADY             26 // Operation is already in progress
#define  AEE_EADSAUTHBAD          27 // ADS mutual authorization failed
#define  AEE_ENEEDSERVICEPROG     28 // Need service programming
#define  AEE_EMEMPTR              29 // bad memory pointer
#define  AEE_EHEAP                30 // heap corruption
#define  AEE_EIDLE                31 // Context (system, interface, etc.) is idle
#define  AEE_EITEMBUSY            32 // Context (system, interface, etc.) is busy
#define  AEE_EBADSID              33 // Invalid subscriber ID
#define  AEE_ENOTYPE              34 // No type detected/found
#define  AEE_ENEEDMORE            35 // Need more data/info
#define  AEE_EADSCAPS             36 // ADS Capabilities do not match those required for phone
#define  AEE_EBADSHUTDOWN         37 // App failed to close properly
#define  AEE_EBUFFERTOOSMALL      38 // Destination buffer given is too small
#define  AEE_ENOSUCH              39 // No such name/port/socket/service exists or valid
#define  AEE_EACKPENDING          40 // ACK pending on application
#define  AEE_ENOTOWNER            41 // Not an owner authorized to perform the operation
#define  AEE_EINVALIDITEM         42 // Current item is invalid
#define  AEE_ENOTALLOWED          43 // Not allowed to perform the operation
#define  AEE_EBADHANDLE           44 // Invalid handle
#define  AEE_EOUTOFHANDLES        45 // Out of handles
#define  AEE_EINTERRUPTED         46 // Waitable call is interrupted
#define  AEE_ENOMORE              47 // No more items available -- reached end
#define  AEE_ECPUEXCEPTION        48 // A CPU exception occurred
#define  AEE_EREADONLY            49 // Cannot change read-only object or parameter

#ifdef ZDT_VIDEOCHAT_SUPPORT

#define MAX_VIDEO_USERS_COUNT 24

typedef enum
{
    VIDEO_CALL_INCOMING     = 1,
    VIDEO_CALL_OUT   = 2,
} VIDEO_CALL_TYPE_E;

typedef struct _VIDEO_USER_INFO_
{
    uint8 phone_user_id[64];
    uint16 family_relative[16];//family_name
    BOOLEAN has_avatar;//photo
    uint8 phone_num[24];
    BOOLEAN video_status;///1 can video call , 0 no
}VIDEO_USER_INFO;

typedef struct _VIDEO_CALL_INFO_
{
    uint8 mUseIdStr[32];
    uint8 mPassword[32];
    uint8 appId[64];
    uint8 video_id[64];
    uint16 family_relative[16];
    uint8 channel_name[32];
    VIDEO_CALL_TYPE_E call_type; // FALSE incoming TRUE out call 
    
}VIDEO_CALL_INFO;

//VIDEO_USER_INFO video_users[MAX_VIDEO_USERS_COUNT];

//MY_VIDEO_CALL_INFO mVideoCallInfo = {0};

PUBLIC BOOLEAN VideoChat_IsInCall();

#endif
// Type to get parameter
typedef enum
{
    ATP_GET_NUM_DEC = 0,  // Number - decimal
    ATP_GET_NUM_HEX,      // Number - hexadecimal
    ATP_GET_STR_WITH_DQ,  // "String"
    ATP_GET_STR_WO_DQ     // String
} T_ATP_GET_PARA_TYPE;

extern BOOLEAN ZDT_SIM_Exsit(void);
extern uint16  ZDT_DeleteStrHead(
                             uint8*  src_ptr, //[IN]  String的源指针
                             uint16  src_len, //[IN]  源String的长度，byte数
                             uint8*  out_str
                             );

extern uint8 ZDT_Nibble2HexChar(uint8 bNibble);
extern uint8 ZDT_HexChar2Nibble(uint8 bHexChar);

extern uint8 ZDT_GetNextPara(T_ATP_GET_PARA_TYPE tType, uint8** pCmd, uint16* pLen, uint32* pNum, uint8* pStr, uint16 wMaxStrLen);
extern uint16  ZDT_IsAllNumber(
                             uint8*  src_ptr, //[IN]  String的源指针
                             uint16  src_len //[IN]  源String的长度，byte数
                             );
extern uint16 ZDT_Str16_to_Int(
                           uint8*  src_ptr, //[IN]  String的源指针
                           uint16  src_len, //[IN]  源String的长度，byte数
                           uint8*  dest_ptr //[OUT] String的目标指针
                           );
extern uint16 ZDT_UCS_Str16_to_uint16(
                           uint8*  src_ptr, //[IN]  String的源指针
                           uint16  src_len, //[IN]  源String的长度，byte数
                           uint16*  dest_ptr, //[OUT] String的目标指针
                           uint16 dest_max_len
                           );
extern uint32  ZDT_Get_DayTimeSecond(void);
extern uint32  ZDT_Get_TimeSecond(void);

extern BOOLEAN  ZDT_DBFile_Read(char * path, char * filename, void *buffer, uint32 bytes_to_read, uint32 *bytes_read);
extern BOOLEAN  ZDT_DBFile_Write(char * path,char * filename, uint8 * data_ptr, uint32  data_len);
extern BOOLEAN ZDT_DBFile_Exist(char * path, char * filename);
extern uint32  ZDT_DBFile_GetSize(char * path, char * filename);
extern BOOLEAN  ZDT_DBFile_RemoveDir(char * path	);
extern BOOLEAN  ZDT_DBFile_RemoveFile(char * path, char * filename);

extern BOOLEAN ZDT_File_MemFull(BOOLEAN is_tf);
extern BOOLEAN ZDT_File_MemoryExsit(BOOLEAN is_tf);
extern BOOLEAN ZDT_File_Save(const char * file_name,uint8  * data_ptr, uint32 data_len,BOOLEAN is_add);
extern BOOLEAN ZDT_File_Remove(const char * file_name);
extern BOOLEAN ZDT_File_Read(const char * file_name,uint8  * data_ptr, uint32  data_len, uint32 * read_len);
extern BOOLEAN ZDT_File_Exsit(const char * file_name);
extern BOOLEAN ZDT_Dir_Exsit(const char * dir_path);
extern BOOLEAN ZDT_File_RemoveDirFile(const char * dir_path);
extern MMIFILE_HANDLE  ZDT_File_Open(const char * file_name,BOOLEAN is_read);
extern uint32  ZDT_File_Write(MMIFILE_HANDLE file_handle,uint8  * data_ptr, uint32 data_len);
extern void  ZDT_File_Close(MMIFILE_HANDLE file_handle);
extern uint32  ZDT_File_GetSize(const char * file_name);
extern BOOLEAN  ZDT_File_Rename(const char * file_name, const char * new_filename);

extern void timestampToDateTime(long time_second, SCI_DATE_T  *sys_date, SCI_TIME_T  *sys_time);//yangyu add
extern BOOLEAN MMIZDT_MakeAll(uint8 *tele_num, uint8 tele_len, wchar *name);

#endif
