/******************************************************************************
 ** File Name:      mn_gps_type.h                                                 *
 ** Author:         CUIFANG DENG                                              *
 ** Date:           05/31/2017                                               *
 ** Copyright:      Copyright 2001-2017 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *                                                      
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 06/01/2017     cuifang.deng      Create.                                   *
 ******************************************************************************/
//#ifdef _MNGPS_SUPPORT_
#if !defined (_MN_GPS_TYPE_H)
#define _MN_GPS_TYPE_H

#include "sci_types.h"
#include "os_api.h"
#include "mn_type.h"

typedef enum
{
    MNGPS_NOT_RUN,   
    MNGPS_NOT_FIX,      
    MNGPS_2D_FIX,    
    MNGPS_3D_FIX
} MNGPS_STATUS_E;

typedef enum 
{
    MNGPS_COLD_MODE,      //冷启动
    MNGPS_HOT_MODE,       //热启动
    MNGPS_WARM_MODE    //温启动
} MNGPS_RESET_MODE_E;


typedef struct
{
    SIGNAL_VARS
    MN_DUAL_SYS_E dual_sys;
    BOOLEAN         is_ok;
} APP_MN_GPS_DOWNLOAD_CNF_T,
   APP_MN_GPS_INIT_CNF_T,
   APP_MN_GPS_START_CNF_T,
   APP_MN_GPS_STOP_CNF_T,
   APP_MN_GPS_RESET_CNF_T,
   APP_MN_GPS_OUTPUT_CNF_T,
   APP_MN_GPS_POWEROFF_CNF_T;

typedef struct
{
    SIGNAL_VARS
    MN_DUAL_SYS_E dual_sys;
} APP_MN_GPS_ERR_IND_T;

typedef struct
{
    SIGNAL_VARS
    MN_DUAL_SYS_E dual_sys;
    float       longitude;  //经度
    float       latitude;   //纬度
    uint32     ttff_time; //第一次定位所用时间，单位s
} APP_MN_GPS_FIX_IND_T;

typedef struct 
{
    SIGNAL_VARS
    MN_DUAL_SYS_E dual_sys;
    MNGPS_STATUS_E     fix_status;   //定位状态
} APP_MN_GPS_READ_STATUS_CNF_T;

typedef struct {
    uint16      year;
    uint16      month;
    uint16      day;
    uint16      hour;
    uint16      minute;
    uint16      second;
} MNGPS_TIMESTAMP_T;

typedef struct 
{
    float       longitude;  //经度
    float       latitude;   //纬度
    float       altitude;  //高度
    MNGPS_TIMESTAMP_T      utc_time;  // 世界时间
    uint32     ttff_time; //第一次定位所用时间，单位s
    uint8      satellite_num; //卫星数目
    float       speed;    //对地速度
    float       course;   //对地方向
} MNGPS_INFO_T;


typedef struct
{
    SIGNAL_VARS
    MN_DUAL_SYS_E dual_sys;
    MNGPS_INFO_T       gps_info;
} APP_MN_GPS_READ_INFO_CNF_T;


typedef struct 
{
    SIGNAL_VARS
    MN_DUAL_SYS_E dual_sys; 
    uint16     length;
    uint16     type; // BIT0=1--GGA,  BIT1=1--GLL,  BIT2=1--GSA,  BIT3=1--GSV,  BIT4=1--RMC, BIT5=1--VTG
    uint8       nmea_data[1024]; //NMEA格式数据
} APP_MN_GPS_OUTPUT_INFO_IND_T;

typedef struct 
{
    BOOLEAN     is_gps_power_on;   //gps是否打开
    BOOLEAN     is_fix_on;   //gps定位是否打开
    MNGPS_RESET_MODE_E     reset_mode; //重启模式
    uint16     ouput_type;   //输出类型
    uint32      time_interval;
} MNGPS_STATUS_INFO_T;

#endif /* end of _MN_TYPE_H */
//#endif
