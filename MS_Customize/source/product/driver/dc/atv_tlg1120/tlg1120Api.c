/*****************************************************************************
 * FILENAME
 * tlg1120Api.c
 *
 *
 * ABSTRACT
 * This file contains the code for the TLG1120 Ultra Low Power NTSC/PAL
 * Tuner/Decoder Front End Application Programmer's Interface library.
 *
 * $Revision: #13 $
 *
 * (c) 2008 Telegent Systems
 *****************************************************************************/

#include "tlgInclude.h"

/*lint -save -e514 -e527 -e569 -e616 -e644 -e650 -e718 -e737 -e774 -e825 */

#if TLG_CHIP_TYPE == TLG_CHIP_TYPE_TLG1120_1

#define JUP_1_1_VHF1_REG_3C             0x88
#define JUP_1_1_VHF3_UHF_REG_3C         0xa8
#define JUP_1_2_REG_3C                  0xa8
#define JUP_2_1_REG_3C                  0xa8

#define BASE_VALUE_MHZ      ((uint32)1600)
#define BASE_VALUE_CVT      (MHZCVT(BASE_VALUE_MHZ))
#define BASE_VALUE_MHZ_3_1      ((uint32)1740)
#define BASE_VALUE_CVT_3_1       (MHZCVT(BASE_VALUE_MHZ_3_1))

#define NR_REF_DIVIDE               ((uint32)108)  
#define NR_REF_DIVIDE_VHF3          ((uint32) 81)  
#define JUPITER_NR_REF_DIVIDE_VHF3  ((uint32)108)  

struct {
    unsigned int vhf1_cal_done : 1;
    unsigned int vhf3_cal_done : 1;
    unsigned int uhf_cal_done : 1;
} peak_detector_flags;

static uint16 vhf1_pd_cal_value;
static uint16 vhf3_pd_cal_value;
static uint16 uhf_pd_cal_value;


#define TLG_MODE_HIGH_SNR (0)
#define TLG_MODE_LOW_SNR (1)
static int    snr_mode = TLG_MODE_HIGH_SNR;


#define TLG_REG5A_MODE_1 (1) 
#define TLG_REG5A_MODE_2 (2) 
#define TLG_REG5A_MODE_3 (3) 

static int g_reg5a_mode = TLG_REG5A_MODE_1;


#define TLG_SECAM_COLOR_MODE_1 (1)
#define TLG_SECAM_COLOR_MODE_2 (2)
#define TLG_SECAM_COLOR_MODE_3 (3)
#define TLG_SECAM_COLOR_MODE_4 (4)

static int g_secam_color_mode = TLG_SECAM_COLOR_MODE_1;


#define TLG_SECAM_FLICKER_MODE_1 (1)
#define TLG_SECAM_FLICKER_MODE_2 (2)

static int g_secam_flicker_mode = TLG_SECAM_FLICKER_MODE_2;


static tlg_sys_clk_source_t g_sys_clk_source = TLG_SYS_CLK_SOURCE_XTAL;

#ifdef DEBUG_FILE_LOG
#define dbg_printf(a)     tlg_dbg_printf a 
#define log_printf(a)        tlg_log_printf a
void log_printf((char * format, ...));
void dbg_printf((int num, char * format, ...));
#else 
#define dbg_printf(a)
#define log_printf(a)
#endif 


#define ICP_SCALE      ((uint32)10000)
#define LO_SLOPE       ((uint32)1185)
#define LO_OFFSET      ((uint32)346980)
#define MID_SLOPE      ((uint32)1174)
#define MID_OFFSET     ((uint32)443638)
#define HI_SLOPE       ((uint32)1246)
#define HI_OFFSET      ((uint32)625761)
#define TARGET         ((uint32)30)
#define LO_SLOPE_3_1        ((uint32)1104)
#define LO_OFFSET_3_1       ((uint32)302392)
#define MID_SLOPE_3_1       ((uint32)1128)
#define MID_OFFSET_3_1      ((uint32)420246)
#define HI_SLOPE_3_1        ((uint32)102176)
#define HI_OFFSET_3_1       ((uint32)485383)

#define HALF_SYS_CLOCK_CVT  (HZCVT(13500000))
#define HALF_SYS_CLOCK_HZ_DIV_32 (13500000 >> 5)
#define BW_8_VIDEO_CVT (HZCVT(2750000))
#define BW_8_AUDIO_CVT (HZCVT(3750000))
#define BW_8_CHROM_CVT (HZCVT(1680000))
#define BW_7_VIDEO_CVT (HZCVT(2250000))
#define BW_7_AUDIO_CVT (HZCVT(3250000))
#define BW_7_CHROM_CVT (HZCVT(2180000))
#define BW_6_VIDEO_CVT (HZCVT(1750000))
#define BW_6_AUDIO_CVT (HZCVT(2750000))
#define BW_6_CHROM_CVT (HZCVT(1830000))

#define CHANNEL_EXIST      1
#define CHANNEL_NOT_EXIST  0


#define FAST_TVSCAN_WAITFOR_LOCK_TIMEOUT1    5      
#define TLG_LUMAPWR_THRESHOLD                150
uint16 tlg_tv_ssi_threshold = TLG_LUMAPWR_THRESHOLD;


#define TLG_SSI_THRESHOLD           (20)
#define TLG_FMSSI_THRESHOLD         (20) // (10)

#define TLG_NOTCH_ADJ_1_2_BW        ( 4)

#define TLG_NOTCH_ADJ_3_4_BW        ( 4)

#define TLG_NOTCH_AUD_MAIN_BW       ( 4)

#define TLG_NOTCH_MAIN_BW_TLG1120_VHF1                            (6)

#define TLG_NOTCH_MAIN_BW_TLG1120           ( 8)

#define TLG_NOTCH_MAIN_BW_TLG1121           ( 12)

#define MAX_AUDIO_READ_NOTCH_NUM 10

#define FM_AUDIO_STEP_SIZE_DIV      (2)


#define CHANNEL_FREQ_OFFSET_HZ ((uint32)2750000)    

#define TLG_PAL_LUMA_SCALE (0x2c)
#define TLG_NTSC_LUMA_SCALE (0x28)
#define TLG_SECAM_LUMA_SCALE    (0x32)

#define TLGAPI_CHROMA_COEFF_2_LO_SNR_NTSC_PAL  (0x0100)
#define TLGAPI_CHROMA_COEFF_2_HI_SNR_NTSC_PAL  (0x1000)
#define TLGAPI_CHROMA_COEFF_2_DEF_SECAM     (0x0400)
#define TLGAPI_CHROMA_COEFF_3_DEF_NTSC_PAL  (0x8080)
#define TLGAPI_CHROMA_COEFF_3_DEF_SECAM     (0x00d0)


#define TLGAPI_HSYNC_3_DELAY1_DEFAULT   (0x8)
#define TLGAPI_HSYNC_3_DELAY1_LOW_SNR   (0xc)


#ifndef abs
#define abs(x) (((x) >= 0) ? (x) : (-(x)))
#endif 


uint32 current_freq_hz = 0;

#ifdef TLG_FM_NEW_INTERFACE
uint32 tlg_fm_step_size = 0;
#endif 

#ifdef TLG_NEW_CALLBACK
int __stdcall dummy_test_call_back(int testNum, void *param)
{
    return(TLG_ERR_SUCCESS);
}
fp_call_back   test_call_back_func    = dummy_test_call_back;
void          *test_call_back_param   = NULL;
#endif 


uint32 g_step_size_hz = 100000;   
uint16 g_lna_mode = TLG_LNA_VHF1;



uint8 g_tlg_rssi_onetime    = 0;



uint8 g_tlg_vsync_adjust_onetime = 0;


uint8 g_tlg_hybrid          = 0;


uint8 g_tlg_disable_mve     = 0;


static uint16 tlg_antenna = TLG_ANTENNA_DEF;


uint8 g_tlg_fmunlock_time_count = 0;
#define TLG_FM_UNLOCK_TIME_COUNT  (10)

const tlg_tone_notch_reg_t tone_notch_reg[NUM_TONE_NOTCH] =
{
    {
        REG_004A      ,      
        REG_004B      ,      
        REG_0120,        
        0,0
    },
    {
        REG_00B8      ,    
        REG_00B9      ,    
        REG_015D,      
        0,0
    },
    {
        REG_00BA      ,    
        REG_00BB      ,    
        REG_015F ,     
        0,0
    },
    {
        REG_00C0      ,    
        REG_00C1      ,    
        REG_016A,      
        REG_00D7_BIT15_12_SHIFT,      
        REG_00D7_BIT15_12_MASK
    },
    {
        REG_00C2      ,    
        REG_00C3      ,    
        REG_016C,      
        0,0
    },
    {
        REG_00D9      ,    
        REG_00DA      ,    
        REG_016E ,     
        0,0
    }
};

typedef struct _tlg_audio_mode_select_s
{                                                              
    uint16 fm_demph_bypass ;  
    uint16 fm_auto_mono    ;  
    uint16 fm_i2s_sel      ;  
    uint16 fm_mono_sel     ;  
    uint16 fm_stereo_sel   ;  
    uint16 sound_master_en ;  
    uint16 dsm_dither_en   ;  
    uint16 dsm_stereo      ;  
    uint16 dsm_en          ;  

} tlg_audio_mode_select_t;

enum
{
    AUDIO_DSM_MONO      ,
    AUDIO_DSM_STEREO    ,
    AUDIO_I2S_MONO      ,
    AUDIO_I2S_STEREO    ,
    AUDIO_I2S_AUTO      ,
    AUDIO_DSM_AUTO  
};

typedef struct tlg_audio_gain_s
{
    uint8 input_gain;
    uint8 output_gain;
} tlg_audio_gain_t;

#ifdef USE_ORG_DSM_TBL
tlg_audio_gain_t audio_gain_dsm_tbl[] = 
{
    {  
        0, 3
    },
    {  
        0, 2
    },
    {  
        0, 1
    },
    {  
        0, 0
    },
    {  
        1, 0
    },
    {  
        2, 0
    },
    {  
        3, 0
    },
    {  
        4, 0
    },
    {  
        5, 0
    },
    {  
        6, 0
    },
    {  
        7, 0
    }
};
#else 
tlg_audio_gain_t audio_gain_dsm_tbl[] = 
{
    {  
        0, 3
    },
    {  
        1, 3
    },
    {  
        2, 3
    },
    {  
        2, 2
    },
    {  
        3, 2
    },
    {  
        4, 2
    },
    {  
        4, 1
    },
    {  
        5, 1
    },
    {  
        5, 0
    },
    {  
        6, 0
    },
    {  
        7, 0
    }
};
#endif 
tlg_audio_gain_t audio_gain_i2s_tbl[] = 
{
    {  
        0, 0
    },
    {  
        0, 1
    },
    {  
        0, 2
    },
    {  
        0, 3
    },
    {  
        0, 4
    },
    {  
        1, 4
    },
    {  
        2, 4
    },
    {  
        3, 4
    },
    {  
        4, 4
    },
    {  
        4, 5
    },
    {  
        4, 6
    },
    {  
        4, 7
    },
    {  
        5, 7
    },
    {  
        6, 7
    },
    {  
        7, 7
    }
};


int agc_mode = TLG_AGC_NORMAL_MODE;

const tlg_reg_pair_t default_init_reg_1_2_AGC_in_VBI []=
{
    {0x0015, 0x0A10}, 

    
    {0x0016, 0x4012},

    {0x0017, 0x5110},

    {0x0018, 0x5112},

    
    {0x001F, 0x9246},

    {0x0026, 0x0000},
    {0x0027, 0x0000},

    
    {0x0031, 0x2560},
    {0x0032, 0x0000},

    {0xFFFF, 0xFFFF}
};


const tlg_reg_pair_t default_init_reg_1_2_AGC_Always []=
{
    
    {0x0015, 0x0A08},

    
    
    {0x0016, 0x400a},

    
    {0x0017, 0x6908},

    {0x0018, 0x510A},

    
    {0x001F, 0x9240},

    {0x0026, 0x000A},

    
    {0x0027, 0x0009},


    
    {0x0031, 0x056A},
    {0x0032, 0x0009},

    {0xFFFF, 0xFFFF}
};




const tlg_reg_pair_t *p_init_reg_1_2_AGC_in_VBI       = default_init_reg_1_2_AGC_in_VBI;
const tlg_reg_pair_t *p_init_reg_1_2_AGC_Always       = default_init_reg_1_2_AGC_Always;

const tlg_reg_pair_t *custom_init_reg_1_2_AGC_in_VBI  = default_init_reg_1_2_AGC_in_VBI;
const tlg_reg_pair_t *custom_init_reg_1_2_AGC_Always  = default_init_reg_1_2_AGC_Always;

const tlg_reg_pair_t *saved_init_reg_1_2_AGC_in_VBI  = default_init_reg_1_2_AGC_in_VBI;



typedef struct _tlg_uhf_tbl_s
{
    uint16  to_freq;
    uint8   cgstune;
    uint8   citune;
} tlg_uhf_tbl_t ;

const tlg_uhf_tbl_t uhf_tbl_1_1[] =
{
    {   
        480 , 
        0x0C, 
        0x12  
    },
    {
        495 , 
        0x0B, 
        0x11  
    },
    {
        510 , 
        0x0A, 
        0x10  
    },
    {
        525 , 
        0x08, 
        0x0F  
    },
    {
        540 , 
        0x07, 
        0x0E  
    },
    {
        555 , 
        0x06, 
        0x0D  
    },
    {
        570 , 
        0x05, 
        0x0C  
    },
    {
        585 , 
        0x04, 
        0x0B  
    },
    {
        600 , 
        0x03, 
        0x0A  
    },
    {
        615 , 
        0x03, 
        0x08  
    },
    {
        630 , 
        0x02, 
        0x07  
    },
    {
        645 , 
        0x01, 
        0x07  
    },
    {
        660 , 
        0x01, 
        0x06  
    },
    {
        675 , 
        0x00, 
        0x05  
    },
    {
        690 , 
        0x00, 
        0x04  
    },
    {
        705 , 
        0x00, 
        0x03  
    },
    {
        725 , 
        0x00, 
        0x02  
    },
    {
        750 , 
        0x00, 
        0x01  
    },
    {
        0 , 
        0x00, 
        0x00  
    }
};

#ifdef USE_OLD_UHF_TBL
const tlg_uhf_tbl_t uhf_tbl_1_2[] =
{
    {   
        475 , 
        0x0d, 
        0x13  
    },
    {
        490 , 
        0x0C, 
        0x12  
    },
    {
        505 , 
        0x0B, 
        0x11  
    },
    {
        520 , 
        0x0A, 
        0x10  
    },
    {
        535 , 
        0x09, 
        0x0F  
    },
    {
        550 , 
        0x08, 
        0x0E  
    },
    {
        565 , 
        0x07, 
        0x0D  
    },
    {
        580 , 
        0x06, 
        0x0C  
    },
    {
        595 , 
        0x05, 
        0x0B  
    },
    {
        610 , 
        0x04, 
        0x0A  
    },
    {
        625 , 
        0x04, 
        0x09  
    },
    {
        640 , 
        0x03, 
        0x08  
    },
    {
        655 , 
        0x02, 
        0x07  
    },
    {
        670 , 
        0x02, 
        0x06  
    },
    {
        685 , 
        0x01, 
        0x05  
    },
    {
        700 , 
        0x01, 
        0x04  
    },
    {
        715 , 
        0x00, 
        0x03  
    },
    {
        730 , 
        0x00, 
        0x02  
    },
    {
        750 , 
        0x00, 
        0x01  
    },
    {
        0 , 
        0x00, 
        0x00  
    }
};

#else 

const tlg_uhf_tbl_t uhf_tbl_1_2[] =
{
    {   
        465 , 
        0x09, 
        0x08  
    },
    {
        480 , 
        0x08, 
        0x06  
    },
    {
        500 , 
        0x07, 
        0x05  
    },
    {
        520 , 
        0x07, 
        0x04  
    },
    {
        570 , 
        0x07, 
        0x03  
    },
    {
        635 , 
        0x07, 
        0x02  
    },
    {
        650 , 
        0x06, 
        0x02  
    },
    {
        665 , 
        0x05, 
        0x02  
    },
    {
        680 , 
        0x05, 
        0x01  
    },
    {
        695 , 
        0x04, 
        0x01  
    },
    {
        710 , 
        0x04, 
        0x00  
    },
    {
        735 , 
        0x03, 
        0x00  
    },
    {
        760 , 
        0x02, 
        0x00  
    },
    {
        800 , 
        0x01, 
        0x00  
    },
    {
        0 , 
        0x00, 
        0x00  
    }
};
#endif 


const tlg_uhf_tbl_t uhf_tbl_2_1_se[] =
{
    {   
        585 , 
        0x1f, 
        0x00  
    },
    {   
        603 , 
        0x1f, 
        0x0f  
    },
    {   
        611 , 
        0x19, 
        0x19  
    },
    {   
        619 , 
        0x16, 
        0x19  
    },
    {   
        627 , 
        0x12, 
        0x14  
    },
    {   
        635 , 
        0x11, 
        0x10  
    },
    {   
        648 , 
        0x0e, 
        0x10  
    },
    {   
        658 , 
        0x0a, 
        0x0c  
    },
    {   
        667 , 
        0x09, 
        0x0a  
    },
    {   
        675 , 
        0x06, 
        0x0f  
    },
    {   
        689 , 
        0x05, 
        0x0a  
    },
    {   
        699 , 
        0x04, 
        0x0a  
    },
    {   
        707 , 
        0x04, 
        0x08  
    },
    {   
        720 , 
        0x03, 
        0x08  
    },
    {   
        731 , 
        0x03, 
        0x05  
    },
    {   
        740 , 
        0x03, 
        0x04  
    },
    {   
        748 , 
        0x03, 
        0x03  
    },
    {   
        756 , 
        0x02, 
        0x03  
    },
    {   
        764 , 
        0x02, 
        0x02  
    },
    {   
        791 , 
        0x02, 
        0x00  
    },
    {
        0 , 
        0x00, 
        0x00  
    }
};





const tlg_uhf_tbl_t uhf_tbl_3_1[] =
{
    {   
        465 , 
        0x05, 
        0x08  
    },
    {
        480 , 
        0x04, 
        0x06  
    },
    {
        500 , 
        0x03, 
        0x05  
    },
    {
        520 , 
        0x02, 
        0x04  
    },
    {
        570 , 
        0x02, 
        0x03  
    },
    {
        635 , 
        0x02, 
        0x02  
    },
    {
        650 , 
        0x02, 
        0x02  
    },
    {
        665 , 
        0x02, 
        0x02  
    },
    {
        680 , 
        0x02, 
        0x01  
    },
    {
        695 , 
        0x01, 
        0x01  
    },
    {
        710 , 
        0x01, 
        0x00  
    },
    {
        735 , 
        0x01, 
        0x00  
    },
    {
        760 , 
        0x01, 
        0x00  
    },
    {
        800 , 
        0x00, 
        0x00  
    },
    {
        0 , 
        0x00, 
        0x00  
    }
};

const tlg_uhf_tbl_t *uhf_tbl = uhf_tbl_1_2;

#ifdef USE_APOLLO_AUDIO_MODE_ONLY
const tlg_audio_mode_select_t audio_mode_select_old[] =
{
    {   
        0, 
        0, 
        1, 
        1, 
        0, 
        1  
        1, 
        1  
    },
    {   
        0, 
        0, 
        0, 
        1, 
        0, 
        1  
        1, 
        1  
    },
    {   
        0, 
        1, 
        1, 
        1, 
        1, 
        0  
        0, 
        0  
    },
    {   
        0, 
        1, 
        0, 
        1, 
        1, 
        0  
        0, 
        0  
    },
    {   
        1, 
        1, 
        0, 
        1, 
        1, 
        0  
        0, 
        0  
    },
    {   
        1, 
        0, 
        0, 
        1, 
        0, 
        1  
        1, 
        1  
    },
};
#else 
const tlg_audio_mode_select_t audio_mode_select_old[] =
{
    {   
        0, 
        0, 
        0, 
        1, 
        0, 
        0, 
        1, 
        1, 
        1  
    },
    {   
        0, 
        0, 
        0, 
        0, 
        1, 
        0, 
        1, 
        1, 
        1  
    },
    {   
        0, 
        0, 
        0, 
        1, 
        0, 
        1, 
        0, 
        0, 
        0  
    },
    {   
        0, 
        0, 
        1, 
        0, 
        1, 
        1, 
        0, 
        0, 
        0  
    },
    {   
        0, 
        1, 
        1, 
        0, 
        1, 
        1, 
        0, 
        0, 
        0  
    },
    {   
        0, 
        0, 
        0, 
        0, 
        1, 
        0, 
        1, 
        1, 
        1  
    },
};
#endif 



const tlg_audio_mode_select_t audio_mode_select_new[] =
{
    {   
        1, 
        0, 
        0, 
        1, 
        0, 
        0, 
        1, 
        1, 
        1  
    },
    {   
        1, 
        0, 
        0, 
        0, 
        0, 
        0, 
        1, 
        1, 
        1  
    },
    {   
        0, 
        0, 
        0, 
        1, 
        0, 
        1, 
        0, 
        0, 
        0  
    },
    {   
        0, 
        0, 
        0, 
        0, 
        0, 
        1, 
        0, 
        0, 
        0  
    },
    {   
        0, 
        1, 
        1, 
        0, 
        0, 
        1, 
        0, 
        0, 
        0  
    },
    {   
        1, 
        1, 
        0, 
        0, 
        0, 
        0, 
        1, 
        1, 
        1  
    },
};

const tlg_audio_mode_select_t *audio_mode_select = audio_mode_select_new;

extern int tv_mode;



static const uint16 tlg_sfa_ntsc_pal_thresh[] =
{ 600, 400, 280, 176, 90, 75, 50 };



static const uint16 tlg_sfa_secam_thresh[] = 
{ 400, 175, 175, 130, 65, 55, 45 };

static const uint8 tlg_sfa_val[] =
{ 0xd, 0xc, 0xc, 0xb, 0xa, 0x9, 0x8, 0x6 };

static const uint8 tlg_sfa_tgt[] =
{ 0x38, 0x30, 0x28, 0x20, 0x20, 0x20, 0x20, 0x20 };


#define TLG_SFA_NUM_THRESH   (sizeof(tlg_sfa_ntsc_pal_thresh)/sizeof(uint16))


#define TLG_SFA_HIGH_SNR     (TLG_SFA_NUM_THRESH - 1)

#define TLG_SFA_MIN_VAL      (6)
#define TLG_SFA_MAX_VAL      (0xd)
#define TLG_SFA_NO_VAL       (0)

static uint8 g_tlg_sfa_cur_val  = TLG_SFA_NO_VAL;
static uint16 g_tlg_sfa_cur_tgt = 0;


static uint8 g_tlg_cc_mve      = TLG_MVE_NONE;

static int   g_tlg_cc_mve_cnt  = 0;




#ifdef TLG_SUPPORT_FM_AVC
typedef enum {
    TLG_FM_AVC_LEVEL_0,
    TLG_FM_AVC_LEVEL_1,
    TLG_FM_AVC_LEVEL_2,
    TLG_FM_AVC_LEVEL_3,
    TLG_FM_AVC_LEVEL_4,
    TLG_FM_AVC_LEVEL_TOTAL
} TLG_FM_AVC_LEVEL;

int fm_SignalCounter[TLG_FM_AVC_LEVEL_TOTAL];
TLG_FM_AVC_LEVEL  fm_Level_cur;                                                     
uint16 fm_demph_bypass  = 0;  
uint16 fm_mu_sel = 0;   
uint16 fm_scale_sel = 5;
uint16 g_tlg_reg_f0_min = 0;
int TLG_AutoVolumeControlReset(uint32 base_addr);
int TLG_AutoVolumeControlCheck(uint32 base_addr);
int TLG_AutoVolumeControlInit(uint32 base_addr, uint16 mode);
#endif 

TLGDLL_API int TLG_Set_FM_AudioStereoModeHelper(uint32 base_addr, tlg_audio_mode_select_t *p_audio_mode_select) ;
TLGDLL_API int TLG_Get_FM_AudioStereoModeHelper(uint32 base_addr, tlg_audio_mode_select_t *p_audio_mode_select) ;


TLGDLL_API void TLG_Fix_LNA_AGC(uint32 base_addr, tlg_statep state, int *ms);

int TLG_SetAudioNotches(
        uint32 base_addr, 
        uint32 freq_hz          , 
        uint32 step_size_hz     , 
        uint32 is_read_RSSI_Mode);

int TLG_DisableAudioAuxNotches(
        uint32 base_addr 
        );


TLGDLL_API void TLG_SetSynthesizer(
        uint32 base_addr,
        uint32 center_freq_in_hz,  
        uint32 nr_ref_divide);

TLGDLL_API void TLG_VideoADC_Calibration(
        uint32                  base_addr               ,
        uint32                  phase
        );

TLGDLL_API void TLG_ReadRegADC(
        uint32                  base_addr,
        uint32                  reg_addr,
        uint16                  *p_reg_val
        );

TLGDLL_API void TLG_WriteRegADC(
        uint32                  base_addr,
        uint32                  reg_addr,
        uint16                  reg_val
        );

TLGDLL_API void TLG_FixAGC_1_1(uint32 base_addr, uint32 lna_mode);

TLGDLL_API void TLG_SetChnFreqASYNC(uint32 base_addr, uint32 center_freq_in_hz,
        bool isCallCali, uint16 trk_code,
        tlg_statep state, int *ms);

void TLG_TFCalibrationASYNC_VHF3( uint32 base_addr,
        uint32 freq_hz,
        uint16 lna_mode,
        uint16        seed,
        uint16        avg1,
        uint16        avg0,
        tlg_statep state,
        int *ms);
enum {
        TLG_NOTCH_SET_HZ  = 0x0001,
        TLG_NOTCH_SET_BW  = 0x0002,
        TLG_NOTCH_SET_BYP = 0x0004,
        TLG_NOTCH_SET_AVG = 0x0008,
        TLG_NOTCH_SET_EN  = 0x0010,
        TLG_NOTCH_SET_ALL = 0x001F
};

void  TLG_SetNotchAttrib(
        uint32 base_addr        , 
        int32 freq_hz          , 
        uint32 notch_num        ,
        uint16 bw               ,
        uint16 by_pass          ,
        uint16 hw_avg           ,
        uint16 tone_en,
        uint16 actions);

void  TLG_SetNotchHz(uint32 base_addr, int32 center_freq_in, uint32 notch_num);

static void TLG_TFCalibrationSaveReg(uint32 base_addr, uint16 * state);

static void TLG_TFCalibrationRestoreReg(uint32 base_addr, uint16 * state);

int TLG_GetAudioCarrierPwr(uint32 base_addr, int read_num);

TLGDLL_API int TLG_ForceLnaAgc_Gain(uint32 base_addr);
TLGDLL_API int TLG_ReleaseLnaAgc_Gain(uint32 base_addr);



#define TLG_MODE_SETUP_TV 1
#define TLG_MODE_SETUP_FM 0

void TLG_ModeSetup(uint32 base_addr, uint16 mode, uint16 force);



TLGDLL_API void TLG_AlternativeMode(
            uint32                  base_addr               ,
            uint32                  atop_bypass_mode_cseldel,
            uint32                  hera_mode_iqdel         ,
            TLG_ALTERNATIVE_MODE    alternative_mode);

TLGDLL_API uint32 tlg_cur_fm_ssi = 0;
tlg_avg_filter_t    tlg_agc1_filt, tlg_agc2_filt, tlg_lock_filt;

TLGDLL_API int TLG_FixAGC(uint32 base_addr);

static void TLG_PowerDownMixer(uint32 base_addr);
static void TLG_PowerUpMixer(uint32 base_addr);
#ifdef TLG_SUPPORT_AUTO_AGC_CHANGE
int TLG_CheckAudioPerf(uint32 base_addr);
#endif 

static void TLG_ChangeSNRmode(uint32 base_addr, int snrmode, uint16 vstd);
static void TLG_ChangeREG5Amode(uint32 base_addr, int reg5a_mode, uint16 vstd);
static void TLG_ResetSFA(void);
static void TLG_ChangeSecamColorMode(uint32 base_addr, int secam_color_mode);
static void TLG_ChangeSecamFlickerMode(uint32 base_addr, int secam_flicker_mode);



#ifdef DEBUG_FILE_LOG
#include <stdarg.h>
uint16    debug_num = 0;
void tlg_log_printf(char * format, ...)
{
    char tmp_str[128];
    va_list argp;
    FILE *fp;
    char fileName[128];

    va_start (argp, format);
    vsprintf (tmp_str, format, argp);
    va_end (argp);

    sprintf(fileName, "c:\\tlg_log_%01d.txt", (int) debug_num);
    fp = fopen(fileName, "a");
    if (fp != NULL)
    {
        fprintf(fp, "%s", tmp_str);
        fclose(fp);
    }
    return;
}
void tlg_dbg_printf(int num, char * format, ...)
{
    char tmp_str[128];
    va_list argp;
    FILE *fp;
    char fileName[128];

    va_start (argp, format);
    vsprintf (tmp_str, format, argp);
    va_end (argp);

    sprintf(fileName, "tlg_dbg_%01d.txt", (int) num);
    fp = fopen(fileName, "a");
    if (fp != NULL)
    {
        fprintf(fp, "%s", tmp_str);
        fclose(fp);
    }
    return;
}
#endif 

static int TLG_IsSECAMChnStandard(uint16 std)
{
    return (std & REG_0002_BIT4_MASK)
            == REG_0002_BIT4_MASK;
}

static int TLG_IsNTSCorPALMChnStandard(uint16 std)
{
    return (std == TLG_VSTD_NTSC_M) || (std == TLG_VSTD_PAL_M);
}


TLGDLL_API void TLG_Fix_LNA_AGC(uint32 base_addr, tlg_statep state, int *ms)
{
    TLG_CHIP_VERS_1_2_BEGIN
#if defined(TLG_CMP_CHIP_1120_1_2) || defined(TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)
    {
        uint16 reg115 = 0x0;
        uint16 reg31  = 0x0;
        uint16 reg31_bit_9_5 = 0;
        uint16 data;
        uint16 reg1b6;
        uint16 lna_mode = g_lna_mode;
        uint16 cal_done = 1;
        TLG_REG_LOG((b, "g_lna_mode = %d", g_lna_mode));

        switch(g_lna_mode)
        {
            case TLG_LNA_VHF1 :
                TLG_REG_LOG((b, "vhf1_cal_done = %d, TLG_LNA_VHF1 in: ", peak_detector_flags.vhf1_cal_done));
               if (peak_detector_flags.vhf1_cal_done)
                {
                    cal_done = 0;
                    TLG_ReadReg (base_addr,      REG_0025, &data);
                    TLGHAL_SET(data, vhf1_pd_cal_value,
                        REG_0025_BIT9_0_SHIFT,
                        REG_0025_BIT9_0_MASK);
                    TLG_WriteReg(base_addr,      REG_0025, data);                
                }
                break;
            case TLG_LNA_VHF3 :
                TLG_REG_LOG((b, "vhf3_cal_done = %d, TLG_LNA_VHF3 in: ", peak_detector_flags.vhf3_cal_done));
                if (peak_detector_flags.vhf3_cal_done)
                {
                    cal_done = 0;
                    TLG_ReadReg (base_addr,      REG_002B, &data);
                    TLGHAL_SET(data, vhf3_pd_cal_value,
                        REG_002B_BIT9_0_SHIFT,
                        REG_002B_BIT9_0_MASK);
                    TLG_WriteReg(base_addr,      REG_002B, data);                
                }
                break;            
            case TLG_LNA_UHF :
                TLG_REG_LOG((b, "uhf_cal_done = %d, TLG_LNA_UHF in:", peak_detector_flags.uhf_cal_done));

                if (peak_detector_flags.uhf_cal_done)
                {
                    cal_done = 0;
                    TLG_ReadReg (base_addr,      REG_002B, &data);
                    TLGHAL_SET(data, uhf_pd_cal_value,
                        REG_002B_BIT9_0_SHIFT,
                        REG_002B_BIT9_0_MASK);
                    TLG_WriteReg(base_addr,      REG_002B, data);                
                }            
                break;
            default:
                break;
        }

        if(cal_done)
        {
            switch (state[0])
            {
                case TLG_STATE0:
                    
                    TLG_ForceLnaAgc_Gain(base_addr);
                    
                    TLG_ReadReg(base_addr,      REG_01B6, &reg1b6);
                    TLG_REG_LOG((b, "\n\nFIX_BUG_1657: Reg0x%0X = 0x%04X\n", 
                                REG_01B6,
                                reg1b6));
                    TLGHAL_SET(reg1b6, 1,
                                REG_01B6_BIT3_SHIFT,
                                REG_01B6_BIT3_MASK);
                    TLG_WriteReg(base_addr,     REG_01B6, reg1b6);
                    
                    TLG_ReadReg (base_addr,      REG_0031, &reg31);
                    
                    TLGHAL_GET(reg31_bit_9_5, reg31,
                                REG_0031_BIT9_5_SHIFT,
                                REG_0031_BIT9_5_MASK);
                    TLGHAL_SET(reg31, 0,
                                REG_0031_BIT9_5_SHIFT,
                                REG_0031_BIT9_5_MASK);
                    TLG_WriteReg(base_addr,      REG_0031, reg31);
                    state[1] = reg31_bit_9_5;
                    state[0] = TLG_STATE1;
                    *ms = 25;
                    break;
                case TLG_STATE1:
                    reg31_bit_9_5 = state[1];
                    switch (lna_mode)
                    {
                        case TLG_LNA_VHF1:

                            TLG_ReadRegNS(base_addr,      REG_0115  , &reg115);
                            TLG_REG_LOG((b, "FIX_BUG_1657: Reg0x%0X = 0x%04X\n", 
                                        REG_0115,
                                        reg115));
                            
                            reg115 += 10;
                            TLG_REG_LOG((b, "TLG_Fix_LNA_AGC VHF1\n"));
                            TLG_ReadReg (base_addr,      REG_0025, &data);
                            TLGHAL_SET(data, reg115,
                                        REG_0025_BIT9_0_SHIFT,
                                        REG_0025_BIT9_0_MASK);
                            TLG_WriteReg(base_addr,      REG_0025, data);
                            peak_detector_flags.vhf1_cal_done = 1;
                            vhf1_pd_cal_value = reg115;
                            break;
                        case TLG_LNA_VHF3:
                        case TLG_LNA_UHF:
                        default:
                            TLG_REG_LOG((b, "TLG_Fix_LNA_AGC UHF/VHF3\n"));
                            TLG_ReadRegNS(base_addr,      REG_0115  , &reg115);
                            TLG_REG_LOG((b, "FIX_BUG_1657: Reg0x%0X = 0x%04X\n\n", 
                                        REG_0115,
                                        reg115));
                            
                            reg115 += 10;
                            TLG_ReadReg (base_addr,      REG_002B, &data);
                            TLGHAL_SET(data, reg115,
                                        REG_002B_BIT9_0_SHIFT,
                                        REG_002B_BIT9_0_MASK);
                            TLG_WriteReg(base_addr,      REG_002B, data);
                             
                             if (lna_mode == TLG_LNA_VHF3)
                            {
                                peak_detector_flags.vhf3_cal_done = 1;
                                vhf3_pd_cal_value = reg115;                    
                            }
                            else
                            {
                                
                                peak_detector_flags.uhf_cal_done = 1;
                                uhf_pd_cal_value = reg115;
                            }    
                            break;

                    }
                    
                    TLG_ReadReg (base_addr,      REG_0031, &reg31);
                    TLGHAL_SET(reg31, reg31_bit_9_5,
                                REG_0031_BIT9_5_SHIFT,
                                REG_0031_BIT9_5_MASK);
                    TLG_WriteReg(base_addr,      REG_0031, reg31);

                    TLG_ReadReg(base_addr,      REG_01B6, &reg1b6);
                    TLGHAL_SET(reg1b6, 0,
                                REG_01B6_BIT3_SHIFT,
                                REG_01B6_BIT3_MASK);
                    TLG_WriteReg(base_addr,     REG_01B6, reg1b6);
                    TLG_ReleaseLnaAgc_Gain(base_addr);   
                    *ms = 0;
                    state[0] = TLG_STATE_FINAL;
                    break;
                default: 
                        TLG_REG_LOG((b,"TLG_Fix_LNA_AGC: %d BADSTATE!\n", state[0]));
                        break;
            }
        }

        else
        {
            TLG_ForceLnaAgc_Gain(base_addr);    
            TLG_ReleaseLnaAgc_Gain(base_addr);    
            *ms = 0;
            state[0] = TLG_STATE_FINAL;
        }

    }
#endif 
    TLG_CHIP_VERS_1_1_ELSE
#ifdef TLG_CMP_CHIP_1120_1_1
#endif 
            TLG_CHIP_VERS_END
}


TLGDLL_API void TLG_FixAGC_1_1(uint32 base_addr, uint32 lna_mode)
{
uint16 reg190 = 0x0;

TLG_CHIP_VERS_1_2_BEGIN
#if defined(TLG_CMP_CHIP_1120_1_2) || defined(TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)
    if (lna_mode == TLG_LNA_VHF1)
    {
        reg190 = REG_0190_VALUE_1;
    }
#endif 
TLG_CHIP_VERS_1_1_ELSE

#ifdef TLG_CMP_CHIP_1120_1_1
    {
        uint16 reg194 = 0x1410;
        uint16 reg195 = 0xBC10;
        
        switch (lna_mode)
        {
            case TLG_LNA_VHF1:
                
                reg190 = REG_0190_VALUE_1;
                reg194 = 0x0000;
                reg195 = 0x0000;
                break;
            case TLG_LNA_UHF:
                
                reg190 = REG_0190_VALUE_2;
                reg194 = 0x1410;
                reg195 = 0xBC10;
                break;
            default:
                
                reg190 = REG_0190_VALUE_3;
                reg194 = 0x1410;
                reg195 = 0xBC10;
                break;

        }
        TLG_WriteReg(base_addr, REG_0194 , reg194);
        TLG_WriteReg(base_addr, REG_0195 , reg195);

    }
#endif 
    TLG_CHIP_VERS_END

            TLG_WriteReg(base_addr, REG_0190 , reg190);
}

TLGDLL_API int TLG_FixAGC(uint32 base_addr)
{
        
        TLG_CHIP_VERS_2_1_BEGIN
#ifdef TLG_CMP_CHIP_1120_2_1
#endif 
        TLG_CHIP_VERS_1_2_ELSE
#ifdef TLG_CMP_CHIP_1120_1_2
#endif 
        TLG_CHIP_VERS_1_1_ELSE

#ifdef TLG_CMP_CHIP_1120_1_1

#ifdef USE_FREEZE_AGC_LOOP
    uint16 lna2_gain, bb1_gain, bb2_gain;
    uint16 state1, state2;
    uint16 minmax = TLG_FREEZE_MIN;

    if (minmax == TLG_FREEZE_MAX)
    {
        lna2_gain = 0x7600;
        bb1_gain  = 0x8003;
        bb2_gain  = 0xdc00;
    } 
    else 
    {
        lna2_gain = 0x7600;
        bb1_gain  = 0x0000;
        bb2_gain  = 0xdc00;
    }
    TLG_ReadReg(base_addr, REG_0030, &state1);
    TLG_ReadReg(base_addr, REG_001B, &state2);

    
    TLG_WriteReg(base_addr,REG_0030, 0xC000);
    
    TLG_WriteReg(base_addr,REG_002E,  0x3B0D);
    
    TLG_WriteReg(base_addr,REG_002F, lna2_gain);
    
    TLG_WriteReg(base_addr,REG_001B, 0x8003);
    
    TLG_WriteReg(base_addr,REG_001D, bb1_gain);
    
    TLG_WriteReg(base_addr,REG_001C, bb2_gain);

    TLG_FixAGC_1_1(base_addr, TLG_LNA_VHF1);

    TLG_WriteReg(base_addr, REG_0030, state1);
    TLG_WriteReg(base_addr, REG_001B, state2);

#else 
    TLG_WriteReg(base_addr, REG_0030, 0xc000);
    TLG_WriteReg(base_addr, REG_0030, 0x0000);
#endif 

#endif 
    TLG_CHIP_VERS_END

    return(TLG_ERR_SUCCESS);

}



uint32 ceil_log2(uint32 a, bool ceil)
{
    uint32 pwr = ((uint32)1)<<31;
    int    l2  = 31;

    if (a == 0) return 0;

    
    while ((a & pwr) == 0)
    {
        l2--;
        pwr >>= 1;
}
    
    if (ceil)
            if (a & ~pwr) 
                l2++;
    return(l2);
}


static int TLG_AvgFiltInit(p_tlg_avg_filter_t filter)
{
    int i;

    TLG_NULLPTR_CHECK(filter);
    for (i = 0; i < TLG_AVG_FILTER_DEPTH; i++)
        filter->taps[i] = 0;
    filter->num_sample  = 0;
    filter->cur_tap     = 0;

    return TLG_ERR_SUCCESS;
}


static uint32 TLG_AvgFiltGetNext(p_tlg_avg_filter_t filter, uint32 *val,
                uint32 *divisor)
{
    int bucket_start, bucket_end;
    int    i;

    TLG_NULLPTR_CHECK(filter);
    TLG_NULLPTR_CHECK(divisor);
    TLG_NULLPTR_CHECK(val);

    *divisor = 1;           

    if (filter->num_sample == TLG_AVG_FILTER_DEPTH)
    {
        
        for (i = 0; i < TLG_AVG_FILTER_DEPTH; i++)
        {
            filter->taps[i] += *val;
        }
        *divisor = TLG_AVG_FILTER_DEPTH;
    }
    else 
    {
        
        filter->num_sample++;
        bucket_start = 1;       
        bucket_end   = 2;       
        while (bucket_start < TLG_AVG_FILTER_DEPTH*2)
        {
            if (filter->num_sample < bucket_start)
                for (i = bucket_start; i < bucket_start + filter->num_sample; i++)
                {
                    filter->taps[TLG_AVG_FILTER_IDX(i-1)] += *val;
                }
            else if (bucket_start <= filter->num_sample && bucket_start < bucket_end)
            {
                
                *divisor = bucket_start;
                for (i = filter->num_sample; i < bucket_end; i++)
                {
                    filter->taps[TLG_AVG_FILTER_IDX(i-1)] += *val;
                }
            }
            bucket_start   = bucket_end;
            bucket_end    *= 2;
        }
    }
    
    *val                          = filter->taps[filter->cur_tap];
    filter->taps[filter->cur_tap] = 0;
    filter->cur_tap               = TLG_AVG_FILTER_IDX(filter->cur_tap + 1);
    return TLG_ERR_SUCCESS;
}

#ifdef TLG_USE_SHADOW_REGISTERS
uint16 _TLG_shadow_reg[TLG_MAX_REG_ADDR+1];
uint8  _TLG_shadow_reg_use[TLG_MAX_REG_ADDR+1]={
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0};
#endif 







TLGDLL_API int TLG_GetVersion(uint32 base_addr, uint16 *major, uint16 *minor, uint16 *patch)
{
    TLG_NULLPTR_CHECK(major);
    TLG_NULLPTR_CHECK(minor);
    TLG_NULLPTR_CHECK(patch);

    *major = TLG_VERSION_MAJOR;
    *minor = TLG_VERSION_MINOR;
    *patch = TLG_VERSION_PATCH;

    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_GetBuildNumber(uint32 base_addr, uint16 *build)
{
    TLG_NULLPTR_CHECK(build);
    *build = TLG_VERSION_BUILD;
    return(TLG_ERR_SUCCESS);
}





typedef struct tlg_chip_id_pblk_s
{
    uint16                            id_reg[4];
    uint32                            chip_vers;
    const tlg_uhf_tbl_t *             uhf_tbl;
    const tlg_audio_mode_select_t *   aud_tbl;
    const char *                      desc;

} tlg_chip_id_pblk_t, *p_chip_id_pblk_t;



static const tlg_chip_id_pblk_t tlg_chip_id_tbl[] =
{
{{0, 0, 0, 0x4a55},   TLG1120_VERS_1_1, uhf_tbl_1_1, audio_mode_select_old, "TLG1120 1.1"},
{{1, 0, 0, 0x4a55},   TLG1120_VERS_1_2, uhf_tbl_1_2, audio_mode_select_old, "TLG1120 1.2"},
{{2, 0, 0, 0x4a55},   TLG1120_VERS_2_1, uhf_tbl_1_2, audio_mode_select_new, "TLG1120 2.1"},
{{3, 0, 0, 0x4a55},   TLG1116_VERS_1_1, uhf_tbl_1_2, audio_mode_select_new, "TLG1116 1.1"},
{{7, 0, 0, 0x4a55},   TLG1120_VERS_2_3, uhf_tbl_1_2, audio_mode_select_new, "TLG1120 2.3"},
{{0xf, 0, 0, 0x4a55}, TLG1116_VERS_1_2, uhf_tbl_1_2, audio_mode_select_new, "TLG1116 1.2"},
{{0, 0, 0, 0x4d49},   TLG1121_VERS_A_0, uhf_tbl_3_1, audio_mode_select_new, "TLG1121 A0"},
{{0, 1, 0, 0x4d49},   TLG1121_VERS_A_0, uhf_tbl_3_1, audio_mode_select_new, "TLG1121 A0 (se)"},
{{1, 0, 0, 0x4d49},   TLG1121_VERS_A_1, uhf_tbl_3_1, audio_mode_select_new, "TLG1121 A1"},
{{3, 0, 0, 0x4d49},   TLG1121_VERS_A_2, uhf_tbl_3_1, audio_mode_select_new, "TLG1121 A2"},
};


#define TLG_CHIP_ID_PBLK_MAX    (sizeof(tlg_chip_id_tbl)/sizeof(tlg_chip_id_pblk_t))


#define TLG_CHIP_ID_TBL_TLG1116_1_1     (&tlg_chip_id_tbl[3])
#define TLG_CHIP_ID_TBL_TLG1116_1_2     (&tlg_chip_id_tbl[5])



int                 g_tlg_chip_ver = TLG_NO_VERS;


p_chip_id_pblk_t    g_tlg_p_chip   = NULL;


static int TLG_SetChipTbl(const p_chip_id_pblk_t p_chip)
{
    if (p_chip == NULL) return TLG_ERR_PARAM;
 
    
    uhf_tbl             = p_chip->uhf_tbl;
    audio_mode_select   = p_chip->aud_tbl;
    g_tlg_chip_ver      = p_chip->chip_vers;
    TLG_REG_LOG((b, "Setting Chip Version to %s\n", p_chip->desc));

    
    if ((p_chip == TLG_CHIP_ID_TBL_TLG1116_1_1) ||
        (p_chip == TLG_CHIP_ID_TBL_TLG1116_1_2))
    {
        TLG_REG_LOG((b, "%s Detected\n", p_chip->desc)); 
        g_tlg_disable_mve = TRUE;
    }

    
    
    
    
    
    
    TLG_SetAntennaType(NULL, tlg_antenna);

    g_tlg_p_chip = p_chip;
    return TLG_ERR_SUCCESS;
}


static const p_chip_id_pblk_t TLG_FindChipTblByRegs(uint16 *chip_id)
{
    int i;

    if (!chip_id) return NULL;

    for (i=0; i < TLG_CHIP_ID_PBLK_MAX; i++)
        if (chip_id[0] == tlg_chip_id_tbl[i].id_reg[0] &&
            chip_id[1] == tlg_chip_id_tbl[i].id_reg[1] &&
            chip_id[2] == tlg_chip_id_tbl[i].id_reg[2] &&
            chip_id[3] == tlg_chip_id_tbl[i].id_reg[3])
            return((const p_chip_id_pblk_t)&tlg_chip_id_tbl[i]);

    return NULL;
}


static const p_chip_id_pblk_t TLG_FindChipTblByVers(uint32 chip_vers)
{
    int i;

    
    for (i=0; i < TLG_CHIP_ID_PBLK_MAX; i++)
        if (chip_vers == tlg_chip_id_tbl[i].chip_vers)
            return((const p_chip_id_pblk_t)&tlg_chip_id_tbl[i]);

    
    return (const p_chip_id_pblk_t)&tlg_chip_id_tbl[TLG_CHIP_ID_PBLK_MAX-1];
}
   

TLGDLL_API int TLG_SetChipVersion(uint32 chip_vers)
{

#ifdef TLG_SUPPORT_RUNTIME_CHECK
    return TLG_SetChipTbl(TLG_FindChipTblByVers(chip_vers));
#else
    return TLG_ERR_NOT_SUPPORTED;
#endif
}


TLGDLL_API int TLG_GetChipVersion(uint32 base_addr, uint32 *chip_vers)
{
    p_chip_id_pblk_t p_chip = NULL;

    TLG_NULLPTR_CHECK(chip_vers);

    if (g_tlg_chip_ver == TLG_NO_VERS)
    {
        
#ifdef TLG_SUPPORT_RUNTIME_CHECK
        uint16 chip_id[4];
        
        TLG_BurstRead(base_addr, REG_0143, 4, chip_id);
        TLG_REG_LOG((b, "TLG_GetChipVersion ID: 0x%04x, 0x%04x, 0x%04x, 0x%04x\n", 
                        chip_id[0], chip_id[1], chip_id[2], chip_id[3]));
        p_chip = TLG_FindChipTblByRegs(chip_id);
#endif 

        
        if (!p_chip)
            p_chip = TLG_FindChipTblByVers(TLG_CHIP_VERS);
                    
        
        TLG_SetChipTbl(p_chip);
    }

    *chip_vers = g_tlg_chip_ver;
    return TLG_ERR_SUCCESS;
}



TLGDLL_API void TLG_GetChipVersionString(uint32 base_addr, char *ver_str, int max_len)
{
    const char  *s;
    char        *d = ver_str;

    
    if (g_tlg_p_chip == NULL)
        s = "Unknown Chip";
    else
        s = g_tlg_p_chip->desc;

    
    while (*s && max_len > 0)
    {
        *d++ = *s++;
        max_len--;
    }
    if (max_len == 0)
        *d = '\0';
    else
        *d = *s;
}
            


TLGDLL_API int TLG_SoftResetASYNC(uint32 base_addr, tlg_statep state, int *ms)
{
    uint16 data;

    switch (state[0])
    {
        case TLG_STATE0:
        default:
            
            TLG_WriteReg(base_addr, REG_0001, REG_0001_BIT0_MASK);
            TLG_WriteReg(base_addr, REG_0001, 0x0000);

            TLG_CHIP_VERS_2_1_BEGIN
#if defined(TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)
                
                TLG_ReadReg(base_addr, REG_00B5, &data);
            TLGHAL_SET(data, 1, 
                        REG_00B5_BIT0_SHIFT, 
                        REG_00B5_BIT0_MASK);
            TLG_WriteReg(base_addr, REG_00B5, data);
            TLGHAL_SET(data, 0, 
                        REG_00B5_BIT0_SHIFT, 
                        REG_00B5_BIT0_MASK);
            TLG_WriteReg(base_addr, REG_00B5, data);
#endif 
            TLG_CHIP_VERS_END
            *ms      = 0;
            state[0] = TLG_STATE_FINAL;
            break;

    }

    return TLG_ERR_SUCCESS;
}


TLGDLL_API int TLG_SoftReset(uint32 base_addr)
{
        tlg_statep  state;
        int         ms    = 0;

#ifndef TLG_EMBEDDED_TIMERS
        return(TLG_ERR_NOT_SUPPORTED);
#else
        state[0] = TLG_STATE0;
        do 
        {
            TLG_SoftResetASYNC(base_addr, state, &ms);
            if (ms)
            TLG_Delay((uint16)ms);
        } while (state[0] != TLG_STATE_FINAL);
        return(TLG_ERR_SUCCESS);
#endif 
}

TLGDLL_API int TLG_InitASYNC(uint32 base_addr, tlg_statep state, int *ms)
{
    uint16 data;
    TLG_NULLPTR_CHECK(state);
    TLG_NULLPTR_CHECK(ms);

    switch (state[0])
    {
        case TLG_STATE0:
            {
                uint32 version;
                TLG_GetChipVersion(base_addr, &version);
            }
            TLG_REG_LOG((b, "TLG_InitASYNC start\n"));

    peak_detector_flags.vhf1_cal_done = 0;
    peak_detector_flags.vhf3_cal_done = 0;
    peak_detector_flags.uhf_cal_done = 0;
                

            TLG1121_VERS_A_0_BEGIN
            #if defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1121_A_2)
            TLG_WriteReg(base_addr, REG_0000, REG_0000_VALUE_2);
            #endif 
            TLG_CHIP_VERS_ELSE
            #if defined(TLG_CMP_CHIP_1120_2_1) ||defined(TLG_CMP_CHIP_1120_1_2) ||defined(TLG_CMP_CHIP_1120_1_1) ||defined(TLG_CMP_CHIP_1120_2_3)
            TLG_WriteReg(base_addr, REG_0000, REG_0000_VALUE_1);
            #endif
           TLG_CHIP_VERS_END     
#ifdef OPTIMIZE_AUDIO_NOTCH
            {
                TLG_SetNotchAttrib(
                            base_addr           , 
                            2750000            , 
                            AUDIO_MAIN_NOTCH    ,
                            TLG_NOTCH_AUD_MAIN_BW      , 
                            1                   , 
                            7                                               , 
                            1                                                 
                            );
                TLG_SetNotchHz(base_addr, 2750000, AUDIO_MAIN_NOTCH);
                
                TLG_SetBits(base_addr, 
                            REG_004A, 
                            7                   , 
                            REG_004A_BIT11_8_SHIFT   ,
                            REG_004A_BIT11_8_MASK);
            }
#endif 
#ifdef OPTIMIZE_VHF3
            
            TLG_WriteReg(base_addr, REG_0185, 0x0104);

            
            TLG_SetBits(base_addr, REG_0183, 0,
                        REG_0183_BIT8_SHIFT,
                        REG_0183_BIT8_MASK);

            
            TLG_SetBits(base_addr, REG_0187, 0,
                        REG_0187_BIT15_SHIFT,
                        REG_0187_BIT15_MASK);
            
            TLG_WriteReg(base_addr, REG_003A, 0x0);
            TLG_WriteReg(base_addr, REG_003B, 0x0);
            TLG_ReadReg (base_addr, REG_0037, &data );
            TLGHAL_SET(data, avg1, 7, BIT7);
            TLGHAL_SET(data, avg0, 6, BIT6);
            TLG_WriteReg(base_addr, REG_0037, data);

#endif 
            
            
            
            TLG_SetBits(base_addr, REG_01A8, 0x02,
                        REG_01A8_BIT11_9_SHIFT,
                        REG_01A8_BIT11_9_MASK);

            TLG_WriteReg(base_addr, REG_0180, 0xfffe);
            
            TLG_WriteReg(base_addr, REG_0181,0x0E0F);
            state[0] = TLG_STATE1;
            *ms    = 5;
            break;

        case TLG_STATE1:
            
            TLG_WriteReg(base_addr, REG_0180,0xF81E);
            state[0] = TLG_STATE2;
            *ms    = 5;
            break;

        case TLG_STATE2:
            

            TLG_REG_LOG((b, "TLG_VideoADC_Calibration start\n"));
            TLG_VideoADC_Calibration(
                        base_addr               ,
                        0);
            state[0] = TLG_STATE3;
            *ms    = 10;
            break;

        case TLG_STATE3:
            TLG_VideoADC_Calibration(
                        base_addr               ,
                        1);
            state[0] = TLG_STATE4;
            *ms    = 100;
            break;

        case TLG_STATE4:
            TLG_VideoADC_Calibration(
                        base_addr               ,
                        2);
            TLG_REG_LOG((b, "TLG_VideoADC_Calibration end\n"));
            

            TLG_SetChnBandwidth(base_addr, TLG_BW_6);

            
            

            

            

            
            TLG_ReadReg(base_addr, REG_01A7, &data);
            TLGHAL_CLEAR(data, REG_01A7_BIT8_MASK);
            TLG_WriteReg(base_addr, REG_01A7, data);

            
            TLGHAL_SET(data, TLG_ON,
                        REG_01A7_BIT8_SHIFT,
                        REG_01A7_BIT8_MASK);
            TLG_WriteReg(base_addr, REG_01A7, data);
            

            
            TLG_SetBits(base_addr, REG_018F, 1,
                        REG_018F_BIT5_3_SHIFT,
                        REG_018F_BIT5_3_MASK);
            
            TLG_WriteReg(base_addr, REG_0180, 0xC81E);
            

            

            
            TLG_WriteReg(base_addr, REG_018D, 0x8081);
            
            TLG_SetBits(base_addr, REG_018A, 1,
                        REG_018A_BIT9_SHIFT,
                        REG_018A_BIT9_MASK);

            TLG_WriteReg(base_addr, REG_0186, REG_0186_BIT3_0_MASK);
#if 1
            
            TLG_SetBits(base_addr, REG_0189, 0x7,
                        REG_0189_BIT3_0_SHIFT, 
                        REG_0189_BIT3_0_MASK);
#else
                TLG_WriteReg(base_addr, REG_0189, REG_0189_BIT3_0_MASK);
#endif

            TLG_WriteReg(base_addr, REG_0036, 0x30aa); 
            TLG_WriteReg(base_addr, REG_0037, 0xeed0);

            TLG_WriteReg(base_addr, REG_0039, REG_0039_VALUE_2);

            

            

            

            
            TLG_ReadReg(base_addr,      REG_0033, &data);
            TLGHAL_SET(data, 
                        1, 
                        REG_0033_BIT12_SHIFT, 
                        REG_0033_BIT12_MASK);
            TLGHAL_SET(data,            
                        0xd,
                        REG_0033_BIT11_8_SHIFT,
                        REG_0033_BIT11_8_MASK);
            TLG_WriteReg(base_addr,     REG_0033, data);
            
            TLG_WriteReg(base_addr, REG_0038,0x0000);

            state[0] = TLG_STATE5;
            *ms    = 100;
            break;

        case TLG_STATE5:
            
            TLG_ReadReg(base_addr,      REG_0033, &data);
            TLGHAL_SET(data, 
                        0, 
                        REG_0033_BIT12_SHIFT, 
                        REG_0033_BIT12_MASK);
            TLGHAL_SET(data,            
                        0xd,
                        REG_0033_BIT11_8_SHIFT,
                        REG_0033_BIT11_8_MASK);
            TLG_WriteReg(base_addr,     REG_0033, data);
            
            TLG_WriteReg(base_addr, REG_018D, 0x0081);

            
            TLG_SetBits(base_addr, REG_018A, 0,
                        REG_018A_BIT9_SHIFT,
                        REG_018A_BIT9_MASK);

            TLG_SetBits(base_addr, REG_0198, 
                        TLG_OFF,
                        REG_0198_BIT8_SHIFT,
                        REG_0198_BIT8_MASK);

            TLG_ReadReg(base_addr,      REG_008C, &data);
            TLGHAL_SET(data, 1,         
                        REG_008C_BIT3_SHIFT,
                        REG_008C_BIT3_MASK);
            TLGHAL_CLEAR(data,          REG_008C_BIT2_0_MASK);
            TLG_WriteReg(base_addr,     REG_008C, data);

            TLG_ReadReg(base_addr,      REG_008D, &data);
            TLGHAL_SET(data, 1,         REG_008D_BIT9_SHIFT,
                        REG_008D_BIT9_MASK);
            TLGHAL_SET(data, 4,         REG_008D_BIT7_4_SHIFT,
                        REG_008D_BIT7_4_MASK);
            TLGHAL_SET(data, 4,         REG_008D_BIT3_0_SHIFT,
                        REG_008D_BIT3_0_MASK);
            TLG_WriteReg(base_addr,     REG_008D, data);
            state[0] = TLG_STATE6;
            *ms    = 5;
            break;

    case TLG_STATE6:
            

            TLG_ReadReg(base_addr,      REG_018F, &data);
            TLGHAL_SET(data, 2,         REG_018F_BIT9_8_SHIFT,
                        REG_018F_BIT9_8_MASK);
            TLG_WriteReg(base_addr,     REG_018F, data);
            
            TLG_ReadReg(base_addr,      REG_002D, &data);
            TLGHAL_SET(data, 2,         
                        REG_002D_BIT9_0_SHIFT,
                        REG_002D_BIT9_0_MASK);
            TLG_WriteReg(base_addr,     REG_002D, data);
       
            if (g_tlg_disable_mve)
            {
                TLG_SetMobileVidEnhancement(base_addr, TLG_MVE_MANUAL, 0);
            }

            TLG_SoftReset(base_addr);
            state[0] = TLG_STATE_FINAL;
            *ms    = 0;
            break;

        default:
            break;
    }
    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_Init(uint32 base_addr)
{
    tlg_statep  state;
    int         ms    = 0;

#ifndef TLG_EMBEDDED_TIMERS
    return(TLG_ERR_NOT_SUPPORTED);
#else
    state[0] = TLG_STATE0;
    do
    {
        TLG_InitASYNC(base_addr, state, &ms);
        if (ms)
        TLG_Delay((uint16)ms);
    } while (state[0] != TLG_STATE_FINAL);
    CHECK_POINT(TLG_INIT_TEST);

    TLG_REG_LOG((b, "TLG_Init DONE"));
    return(TLG_ERR_SUCCESS);
#endif 
}



TLGDLL_API int TLG_ReInitFromDeepSleep(uint32 base_addr)
{
    tlg_statep  state;
    int         ms    = 0;

#ifndef TLG_EMBEDDED_TIMERS
    return(TLG_ERR_NOT_SUPPORTED);
#else
    state[0] = TLG_STATE1;
    do
    {
        TLG_InitASYNC(base_addr, state, &ms);
        if (ms)
        TLG_Delay((uint16)ms);
    } while (state[0] != TLG_STATE_FINAL);
    return(TLG_ERR_SUCCESS);
#endif 
}


TLGDLL_API int TLG_GetYUVScale(uint32 base_addr, uint16 *y, uint16 *u, uint16 *v)
{
    uint16 chromaScale;
    uint16 lumaScale;

    TLG_NULLPTR_CHECK(y);
    TLG_NULLPTR_CHECK(u);
    TLG_NULLPTR_CHECK(v);
    
    TLG_ReadReg(base_addr, REG_0068, &chromaScale);
    TLG_ReadReg(base_addr, REG_006D, &lumaScale);
    TLGHAL_GET(*v, chromaScale, REG_0068_BIT15_8_SHIFT,
                REG_0068_BIT15_8_MASK);
    TLGHAL_GET(*u, chromaScale, REG_0068_BIT7_0_SHIFT,
                REG_0068_BIT7_0_MASK);
    TLGHAL_GET(*y, lumaScale, REG_006D_BIT4_0_SHIFT,
                REG_006D_BIT4_0_MASK);
    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_SetYUVScale(uint32 base_addr, uint16 y,uint16 u, uint16 v)
{
    uint16 chromaScale;
    uint16 lumaScale;

    TLG_BOUNDS_CHECK(y, TLG_Y_MAX);
    TLG_BOUNDS_CHECK(u, TLG_U_MAX);
    TLG_BOUNDS_CHECK(v, TLG_V_MAX);

    
    TLG_ReadReg(base_addr, REG_0068, &chromaScale);
    TLG_ReadReg(base_addr, REG_006D, &lumaScale);
    TLGHAL_SET(chromaScale, v, REG_0068_BIT15_8_SHIFT,
                REG_0068_BIT15_8_MASK);
    TLGHAL_SET(chromaScale, u, REG_0068_BIT7_0_SHIFT,
                REG_0068_BIT7_0_MASK);
    TLGHAL_SET(lumaScale, y, REG_006D_BIT4_0_SHIFT,
                REG_006D_BIT4_0_MASK);
    TLG_WriteReg(base_addr, REG_0068, chromaScale);
    TLG_WriteReg(base_addr, REG_006D, lumaScale);

    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_GetBWVideoMode(uint32 base_addr, uint16 *val)
{
    uint16 videoConfig3;

    TLG_NULLPTR_CHECK(val);
    TLG_ReadReg(base_addr, REG_0070, &videoConfig3);
    TLGHAL_GET(*val, videoConfig3, REG_0070_BIT0_SHIFT,
                REG_0070_BIT0_MASK);
    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_SetBWVideoMode(uint32 base_addr, uint16 val)
{
    TLG_BOUNDS_CHECK(val, TLG_ON);

    TLG_SetBits(base_addr, REG_0070, val,
                REG_0070_BIT0_SHIFT,
                REG_0070_BIT0_MASK);
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_GetVideoTestPatternState(uint32 base_addr, uint16 *val)
{
    uint16 videoConfig3;
    uint16 videoConfig5;
    uint16 freezeFrame;
    uint16 useRom;

    TLG_NULLPTR_CHECK(val);

    
    TLG_ReadReg(base_addr, REG_0072,  &videoConfig5);
    TLG_ReadReg(base_addr, REG_0070,  &videoConfig3);
    TLGHAL_GET(freezeFrame, videoConfig5, REG_0072_BIT0_SHIFT,
                REG_0072_BIT0_MASK);

    TLGHAL_GET(useRom, videoConfig3, REG_0070_BIT7_SHIFT,
                REG_0070_BIT7_MASK);

    *val = (freezeFrame == TLG_ON) && (useRom == TLG_ON);

    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_SetVideoTestPatternState(uint32 base_addr, uint16 val)
{
    uint16  videoConfig2,
            videoConfig3,
            videoConfig5,
            videoConfig6,
            videoConfig9,
            videoConfig10,
            
            tempReg1,
            tempReg2,
            tempReg3;

    TLG_ReadReg(base_addr, REG_0070,  &videoConfig3);
    TLG_ReadReg(base_addr, REG_0072,  &videoConfig5);
    TLG_ReadReg(base_addr, REG_0076,  &videoConfig9);
    TLG_ReadReg(base_addr, REG_0077, &videoConfig10);

    if (val == TLG_ON)
    {
            TLG_REG_LOG((b, "TLG_SetVideoTestPatternState: TLG_ON\n"));
            
            TLG_ReadReg(base_addr, REG_006F,  &videoConfig2);
            TLG_ReadReg(base_addr, REG_0073,  &videoConfig6);

            
            TLGHAL_CLEAR(videoConfig10, REG_0077_BIT12_MASK);
            TLGHAL_SET(videoConfig5, TLG_ON, REG_0072_BIT0_SHIFT,
                        REG_0072_BIT0_MASK);

            
            TLGHAL_SET(videoConfig5, REG_0072_VALUE_1,
                        REG_0072_BIT6_5_SHIFT,
                        REG_0072_BIT6_5_MASK);

            
            TLGHAL_CLEAR(videoConfig6, REG_0073_BIT15_MASK);

            
            TLGHAL_SET(videoConfig2, TLG_ON, REG_006F_BIT6_SHIFT,
                        REG_006F_BIT6_MASK);

            
            TLGHAL_SET(videoConfig3, TLG_ON, REG_0070_BIT15_SHIFT,
                        REG_0070_BIT15_MASK);

            
            TLGHAL_CLEAR(videoConfig9, REG_0076_BIT7_MASK);

            
            TLGHAL_SET(videoConfig3, TLG_ON, REG_0070_BIT7_SHIFT,
                         REG_0070_BIT7_MASK);

            TLG_WriteReg(base_addr, REG_006F, videoConfig2);
            TLG_WriteReg(base_addr, REG_0073, videoConfig6);
    } 
    else 
    {
        
        TLGHAL_CLEAR(videoConfig3, REG_0070_BIT7_MASK);

        
        TLGHAL_CLEAR(videoConfig5, REG_0072_BIT0_MASK);

        
        TLGHAL_SET(videoConfig9, TLG_ON, REG_0076_BIT7_SHIFT,
                        REG_0076_BIT7_MASK);
        TLG_ReadReg(base_addr, REG_009B,  &tempReg1);
        TLG_ReadReg(base_addr, REG_005A,  &tempReg2);
        TLG_ReadReg(base_addr, REG_0065,  &tempReg3);
        if (val == TLG_OFF) 
        {
            TLG_REG_LOG((b, "TLG_SetVideoTestPatternState: TLG_OFF\n"));
            
            TLGHAL_SET(videoConfig10, TLG_ON, REG_0077_BIT12_SHIFT,
                         REG_0077_BIT12_MASK);
            
            TLGHAL_CLEAR(tempReg1, REG_009B_BIT7_MASK);
            
            TLGHAL_SET(tempReg2, 4,
                        REG_005A_BIT11_8_SHIFT,
                        REG_005A_BIT11_8_MASK);
            
            TLGHAL_SET(tempReg3, TLG_ON,
                        REG_0065_BIT3_SHIFT,
                        REG_0065_BIT3_MASK);
            
            TLGHAL_SET(videoConfig3, TLG_ON,
                        REG_0070_BIT8_SHIFT,
                        REG_0070_BIT8_MASK);
            
            TLGHAL_SET(videoConfig9, 0,
                        REG_0076_BIT6_0_SHIFT,
                        REG_0076_BIT6_0_MASK);
            TLGHAL_SET(videoConfig10, 0,
                        REG_0077_BIT10_8_SHIFT,
                        REG_0077_BIT10_8_MASK);
        } 
        else if (val == TLG_COLOR_BAR) 
        {
            TLG_REG_LOG((b, "TLG_SetVideoTestPatternState: TLG_COLOR_BAR\n"));
            
            TLGHAL_CLEAR(videoConfig10, REG_0077_BIT12_MASK);
            
            TLGHAL_SET(tempReg1, TLG_ON,
                        REG_009B_BIT7_SHIFT,
                        REG_009B_BIT7_MASK);
            
            TLGHAL_SET(tempReg2, 0,
                        REG_005A_BIT11_8_SHIFT,
                        REG_005A_BIT11_8_MASK);
            
            TLGHAL_CLEAR(tempReg3,
                        REG_0065_BIT3_MASK);
            
            TLGHAL_CLEAR(videoConfig3, REG_0070_BIT8_MASK);
            
            TLGHAL_SET(videoConfig9, 1,
                        REG_0076_BIT6_0_SHIFT,
                        REG_0076_BIT6_0_MASK);
            TLGHAL_SET(videoConfig10, 0,
                        REG_0077_BIT10_8_SHIFT,
                        REG_0077_BIT10_8_MASK);
        }
        TLG_WriteReg(base_addr, REG_009B, tempReg1);
        TLG_WriteReg(base_addr, REG_005A, tempReg2);
        TLG_WriteReg(base_addr, REG_0065, tempReg3);
    }

    TLG_WriteReg(base_addr, REG_0070,  videoConfig3);
    TLG_WriteReg(base_addr, REG_0072,  videoConfig5);
    TLG_WriteReg(base_addr, REG_0076,  videoConfig9);
    TLG_WriteReg(base_addr, REG_0077, videoConfig10);
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_Get601VideoOutClock(uint32 base_addr, uint16 *val)
{
    uint16 videoConfig3;

    TLG_NULLPTR_CHECK(val);

    TLG_ReadReg(base_addr, REG_0070, &videoConfig3);
    TLGHAL_GET(*val, videoConfig3, REG_0070_BIT4_SHIFT,
                REG_0070_BIT4_MASK);
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_Set601VideoOutClock(uint32 base_addr, uint16 val)
{
    uint16 videoConfig3;

    TLG_BOUNDS_CHECK(val, TLG_INVERT);

    TLG_ReadReg(base_addr, REG_0070, &videoConfig3);
    TLGHAL_SET(videoConfig3, val, REG_0070_BIT4_SHIFT,
                REG_0070_BIT4_MASK);
    TLG_WriteReg(base_addr, REG_0070, videoConfig3);
    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_GetChromaLumaOrder(uint32 base_addr, uint16 *val)
{
    uint16 videoConfig2;

    TLG_NULLPTR_CHECK(val);

    TLG_ReadReg(base_addr, REG_006F, &videoConfig2);
    TLGHAL_GET(*val, videoConfig2, REG_006F_BIT4_SHIFT,
                REG_006F_BIT4_MASK);
    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_SetChromaLumaOrder(uint32 base_addr, uint16 val)
{

    TLG_BOUNDS_CHECK(val, TLG_CHRLUM_SWAP);

    TLG_SetBits(base_addr, REG_006F, val,
                REG_006F_BIT4_SHIFT,
                REG_006F_BIT4_MASK);
    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_GetCbCrOrder(uint32 base_addr, uint16 *val)
{
    uint16 videoConfig3;

    TLG_NULLPTR_CHECK(val);

    TLG_ReadReg(base_addr, REG_0070, &videoConfig3);
    TLGHAL_GET(*val, videoConfig3, REG_0070_BIT2_SHIFT,
                REG_0070_BIT2_MASK);

    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_SetCbCrOrder(uint32 base_addr, uint16 val)
{
    TLG_BOUNDS_CHECK(val, TLG_CBCR_SWAP);

    TLG_SetBits(base_addr, REG_0070, val,
                REG_0070_BIT2_SHIFT,
                REG_0070_BIT2_MASK);
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_GetVideoResolution(uint32 base_addr, uint16 *h, uint16 *v)
{
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_SetVideoResolution(uint32 base_addr, uint16 h, uint16 v, uint16 vmode)
{
    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_GetAudioChan(uint32 base_addr, uint16 *val)
{
    uint16 soundDsmDac;

    TLG_NULLPTR_CHECK(val);

    TLG_ReadReg(base_addr, REG_0097, &soundDsmDac);
    TLGHAL_GET(*val, soundDsmDac, REG_0097_BIT1_SHIFT,
                REG_0097_BIT1_MASK);
    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_SetAudioChan(uint32 base_addr, uint16 val)
{
    TLG_BOUNDS_CHECK(val, TLG_LEFT_RIGHT);

    TLG_SetBits(base_addr, REG_0097, val,
                REG_0097_BIT1_SHIFT,
                REG_0097_BIT1_MASK);

    return(TLG_ERR_SUCCESS);
}

TLGDLL_API int TLG_GetAudioEnable(uint32 base_addr, uint16 *val)
{
    uint16 soundDemod1;

    
    TLG_NULLPTR_CHECK(val);

    TLG_ReadReg(base_addr, REG_008B, &soundDemod1);

    TLGHAL_GET(*val, soundDemod1, REG_008B_BIT0_SHIFT,
                REG_008B_BIT0_MASK);

    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_SetAudioEnable(uint32 base_addr, uint16 val)
{
    uint16 soundDemod1;

    TLG_BOUNDS_CHECK(val, TLG_ENABLE);

    TLG_ReadReg(base_addr, REG_008B, &soundDemod1);
    TLGHAL_SET(soundDemod1, val,
                REG_008B_BIT0_SHIFT,
                REG_008B_BIT0_MASK);
    TLG_WriteReg(base_addr, REG_008B, soundDemod1);
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_GetAudioMode(uint32 base_addr, uint16 *val)
{
    uint16 soundDsmDac;

    TLG_NULLPTR_CHECK(val);

    TLG_ReadReg(base_addr, REG_0097, &soundDsmDac);

    
    if (soundDsmDac & REG_0097_BIT0_MASK) 
    {
        if (soundDsmDac & REG_0097_BIT9_MASK)
            *val = TLG_DSM_DIFF;
        else
            *val = TLG_DAC;
    } 
    else
        *val = TLG_MASTER;
    TLG_REG_LOG((b, "TLG_GetAudioMode: aud=%d, reg97=0x%x\n", *val, soundDsmDac));
    if (*val == TLG_UNDEFINED)
            return(TLG_ERR_FAIL);

    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_SetAudioMode(uint32 base_addr, uint16 val)
{
    uint16 stereo_mode;
    uint16 regData1;
    uint16 audio_mode_idx = AUDIO_DSM_MONO;
    tlg_audio_mode_select_t audio_mode_s;

    TLG_Get_FM_AudioStereoMode(base_addr, &stereo_mode);

    switch (val)
    {
        case TLG_MASTER:
                
            switch (stereo_mode)
            {
                case (TLG_FM_STEREO)      : audio_mode_idx = AUDIO_I2S_STEREO   ; break;
                case (TLG_FM_MONO)        : audio_mode_idx = AUDIO_I2S_MONO     ; break;
                case (TLG_FM_AUTO_DETECT) : audio_mode_idx = AUDIO_I2S_AUTO     ; break;
                default                   : return (TLG_ERR_PARAM);
            }
            audio_mode_s = audio_mode_select[audio_mode_idx];
            break;
        case TLG_SLAVE:
            return TLG_ERR_NOT_SUPPORTED;
        case TLG_DAC:
            switch (stereo_mode)
            {
                case (TLG_FM_STEREO)      : audio_mode_idx = AUDIO_DSM_STEREO   ; break;
                case (TLG_FM_MONO)        : audio_mode_idx = AUDIO_DSM_MONO     ; break;
                case (TLG_FM_AUTO_DETECT) : audio_mode_idx = AUDIO_DSM_AUTO     ; break;
                default                   : return (TLG_ERR_PARAM);
            }
            TLG_ReadReg(base_addr, REG_0097, &regData1);
            TLGHAL_SET(regData1,0,
                REG_0097_BIT9_SHIFT,
                REG_0097_BIT9_MASK);
            TLG_WriteReg(base_addr, REG_0097, regData1);
            audio_mode_s = audio_mode_select[audio_mode_idx];
            break;
        case TLG_DSM_DIFF:
            switch (stereo_mode)
            {
                case (TLG_FM_STEREO)      : audio_mode_idx = AUDIO_DSM_STEREO   ; break;
                case (TLG_FM_MONO)        : audio_mode_idx = AUDIO_DSM_MONO     ; break;
                case (TLG_FM_AUTO_DETECT) : audio_mode_idx = AUDIO_DSM_AUTO     ; break;
                default                   : return (TLG_ERR_PARAM);
            }
            TLG_ReadReg(base_addr, REG_0097, &regData1);
            TLGHAL_SET(regData1,1,
                REG_0097_BIT9_SHIFT,
                REG_0097_BIT9_MASK);
            TLG_WriteReg(base_addr, REG_0097, regData1);

            TLG_ReadReg(base_addr, REG_0000, &regData1);
            TLGHAL_SET(regData1,1,
                REG_0000_BIT14_SHIFT,
                REG_0000_BIT14_MASK);
            TLGHAL_SET(regData1,1,
                REG_0000_BIT13_SHIFT,
                REG_0000_BIT13_MASK);
            TLG_WriteReg(base_addr, REG_0000, regData1);
            audio_mode_s = audio_mode_select[audio_mode_idx];
            break;
        default:
            return TLG_ERR_PARAM;
    }
    TLG_REG_LOG((b, "TLG_SetAudioMode: aud=%d, stereo=%d, idx=%d\n", val, stereo_mode, audio_mode_idx));
    return TLG_Set_FM_AudioStereoModeHelper(base_addr, &audio_mode_s);
}


TLGDLL_API int TLG_GetAudioGain(uint32 base_addr, uint16 aud_mode, uint16 *gain)
{
    uint16 input_gain, output_gain, max_gain;
    uint16 data;

    TLG_NULLPTR_CHECK(gain);

    TLG_CHIP_VERS_2_1_BEGIN

 #if defined (TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)
    switch (aud_mode)
    {
         case TLG_MASTER:
             {
                uint16 reg1, reg2;
                TLG_ReadReg(base_addr, REG_00F0, &reg1);
                TLG_ReadReg(base_addr, REG_008C, &reg2);
                TLGHAL_GET(input_gain, reg1,
                             REG_00F0_BIT15_13_SHIFT,
                                             REG_00F0_BIT15_13_MASK);
                TLGHAL_GET(output_gain, reg2,
                                 REG_008C_BIT6_4_SHIFT,
                                             REG_008C_BIT6_4_MASK);
                    *gain = input_gain + output_gain;                    
                    return TLG_ERR_SUCCESS;
              }
         case TLG_DAC:
         case TLG_DSM_DIFF:   
             {
                TLG_ReadReg(base_addr, REG_00F0, &data);
                TLGHAL_GET(output_gain, data,
                             REG_00F0_BIT12_10_SHIFT,
                REG_00F0_BIT12_10_MASK);
                TLG_ReadReg(base_addr, REG_0097, &data);
                TLGHAL_GET(input_gain, data,
                 REG_0097_BIT5_4_SHIFT,
                 REG_0097_BIT5_4_MASK);

                *gain = TLG_MAX_DSM_VOLUME_2_1 - output_gain - input_gain;


                TLG_REG_LOG((b, "NEW DSM mode: get gain: 0x%02X\n", *gain));
                return TLG_ERR_SUCCESS;
             }
        case TLG_SLAVE:
            return TLG_ERR_NOT_SUPPORTED;
        default:
            return TLG_ERR_PARAM;
    }  

#endif 

    TLG_CHIP_VERS_1_1_ELSE

#if defined(TLG_CMP_CHIP_1120_1_1) || defined(TLG_CMP_CHIP_1120_1_2)

    switch (aud_mode)
    {
        case TLG_MASTER:
            max_gain = TLG_MAX_I2S_VOLUME;
            break;
        case TLG_DAC:
        case TLG_DSM_DIFF:
            max_gain = TLG_MAX_DSM_VOLUME;
            break;
        case TLG_SLAVE:
            return TLG_ERR_NOT_SUPPORTED;
        default:
            return TLG_ERR_PARAM;
    }

#ifdef USE_APOLLO_AUDIO_MODE_ONLY
        

        
        TLG_ReadReg(base_addr, REG_0093, &data);
        TLGHAL_GET(input_gain,
                        data,
                        REG_0093_BIT10_8_SHIFT,
                        REG_0093_BIT10_8_MASK);
        TLG_REG_LOG((b, "Apollo mode: input_gain: 0x%02X\n", input_gain));

#else 
        

        {
            uint16 stereo_mode;
            TLG_Get_FM_AudioStereoMode(base_addr, &stereo_mode);
            
            if (stereo_mode)
            {
                
                TLG_ReadReg(base_addr, REG_0093, &data);
                TLGHAL_GET(input_gain,
                            data,
                            REG_0093_BIT10_8_SHIFT,
                            REG_0093_BIT10_8_MASK);
                TLG_REG_LOG((b, "Apollo mode: input_gain: 0x%02X\n", input_gain));
            }
            else
            {
                
                TLG_ReadReg(base_addr, REG_008D, &data);
                TLGHAL_GET(input_gain,
                            data,
                            REG_008D_BIT15_13_SHIFT,
                            REG_008D_BIT15_13_MASK);
                TLG_REG_LOG((b, "Zeus mode: input_gain: 0x%02X\n", input_gain));
            }
        }

#endif 

        switch (aud_mode)
        {
            case TLG_MASTER:
                TLG_ReadReg(base_addr, REG_008C, &data);
                TLGHAL_GET(output_gain, data,
                            REG_008C_BIT6_4_SHIFT,
                            REG_008C_BIT6_4_MASK);
                TLG_REG_LOG((b, "I2S mode: output_gain: 0x%02X\n", output_gain));
                *gain = input_gain + output_gain;
                break;
            case TLG_DAC:
            case TLG_DSM_DIFF:          
                TLG_ReadReg(base_addr, REG_0097, &data);
                TLGHAL_GET(output_gain, data,
                            REG_0097_BIT5_4_SHIFT,
                            REG_0097_BIT5_4_MASK);
                TLG_REG_LOG((b, "DSM mode: output_gain: 0x%02X\n", output_gain));
                *gain = input_gain + (3 - output_gain);
                break;
            case TLG_SLAVE:
                return TLG_ERR_NOT_SUPPORTED;
            default:
                return TLG_ERR_PARAM;
        }

    #endif 

    TLG_CHIP_VERS_END

    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_SetAudioGain(uint32 base_addr, uint16 aud_mode, uint16 gain)
{
    uint16 max_gain = 0;
    uint16 input_gain, output_gain;
    tlg_audio_gain_t *p_audio_gain_tbl = audio_gain_i2s_tbl;

    TLG_CHIP_VERS_2_1_BEGIN
 #if defined (TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)
            TLG_REG_LOG((b, "TLG_SetAudioGain: audio_mode=%d, gain=%d\n", aud_mode, gain));
    switch (aud_mode)
    {
        case TLG_MASTER:
            { int vol = gain;
                TLG_BOUNDS_CHECK(gain, TLG_MAX_I2S_VOLUME_2_1)
                        output_gain = max(0,vol-7);
                input_gain = max(0,min(7,vol));
                TLG_SetBits(base_addr, REG_00F0,
                            output_gain,
                            REG_00F0_BIT15_13_SHIFT,
                            REG_00F0_BIT15_13_MASK);
                TLG_SetBits(base_addr, REG_008C,
                            input_gain,
                            REG_008C_BIT6_4_SHIFT,
                            REG_008C_BIT6_4_MASK);
                break;
            }
        case TLG_DAC:
        case TLG_DSM_DIFF:
            TLG_BOUNDS_CHECK(gain, TLG_MAX_DSM_VOLUME_2_1)
                    output_gain = abs(min(0, max(0, (int)gain - 2) - 7));
            input_gain  = abs(min(0, (int)gain - 2));
            TLG_SetBits(base_addr, REG_00F0, 
                        output_gain,
                        REG_00F0_BIT12_10_SHIFT,
                        REG_00F0_BIT12_10_MASK);
            TLG_SetBits(base_addr, REG_0097,
                        input_gain,
                        REG_0097_BIT5_4_SHIFT,
                        REG_0097_BIT5_4_MASK);
            TLG_REG_LOG((b, "NEW DSM mode: set gain: 0x%02X\n", gain));
            break;
        case TLG_SLAVE:
            return TLG_ERR_NOT_SUPPORTED;
        default:
            return TLG_ERR_PARAM;
    }

#endif 

    TLG_CHIP_VERS_1_1_ELSE
#if defined(TLG_CMP_CHIP_1120_1_1) || defined(TLG_CMP_CHIP_1120_1_2)

        switch (aud_mode)
        {
            case TLG_MASTER:
                p_audio_gain_tbl = audio_gain_i2s_tbl;
                max_gain = TLG_MAX_I2S_VOLUME;
                break;
            case TLG_DAC:
            case TLG_DSM_DIFF:
                p_audio_gain_tbl = audio_gain_dsm_tbl;
                max_gain = TLG_MAX_DSM_VOLUME;
                break;
            case TLG_SLAVE:
                return TLG_ERR_NOT_SUPPORTED;
            default:
                return TLG_ERR_PARAM;
        }
    TLG_BOUNDS_CHECK(gain, max_gain);

    input_gain  = p_audio_gain_tbl[gain].input_gain;
    output_gain = p_audio_gain_tbl[gain].output_gain;
#ifdef USE_APOLLO_AUDIO_MODE_ONLY
    
    
    TLG_SetBits(base_addr, REG_0093,
                input_gain,
                REG_0093_BIT10_8_SHIFT,
                REG_0093_BIT10_8_MASK);

    TLG_SetBits(base_addr, REG_008C, 0,
                REG_008C_BIT7_SHIFT,
                REG_008C_BIT7_MASK);

#else 
    
    {
        uint16 stereo_mode;
        TLG_Get_FM_AudioStereoMode(base_addr, &stereo_mode);
        
        if (stereo_mode)
        {
            
            TLG_SetBits(base_addr, REG_0093,
                        input_gain,
                        REG_0093_BIT10_8_SHIFT,
                        REG_0093_BIT10_8_MASK);

            TLG_SetBits(base_addr, REG_008C, 0,
                        REG_008C_BIT7_SHIFT,
                        REG_008C_BIT7_MASK);

        }
        else
        {
            
            TLG_SetBits(base_addr, REG_008C, 1,
                        REG_008C_BIT7_SHIFT,
                        REG_008C_BIT7_MASK);
            TLG_SetBits(base_addr, REG_008D,
                        input_gain,
                        REG_008D_BIT15_13_SHIFT,
                        REG_008D_BIT15_13_MASK);
        }
    }
#endif 

    switch (aud_mode)
    {
        case TLG_MASTER:
            
            TLG_SetBits(base_addr, REG_008C,
                        output_gain,
                        REG_008C_BIT6_4_SHIFT,
                        REG_008C_BIT6_4_MASK);
            break;
        case TLG_DAC:
        case TLG_DSM_DIFF:
            
            TLG_SetBits(base_addr, REG_0097,
                        output_gain,
                        REG_0097_BIT5_4_SHIFT,
                        REG_0097_BIT5_4_MASK);
            break;
        case TLG_SLAVE:
            return TLG_ERR_NOT_SUPPORTED;
        default:
            return TLG_ERR_PARAM;
    }
#endif 
    TLG_CHIP_VERS_END

#ifdef TLG_SUPPORT_FM_AVC 
        fm_scale_sel = output_gain;
#endif
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_GetAudioOutput(uint32 base_addr, uint16 *aud_mode, uint16 *gain) 
{
    uint16 regData1;
    uint16 regData2;
    uint16 pll, aud;

    TLG_NULLPTR_CHECK(aud_mode);
    TLG_NULLPTR_CHECK(gain);

    TLG_ReadReg(base_addr, REG_0097, &regData2);

    
        if (regData2 & REG_0097_BIT0_MASK) 
        {
            if (regData2 & REG_0097_BIT9_MASK)
                *aud_mode = TLG_DSM_DIFF;
            else
                *aud_mode = TLG_DAC;
        }

        else
            *aud_mode = TLG_MASTER;
        
    TLG_ReadReg(base_addr, REG_008C, &regData1);
    TLGHAL_GET(pll, regData1, REG_008C_BIT7_SHIFT, REG_008C_BIT7_MASK);

    switch (*aud_mode)
    {
        case TLG_MASTER:
            TLGHAL_GET(aud, regData1, REG_008C_BIT6_4_SHIFT,
                        REG_008C_BIT6_4_MASK);
            break;
        case TLG_DAC:
        case TLG_DSM_DIFF:
            TLG_ReadReg(base_addr, REG_0097, &regData2);
            TLGHAL_GET(aud, regData2, REG_0097_BIT5_4_SHIFT,
                        REG_0097_BIT5_4_MASK);
            aud = 3 - aud;
            break;
        default:
            return TLG_ERR_PARAM;
    }

    *gain = 2*pll + aud; 

    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_SetAudioOutput(uint32 base_addr, uint16 aud_mode, uint16 gain)
{
    uint16 regData1;
    uint16 regData2;
    uint16 regData3;
    uint16 pll, aud, max_gain=0;

    TLG_BOUNDS_CHECK(aud_mode, TLG_DAC);

    TLG_ReadReg(base_addr, REG_008B, &regData1);
    TLG_ReadReg(base_addr, REG_0097, &regData2);

    switch (aud_mode)
    {
        case TLG_MASTER:
            TLGHAL_SET(regData1, 1,
                        REG_008B_BIT3_SHIFT,
                        REG_008B_BIT3_MASK);
            TLGHAL_CLEAR(regData2, REG_0097_BIT0_MASK);
            max_gain = TLG_MAX_AUD_GAIN_DIG;
            break;
        case TLG_DAC:
            TLGHAL_CLEAR(regData1, REG_008B_BIT3_MASK);
            TLGHAL_SET(regData2, 1, REG_0097_BIT0_SHIFT, REG_0097_BIT0_MASK);
            TLGHAL_SET(regData2,0,
                    REG_0097_BIT9_SHIFT,
                    REG_0097_BIT9_MASK);
            TLG_WriteReg(base_addr, REG_0097, regData2);
            max_gain = TLG_MAX_AUD_GAIN_DAC;     
            break;
        case TLG_DSM_DIFF :
            TLGHAL_SET(regData2, 1, REG_0097_BIT0_SHIFT, REG_0097_BIT0_MASK);
            TLGHAL_SET(regData2,1,
                REG_0097_BIT9_SHIFT,
                REG_0097_BIT9_MASK);
            TLG_WriteReg(base_addr, REG_0097, regData2);

            TLG_ReadReg(base_addr, REG_0000, &regData3);
            TLGHAL_SET(regData3,1,
                REG_0000_BIT14_SHIFT,
                REG_0000_BIT14_MASK);
            TLGHAL_SET(regData3,1,
                REG_0000_BIT13_SHIFT,
                REG_0000_BIT13_MASK);
            TLG_WriteReg(base_addr, REG_0000, regData3);
            break;
        case TLG_SLAVE:
            return TLG_ERR_NOT_SUPPORTED;

        default:
#ifdef TLG_DO_ERROR_CHECKING
        return(TLG_ERR_PARAM);
#else
        break;
#endif 
    }

    TLG_WriteReg(base_addr, REG_008B, regData1);
    TLG_WriteReg(base_addr, REG_0097, regData2);

    TLG_BOUNDS_CHECK(gain, max_gain);
    
    if (gain > (max_gain-2))
    {
        pll = 1;
        aud = gain - 2;
    } 
    else 
    {
        pll = 0;
        aud = gain;
    }
    
    TLG_ReadReg(base_addr, REG_008C, &regData1);
    TLGHAL_SET(regData1, pll, REG_008C_BIT7_SHIFT, REG_008C_BIT7_MASK);

    switch (aud_mode)
    {
        case TLG_MASTER:            TLGHAL_SET(regData1, aud, REG_008C_BIT6_4_SHIFT,
                        REG_008C_BIT6_4_MASK);
            break;
        case TLG_DAC:
        case TLG_DSM_DIFF:
            TLG_ReadReg(base_addr, REG_0097, &regData2);
            TLGHAL_SET(regData2, (3-aud), REG_0097_BIT5_4_SHIFT,
                        REG_0097_BIT5_4_MASK);
            TLG_WriteReg(base_addr, REG_0097, regData2);
            break;
        case TLG_SLAVE:
            return TLG_ERR_NOT_SUPPORTED;
        default:
            return TLG_ERR_PARAM;
    }
    TLG_WriteReg(base_addr, REG_008C, regData1);

    return(TLG_ERR_SUCCESS);
}

TLGDLL_API int TLG_Set_FM_AudioStereoModeHelper(uint32 base_addr, tlg_audio_mode_select_t *p_audio_mode_select)
{
    uint16 regData;
    TLG_REG_LOG((b, "TLG_Set_FM_AudioStereoModeHelper: Enter\n"));
    TLG_ReadReg(base_addr, REG_00B4, &regData);
    
    TLGHAL_SET(regData, p_audio_mode_select->fm_demph_bypass,
                REG_00B4_BIT6_SHIFT,
                REG_00B4_BIT6_MASK);
    
    TLGHAL_SET(regData, p_audio_mode_select->fm_auto_mono,
                REG_00B4_BIT4_SHIFT,
                REG_00B4_BIT4_MASK);
    
    TLGHAL_SET(regData, p_audio_mode_select->fm_i2s_sel,
                REG_00B4_BIT2_SHIFT,
                REG_00B4_BIT2_MASK);
    
    TLGHAL_SET(regData, p_audio_mode_select->fm_mono_sel,
                REG_00B4_BIT1_SHIFT,
                REG_00B4_BIT1_MASK);
    
    TLGHAL_SET(regData, p_audio_mode_select->fm_stereo_sel,
                REG_00B4_BIT0_SHIFT,
                REG_00B4_BIT0_MASK);
    TLG_WriteReg(base_addr, REG_00B4, regData);
    
    TLG_ReadReg(base_addr, REG_008B, &regData);
    TLGHAL_SET(regData, p_audio_mode_select->sound_master_en,
                REG_008B_BIT3_SHIFT,
                REG_008B_BIT3_MASK);
    TLG_WriteReg(base_addr, REG_008B, regData);

    TLG_ReadReg(base_addr, REG_0097, &regData);
    
    TLGHAL_SET(regData, p_audio_mode_select->dsm_dither_en,
                REG_0097_BIT8_SHIFT,
                REG_0097_BIT8_MASK);
    
    TLGHAL_SET(regData, p_audio_mode_select->dsm_stereo,
                REG_0097_BIT1_SHIFT,
                REG_0097_BIT1_MASK);
    
    TLGHAL_SET(regData, p_audio_mode_select->dsm_en,
                REG_0097_BIT0_SHIFT,
                REG_0097_BIT0_MASK);
    TLG_WriteReg(base_addr, REG_0097, regData);
    TLG_REG_LOG((b, "TLG_Set_FM_AudioStereoModeHelper: Exit\n"));
    return(TLG_ERR_SUCCESS);
}

TLGDLL_API int TLG_Get_FM_AudioStereoModeHelper(uint32 base_addr, tlg_audio_mode_select_t *p_audio_mode_select)
{
    uint16 regData;

    TLG_ReadReg(base_addr, REG_00B4, &regData);
    
    TLGHAL_GET(p_audio_mode_select->fm_demph_bypass, regData,
                REG_00B4_BIT6_SHIFT,
                REG_00B4_BIT6_MASK);
    
    TLGHAL_GET(p_audio_mode_select->fm_auto_mono, regData,
                REG_00B4_BIT4_SHIFT,
                REG_00B4_BIT4_MASK);
    
    TLGHAL_GET(p_audio_mode_select->fm_i2s_sel, regData,
                REG_00B4_BIT2_SHIFT,
                REG_00B4_BIT2_MASK);
    
    TLGHAL_GET(p_audio_mode_select->fm_mono_sel, regData,
                REG_00B4_BIT1_SHIFT,
                REG_00B4_BIT1_MASK);
    
    TLGHAL_GET(p_audio_mode_select->fm_stereo_sel, regData,
                REG_00B4_BIT0_SHIFT,
                REG_00B4_BIT0_MASK);
    
    TLG_ReadReg(base_addr, REG_008B, &regData);
    TLGHAL_GET(p_audio_mode_select->sound_master_en, regData,
                REG_008B_BIT3_SHIFT,
                REG_008B_BIT3_MASK);
    
    TLG_ReadReg(base_addr, REG_0097, &regData);
    TLGHAL_GET(p_audio_mode_select->dsm_dither_en, regData,
                REG_0097_BIT8_SHIFT,
                REG_0097_BIT8_MASK);
    
    TLGHAL_GET(p_audio_mode_select->dsm_stereo, regData,
                REG_0097_BIT1_SHIFT,
                REG_0097_BIT1_MASK);
    
    TLGHAL_GET(p_audio_mode_select->dsm_en, regData,
                REG_0097_BIT0_SHIFT,
                REG_0097_BIT0_MASK);
    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_Set_FM_AudioStereoMode(uint32 base_addr, uint16 stereo_mode)
{
    uint16 regData;
    uint16 aud_mode;
    uint16 audio_mode_idx = AUDIO_DSM_MONO;
    
    TLG_GetAudioMode(base_addr, &aud_mode);

    
    TLG_REG_LOG((b,"TLG_Set_FM_AudioStereoMode: mode=%d\n", stereo_mode));
    TLG_ReadReg(base_addr, REG_008B, &regData);
    TLGHAL_SET(regData, 0,
                REG_008B_BIT8_SHIFT,
                REG_008B_BIT8_MASK);
    TLG_WriteReg(base_addr, REG_008B, regData);

    switch (aud_mode)
    {
        case TLG_MASTER:
            
            switch (stereo_mode)
            {
                case TLG_FM_STEREO :     audio_mode_idx = AUDIO_I2S_STEREO; break;
                case TLG_FM_MONO   :     audio_mode_idx = AUDIO_I2S_MONO;   break;
                case TLG_FM_AUTO_DETECT: audio_mode_idx = AUDIO_I2S_AUTO;   break;
                default: return TLG_ERR_PARAM;
            }
            break;
        case TLG_DAC:
        case TLG_DSM_DIFF:
            
            switch (stereo_mode)
            {
                case TLG_FM_STEREO :     audio_mode_idx = AUDIO_DSM_STEREO; break;
                case TLG_FM_MONO   :     audio_mode_idx = AUDIO_DSM_MONO;   break;
                case TLG_FM_AUTO_DETECT: audio_mode_idx = AUDIO_DSM_AUTO;   break;
                default: return TLG_ERR_PARAM;
            }
            break;
        default:
            return TLG_ERR_PARAM;
    }
    TLG_REG_LOG((b, "TLG_Set_FM_AudioStereoMode: aud=%d, stereo=%d, idx=%d\n", aud_mode, stereo_mode, audio_mode_idx));
    return TLG_Set_FM_AudioStereoModeHelper(base_addr, (tlg_audio_mode_select_t *)(&audio_mode_select[audio_mode_idx]));
}


TLGDLL_API int TLG_Get_FM_AudioStereoMode(uint32 base_addr, uint16 *p_stereo_mode)
{
    tlg_audio_mode_select_t audio_mode_s;
    TLG_Get_FM_AudioStereoModeHelper(base_addr, &audio_mode_s);

#ifdef USE_APOLLO_AUDIO_MODE_ONLY
    
    *p_stereo_mode = (audio_mode_s.fm_mono_sel == 1) ? 0 : 1;
#else 
    

    if (audio_mode_s.dsm_en == 0)
    {
        
        if (audio_mode_s.fm_auto_mono)
            *p_stereo_mode = TLG_FM_AUTO_DETECT; 
        else if (audio_mode_s.fm_mono_sel)
            *p_stereo_mode = TLG_FM_MONO;        
        else
            *p_stereo_mode = TLG_FM_STEREO;      
    }
    else
    {
        
        if (audio_mode_s.fm_auto_mono)
            *p_stereo_mode = TLG_FM_AUTO_DETECT; 
        else if (audio_mode_s.fm_mono_sel)
            *p_stereo_mode = TLG_FM_MONO;        
        else
            *p_stereo_mode = TLG_FM_STEREO;      
    }
#endif 
    TLG_REG_LOG((b, "TLG_Get_FM_AudioStereoMode: auto=%d, stereo=%d, mode=%d\n", audio_mode_s.fm_auto_mono, 
                            audio_mode_s.fm_stereo_sel,
                            *p_stereo_mode));
    return(TLG_ERR_SUCCESS);
}




TLGDLL_API int TLG_GetI2SAudioData(uint32 base_addr, uint16 *R) 
{
    uint16 regData;

    TLG_NULLPTR_CHECK(R);


    TLG_CHIP_VERS_2_1_BEGIN
 #if defined (TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)
    {
            
        TLG_ReadReg(base_addr, REG_00B0,  &regData);
        TLGHAL_GET(*R,regData,
                    REG_00B0_BIT15_8_SHIFT,
                    REG_00B0_BIT15_8_MASK);
        
            
    }

#endif 
    TLG_CHIP_VERS_ELSE

#if defined(TLG_CMP_CHIP_1120_1_2) ||defined(TLG_CMP_CHIP_1120_1_1)
    {
        TLG_ReadReg(base_addr, REG_008E, &regData);
        TLGHAL_GET(*R, regData, REG_008E_BIT7_0_SHIFT,
                    REG_008E_BIT7_0_MASK);
        if (*R == 0) 
            *R = 255;
        else
            *R = (*R) -1;
    }

#endif

    TLG_CHIP_VERS_END
    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_SetI2SAudioData(uint32 base_addr, uint16 R) 
{
    uint16 regData;

    TLG_BOUNDS_CHECK(R, 256);

    TLG_CHIP_VERS_2_1_BEGIN
 #if defined (TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)
    {
            
        TLG_ReadReg(base_addr, REG_00B0,  &regData);
        TLGHAL_SET(regData, R-1,
                    REG_00B0_BIT15_8_SHIFT,
                    REG_00B0_BIT15_8_MASK);
        TLG_WriteReg(base_addr, REG_00B0, regData);
    }
#endif 
    TLG_CHIP_VERS_ELSE

#if defined(TLG_CMP_CHIP_1120_1_2) ||defined(TLG_CMP_CHIP_1120_1_1)
    {
        if (R == 256) R = 0;

        TLG_ReadReg(base_addr, REG_008E, &regData);
        TLGHAL_SET(regData, R, REG_008E_BIT7_0_SHIFT,
                    REG_008E_BIT7_0_MASK);
        TLG_WriteReg(base_addr, REG_008E, regData);
    }
#endif

    TLG_CHIP_VERS_END
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_SetVideoStreamOutputMode(uint32 base_addr, uint32 videoout_mode) 
{
    uint16 regData;
    uint16 videoStreamOutputFormat = REG_0072_VALUE_4;

    switch (videoout_mode) 
    {
        case TLG_TUNE_VID_FORMAT_YCBCR:
            videoStreamOutputFormat = REG_0072_VALUE_4;
            break;
        case TLG_TUNE_VID_FORMAT_YUV:
            videoStreamOutputFormat = REG_0072_VALUE_5;
            break;
        case TLG_TUNE_VID_FORMAT_RGB_565:
            videoStreamOutputFormat = REG_0072_VALUE_6;
            break;
        default:
            return(TLG_ERR_PARAM);
            break;
    }
    TLG_ReadReg(base_addr, REG_0072, &regData);
    TLGHAL_SET(regData, videoStreamOutputFormat,
                    REG_0072_BIT2_1_SHIFT,
                    REG_0072_BIT2_1_MASK);
    TLG_WriteReg(base_addr, REG_0072, regData);

    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_GetVideoStreamOutputMode(uint32 base_addr, uint32 *p_videoout_mode)
{
    uint16 regData;
    uint16 videoStreamOutputFormat;

    TLG_ReadReg(base_addr, REG_0072, &regData);
    TLGHAL_GET(videoStreamOutputFormat, regData, 
                REG_0072_BIT2_1_SHIFT,
                REG_0072_BIT2_1_MASK);

    switch (videoStreamOutputFormat)
    {
        case REG_0072_VALUE_4:
            *p_videoout_mode = TLG_TUNE_VID_FORMAT_YCBCR;
            break;
        case REG_0072_VALUE_5:
            *p_videoout_mode = TLG_TUNE_VID_FORMAT_YUV;
            break;
        case REG_0072_VALUE_6:
            *p_videoout_mode = TLG_TUNE_VID_FORMAT_RGB_565;
            break;
        default:
            return(TLG_ERR_PARAM);
    }
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_GetI2SMasterAudioClock(uint32 base_addr, uint16 *M, uint16 *S) 
{
    uint16 regData;

    TLG_NULLPTR_CHECK(M);
    TLG_NULLPTR_CHECK(S);

    TLG_ReadReg(base_addr, REG_0096, &regData);
    TLGHAL_GET(*M, regData, REG_0096_BIT12_8_SHIFT, REG_0096_BIT12_8_MASK);
    TLGHAL_GET(*S, regData, REG_0096_BIT7_0_SHIFT, REG_0096_BIT7_0_MASK);

    if (*M == 0) *M = 32;
    if (*S == 0) *S = 64;

    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_SetI2SMasterAudioClock(uint32 base_addr, uint16 M, uint16 S) 
{
    uint16 regData;

    TLG_BOUNDS_CHECK(M, 32);
    TLG_BOUNDS_CHECK(S, 64);

    if (M == 32) M = 0;
    if (S == 64) S = 0;

    TLG_ReadReg(base_addr, REG_0096, &regData);
    TLGHAL_SET(regData, M, REG_0096_BIT12_8_SHIFT, REG_0096_BIT12_8_MASK);
    TLGHAL_SET(regData, S, REG_0096_BIT7_0_SHIFT, REG_0096_BIT7_0_MASK);
    TLG_WriteReg(base_addr, REG_0096, regData);

    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_GetAudioTestToneMode(uint32 base_addr, uint16 *val)
{
    uint16 soundDemod1;

    TLG_NULLPTR_CHECK(val);

    TLG_ReadReg(base_addr, REG_008B, &soundDemod1);
    TLGHAL_GET(*val, soundDemod1, REG_008B_BIT1_SHIFT,
                REG_008B_BIT1_MASK);

    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_SetAudioTestToneMode(uint32 base_addr, uint16 val)
{
    TLG_BOUNDS_CHECK(val, TLG_ON);

    
    TLG_SetBits(base_addr, REG_008B, val,
                REG_008B_BIT1_SHIFT,
                REG_008B_BIT1_MASK);
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_GetAudioTestToneL(uint32 base_addr, uint16 *val)
{
    uint16 soundDemod1;

    TLG_NULLPTR_CHECK(val);

    TLG_ReadReg(base_addr, REG_008B, &soundDemod1);
    TLGHAL_GET(*val, soundDemod1, REG_008B_BIT5_4_SHIFT,
                REG_008B_BIT5_4_MASK);
    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_SetAudioTestToneL(uint32 base_addr, uint16 val)
{

    TLG_BOUNDS_CHECK(val, TLG_8KHZ);

    TLG_SetBits(base_addr, REG_008B, val,
                REG_008B_BIT5_4_SHIFT,
                REG_008B_BIT5_4_MASK);
    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_GetAudioTestToneR(uint32 base_addr, uint16 *val)
{
    uint16 soundDemod1;

    TLG_NULLPTR_CHECK(val);

    TLG_ReadReg(base_addr, REG_008B, &soundDemod1);
    TLGHAL_GET(*val, soundDemod1, REG_008B_BIT7_6_SHIFT,
                REG_008B_BIT7_6_MASK);
    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_SetAudioTestToneR(uint32 base_addr, uint16 val)
{
    TLG_BOUNDS_CHECK(val, TLG_8KHZ);

    TLG_SetBits(base_addr, REG_008B, val,
                REG_008B_BIT7_6_SHIFT,
                REG_008B_BIT7_6_MASK);

    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_GetI2SDebugWaveform(uint32 base_addr, uint16 *val)
{
    uint16 regData;

    TLG_NULLPTR_CHECK(val);

    TLG_ReadReg(base_addr, REG_0095, &regData);
    TLGHAL_GET(*val, regData, REG_0095_VALUE_2,
                REG_0095_VALUE_1);

    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_SetI2SDebugWaveform(uint32 base_addr, uint16 val) 
{
    uint16 regData;

    TLG_BOUNDS_CHECK(val, TLG_ON);

    TLG_ReadReg(base_addr, REG_0095, &regData);
    TLGHAL_SET(regData, val, REG_0095_VALUE_2,
                 REG_0095_VALUE_1);
    TLGHAL_SET(regData, 85, REG_0095_BIT7_1_SHIFT,
                REG_0095_BIT7_1_MASK);
    TLG_WriteReg(base_addr, REG_0095, regData);

    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_GetExtHSync(uint32 base_addr,
                uint16 *assert_start,        uint16 *assert_end,
                uint16 *deassert1_start,     uint16 *deassert1_end,
                uint16 *deassert2_start,     uint16 *deassert2_end)
{

    TLG_NULLPTR_CHECK(assert_start);
    TLG_NULLPTR_CHECK(assert_end);
    TLG_NULLPTR_CHECK(deassert1_start);
    TLG_NULLPTR_CHECK(deassert1_end);
    TLG_NULLPTR_CHECK(deassert2_start);
    TLG_NULLPTR_CHECK(deassert2_end);

    TLG_ReadReg(base_addr, REG_007F, assert_start     );
    TLG_ReadReg(base_addr, REG_0080, assert_end       );
    TLG_ReadReg(base_addr, REG_0081, deassert1_start  );
    TLG_ReadReg(base_addr, REG_0082, deassert1_end    );
    TLG_ReadReg(base_addr, REG_0083, deassert2_start  );
    TLG_ReadReg(base_addr, REG_0084, deassert2_end    );

    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_GetExtVSync(uint32 base_addr,
                uint16 *assert1_start,       uint16 *assert1_end,
                uint16 *assert2_start,       uint16 *assert2_end,
                uint16 *assert3_start,       uint16 *assert3_end)
{
    TLG_NULLPTR_CHECK(assert1_start);
    TLG_NULLPTR_CHECK(assert1_end);
    TLG_NULLPTR_CHECK(assert2_start);
    TLG_NULLPTR_CHECK(assert2_end);
    TLG_NULLPTR_CHECK(assert3_start);
    TLG_NULLPTR_CHECK(assert3_end);

    TLG_ReadReg(base_addr, REG_0085, assert1_start);
    TLG_ReadReg(base_addr, REG_0086, assert1_end);
    TLG_ReadReg(base_addr, REG_0087, assert2_start);
    TLG_ReadReg(base_addr, REG_0088, assert2_end);
    TLG_ReadReg(base_addr, REG_0089, assert3_start);
    TLG_ReadReg(base_addr, REG_008A, assert3_end);
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_SetExtHSync(uint32 base_addr,
                uint16 assert_start,        uint16 assert_end,
                uint16 deassert1_start,     uint16 deassert1_end,
                uint16 deassert2_start,     uint16 deassert2_end)
{
    TLG_WriteReg(base_addr, REG_007F, assert_start);
    TLG_WriteReg(base_addr, REG_0080, assert_end);
    TLG_WriteReg(base_addr, REG_0081, deassert1_start);
    TLG_WriteReg(base_addr, REG_0082, deassert1_end);
    TLG_WriteReg(base_addr, REG_0083, deassert2_start);
    TLG_WriteReg(base_addr, REG_0084, deassert2_end);
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_SetExtVSync(uint32 base_addr,
                uint16 assert1_start,       uint16 assert1_end,
                uint16 assert2_start,       uint16 assert2_end,
                uint16 assert3_start,       uint16 assert3_end)
{

    TLG_WriteReg(base_addr, REG_0085, assert1_start);
    TLG_WriteReg(base_addr, REG_0086, assert1_end);
    TLG_WriteReg(base_addr, REG_0087, assert2_start);
    TLG_WriteReg(base_addr, REG_0088, assert2_end);
    TLG_WriteReg(base_addr, REG_0089, assert3_start);
    TLG_WriteReg(base_addr, REG_008A, assert3_end);

    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_SetHDecimate(uint32 base_addr, uint16 decimate)
{

    TLG_BOUNDS_CHECK(decimate, TLG_ON);

    TLG_SetBits(base_addr, REG_0071, decimate,
                REG_0071_BIT9_8_SHIFT,
                REG_0071_BIT9_8_MASK);
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_GetHDecimate(uint32 base_addr, uint16 *decimate)
{
    uint16 vidConfig4;

    TLG_NULLPTR_CHECK(decimate);

    TLG_ReadReg(base_addr, REG_0071, &vidConfig4);
    TLGHAL_GET(*decimate, vidConfig4, REG_0071_BIT9_8_SHIFT,
                REG_0071_BIT9_8_MASK);
    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_GetVidOutType(uint32 base_addr, uint16 *vidOutType)
{
    uint16 vidConfig3;

    TLG_NULLPTR_CHECK(vidOutType);

    TLG_ReadReg(base_addr, REG_0070, &vidConfig3);
    TLGHAL_GET(*vidOutType, vidConfig3, REG_0070_BIT6_5_SHIFT,
                REG_0070_BIT6_5_MASK);
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_SetVidOutType(uint32 base_addr, uint16 vidOutType)
{
    TLG_BOUNDS_CHECK(vidOutType, TLG_VID_DENC);

    TLG_SetBits(base_addr, REG_0070, vidOutType,
                REG_0070_BIT6_5_SHIFT,
                REG_0070_BIT6_5_MASK);
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_SetVGA_601_NTSC(uint32 base_addr)
{

    TLG_SetExtHSync(base_addr, 179, 818, 1, 22, 263, 285);
    TLG_SetExtVSync(base_addr, 1, 2, 18, 525,0,0);
    TLG_SetVidOutType(base_addr, TLG_VID_LCD);
    TLG_Set601VideoOutClock(base_addr, TLG_INVERT);
    TLG_SetHDecimate(base_addr, TLG_OFF);
    TLG_SetVSyncPolarity(base_addr, TLG_ACTIVE_LOW);
    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_SetVGA_601_PAL(uint32 base_addr)
{
    TLG_SetExtHSync(base_addr, 185, 824, 1, 22, 311, 335);
    TLG_SetExtVSync(base_addr, 1, 2,20,625,0,0);
    TLG_SetVidOutType(base_addr, TLG_VID_LCD);
    TLG_Set601VideoOutClock(base_addr, TLG_INVERT);
    TLG_SetHDecimate(base_addr, TLG_OFF);
    TLG_SetVSyncPolarity(base_addr, TLG_ACTIVE_LOW);
    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_Set240x240_LCD_NTSC(uint32 base_addr)
{

    TLG_SetExtHSync(base_addr, 259, 738, 1, 22, 263, 525);
    TLG_SetExtVSync(base_addr, 1, 2, 18, 525, 0, 0);
    TLG_SetHDecimate(base_addr, TLG_ON);
    TLG_Set601VideoOutClock(base_addr, TLG_NORMAL);
    TLG_SetVidOutType(base_addr, TLG_VID_LCD);
    TLG_SetVSyncPolarity(base_addr, TLG_ACTIVE_LOW);
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_Set240x240_LCD_PAL(uint32 base_addr)
{
    TLG_SetExtHSync(base_addr, 265, 744, 1, 47, 288, 625);
    TLG_SetExtVSync(base_addr, 1, 2, 45, 625, 0, 0);
    TLG_SetHDecimate(base_addr, TLG_ON);
    TLG_Set601VideoOutClock(base_addr, TLG_NORMAL);
    TLG_SetVidOutType(base_addr, TLG_VID_LCD);
    TLG_SetVSyncPolarity(base_addr, TLG_ACTIVE_LOW);
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_Set480x480_NTSC(uint32 base_addr)
{

    TLG_SetExtHSync(base_addr, 259, 738, 1, 22, 263, 285);
    TLG_SetExtVSync(base_addr, 1, 2, 18, 525, 0, 0);
    TLG_SetHDecimate(base_addr, TLG_OFF);
    TLG_Set601VideoOutClock(base_addr, TLG_INVERT);
    TLG_SetVidOutType(base_addr, TLG_VID_LCD);
    TLG_SetVSyncPolarity(base_addr, TLG_ACTIVE_LOW);
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_Set480x480_PAL(uint32 base_addr)
{
    TLG_SetExtHSync(base_addr, 265, 744, 1, 47, 287, 360);
    TLG_SetExtVSync(base_addr, 1, 2, 45, 625, 0, 0);
    TLG_SetHDecimate(base_addr, TLG_OFF);
    TLG_Set601VideoOutClock(base_addr, TLG_INVERT);
    TLG_SetVidOutType(base_addr, TLG_VID_LCD);
    TLG_SetVSyncPolarity(base_addr, TLG_ACTIVE_LOW);
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_Set640x480_NTSC(uint32 base_addr)
{
    TLG_SetExtHSync(base_addr, 179, 818, 1, 22, 263, 285);
    TLG_SetExtVSync(base_addr, 1, 2, 18, 525, 0, 0);
    TLG_SetHDecimate(base_addr, TLG_OFF);
    TLG_Set601VideoOutClock(base_addr, TLG_INVERT);
    TLG_SetVidOutType(base_addr, TLG_VID_LCD);
    TLG_SetVSyncPolarity(base_addr, TLG_ACTIVE_LOW);
    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_Set640x480_PAL(uint32 base_addr)
{
    TLG_SetExtHSync(base_addr, 179, 818, 1, 47, 287, 360);
    TLG_SetExtVSync(base_addr, 1, 2, 45, 625, 0, 0);
    TLG_SetHDecimate(base_addr, TLG_OFF);
    TLG_Set601VideoOutClock(base_addr, TLG_INVERT);
    TLG_SetVidOutType(base_addr, TLG_VID_LCD);
    TLG_SetVSyncPolarity(base_addr, TLG_ACTIVE_LOW);
    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_Set720x480_NTSC(uint32 base_addr)
{
    TLG_SetExtHSync(base_addr, 139, 858, 1, 22, 263, 285);
    TLG_SetExtVSync(base_addr, 1, 2, 18, 525, 0, 0);
    TLG_SetHDecimate(base_addr, TLG_OFF);
    TLG_Set601VideoOutClock(base_addr, TLG_INVERT);
    TLG_SetVidOutType(base_addr, TLG_VID_LCD);
    TLG_SetVSyncPolarity(base_addr, TLG_ACTIVE_LOW);
    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_Set720x576_PAL(uint32 base_addr)
{
    TLG_SetExtHSync(base_addr, 145, 864, 1, 22, 311, 335);
    TLG_SetExtVSync(base_addr, 1, 2, 20, 625, 0, 0);
    TLG_SetHDecimate(base_addr, TLG_OFF);
    TLG_Set601VideoOutClock(base_addr, TLG_INVERT);
    TLG_SetVidOutType(base_addr, TLG_VID_LCD);
    TLG_SetVSyncPolarity(base_addr, TLG_ACTIVE_LOW);
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_Set360x240_NTSC(uint32 base_addr)
{
    TLG_SetExtHSync(base_addr, 139, 858, 1, 22, 263, 525);
    TLG_SetExtVSync(base_addr, 1, 2, 18, 525, 0, 0);
    TLG_SetHDecimate(base_addr, TLG_ON);
    TLG_Set601VideoOutClock(base_addr, TLG_NORMAL);
    TLG_SetVidOutType(base_addr, TLG_VID_LCD);
    TLG_SetVSyncPolarity(base_addr, TLG_ACTIVE_LOW);
    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_Set360x288_PAL(uint32 base_addr)
{
    TLG_SetExtHSync(base_addr, 145, 864, 1, 22, 311, 625);
    TLG_SetExtVSync(base_addr, 1, 2, 20, 625, 0, 0);
    TLG_SetHDecimate(base_addr, TLG_ON);
    TLG_Set601VideoOutClock(base_addr, TLG_NORMAL);
    TLG_SetVidOutType(base_addr, TLG_VID_LCD);
    TLG_SetVSyncPolarity(base_addr, TLG_ACTIVE_LOW);
    return(TLG_ERR_SUCCESS);
}

TLGDLL_API int TLG_SetQVGA_NTSC(uint32 base_addr)
{
    TLG_SetExtHSync(base_addr, 179, 818, 1, 22, 263, 525);
    TLG_SetExtVSync(base_addr, 1, 2, 18, 525, 0, 0);
    TLG_SetHDecimate(base_addr, TLG_ON);
    TLG_Set601VideoOutClock(base_addr, TLG_NORMAL);
    TLG_SetVidOutType(base_addr, TLG_VID_LCD);
    TLG_SetVSyncPolarity(base_addr, TLG_ACTIVE_LOW);
    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_SetQVGA_PAL(uint32 base_addr)
{
    TLG_SetExtHSync(base_addr, 185, 824, 1, 46, 287, 625);
    TLG_SetExtVSync(base_addr, 1, 2, 45, 625, 0, 0);
    TLG_SetHDecimate(base_addr, TLG_ON);
    TLG_Set601VideoOutClock(base_addr, TLG_NORMAL);
    TLG_SetVidOutType(base_addr, TLG_VID_LCD);
    TLG_SetVSyncPolarity(base_addr, TLG_ACTIVE_LOW);
    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_GetHSyncPolarity(uint32 base_addr, uint16 *polarity)
{
    uint16 syncCfg;

    TLG_NULLPTR_CHECK(polarity);

    TLG_ReadReg(base_addr, REG_0078, &syncCfg);
    TLGHAL_GET(*polarity, syncCfg, REG_0078_BIT0_SHIFT,
                REG_0078_BIT0_MASK);
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_SetHSyncPolarity(uint32 base_addr, uint16 polarity)
{
    TLG_BOUNDS_CHECK(polarity, TLG_ON);

    TLG_SetBits(base_addr, REG_0078, polarity,
                REG_0078_BIT0_SHIFT,
                REG_0078_BIT0_MASK);
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_GetVSyncPolarity(uint32 base_addr, uint16 *polarity)
{
    uint16 syncCfg;

    TLG_NULLPTR_CHECK(polarity);

    TLG_ReadReg(base_addr, REG_0078, &syncCfg);
    TLGHAL_GET(*polarity, syncCfg, REG_0078_BIT1_SHIFT,
                REG_0078_BIT1_MASK);
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_SetVSyncPolarity(uint32 base_addr, uint16 polarity)
{

    TLG_BOUNDS_CHECK(polarity, TLG_ON);

    TLG_SetBits(base_addr, REG_0078, polarity,
                REG_0078_BIT1_SHIFT,
                REG_0078_BIT1_MASK);
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_SquelchEvenFrames(uint32 base_addr, uint16 squelch)
{
    TLG_BOUNDS_CHECK(squelch, TLG_ON);

    TLG_SetBits(base_addr, REG_0078, squelch,
                REG_0078_BIT10_SHIFT,
                REG_0078_BIT10_MASK);
    return(TLG_ERR_SUCCESS);
}

TLGDLL_API int TLG_GetGPIOEnable(uint32 base_addr, uint16 *value)
{
    uint16 gpioEnable;

    TLG_NULLPTR_CHECK(value);

    TLG_ReadReg(base_addr, REG_009E, &gpioEnable);
    TLGHAL_GET(*value, gpioEnable, REG_009E_BIT5_0_SHIFT,
                REG_009E_BIT5_0_MASK);

    return (TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_SetGPIOEnable(uint32 base_addr, uint16 mask, uint16 value)
{
    uint16 gpioEnable;
    uint16 gpios;

    TLG_BOUNDS_CHECK(mask,  TLG_GPIO1|TLG_GPIO2|TLG_GPIO3|TLG_GPIO4|TLG_GPIO5|TLG_GPIO6);
    TLG_BOUNDS_CHECK(value, TLG_GPIO1|TLG_GPIO2|TLG_GPIO3|TLG_GPIO4|TLG_GPIO5|TLG_GPIO6);

    TLG_ReadReg(base_addr, REG_009E, &gpioEnable);
    TLGHAL_GET(gpios, gpioEnable, REG_009E_BIT5_0_SHIFT,
                REG_009E_BIT5_0_MASK);
    value = (value & mask) | (gpios & ~mask);
    TLGHAL_SET(gpioEnable, value, REG_009E_BIT5_0_SHIFT,
                 REG_009E_BIT5_0_MASK);
    TLG_WriteReg(base_addr, REG_009E, gpioEnable);

    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_GetGPIOOutput(uint32 base_addr, uint16 *value) 
{
    uint16 gpioOutput;

    TLG_NULLPTR_CHECK(value);

    TLG_ReadReg(base_addr, REG_009D, &gpioOutput);
    TLGHAL_GET(*value, gpioOutput, REG_009E_BIT5_0_SHIFT,
                REG_009D_BIT5_0_MASK);

    return (TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_SetGPIOOutput(uint32 base_addr, uint16 mask, uint16 value)
{
    uint16 gpioOutput;
    uint16 gpios;

    TLG_BOUNDS_CHECK(mask,  TLG_GPIO1|TLG_GPIO2|TLG_GPIO3|TLG_GPIO4|TLG_GPIO5|TLG_GPIO6);
    TLG_BOUNDS_CHECK(value, TLG_GPIO1|TLG_GPIO2|TLG_GPIO3|TLG_GPIO4|TLG_GPIO5|TLG_GPIO6);

    TLG_ReadReg(base_addr, REG_009D, &gpioOutput);
    TLGHAL_GET(gpios, gpioOutput, REG_009E_BIT5_0_SHIFT,
                REG_009D_BIT5_0_MASK);
    value = (value & mask) | (gpios & ~mask);
    TLGHAL_SET(gpioOutput, value, REG_009E_BIT5_0_SHIFT,
                REG_009D_BIT5_0_MASK);
    TLG_WriteReg(base_addr, REG_009D, gpioOutput);

    return(TLG_ERR_SUCCESS);
}

const uint8 tlg_aud_cic_rate[]  = {   255,    209,    152,    139,    127};
const uint8 tlg_aud_deci2_sel[] = {   0,      0,      1,      1,      1};
const uint8 tlg_aud_mlen[]      = {   8,      7,      6,      5,      4};

const uint8 tlg_aud_slen[]      = {   64,     60,     51,     56,      64};


TLGDLL_API int TLG_GetAudioSampleRate(uint32 base_addr, uint16 *rate)
{
    uint16 cic_rate;
    int i;

    TLG_NULLPTR_CHECK(rate);

    *rate = TLG_AUD_RATE_NONE;

    TLG_GetI2SAudioData(base_addr,&cic_rate);

    for (i = 0; i < TLG_AUD_RATE_52_734KHZ; i++)
    {
        if (cic_rate  == tlg_aud_cic_rate[i])
        {
            *rate = i + 1;
            break;
        }
    }
    return TLG_ERR_SUCCESS;
}


TLGDLL_API int TLG_SetAudioSampleRate(uint32 base_addr, uint16 rate)
{
    uint16 reg;
    uint16 fm_lp_select;

    TLG_BOUNDS_CHECK(rate, TLG_AUD_RATE_52_734KHZ);

    if (rate != TLG_AUD_RATE_NONE)
    {
    TLG_CHIP_VERS_2_1_BEGIN
 #if defined (TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)
        {

            TLG_ReadReg(base_addr, REG_00B0, &reg);
            TLGHAL_SET(reg, tlg_aud_cic_rate[rate-1],
                        REG_00B0_BIT15_8_SHIFT,
                        REG_00B0_BIT15_8_MASK);
            TLG_WriteReg(base_addr, REG_00B0, reg);
                            fm_lp_select = (tlg_aud_cic_rate[rate-1] < 209) ? 1 : 0;
            TLG_SetBits(base_addr, REG_00B4, fm_lp_select,
                        REG_00B4_BIT3_SHIFT,
                        REG_00B4_BIT3_MASK);
        }

    #endif 
    TLG_CHIP_VERS_ELSE
    
    #if defined(TLG_CMP_CHIP_1120_1_2) ||defined(TLG_CMP_CHIP_1120_1_1)
        {
            uint8 temp;
            #ifndef USE_POSEIDON
            
            TLG_ReadReg(base_addr, REG_008E, &reg);
            temp = tlg_aud_cic_rate[rate-1] + 1;
            TLGHAL_SET(reg, temp,
            REG_008E_BIT7_0_SHIFT,
            REG_008E_BIT7_0_MASK);
            TLG_WriteReg(base_addr, REG_008E, reg);
            #endif 
            fm_lp_select = (rate  >  TLG_AUD_RATE_32_143KHZ) ? 1 : 0;
            TLG_SetBits(base_addr, REG_008F, fm_lp_select,
            REG_008F_BIT0_SHIFT,
            REG_008F_BIT0_MASK);
        }
    #endif
    TLG_CHIP_VERS_END
        {
            TLG_ReadReg(base_addr, REG_0096, &reg);
            TLGHAL_SET(reg, tlg_aud_mlen[rate-1],
                        REG_0096_BIT12_8_SHIFT,
                        REG_0096_BIT12_8_MASK);
            TLGHAL_SET(reg,
                        tlg_aud_slen[rate-1],
                        REG_0096_BIT7_0_SHIFT,
                        REG_0096_BIT7_0_MASK);
            TLG_WriteReg(base_addr, REG_0096, reg);
        }

    }


    return TLG_ERR_SUCCESS;
}


TLGDLL_API int TLG_DeepSleep(uint32 base_addr, tlg_statep state)
{
    uint16 reg;

    TLG_NULLPTR_CHECK(state);

    if (state[0] != TLG_STATE0)
            return TLG_ERR_FAIL;

    
    TLG_ReadReg(base_addr, REG_0180    , &state[1]);
    TLG_ReadReg(base_addr, REG_0181  , &state[2]);
    TLG_ReadReg(base_addr, REG_01A7     , &state[3]);
    TLG_ReadReg(base_addr, REG_01A8, &state[4]);

    
    TLG_WriteReg(base_addr, REG_0180,      0xffff);

    
    TLG_ReadReg(base_addr,  REG_018D,        &reg);
    if (reg & (REG_018D_BIT15_MASK | REG_018D_BIT0_MASK))
    {
        TLGHAL_CLEAR(reg,       REG_018D_BIT15_MASK);
        TLGHAL_CLEAR(reg,       REG_018D_BIT0_MASK);
        TLG_WriteReg(base_addr, REG_018D,        reg);
    }
    TLG_WriteReg(base_addr, REG_0181,    0xffff);

    
    TLG_ReadReg(base_addr,  REG_01A7,       &reg);
    TLGHAL_CLEAR(reg,       REG_01A7_BIT8_MASK);
    TLG_WriteReg(base_addr, REG_01A7,       reg);

    
    reg = state[4];
    TLGHAL_CLEAR(reg,       REG_01A8_BIT11_9_MASK);
    TLG_WriteReg(base_addr, REG_01A8,  reg);

    
    TLG_ReadReg(base_addr,  REG_0000, &reg);
    TLGHAL_SET(reg, TLG_ON, REG_0000_BIT7_SHIFT,
                REG_0000_BIT7_MASK);
    TLG_WriteReg(base_addr, REG_0000, reg);
    TLGHAL_SET(reg, TLG_ON, REG_0000_BIT2_SHIFT,
                REG_0000_BIT2_MASK);
    TLGHAL_SET(reg, TLG_ON, REG_0000_BIT0_SHIFT,
                REG_0000_BIT0_MASK);
    TLG_WriteReg(base_addr, REG_0000, reg);

    TLG_ClearRegShadow(base_addr);

    state[0] = TLG_STATE1;

    return TLG_ERR_SUCCESS;
}


TLGDLL_API int TLG_ReturnFromDeepSleep(uint32 base_addr, tlg_statep state)
{
    return TLG_ERR_SUCCESS;
}


TLGDLL_API int TLG_GetHsyncLowPower(uint32 base_addr, uint16 *val)
{
    uint16 syncCfg;

    TLG_NULLPTR_CHECK(val);

    TLG_ReadReg(base_addr, REG_0078, &syncCfg);
    TLGHAL_GET(*val, syncCfg, REG_0078_BIT6_SHIFT,
                REG_0078_BIT6_MASK);
    return TLG_ERR_SUCCESS;
}


TLGDLL_API int TLG_SetHsyncLowPower(uint32 base_addr, uint16 val)
{
    uint16 syncCfg;

    TLG_BOUNDS_CHECK(val, TLG_ON);

    TLG_ReadReg(base_addr, REG_0078, &syncCfg);
    if (syncCfg & REG_0078_BIT0_MASK)
    {
        TLGHAL_CLEAR(syncCfg, REG_0078_BIT6_MASK);
        TLGHAL_SET(syncCfg, val, REG_0078_BIT7_SHIFT,
                    REG_0078_BIT7_MASK);
    } 
    else 
    {
        TLGHAL_CLEAR(syncCfg, REG_0078_BIT7_MASK);
        TLGHAL_SET(syncCfg, val, REG_0078_BIT6_SHIFT,
                    REG_0078_BIT6_MASK);
    }
    TLG_WriteReg(base_addr, REG_0078, syncCfg);
    return TLG_ERR_SUCCESS;
}


TLGDLL_API int TLG_GetVDecimate(uint32 base_addr, uint16 *val)
{
    uint16 syncCfg;
    uint16 VDecimateBits;
    uint16 i;

    TLG_NULLPTR_CHECK(val);


    TLG_ReadReg(base_addr, REG_0078, &syncCfg);
    TLGHAL_GET(VDecimateBits, syncCfg, REG_0078_BIT14_12_SHIFT,
                REG_0078_BIT14_12_MASK);
    *val = 0;
    for (i = 0; i < 3; i++)
    {
        if (VDecimateBits & (1 << i))
        {
            *val = i + 1;
            break;
        }
    }
    return TLG_ERR_SUCCESS;
}


TLGDLL_API int TLG_SetVDecimate(uint32 base_addr, uint16 val)
{
    uint16 VDecimateBits;
    TLG_BOUNDS_CHECK(val, 4);

    
    VDecimateBits = (1 << val) >> 1;
    TLG_SetBits(base_addr, REG_0078, VDecimateBits,
                REG_0078_BIT14_12_SHIFT,
                REG_0078_BIT14_12_MASK);
    return TLG_ERR_SUCCESS;
}
static int TLG_GetMVEMode(uint32 base_addr)
{
    uint16 fAvg;
    uint16 fAvg_auto;
    uint16 fAvg_auto2;
    uint16 fAvg_auto3;

    TLG_ReadReg(base_addr, REG_006C,  &fAvg);
    TLGHAL_GET(fAvg_auto, fAvg, REG_006C_BIT8_SHIFT,
                REG_006C_BIT8_MASK);
    TLGHAL_GET(fAvg_auto2, fAvg, REG_006C_BIT9_SHIFT,
                REG_006C_BIT9_MASK);
    TLGHAL_GET(fAvg_auto3, fAvg, REG_006C_BIT10_SHIFT,
                REG_006C_BIT10_MASK);

    
    return  ((fAvg_auto  == TLG_ON)  || 
             (fAvg_auto2 == TLG_ON)  || 
             (fAvg_auto3 == TLG_ON)) ? 1 : 0;
}


TLGDLL_API int TLG_GetMobileVidEnhancement(uint32 base_addr, uint16 *mode,
                uint16 *val) 
{
    uint16 videoConfig10;
    uint16 mve_mode;

    TLG_NULLPTR_CHECK(mode);
    TLG_NULLPTR_CHECK(val);

    

    
    if (g_tlg_cc_mve != TLG_MVE_NONE)
        *mode = g_tlg_cc_mve;
    else
    {
        mve_mode = TLG_GetMVEMode(base_addr);;
        *mode = mve_mode;
    }
    TLG_REG_LOG((b,"MVE MODE = %d, CC MODE = %d\n", mve_mode, g_tlg_cc_mve));


    TLG_ReadReg(base_addr, REG_0077,  &videoConfig10);


    TLGHAL_GET(*val, videoConfig10, REG_0077_BIT5_0_SHIFT,
                REG_0077_BIT5_0_MASK);

    return(TLG_ERR_SUCCESS);
}


static int TLG_SetMobileVidEnhancementAux(uint32 base_addr, uint16 mode,
                uint16 val) 
{

    
    TLG_ResetSFA();

    
    if (g_tlg_disable_mve)
    {
        mode = TLG_MVE_MANUAL;
        val  = 0;
    }

    TLG_BOUNDS_CHECK(val, TLG_MVE_A_MAX);
    TLG_BOUNDS_CHECK(mode, TLG_MVE_MAX);

    TLG_REG_LOG((b, "MVE MODE SET = %d\n", mode));

    if (mode == TLG_MVE_AUTO) 
    {
        
        
         TLG_SetBits(base_addr, REG_006C, TLG_ON,
                        REG_006C_BIT9_SHIFT,
                    REG_006C_BIT9_MASK);

        
         TLG_SetBits(base_addr, REG_0077, 0x34,
                        REG_0077_BIT5_0_SHIFT,
                        REG_0077_BIT5_0_MASK);
    }
    else 
    {
        
        
         TLG_SetBits(base_addr, REG_006C, TLG_OFF,
                        REG_006C_BIT9_SHIFT,
                    REG_006C_BIT9_MASK);

        
         TLG_SetBits(base_addr, REG_0077, val,
                        REG_0077_BIT5_0_SHIFT,
                    REG_0077_BIT5_0_MASK);

        
        TLG_SetBits(base_addr,
                    REG_005A_VALUE_7,
                    tlg_sfa_tgt[TLG_SFA_HIGH_SNR],
                    REG_005A_BIT15_8_SHIFT,
                    REG_005A_BIT15_8_MASK);

    }
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_SetMobileVidEnhancement(uint32 base_addr, uint16 mode,
                uint16 val) 
{
    
                 
    if (g_tlg_cc_mve == TLG_MVE_NONE)
        return TLG_SetMobileVidEnhancementAux(base_addr, mode, val);
    else if (g_tlg_cc_mve == TLG_MVE_AUTO) 
    {
        
        
        
        
        if (mode == TLG_MVE_MANUAL)
        {
            g_tlg_cc_mve = TLG_MVE_NONE;
            return TLG_SetMobileVidEnhancementAux(base_addr, mode, val);
        }
    }
    else
    {
        TLG_REG_LOG((b,"Bad state. cc_mve = %d\n", g_tlg_cc_mve));
    }

    return TLG_ERR_SUCCESS;
}

tlg_reg_pair_t tlg_hard_reset_tbl[] =
{
    {0x180, 0xffff},
    {0x181, 0xffff},
    
    {0x183, 0x0000},
    {0x184, 0x0000},
    {0x185, 0x0052},
    {0x186, 0x0000},
    {0x187, 0x0000},
    {0x188, 0x0052},
    {0x189, 0x0000},
    {0x18a, 0x0000},
    {0x18b, 0x0052},
    {0x18c, 0x0000},
    {0x18e, 0x0300},
    {0x18f, 0xaa80},
    {0x190, 0x0000},
    {0x191, 0x0000},
    {0x192, 0x0000},
    {0x193, 0x0000},
    {0x194, 0x0000},
    {0x195, 0x0000},
    {0x196, 0x0000},
    {0x197, 0x0000},
    {0x198, 0x0063},
    
    {0x19b, 0x9400},
    
    {0x19e, 0x0000},
    
    {0x1a3, 0x0000},
    {0x1a4, 0x0202},
    {0x1a5, 0x0390},
    {0x1a6, 0x1a6c},
    {0x1a7, 0x0044},
    {0x1a8, 0x048a},
    {0x1a9, 0x0000},
    
    {0x1b0, 0x0000},
    {0x1b1, 0x0040},
    {0x1b2, 0x1f90},
    {0x1b3, 0x27b0},
    {0x1b4, 0x0061},
    {0x1b5, 0x0244},
    {0x1b6, 0x0000},
    {0x1b7, 0x06c1},
    {0x1b8, 0x0000},
    {0xFFFF, 0x0000}
};


TLGDLL_API int TLG_HardReset(uint32 base_addr) 
{

    tlg_reg_pair_t *p_tbl = tlg_hard_reset_tbl;
    uint16 reg;

    
    TLG_ReadRegNS(base_addr,REG_0000,&reg);
    TLG_Delay(5);

    
    TLG_ReadReg(base_addr,REG_0000, &reg);
    TLGHAL_SET(reg, 1, REG_0000_BIT6_SHIFT, REG_0000_BIT6_MASK);
    TLG_WriteReg(base_addr,REG_0000,reg);
    TLG_Delay(5);

    TLG_ReadReg(base_addr,REG_0000, &reg);
    TLGHAL_SET(reg,0, REG_0000_BIT6_SHIFT, REG_0000_BIT6_MASK);
    TLG_WriteReg(base_addr,REG_0000,reg);
    TLG_Delay(5);

    
    TLG_ReadReg(base_addr, REG_0001, &reg);
    TLGHAL_SET(reg,1, REG_0001_BIT0_SHIFT, REG_0001_BIT0_MASK);
    TLG_WriteReg(base_addr, REG_0001, reg);
    TLG_Delay(5);

    
    while ((p_tbl) && (p_tbl->reg != 0xFFFF))
    {
        TLG_WriteReg(base_addr, p_tbl->reg, p_tbl->value);
        p_tbl++;
    }

    return TLG_ERR_SUCCESS;
}



TLGDLL_API int TLG_LockVScreen(uint32 base_addr) 
{
    uint16 reg;
    uint16 valTmp;

    TLG_ReadReg(base_addr, REG_0071, &reg);
    TLGHAL_GET(valTmp,reg,REG_0071_BIT7_4_SHIFT, REG_0071_BIT7_4_MASK);
    if (valTmp) 
    {
        TLGHAL_SET(reg, 0, REG_0071_BIT7_4_SHIFT,
                    REG_0071_BIT7_4_MASK);
        TLG_WriteReg(base_addr, REG_0071, reg);
    }
    return TLG_ERR_SUCCESS;
}


TLGDLL_API int TLG_UnlockVScreen(uint32 base_addr) 
{
    uint16 reg;
    uint16 valTmp;

    TLG_ReadReg(base_addr, REG_0071, &reg);
    TLGHAL_GET(valTmp,reg,REG_0071_BIT7_4_SHIFT, REG_0071_BIT7_4_MASK);
    if (valTmp==0) 
    {
        TLGHAL_SET(reg, 1, REG_0071_BIT7_4_SHIFT,
                    REG_0071_BIT7_4_MASK);
        TLG_WriteReg(base_addr, REG_0071, reg);
    }
    return TLG_ERR_SUCCESS;
}

int tlg_log10(uint16 number)
{
    uint16 i = 0;
    while (number > 0)
    {
        number /= 10;
        i++;
    }
    return(i);
}


TLGDLL_API int TLG_GetGains(uint32 base_addr, 
                int *p_lna1, 
                int *p_lna2, 
                int *p_agc1, 
                int *p_agc2, 
                int *p_agcd, 
                int *p_levl, 
                int *p_leva,
                uint16 *p_totalGain 
                )
{
    uint16 data;
#define  MAGIC_NUM 100

    int res = TLG_ERR_SUCCESS;
    int fix = 32;
    uint16 i_lna1, i_lna2, i_agc1, i_agc2, i_levl, i_leva, i_agcd;
    int lna_gain, lna1, lna2, agc1, agc2, agcd, levl, leva;
    uint16 i_lna_gain;
    TLG_NULLPTR_CHECK(p_lna1); 
    TLG_NULLPTR_CHECK(p_lna2); 
    TLG_NULLPTR_CHECK(p_agc1); 
    TLG_NULLPTR_CHECK(p_agc2); 
    TLG_NULLPTR_CHECK(p_agcd); 
    TLG_NULLPTR_CHECK(p_levl); 
    TLG_NULLPTR_CHECK(p_leva);
    TLG_NULLPTR_CHECK(p_totalGain);

    
    TLG_ReadRegNS(base_addr, REG_0113, &data);
    TLGHAL_GET(i_lna1, data, 
                REG_0113_BIT14_8_SHIFT, 
                REG_0113_BIT14_8_MASK);
    dbg_printf((3, "i_lna1 = %0d\n", i_lna1));
    
    TLG_ReadRegNS(base_addr, REG_0114, &data);
    TLGHAL_GET(i_lna2, data, 
                REG_0114_BIT14_8_SHIFT, 
                REG_0114_BIT14_8_MASK);
    dbg_printf((3, "i_lna2 = %0d\n", i_lna2));
    
    TLG_ReadRegNS(base_addr, REG_010C, &data);
    TLGHAL_GET(i_agc1, data, 
                REG_010C_BIT13_8_SHIFT, 
                REG_010C_BIT13_8_MASK);
    dbg_printf((3, "i_agc1 = %0d\n", i_agc1));
    
    TLGHAL_GET(i_agc2, data, 
                REG_010C_BIT6_0_SHIFT, 
                REG_010C_BIT6_0_MASK);
    dbg_printf((3, "i_agc2 = %0d\n", i_agc2));
    
    TLG_ReadRegNS(base_addr, REG_0123, &i_agcd);
    dbg_printf((3, "i_agcd = %0d\n", i_agcd));
    
    TLG_ReadRegNS(base_addr, REG_0115, &data);
    TLGHAL_GET(i_levl, data, 
                REG_0115_BIT9_0_SHIFT, 
                REG_0115_BIT9_0_MASK);
    dbg_printf((3, "i_levl = %0d\n", i_levl));
    
    TLG_ReadRegNS(base_addr, REG_0112, &data);
    TLGHAL_GET(i_leva, data, 
                REG_0112_BIT15_8_SHIFT, 
                REG_0112_BIT15_8_MASK);
    dbg_printf((3, "i_leva = %0d\n", i_leva));

    
    agc1 = (int) (i_agc1 * MAGIC_NUM) / 2; 
    
    agc2 = (int) ((i_agc2*33) -(12* MAGIC_NUM)); 
    if (i_agcd)
    {
            
        agcd = (int) (20*( tlg_log10((uint16)(((long int)i_agcd*10000 ) / 1024))-4)) * MAGIC_NUM;
    }
    else                              
    {
        agcd = -40 * MAGIC_NUM;                                             
    }
    
    lna1 = (int)(i_lna1 - 59) * 66;  
    if (i_lna2 < 128 && i_lna2 >110) 
    {
        lna2 = (((int) i_lna2 - 127) /2)  * MAGIC_NUM;
    }
    else if (i_lna2 < 111 && i_lna2 > 93) 
    {
        lna2 = ((-9 + ((int) i_lna2 - 110)) /2)  * MAGIC_NUM;     
    }
    else if (i_lna2 < 94 && i_lna2 > 78) 
    {
            
        lna2 =  ((-51 + ((int) i_lna2 - 93) * 2) * MAGIC_NUM) / 4;
    }
    else 
    {
        lna2 = -40 * MAGIC_NUM;
    }
    levl = (int) i_levl * MAGIC_NUM;
    leva = (int) i_leva * MAGIC_NUM;
    TLG_ReadRegNS(base_addr,    REG_0180, &data);
    
    if ( ((data & REG_0180_BIT14_MASK) == 0) || 
                    ((data & REG_0180_BIT13_MASK) == 0) )
    {
            
        lna_gain = (lna1 + agc1 + agc2 + agcd) / MAGIC_NUM + fix;
        i_lna_gain = (lna1 + agc1 + agc2 + agcd) % MAGIC_NUM;
        if ( i_lna_gain > 50)
        {
            lna_gain += 1;
        }           
        dbg_printf((3, "vhf1: lna_gain %d, i_lna_gain %d, lna1 %d, lna2 %d, agc1 %d, agc2 %d, agcd %d, levl %d, leva %d\n", 
                            lna_gain, lna1, i_lna_gain, lna2, agc1, agc2, agcd, levl, leva));
    }
    else
    {
            
        lna_gain = (lna2 + agc1 + agc2 + agcd) / MAGIC_NUM + fix;
        i_lna_gain = (lna2 + agc1 + agc2 + agcd) % MAGIC_NUM;
        if ( i_lna_gain > 50)
        {
            lna_gain += 1;
        }
        dbg_printf((3, "uhf or vhf3: lna_gain %d,  i_lna_gain %d, lna1 %d, lna2 %d, agc1 %d, agc2 %d, agcd %d, levl %d, leva %d\n", 
                            lna_gain, i_lna_gain, lna1, lna2, agc1, agc2, agcd, levl, leva));
    }
    if (res == TLG_ERR_SUCCESS)
    {
        *p_lna1 = lna1 / MAGIC_NUM;
        *p_lna2 = lna2 / MAGIC_NUM;
        *p_agc1 = agc1/ MAGIC_NUM;
        *p_agc2 = agc2/ MAGIC_NUM;
        *p_agcd = agcd/ MAGIC_NUM;
        *p_levl = levl/ MAGIC_NUM;
        *p_leva = leva/ MAGIC_NUM;
        *p_totalGain = (uint16)lna_gain;
    }
    return(res);
}

static void TLG_ResetSFA(void)
{
    g_tlg_sfa_cur_val = TLG_SFA_NO_VAL;
    g_tlg_sfa_cur_tgt = 0;
}


static void TLG_SetSFA(uint32 base_addr, uint16 vstd, uint16 sfa_idx)
{
    int           mve_mode      = TLG_GetMVEMode(base_addr);
    int           is_secam      = TLG_IsSECAMChnStandard(vstd);

    
    if (mve_mode == TLG_MVE_MANUAL && 
        g_tlg_cc_mve == TLG_MVE_NONE)
        return;

    if (g_tlg_sfa_cur_val != tlg_sfa_val[sfa_idx])
    {
        TLG_REG_LOG((b, "Adjust SFA from %d to %d\n",
                        g_tlg_sfa_cur_val, tlg_sfa_val[sfa_idx]));

        g_tlg_sfa_cur_val = tlg_sfa_val[sfa_idx];
        
        
        
        if (g_tlg_cc_mve == TLG_MVE_AUTO)
        {
            TLG_SetBits(base_addr, 
                        REG_0077,
                        g_tlg_sfa_cur_val*4,   
                        REG_0077_BIT5_0_SHIFT,
                        REG_0077_BIT5_0_MASK);
        }
        TLG_SetBits(base_addr,
                    REG_006C,
                    g_tlg_sfa_cur_val,
                    REG_006C_BIT15_12_SHIFT,
                    REG_006C_BIT15_12_MASK);

    }

    if (g_tlg_sfa_cur_tgt != tlg_sfa_tgt[sfa_idx])
    {
        g_tlg_sfa_cur_tgt = tlg_sfa_tgt[sfa_idx];

        
        if (!is_secam)
            TLG_SetBits(base_addr,
                        REG_005A_VALUE_7,
                        tlg_sfa_tgt[sfa_idx],
                        REG_005A_BIT15_8_SHIFT,
                        REG_005A_BIT15_8_MASK);
    }
}


static void TLG_SFA(uint32 base_addr, uint16 vsync_min, uint16 vstd)
{
    const uint16 *thresh = tlg_sfa_ntsc_pal_thresh;
    int           i; 

    if (TLG_IsSECAMChnStandard(vstd))
    {
            thresh = tlg_sfa_secam_thresh;
    }

    
    for (i=0; i < TLG_SFA_NUM_THRESH; i++)
        if (vsync_min > thresh[i])
        {
            break;      
        }

    
    TLG_SetSFA(base_addr, vstd, i);

}



#define TLG_VSYNC_MIN_LOW_THRESHOLD         (0xa0)
#define TLG_VSYNC_MIN_HIGH_THRESHOLD        (0xe0)
#define TLG_VSYNC_MIN_LOW_THRESHOLD_SECAM   (0x40)
#define TLG_VSYNC_MIN_HIGH_THRESHOLD_SECAM  (0x60)

#define TLG_VSYNC_MIN_REG5A_THRESHOLD_1 (0x160) 
#define TLG_VSYNC_MIN_REG5A_THRESHOLD_2 (0x190)
#define TLG_VSYNC_MIN_REG5A_THRESHOLD_3 (0x240)
#define TLG_VSYNC_MIN_REG5A_THRESHOLD_4 (0x2B0)

#define TLG_VSYNC_MIN_SECAM_COLOR_THRESHOLD_1 (0x68)
#define TLG_VSYNC_MIN_SECAM_COLOR_THRESHOLD_2 (0x82)
#define TLG_VSYNC_MIN_SECAM_COLOR_THRESHOLD_3 (0x98)
#define TLG_VSYNC_MIN_SECAM_COLOR_THRESHOLD_4 (0xb8)
#define TLG_VSYNC_MIN_SECAM_COLOR_THRESHOLD_5 (0x100)
#define TLG_VSYNC_MIN_SECAM_COLOR_THRESHOLD_6 (0x160)

#define TLG_VSYNC_MIN_SECAM_FLICKER_THRESHOLD (0x38)

static void TLG_ASA(uint32 base_addr, uint16 vsync_min, uint16 vsync_locked)
{

    int    new_mode     = g_reg5a_mode;
    uint16 thresh_high  = TLG_VSYNC_MIN_HIGH_THRESHOLD;
    uint16 thresh_low   = TLG_VSYNC_MIN_LOW_THRESHOLD;
    uint16 vstd;

    TLG_GetChnStandard(base_addr, &vstd);

    if (TLG_IsSECAMChnStandard(vstd))
    {
        thresh_high = TLG_VSYNC_MIN_HIGH_THRESHOLD_SECAM;
        thresh_low = TLG_VSYNC_MIN_LOW_THRESHOLD_SECAM;
    }        
    if (!vsync_locked)
    {
        
        vsync_min = 0x2b1;
    }

    TLG_REG_LOG((b,"vsync_min=(%d)\n",  vsync_min));

    if (vsync_min > thresh_high)
    {
        if (snr_mode != TLG_MODE_LOW_SNR)
        {
            TLG_REG_LOG((b,"going to low SNR mode (%d)\n", vsync_min));
            TLG_ChangeSNRmode(base_addr, TLG_MODE_LOW_SNR, vstd);
        }
    }
    else if (vsync_min < thresh_low)
    {
        if (snr_mode != TLG_MODE_HIGH_SNR)
        {            
            TLG_REG_LOG((b,"going to high SNR mode (%d)\n", vsync_min));
            TLG_ChangeSNRmode(base_addr, TLG_MODE_HIGH_SNR, vstd);
        }
    }

    
    
    
    
    
    if (vsync_min >= TLG_VSYNC_MIN_REG5A_THRESHOLD_4)
    {
        new_mode = TLG_REG5A_MODE_3;
    } 
    else if (vsync_min <= TLG_VSYNC_MIN_REG5A_THRESHOLD_1)
    {
        new_mode = TLG_REG5A_MODE_1;
    }
    else if (vsync_min >= TLG_VSYNC_MIN_REG5A_THRESHOLD_2 &&
             g_reg5a_mode == TLG_REG5A_MODE_1)
    {
        new_mode = TLG_REG5A_MODE_2;
    }
    else if (vsync_min <= TLG_VSYNC_MIN_REG5A_THRESHOLD_3 &&
             g_reg5a_mode == TLG_REG5A_MODE_3)
    {
        new_mode = TLG_REG5A_MODE_2;
    }
    if (new_mode != g_reg5a_mode)
    {
        TLG_REG_LOG((b,"TVSSI: Changing 5a mode to %d\n", new_mode));
        TLG_ChangeREG5Amode(base_addr, new_mode, vstd);
    }

    if (TLG_IsSECAMChnStandard(vstd))
    {
        int new_secam_mode = g_secam_color_mode;
        int new_secam_flicker_mode = g_secam_flicker_mode;
        
        
        
                
        if (vsync_min <= TLG_VSYNC_MIN_SECAM_COLOR_THRESHOLD_1)
        {
            new_secam_mode = TLG_SECAM_COLOR_MODE_1;
        }
        else if (vsync_min > TLG_VSYNC_MIN_SECAM_COLOR_THRESHOLD_6)
        {
            new_secam_mode = TLG_SECAM_COLOR_MODE_4;
        }
        else if (g_secam_color_mode == TLG_SECAM_COLOR_MODE_1)
        {
            

            if (vsync_min > TLG_VSYNC_MIN_SECAM_COLOR_THRESHOLD_4)
            {
                
                new_secam_mode = TLG_SECAM_COLOR_MODE_3;
            }
            else if (vsync_min > TLG_VSYNC_MIN_SECAM_COLOR_THRESHOLD_2)
            {
                
                new_secam_mode = TLG_SECAM_COLOR_MODE_2;
            }
              
        }
        else if (g_secam_color_mode == TLG_SECAM_COLOR_MODE_2)
        {
            

            if (vsync_min > TLG_VSYNC_MIN_SECAM_COLOR_THRESHOLD_4)
            {
                
                new_secam_mode = TLG_SECAM_COLOR_MODE_3;
            }
                    
        }
        else if (g_secam_color_mode == TLG_SECAM_COLOR_MODE_3)
        {
            

            if (vsync_min <= TLG_VSYNC_MIN_SECAM_COLOR_THRESHOLD_3)
            {
                
                new_secam_mode = TLG_SECAM_COLOR_MODE_2;
            }
            
        }
        else if (g_secam_color_mode == TLG_SECAM_COLOR_MODE_4)
        {
            

            if (vsync_min <= TLG_VSYNC_MIN_SECAM_COLOR_THRESHOLD_3)
            {
                
                new_secam_mode = TLG_SECAM_COLOR_MODE_2;
            }
            else if (vsync_min <= TLG_VSYNC_MIN_SECAM_COLOR_THRESHOLD_5)
            {
                
                new_secam_mode = TLG_SECAM_COLOR_MODE_3;
            }
            
        
        }
        if (new_secam_mode != g_secam_color_mode)
        {
            TLG_REG_LOG((b,"TVSSI: Changing secam color mode to %d\n", new_secam_mode));
            TLG_ChangeSecamColorMode(base_addr, new_secam_mode);
        }
        
        
        if (vsync_min <= TLG_VSYNC_MIN_SECAM_FLICKER_THRESHOLD)
        {
            new_secam_flicker_mode = TLG_SECAM_FLICKER_MODE_1;
        }
        else
        {
            new_secam_flicker_mode = TLG_SECAM_FLICKER_MODE_2;
        }
        if (new_secam_flicker_mode != g_secam_flicker_mode)
        {
            TLG_ChangeSecamFlickerMode(base_addr, new_secam_flicker_mode);
        }
    }

    
    TLG_SFA(base_addr, vsync_min, vstd);
}



TLGDLL_API int TLG_GetTVSSI(uint32 base_addr, uint16 *metric) 
{
    uint16  reg, lock, vsync_min;
    uint32  agc1, agc2;
    bool    restart, vsync_locked = 1;
    uint32  divisor;
#define USE_ZEUS_TVSSI
#ifdef USE_ZEUS_TVSSI
    int     agc_metric, vsync_metric;

    uint32  agc1_max =  REG_010E_BIT15_10_MASK
            >> REG_010E_BIT15_10_SHIFT;
    uint32  agc2_max =  REG_010D_BIT15_9_MASK
            >> REG_010D_BIT15_9_SHIFT;
    

    TLG_NULLPTR_CHECK(metric);

    TLG_ReadRegNS(base_addr, REG_010D, &reg);
    TLGHAL_GET(agc2, reg,    REG_010D_BIT15_9_SHIFT,
                REG_010D_BIT15_9_MASK);

    TLG_ReadRegNS(base_addr, REG_010E, &reg);
    TLGHAL_GET(agc1, reg,    REG_010E_BIT15_10_SHIFT,
                REG_010E_BIT15_10_MASK);

    TLG_REG_LOG((b, "raw: agc1=%d, agc2 = %d\n", agc1, agc2));
    TLG_AvgFiltGetNext(&tlg_agc2_filt, &agc2, &divisor);
    TLG_AvgFiltGetNext(&tlg_agc1_filt, &agc1, &divisor);
    agc1 /= divisor;
    agc2 /= divisor;

    
    TLG_ReadRegNS(base_addr, REG_013B, &lock);

    
    if (agc2 >= agc2_max/2)
            agc_metric = (2 - (agc1*2)/agc1_max) + (6 - (agc2*6)/agc2_max);
    else
            agc_metric = 8 - (8*(agc1 + agc2))/(agc1_max + agc2_max);
    
    
    TLG_ReadRegNS(base_addr, REG_0132, &vsync_min);
    vsync_metric = min(8,max(0, 8 - (vsync_min - 64)/70));
    *metric = (vsync_metric + 2*agc_metric)/3;

    
    if (g_tlg_cc_mve == TLG_MVE_AUTO)
    {
        TLG_REG_LOG((b,"AUTO CC %d\n", g_tlg_cc_mve_cnt));
        g_tlg_cc_mve_cnt++;
        if (g_tlg_cc_mve_cnt > 20)
        {
            TLG_SetMobileVidEnhancementAux(base_addr, TLG_MVE_AUTO, 0);
            g_tlg_cc_mve = TLG_MVE_NONE;
            TLG_REG_LOG((b,"AUTO CC END\n"));
        }
    }

    
    if ( (lock & (REG_013B_BIT5_MASK |
                REG_013B_BIT4_MASK    ))
         == (REG_013B_BIT5_MASK |
                REG_013B_BIT4_MASK    ))
    {
        
        if ( g_tlg_vsync_adjust_onetime >= 5)
        {
            if (g_tlg_vsync_adjust_onetime == 5)
            {
                uint16 vstd;

                TLG_ReadReg(base_addr,  REG_006F, &reg);
                TLGHAL_SET(reg, 3, REG_006F_BIT13_12_SHIFT,
                                   REG_006F_BIT13_12_MASK);
                TLG_WriteReg(base_addr, REG_006F, reg);

                g_tlg_vsync_adjust_onetime++;

                TLG_WriteReg(base_addr, REG_005A_VALUE_2, 0xC7A3);

                
                TLG_GetChnStandard(base_addr, &vstd);
                if (!TLG_IsSECAMChnStandard(vstd))
                    TLG_SetBits(base_addr, REG_005A_VALUE_7, 3,
                                           REG_005A_BIT3_2_SHIFT,
                                           REG_005A_BIT3_2_MASK);
            }
        } else
            g_tlg_vsync_adjust_onetime++;        
    }
    
    if (!(lock & REG_013B_BIT5_MASK))
    {
        *metric = min(1, agc_metric);
        vsync_locked = 0;
    } 
    
    else if (!(lock & REG_013B_BIT2_MASK))
        (*metric)--;
    TLG_REG_LOG((b,"agc_metric=%d, vsync_metric=0x%x, metric=%d\n",
                            agc_metric, vsync_metric, *metric));
    
    if (g_tlg_rssi_onetime == 0)
    {
        g_tlg_rssi_onetime = 1;
        #ifdef TLG_SUPPORT_HYBRID_POLL
            
            
            TLG_SetAlternateStandard(base_addr);
        #endif 
    }

#else 
    uint16    gain;
    int lna1, lna2, agc1, agc2, agcd, levl, leva;

    res = TLG_GetGains(base_addr, 
                &lna1, 
                &lna2, 
                &agc1, 
                &agc2, 
                &agcd, 
                &levl, 
                &leva,
                &gain );

    TLG_ReadRegNS(base_addr, REG_013B, &lock);
    
    if (!(lock & REG_013B_BIT5_MASK))
        *metric = min(1, agc_metric);
    
    else if (!(lock & REG_013B_BIT2_MASK))
        (*metric)--;

#endif 
    if (*metric > 8) 
        *metric = 8;

    TLG_GetChnRestartStatus(base_addr, &restart);
    if (restart) 
    {
        TLG_SoftReset(base_addr);
    }
#ifdef TLG_SUPPORT_AUTO_AGC_CHANGE
    TLG_CheckAudioPerf(base_addr);
#endif 


#ifdef TLG_SUPPORT_FM_AVC 
    TLG_AutoVolumeControlCheck(base_addr);
#endif 

#ifdef TLG_VSCREEN_LOCK
    TLG_LockVScreen(base_addr);
#endif

    
    TLG_ASA(base_addr, vsync_min, vsync_locked);

    return TLG_ERR_SUCCESS;
}


TLGDLL_API int TLG_GetFMSSI(uint32 base_addr, uint16 *metric) 
{
    int i, start = 60;
    uint16 lock_type  = TLG_LOCK_STATE_AUDIO|TLG_LOCK_STATE_SYNTH|TLG_LOCK_STATE_STEREO;
    uint16 lock_atatus;
    uint16 audio_status, stereo_status;
    uint16 data;
    *metric = TLG_GetAudioCarrierPwr(base_addr, 3);
    log_printf(("\n"));
#ifdef TLG_SUPPORT_AUTO_AGC_CHANGE
        TLG_CheckAudioPerf(base_addr);
#endif 


#ifdef TLG_SUPPORT_FM_AVC 
    TLG_AutoVolumeControlCheck(base_addr);
#endif 

    for (i = 2; i < TLG_NUM_SSI_METRIC; i++) 
    {
        if (*metric <= start)
            break;
        start += i*10;
    }
    *metric = i;
    

    TLG_GetLockStatus(base_addr, lock_type, &lock_atatus);
    audio_status = (lock_atatus & TLG_LOCK_STATE_AUDIO)? true : false;
    stereo_status = (lock_atatus & TLG_LOCK_STATE_STEREO)? true : false;
    if (!audio_status)
    {
         g_tlg_fmunlock_time_count++;
    }
    if((audio_status)&&(g_tlg_fmunlock_time_count  >= TLG_FM_UNLOCK_TIME_COUNT))
    {
        TLG_CHIP_VERS_2_1_BEGIN
#if defined(TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)

                TLG_ReadReg(base_addr, REG_00B5, &data);
            TLGHAL_SET(data, 1, 
                            REG_00B5_BIT3_SHIFT,
                            REG_00B5_BIT3_MASK);
            TLG_WriteReg(base_addr, REG_00B5, data);
            TLGHAL_SET(data, 0, 
                            REG_00B5_BIT3_SHIFT,
                            REG_00B5_BIT3_MASK);
            TLG_WriteReg(base_addr, REG_00B5, data);
#endif
            TLG_CHIP_VERS_END
    g_tlg_fmunlock_time_count = 0;
    }
    return TLG_ERR_SUCCESS;
}


TLGDLL_API int TLG_GetChnBandwidth(uint32 base_addr,  uint16 *bw)
{
    uint16 reg22, reg21;
    uint16 forceVal, filt_fcc;

    TLG_NULLPTR_CHECK(bw);

    TLG_CHIP_VERS_1_2_BEGIN
#if defined(TLG_CMP_CHIP_1120_1_2) || defined(TLG_CMP_CHIP_1120_2_1) ||defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)

    TLG_ReadReg(base_addr, REG_0021, &reg21);
    TLGHAL_GET(filt_fcc, reg21, REG_0021_BIT3_0_SHIFT, REG_0021_BIT3_0_MASK);
    TLG_ReadReg(base_addr, REG_0022, &reg22);
    TLGHAL_GET(forceVal, reg22,
                REG_0022_BIT10_0_SHIFT,
                REG_0022_BIT10_0_MASK);

    switch(forceVal) 
    {
        case REG_0022_VALUE_1          :
            *bw = TLG_BW_5        ;
            break;
        case REG_0022_VALUE_2          :
            *bw = TLG_BW_6        ;
            break;
        case REG_0022_VALUE_3          :
            *bw = TLG_BW_7        ;
            break;
        case REG_0022_VALUE_4          :
            *bw = TLG_BW_8        ;
            break;
        case REG_0022_VALUE_5     :
            if (filt_fcc == 1)
            {
                
                *bw = TLG_BW_12   ;
            }
            else
            {
                
                *bw = TLG_BW_15 ;
            }
            break;
        default:
            return (TLG_ERR_FAIL);
    }
#endif 

    TLG_CHIP_VERS_1_1_ELSE
#ifdef TLG_CMP_CHIP_1120_1_1
        *bw = tlg_cur_bw;
#endif 
    TLG_CHIP_VERS_END
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_SetChnBandwidth(uint32 base_addr, uint16 bw)
{
    uint16 reg21 = REG_0021_VALUE_1;
    uint16 reg22 = 0;
    uint16 regData;

    tlg_cur_bw = bw;

    TLG_CHIP_VERS_2_1_BEGIN
#ifdef TLG_CMP_CHIP_1120_2_1
#endif 
    TLG_CHIP_VERS_1_2_ELSE
#ifdef TLG_CMP_CHIP_1120_1_2
#endif 
    TLG_CHIP_VERS_1_1_ELSE
#ifdef TLG_CMP_CHIP_1120_1_1
        
    bw = TLG_BW_12;
#endif 
TLG_CHIP_VERS_END

    switch(bw) 
    {
        case TLG_BW_12: 
            reg21 |= REG_0021_VALUE_6;
            reg22 =  REG_0022_VALUE_5;
            break;
        case TLG_BW_15: 
            reg21 |= REG_0021_VALUE_7;
            reg22 =  REG_0022_VALUE_6;
            break;
        case TLG_BW_8: 
            reg21 |= REG_0021_VALUE_5;
            reg22 =  REG_0022_VALUE_4;
            break;
        case TLG_BW_7: 
            reg21 |= REG_0021_VALUE_4;
            reg22 =  REG_0022_VALUE_3;
            break;

        case TLG_BW_6: 
            reg21 |= REG_0021_VALUE_3;
            reg22 =  REG_0022_VALUE_2;
            break;

        case TLG_BW_5:
            reg21 |= REG_0021_VALUE_2;
            reg22 =  REG_0022_VALUE_1;
            break;
        default:
            return (TLG_ERR_PARAM);
    }
    TLG_ReadReg(base_addr, REG_0180, &regData);
    TLGHAL_SET(regData, 0,
                REG_0180_BIT9_SHIFT,
                REG_0180_BIT9_MASK);
    TLG_WriteReg(base_addr, REG_0180, regData);

    TLG_WriteReg(base_addr, REG_0021, reg21);
    TLG_WriteReg(base_addr, REG_0022, reg22);

    
    TLGHAL_SET(regData, 1,
                REG_0180_BIT9_SHIFT,
                REG_0180_BIT9_MASK);
    TLG_WriteReg(base_addr, REG_0180, regData);

    return(TLG_ERR_SUCCESS);
}


void TLG_TVAudioSetup(uint32 base_addr)
{
    uint16 data;

    
    TLG_REG_LOG((b, "TLG_TVAudioSetup: START\n"));

    TLG_ModeSetup(base_addr, TLG_MODE_SETUP_TV, 1);

    
    TLG_ReadReg(base_addr, REG_003D, &data);
    
    TLGHAL_CLEAR(data,REG_003D_BIT4_MASK);
    
    TLGHAL_CLEAR(data,REG_003D_BIT7_MASK);
    TLG_WriteReg(base_addr, REG_003D,data);

    TLG_REG_LOG((b, "TLG_TVAudioSetup: END\n"));
}


static void TLG_FMAudioSetup(uint32 base_addr)
{
    uint16 data;

    
    TLG_ModeSetup(base_addr, TLG_MODE_SETUP_FM, 1);

    
    TLG_ReadReg(base_addr, REG_003D,&data);
    
    TLGHAL_SET(data, 1, REG_003D_BIT4_SHIFT,
                    REG_003D_BIT4_MASK);
    
    TLGHAL_SET(data, 1, REG_003D_BIT7_SHIFT,
                    REG_003D_BIT7_MASK);
    TLG_WriteReg(base_addr, REG_003D,data);


TLG1121_VERS_A_0_BEGIN
#if defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1121_A_2)
    TLG_SetBits(base_addr, REG_01B0_VALUE_1, 0,
            REG_01B0_BIT9_SHIFT,
            REG_01B0_BIT9_MASK);
#endif 
TLG_CHIP_VERS_END

}


TLGDLL_API int TLG_SetChnMode(uint32 base_addr, uint16 mode)
{
    uint16 data;

    TLG_Init_AGC_Reg(base_addr, mode);

    tv_mode = mode;

    switch (mode)
    {
        case TLG_MODE_ANALOG_TV_UNCOMP  :
        case TLG_MODE_ANALOG_TV_COMP    :
            TLG_REG_LOG((b, "TLG_SetChnMode::TLG_MODE_ANALOG_TV_UNCOMP\n"));
            TLG_REG_LOG((b, "TLG_SetChnMode::TLG_TVAudioSetup  start\n"));
            TLG_TVAudioSetup(base_addr);
            TLG_REG_LOG((b, "TLG_SetChnMode::TLG_TVAudioSetup end\n"));
            break;
        case TLG_MODE_FM_RADIO      :    
            TLG_REG_LOG((b, "TLG_SetChnMode::TLG_MODE_FM_RADIO\n"));
            TLG_FMAudioSetup(base_addr);
            break;
        case TLG_MODE_DVB_T         :    
            TLG_TVAudioSetup(base_addr);
            
            TLG_ReadReg(base_addr, REG_0097, &data);
            TLGHAL_SET(data, 0,
                            REG_0097_BIT0_SHIFT,
                            REG_0097_BIT0_MASK);
            TLG_WriteReg(base_addr, REG_0097, data);
            break;
        case TLG_MODE_NONE          :    
        default:
            return (TLG_ERR_PARAM);
    }


#ifdef TLG_SUPPORT_FM_AVC 
    TLG_AutoVolumeControlInit(base_addr, mode);
#endif 

    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_GetChnStandard(uint32 base_addr, uint16 *mode) 
{
    uint16 videoStandard;

    TLG_NULLPTR_CHECK(mode);

    TLG_ReadReg(base_addr, REG_0002,&videoStandard);
    TLGHAL_GET(*mode, videoStandard, 0,
                    REG_0002_BIT4_0_MASK );

    return(TLG_ERR_SUCCESS);
}

uint16 TLG_GetChromaPllFreqOffset(uint32 base_addr)
{
    uint16 val;
    uint16 regData;
    
    TLG_ReadRegNS(base_addr, REG_0139, &regData);
    
    if (regData & 0x8000)
    {
        
        val = ~(regData) + 1;
    }
    else
    {
        
        val = regData;
    }
    return(val);
}
#define MODE_NONE               0
#define MODE_SECAM              1
#define MODE_PAL                2
#define MODE_VSYN_NOT_LOCKED    3

#define MODE_PAL_LOCK                0x037F
#define MODE_SECAM_LOCK              0x027B

uint16 TLG_CheckPAL_SECAM_Mode(uint32 base_addr)
{
    uint16 regData;
    uint16 val;

    TLG_ReadRegNS(base_addr, REG_013B,&regData);
    if (!((regData & REG_013B_BIT5_MASK         ) &&
            (regData & REG_013B_BIT6_MASK        ) &&
            (regData & REG_013B_BIT9_MASK  ) &&
            (regData & REG_013B_BIT4_MASK           ) 
         ))
    {
        dbg_printf((1, "\t NLOCK\t, 0x13b = 0x%04X\t", regData));
        return(MODE_VSYN_NOT_LOCKED);
    }

    val = TLG_GetChromaPllFreqOffset(base_addr);

    if (val <= TLG_PAL_AVG_DETECT_THRESHOLD)
    {
        if (regData & REG_013B_BIT2_MASK)
        {
            dbg_printf((1, "\t PAL\t, 0x13b = 0x%04X\t", regData));
            return(MODE_PAL);
        }
    }
    if (val >= TLG_SECAM_DETECT_THRESHOLD)
    {
        if (((regData & REG_013B_BIT2_MASK  ) == 0) &&
                        ((regData & REG_013B_BIT8_MASK) == 0) )
        {
            dbg_printf((1, "\t SECAM\t, 0x13b = 0x%04X\t", regData ));
            return MODE_SECAM;
        }
    }
    dbg_printf((1, "\t NONE\t, 0x13b = 0x%04X\t", regData ));
    return(MODE_NONE);
}

#define NEW_STD_INVALID 0
#define NEW_STD_VALID   1

int TLG_CheckAlternateStandard(uint16 cur_std, uint16 *p_new_std)
{
    #ifdef TLG_SUPPORT_HYBRID_POLL
     
    if (g_tlg_hybrid == 0) 
    return NEW_STD_INVALID;
    #endif 

    switch (cur_std)
    {
        case TLG_VSTD_NTSC_M  :
            return(NEW_STD_INVALID);
        case TLG_VSTD_PAL_M   : *p_new_std = TLG_VSTD_SECAM_M ; break;
        case TLG_VSTD_PAL_B   : *p_new_std = TLG_VSTD_SECAM_B ; break;
        case TLG_VSTD_PAL_B1  : *p_new_std = TLG_VSTD_SECAM_B1; break;
        case TLG_VSTD_PAL_D   : *p_new_std = TLG_VSTD_SECAM_D ; break;
        case TLG_VSTD_PAL_D1  : *p_new_std = TLG_VSTD_SECAM_D1; break;
        case TLG_VSTD_PAL_G   : *p_new_std = TLG_VSTD_SECAM_G ; break;
        case TLG_VSTD_PAL_H   : *p_new_std = TLG_VSTD_SECAM_H ; break;
        case TLG_VSTD_PAL_K   : *p_new_std = TLG_VSTD_SECAM_K ; break;
        case TLG_VSTD_PAL_N   : *p_new_std = TLG_VSTD_SECAM_N ; break;
        case TLG_VSTD_PAL_I   : *p_new_std = TLG_VSTD_SECAM_I ; break;
        case TLG_VSTD_PAL_NC  : *p_new_std = TLG_VSTD_SECAM_NC; break;
        case TLG_VSTD_SECAM_M :
        case TLG_VSTD_SECAM_B :
        case TLG_VSTD_SECAM_B1:
        case TLG_VSTD_SECAM_D :
        case TLG_VSTD_SECAM_D1:
        case TLG_VSTD_SECAM_G :
        case TLG_VSTD_SECAM_H :
        case TLG_VSTD_SECAM_K :
        case TLG_VSTD_SECAM_N :
        case TLG_VSTD_SECAM_I :
        case TLG_VSTD_SECAM_NC:
        case TLG_VSTD_SECAM_L :
            return NEW_STD_INVALID;
    }
    return(NEW_STD_VALID);
}
int TLG_CheckMaxMinRange(uint16 max_val, uint16 min_val)
{
    min_val = (min_val < 2) ? 2 : min_val;
    return ((min_val * 10) < max_val) ? 1 : 0;
}



TLGDLL_API void TLG_SetHybrid(uint8 hybrid_mode)
{
    g_tlg_hybrid = hybrid_mode;
}




#define THRESHOLD_AUDIO 10
TLGDLL_API uint16 TLG_setAlternateAudioOffset(uint32 base_addr)
{
    uint16 cur_standard = TLG_VSTD_PAL_D;
#ifdef HYBRID_DK_I
    int32  freq_hz;
    uint32 status = 0;
    int32 result_I,result_DK;

    freq_hz = BAND7_AUDIO_OFFSET;//3250000;
    result_I= TLG_ReadNotchPwrAvgNotches( base_addr, freq_hz, 100000, 10, &status);
    freq_hz = BAND8_AUDIO_OFFSET;//3750000;
    result_DK = TLG_ReadNotchPwrAvgNotches( base_addr, freq_hz, g_step_size_hz, 10, &status);

    TLG_REG_LOG((b,"TLG_setAlternateAudiooffset result_I =%d,result_DK = %d ",result_I,result_DK));

    if(result_I > result_DK && result_I > THRESHOLD_AUDIO)
    {
        TLG_SetChnStandard(base_addr,  TLG_VSTD_PAL_I);
        cur_standard = TLG_VSTD_PAL_I;
    }
#endif 
    return cur_standard;
}



#define TLG_NUM_CHROMA_LOCK_THRESHOLD     12 
#define DETECT_LOOP                       16

#define numPalDetect               (state[1])
#define numSecamDetect             (state[2])
#define loop                       (state[3])
#define total                      (state[4])
#define max_val1                   (state[5])
#define max_val2                   (state[6])
#define max_val3                   (state[7])
#define min_val1                   (state[8])
#define min_val2                   (state[9])
#define min_val3                   (state[10])
#define crm_lock                   (state[11])
#define crm_lock2                  (state[12])
#define new_std                    (state[13])

TLGDLL_API void TLG_InitSetAlternateStandard(tlg_statep state)
{
    state[0]            = TLG_STATE0;
    numPalDetect        = 0;
    numSecamDetect      = 0;
    loop                = 0;
    total               = 0;
    max_val1            = 0;
    max_val2            = 0;
    max_val3            = 0;
    min_val1            = 0xffff;
    min_val2            = 0xffff;
    min_val3            = 0xffff;
    crm_lock            = 0;
    crm_lock2           = 0;
    new_std             = TLG_VSTD_NONE;
}


TLGDLL_API int TLG_SetAlternateStandardASYNC( uint32        base_addr,
                                              tlg_statep    state, 
                                              int           *ms )
{
    
    uint16 cur_std;
    uint16 regData;
    uint16 mode;
    uint16 val;
    uint16 avg = 0;
    TLG_NULLPTR_CHECK(state);
    TLG_NULLPTR_CHECK(ms);
    TLG_REG_LOG((b, "IN TLG_SetAlternateStandardASYNC\n"));

    *ms = 0;   

    switch (state[0])
    {
        case TLG_STATE0:
            if( loop == 0 )
            {
                TLG_GetChnStandard(base_addr, &cur_std);
                TLG_REG_LOG((b, "TLG_SetAlternateStandard Current standard: \n0x%04X\n", cur_std));
				if (TLG_CheckAlternateStandard(cur_std, &new_std) == NEW_STD_INVALID)
				{
					state[0] = TLG_STATE_FINAL;
					return(0);
				}
                
                TLG_WriteReg(base_addr, REG_00BB_VALUE_2, 
                                        TLGAPI_CHROMA_COEFF_2_DEF_SECAM);
                TLG_WriteReg(base_addr, REG_00BB_VALUE_3, 
                                        TLGAPI_CHROMA_COEFF_3_DEF_SECAM);
            }
            TLG_REG_LOG((b, "\nSTART detecting lock\n"));
            mode = TLG_CheckPAL_SECAM_Mode(base_addr);

            switch (mode)
            {
                case MODE_SECAM:
                    numSecamDetect++;
                    break;
                case MODE_PAL:
                    numPalDetect++;
                    break;
                case MODE_NONE:
                    break;
                case MODE_VSYN_NOT_LOCKED:
                    
                    break;
                default:
                    break;
            }
            TLG_REG_LOG((b,"secam detect: %d, pal detect: %d\n",  numSecamDetect, numPalDetect));

            if (numPalDetect >= TLG_NUM_PAL_DETECT_THRESHOLD)
            {
                TLG_REG_LOG((b, "HYBRID: PAL DETECTED in state1\n"));
                state[0] = TLG_STATE3;
                return(0);
            }
            
            if (numSecamDetect >= TLG_NUM_SECAM_DETECT_THRESHOLD)
            {
                state[0] = TLG_STATE2;
                return 0;
            }
            loop++;
            if( loop < TLG_NUM_DETECT_PAL_LOOP )
            {
    
                *ms = 5;
                state[0] = TLG_STATE0;
            }
            else
            {
                *ms = 315;
                state[0] = TLG_STATE1;
                total            = 0;
                max_val1          = 0;
                max_val2          = 0;
                max_val3          = 0;
                min_val1          = 0xffff;
                min_val2          = 0xffff;
                min_val3          = 0xffff;
                crm_lock = 0;
                crm_lock2 = 0;
                loop=0;
            }
            return(0);

        case TLG_STATE1:
            
            TLG_ReadRegNS(base_addr, REG_013B,&regData);
            if( regData&REG_013B_BIT2_MASK )
                    crm_lock++;
            if( regData&REG_013B_BIT8_MASK )
                    crm_lock2++;

            val = TLG_GetChromaPllFreqOffset(base_addr);
            total += val;

            if( val > max_val3 )
            {
                if( val >= max_val1 )
                {
                    max_val3 = max_val2;
                    max_val2 = max_val1;
                    max_val1 = val;
                }
                else if( val >= max_val2 )
                {
                    max_val3 = max_val2;
                    max_val2 = val;
                }
                else
                {
                    max_val3 = val;
                }
            }
            if( val < min_val1 ) 
            {
                if( val <= min_val3 )
                {
                    min_val1 = min_val2;
                    min_val2 = min_val3;
                    min_val3 = val;
                }
                else if( val <= min_val2 )
                {
                    min_val1 = min_val2;
                    min_val2 = val;
                }
                else
                {
                    min_val1 = val;
                }
            }
            loop++;
            if( loop < DETECT_LOOP )  
            {
                *ms = 5;
                state[0] = TLG_STATE1;
                return(0);
            }
            else
            {
                total -= max_val1 + max_val2 + max_val3;
                total -= min_val1 + min_val2 + min_val3;
                avg = total / (DETECT_LOOP - 6 );       
                TLG_REG_LOG((b, "AVG: %d\t crm_lock:%d, crm_lock2:%d", avg,crm_lock, crm_lock2));


                if( avg < TLG_PAL_AVG_DETECT_THRESHOLD )
                {
                    state[0] = TLG_STATE3; 
                    return(0);
                }
                else
                {
                    if( avg <= TLG_SECAM_DETECT_THRESHOLD && avg >= TLG_PAL_AVG_DETECT_THRESHOLD )
                    {
                        uint16 lock;
                        lock = crm_lock + crm_lock2;
                        TLG_REG_LOG((b,"\ttotal lock:%d\n", lock ) );
                        if( lock >= TLG_NUM_CHROMA_LOCK_THRESHOLD )
                        {
    
                            state[0] = TLG_STATE3;
                            return(0);
                        }
                    }
                }

            }
            loop = 0;
            

        case TLG_STATE2:
            
            TLG_REG_LOG((b, "TLG_SetAlternateStandard in SECAM mode\n"));

            TLG_SetChnStandard(base_addr, new_std);
            
            TLG_SoftReset(base_addr);

            
            state[0] = TLG_STATE4;
            return 0;

        case TLG_STATE3: 
            
            TLG_WriteReg(base_addr, REG_00BB_VALUE_2, 
                                    TLGAPI_CHROMA_COEFF_2_LO_SNR_NTSC_PAL);
            TLG_WriteReg(base_addr, REG_00BB_VALUE_3, 
                                    TLGAPI_CHROMA_COEFF_3_DEF_NTSC_PAL);

            
            state[0] = TLG_STATE4;
            return 0;

        case TLG_STATE4:
            
            TLG_ReadRegNS(base_addr, REG_013B,&regData);
            if (regData & (REG_013B_BIT5_MASK | 
                           REG_013B_BIT1_MASK  | 
                           REG_013B_BIT2_MASK))
            {
                state[0] = TLG_STATE_FINAL;
                return regData;
            }
            loop++;
            if( loop < FAST_TVSCAN_WAITFOR_LOCK_TIMEOUT1)
            {
                *ms = 15;
                state[0] = TLG_STATE4;
            }
            else
            {
                
                state[0] = TLG_STATE_FINAL;
            }
            return(0);

        default: 
            TLG_REG_LOG((b,"TLG_SetAlternateStandardASYNC: %d BADSTATE!\n", state[0]));
            break;
    }
    
    return 0;
}

TLGDLL_API int TLG_SetAlternateStandard(uint32 base_addr)
{
    int res = 0;
    tlg_statep state;
    int ms = 0;
    TLG_InitSetAlternateStandard(state);

    do
    {
        res = TLG_SetAlternateStandardASYNC( base_addr, state, &ms );
        if( res != TLG_ERR_SUCCESS)
            break;
        if( ms != 0 )
            TLG_Delay((uint16)ms);
    }while( state[0] != TLG_STATE_FINAL );

    return res;
}

#undef numPalDetect 
#undef numSecamDetect
#undef loop
#undef total
#undef max_val1
#undef max_val2
#undef max_val3
#undef min_val1
#undef min_val2
#undef min_val3 
#undef crm_lock 
#undef crm_lock2
#undef new_std 




static const uint16 tlg_chn_std_regs_addr[] =
{
    0x5c,
    0x63,
    0x65,
    0x6b,
    0x6d,
    0x6f,
    0x74,
    0xa2,
    0xa3,
    0xa7,
    0xbe,
    0xbf
};


#define TLG_MAX_CHN_STD_REG     (sizeof(tlg_chn_std_regs_addr)/sizeof(uint16))


static const uint16 tlg_chn_std_regs_mask[] = 
{

  REG_005A_BIT7_0_MASK,
  REG_005A_BIT15_8_MASK | REG_005A_BIT3_2_MASK,
  REG_0065_BIT9_8_MASK     |
            REG_0065_BIT5_MASK      |
            REG_0065_BIT3_MASK    |
            REG_0065_BIT2_MASK      |
            REG_0065_BIT1_MASK    |
            REG_0065_BIT0_MASK,
  REG_0068_BIT3_0_MASK,
  REG_006D_BIT14_8_MASK,
  REG_006F_BIT3_2_MASK,
  REG_0074_BIT15_12_MASK,
  BIT15_0,
  BIT15_0,
  BIT15_0,
  BIT15_0,
  BIT15_0,
};


static const uint16 tlg_chn_std_regs_ntsc[] =
{

  TLGAPI_HSYNC_3_DELAY1_DEFAULT,
  (3<<REG_005A_BIT3_2_SHIFT)     |
            (0x20 << REG_005A_BIT15_8_SHIFT),
  REG_0065_BIT2_MASK,
  0x0006,
  (TLG_NTSC_LUMA_SCALE << REG_006D_BIT14_8_SHIFT),
  0x0000,
  0x0000,
  0x0408,
  0x050e,
  0x0000,
  TLGAPI_CHROMA_COEFF_2_LO_SNR_NTSC_PAL,
  TLGAPI_CHROMA_COEFF_3_DEF_NTSC_PAL,
};


static const uint16 tlg_chn_std_regs_pal[] =
{

  TLGAPI_HSYNC_3_DELAY1_DEFAULT,
  (3<<REG_005A_BIT3_2_SHIFT)       |
            (0x20 << REG_005A_BIT15_8_SHIFT),
  REG_0065_BIT2_MASK,
  0x0006,
  (TLG_PAL_LUMA_SCALE << REG_006D_BIT14_8_SHIFT),
  0x0000,
  0x0000,
  0x0404,
  0x053d,
  0x0000,
  TLGAPI_CHROMA_COEFF_2_LO_SNR_NTSC_PAL,
  TLGAPI_CHROMA_COEFF_3_DEF_NTSC_PAL,
};


static const uint16 tlg_chn_std_regs_secam[] =
{

  TLGAPI_HSYNC_3_DELAY1_DEFAULT,
  (   3 <<REG_005A_BIT3_2_SHIFT)     |
            (0x20 << REG_005A_BIT15_8_SHIFT),
  0x012f,
  0x0008,
  (TLG_SECAM_LUMA_SCALE << REG_006D_BIT14_8_SHIFT),
  (   1 << REG_006F_BIT3_2_SHIFT),
  0xc000,
  0x0404,
  0x053d,
  0xc020,
  TLGAPI_CHROMA_COEFF_2_DEF_SECAM,
  TLGAPI_CHROMA_COEFF_3_DEF_SECAM,
};



TLGDLL_API int TLG_SetChnStandard(uint32 base_addr, uint16 mode)
{
    uint16 reg;
    const uint16 *field_val = tlg_chn_std_regs_ntsc;
    int     i;

    TLG_BOUNDS_CHECK(mode, TLG_VSTD_NONE);

    TLG_REG_LOG((b,"\n\nTLG_SetChnStandard: mode=%d\n", mode));

    
    mode &= ~(TLG_VSTD_HYBRID_BIT);
    
    
    if(mode == TLG_VSTD_PAL_N)
        mode = TLG_VSTD_PAL_NC;

    
    switch (mode)
    {     
        case TLG_VSTD_NTSC_M  :
        case TLG_VSTD_PAL_M   :
                field_val = tlg_chn_std_regs_ntsc;
                break;              
             
        case TLG_VSTD_PAL_B   :
        case TLG_VSTD_PAL_B1  :
        case TLG_VSTD_PAL_D   :
        case TLG_VSTD_PAL_D1  :
        case TLG_VSTD_PAL_G   :
        case TLG_VSTD_PAL_H   :
        case TLG_VSTD_PAL_K   :
        case TLG_VSTD_PAL_N   :
        case TLG_VSTD_PAL_I   :
        case TLG_VSTD_PAL_NC  :
                field_val = tlg_chn_std_regs_pal;
                break;

        case TLG_VSTD_SECAM_M :
        case TLG_VSTD_SECAM_B :
        case TLG_VSTD_SECAM_B1:
        case TLG_VSTD_SECAM_D :
        case TLG_VSTD_SECAM_D1:
        case TLG_VSTD_SECAM_G :
        case TLG_VSTD_SECAM_H :
        case TLG_VSTD_SECAM_K :
        case TLG_VSTD_SECAM_N :
        case TLG_VSTD_SECAM_I :
        case TLG_VSTD_SECAM_NC:
        case TLG_VSTD_SECAM_L :
                field_val = tlg_chn_std_regs_secam;
                break;
    }

    
    for (i=0; i < TLG_MAX_CHN_STD_REG; i++)
    {
        TLG_SetBits(base_addr,                  
                    tlg_chn_std_regs_addr[i],   
                    field_val[i],               
                    0,                          
                    tlg_chn_std_regs_mask[i]);  
    }

    
    TLG_ReadReg(base_addr, REG_0002, &reg);
    TLGHAL_SET(reg,
                mode,
                REG_0002_BIT4_0_SHIFT,
                REG_0002_BIT4_0_MASK );

    
    TLGHAL_SET(reg,
                (mode==TLG_VSTD_SECAM_G || mode==TLG_VSTD_PAL_G) ? 1 : 0,
                REG_0002_BIT7_SHIFT,
                REG_0002_BIT7_MASK);        
    TLG_WriteReg(base_addr, REG_0002, reg);

    
    TLG_ChangeSNRmode(base_addr, TLG_MODE_HIGH_SNR, mode);
    TLG_ChangeREG5Amode(base_addr, TLG_REG5A_MODE_1, mode);
    TLG_ResetSFA();
    TLG_SetSFA(base_addr, mode, TLG_SFA_HIGH_SNR);
    if (TLG_IsSECAMChnStandard(mode))
    {
        TLG_ChangeSecamColorMode (base_addr, TLG_SECAM_COLOR_MODE_1);
        TLG_ChangeSecamFlickerMode (base_addr, TLG_SECAM_FLICKER_MODE_2);
    }
    return(TLG_ERR_SUCCESS);
}

#define ADJUST_TIME         30   
#define WAIT_DONE_TIMEOUT   20



#define BBFILCAL_F1         (54)                  
#define BBFILCAL_F1_CVT     (MHZCVT(BBFILCAL_F1))
#define BBFILCAL_RF2        (753)
#define BBFILCAL_RLSB2          (16000)
#define BBFILCAL_RCODEMAX   (31)
#define BBFILCAL_ROFFSET    (-2)

static uint32 tlg_bbfilcal_mul[] = {
        (100), 
        (111), 
        (124), 
        (142), 
        (167), 
        (202), 
        (257), 
        (354), 
        (599), 
};


static void TLG_BBFilter_Cal(uint32 base_addr, uint32 freq_hz)
{
    uint32 freq_cvt = HZCVT(freq_hz);
    uint16 filcalStat;
    uint16 rcal;
    uint32 f_cvt, fb_cvt, f1b_cvt;
    int32 fc, f;
    int    i;
    uint16 c=0;
    int32 r=31;
    uint16 vhf1Hz;

    
    if (freq_cvt > MHZCVT(333))
    {
        TLG_SetBits(base_addr, REG_0186, c,
                    REG_0186_BIT3_0_SHIFT,
                    REG_0186_BIT3_0_MASK);
        TLG_SetBits(base_addr, REG_0187, r,
                    REG_0187_BIT14_10_SHIFT,
                    REG_0187_BIT14_10_MASK);
        TLG_SetBits(base_addr, REG_0189, 1,
                    REG_0189_BIT5_SHIFT,
                    REG_0189_BIT5_MASK);
        return;
    }

    

    
    TLG_ReadRegNS(base_addr, REG_0111, &filcalStat);

    TLGHAL_GET(rcal, filcalStat, REG_0111_BIT7_3_SHIFT,
                REG_0111_BIT7_3_MASK);

    f_cvt   = BBFILCAL_F1_CVT;

    
    
    
    
    f1b_cvt = (2124522000  
           /(BBFILCAL_RLSB2 + (BBFILCAL_ROFFSET + rcal)*BBFILCAL_RF2))*10;

    if (freq_cvt <= f_cvt)
    {
        
        fc = freq_cvt/100;
        f  = ((BBFILCAL_F1_CVT * tlg_bbfilcal_mul[0])/100)/100;
        c  = 8;
        
        
        
        
        
        
        
        r  =   ((fc*BBFILCAL_RLSB2)/f         
                + (fc*(rcal + BBFILCAL_ROFFSET)*BBFILCAL_RF2)/f
                - BBFILCAL_RLSB2 + (BBFILCAL_RF2-2))/BBFILCAL_RF2;
        if (r < 0) r = 0;
    } 
    else
    {
        for (i=0; i < 9; i++)
        {
            fb_cvt = (f1b_cvt * tlg_bbfilcal_mul[i])/100;
            if (freq_cvt > f_cvt && freq_cvt <= fb_cvt)
            {
                
                fc = freq_cvt/100;
                f  = ((BBFILCAL_F1_CVT * tlg_bbfilcal_mul[i])/100)/100;
                c  = 8 - i;
                r  =  ((fc*BBFILCAL_RLSB2)/f  
                        + (fc*(rcal + BBFILCAL_ROFFSET)*BBFILCAL_RF2)/f
                         - BBFILCAL_RLSB2 + (BBFILCAL_RF2-2))/BBFILCAL_RF2;
                break;
            }
            f_cvt  = fb_cvt;
        }
    }
    TLG_REG_LOG((b,"TLG_BBFilter_Cal: c=%d, r=%d, hz=%d\n", c, r, freq_hz));
    
    TLG_SetBits(base_addr, REG_0186, c,
                REG_0186_BIT3_0_SHIFT,
                REG_0186_BIT3_0_MASK);
    TLG_SetBits(base_addr, REG_0187, r,
                REG_0187_BIT14_10_SHIFT,
                REG_0187_BIT14_10_MASK);
    TLG_ReadReg(base_addr, REG_0189, &vhf1Hz);
    TLGHAL_SET(vhf1Hz, c, REG_0189_BIT3_0_SHIFT,
                REG_0189_BIT3_0_MASK);
    TLGHAL_CLEAR(vhf1Hz, REG_0189_BIT6_MASK);
    TLGHAL_CLEAR(vhf1Hz, REG_0189_BIT5_MASK);
    TLG_WriteReg(base_addr, REG_0189, vhf1Hz);

    TLG_SetBits(base_addr, REG_018A, r,
                REG_018A_BIT14_10_SHIFT,
                REG_018A_BIT14_10_MASK);
}


#ifdef TLG_TF_CALC_DCYCLE_BY_HZ

#define TLG_PAK_TF2DCYCLE(tf, clk_div)  ((((clk_div)&0x3) << 12) | ((tf)&0x3ff))
#define TLG_UNPAK_TF(a)                 ((a)&0x3ff)
#define TLG_UNPAK_CLKDIV(a) (           ((a)>>12)&0x3)
#define TLG_MAX_TF_CODE                 (0x3ff)

int TLG_TFCal2Dcycle(uint16 cal_code, uint16 *dcycle, uint16 *clk_div, uint16 *jitter) 
{
    *jitter = 0;
    if      (cal_code <= 70)                    *dcycle = 3;
    else if (cal_code > 70 && cal_code <= 100)  *dcycle = 4;
    else if (cal_code > 100 && cal_code <= 200) *dcycle = 5;
    else if (cal_code > 200 && cal_code <= 300) *dcycle = 6;
    else if (cal_code > 300 && cal_code <= 350) *dcycle = 7;
    else if (cal_code > 350 && cal_code <= 400) *dcycle = 8;
    else if (cal_code > 400 && cal_code <= 500) *dcycle = 9;
    else if (cal_code > 500 && cal_code <= 550) *dcycle = 10;
    else if (cal_code > 550 && cal_code <= 600) *dcycle = 11;
    else if (cal_code > 600 && cal_code <= 700) *dcycle = 12;
    else                                        *dcycle = 16;

    if (*dcycle <= 4)       *clk_div = 0;
    else if (*dcycle <=8)   *clk_div = 1;
    else if (*dcycle <=16)  *clk_div = 2;
    else                    *clk_div = 3;

    TLG_REG_LOG((b,"calcode=%d, dcycle=%d, clk_div=%d\n", cal_code, *dcycle, *clk_div));
    return TLG_ERR_SUCCESS;
}


TLGDLL_API int TLG_SetAntennaType(uint32 base_addr, uint16 antenna_type)
{
    tlg_antenna = antenna_type;

    TLG_CHIP_VERS_2_1_FAMILY_BEGIN
    #if defined (TLG_CMP_CHIP_1120_2_1) || defined (TLG_CMP_CHIP_1120_2_2) || defined (TLG_CMP_CHIP_1120_2_3) || defined (TLG_CMP_CHIP_1120_2_4)
    if (tlg_antenna == TLG_ANTENNA_SE)
    {
        uhf_tbl = uhf_tbl_2_1_se;
        TLG_REG_LOG((b,"Setting Antenna to SE\n"));
    }
    #endif 
    TLG_CHIP_VERS_END
        
    return TLG_ERR_SUCCESS;
}

TLGDLL_API int TLG_GetAntennaType(uint32 base_addr, uint16 *antenna_type)
{
    TLG_NULLPTR_CHECK(antenna_type);
    *antenna_type = tlg_antenna;
    return TLG_ERR_SUCCESS;
}
#else  

enum {
    VHF1_DCYCLE4_I=0,
    VHF1_DCYCLE6_I,
    VHF1_DCYCLE8_I,
    VHF2_DCYCLE4_I,
    VHF2_DCYCLE6_I,
    VHF2_DCYCLE8_I,
    UHF_DCYCLE4_I,
    UHF_DCYCLE6_I,
    UHF_DCYCLE8_I,
    UHF_DCYCLE12_I,
    DCYCLE_I_MAX
};


#define  VHF1_DCYCLE4_MHZ  (66)
#define  VHF1_DCYCLE6_MHZ  (82)
#define  VHF1_DCYCLE8_MHZ  (108)

#define  VHF2_DCYCLE4_MHZ  (186)
#define  VHF2_DCYCLE6_MHZ  (216)
#define  VHF2_DCYCLE8_MHZ  (280)

#define  UHF_DCYCLE4_MHZ   (494)
#define  UHF_DCYCLE6_MHZ   (620)
#define  UHF_DCYCLE8_MHZ   (674)
#define  UHF_DCYCLE12_MHZ  (734)

#define  VHF1_DCYCLE4_CVT MHZCVT(VHF1_DCYCLE4_MHZ)
#define  VHF1_DCYCLE6_CVT MHZCVT(VHF1_DCYCLE6_MHZ)
#define  VHF1_DCYCLE8_CVT MHZCVT(VHF1_DCYCLE8_MHZ)

#define  VHF2_DCYCLE4_CVT MHZCVT(VHF2_DCYCLE4_MHZ)
#define  VHF2_DCYCLE6_CVT MHZCVT(VHF2_DCYCLE6_MHZ)
#define  VHF2_DCYCLE8_CVT MHZCVT(VHF2_DCYCLE8_MHZ)

#define  UHF_DCYCLE4_CVT  MHZCVT(UHF_DCYCLE4_MHZ)
#define  UHF_DCYCLE6_CVT  MHZCVT(UHF_DCYCLE6_MHZ)
#define  UHF_DCYCLE8_CVT  MHZCVT(UHF_DCYCLE8_MHZ)
#define  UHF_DCYCLE12_CVT MHZCVT(UHF_DCYCLE12_MHZ)

static const uint32 dcycleAntennaDef[DCYCLE_I_MAX] = {
        VHF1_DCYCLE4_CVT,
        VHF1_DCYCLE6_CVT,
        VHF1_DCYCLE8_CVT,

        VHF2_DCYCLE4_CVT,
        VHF2_DCYCLE6_CVT,
        VHF2_DCYCLE8_CVT,

        UHF_DCYCLE4_CVT,
        UHF_DCYCLE6_CVT,
        UHF_DCYCLE8_CVT,
        UHF_DCYCLE12_CVT
};


#define  VHF1_DCYCLE4_HDPH_MHZ  (0)
#define  VHF1_DCYCLE6_HDPH_MHZ  (0)
#define  VHF1_DCYCLE8_HDPH_MHZ  (66)

#define  VHF2_DCYCLE4_HDPH_MHZ  (150)
#define  VHF2_DCYCLE6_HDPH_MHZ  (151)
#define  VHF2_DCYCLE8_HDPH_MHZ  (178)

#define  VHF1_DCYCLE4_HDPH_CVT MHZCVT(VHF1_DCYCLE4_HDPH_MHZ)
#define  VHF1_DCYCLE6_HDPH_CVT MHZCVT(VHF1_DCYCLE6_HDPH_MHZ)
#define  VHF1_DCYCLE8_HDPH_CVT MHZCVT(VHF1_DCYCLE8_HDPH_MHZ)

#define  VHF2_DCYCLE4_HDPH_CVT MHZCVT(VHF2_DCYCLE4_HDPH_MHZ)
#define  VHF2_DCYCLE6_HDPH_CVT MHZCVT(VHF2_DCYCLE6_HDPH_MHZ)
#define  VHF2_DCYCLE8_HDPH_CVT MHZCVT(VHF2_DCYCLE8_HDPH_MHZ)

static const uint32 dcycleAntennaHdPh[DCYCLE_I_MAX] = {
        VHF1_DCYCLE4_HDPH_CVT,
        VHF1_DCYCLE6_HDPH_CVT,
        VHF1_DCYCLE8_HDPH_CVT,

        VHF2_DCYCLE4_HDPH_CVT,
        VHF2_DCYCLE6_HDPH_CVT,
        VHF2_DCYCLE8_HDPH_CVT,

        UHF_DCYCLE4_CVT,
        UHF_DCYCLE6_CVT,
        UHF_DCYCLE8_CVT,
        UHF_DCYCLE12_CVT
};

static const uint32 *antenna_dcycle_parmlist[] = {
        dcycleAntennaDef,                       
        dcycleAntennaHdPh                       
};
static const uint32 *dcycle_parm = dcycleAntennaDef;



TLGDLL_API int TLG_SetAntennaType(uint32 base_addr, uint16 antenna_type)
{
    TLG_BOUNDS_CHECK(antenna_type, TLG_ANTENNA_MAX);
    tlg_antenna = antenna_type;
    dcycle_parm = antenna_dcycle_parmlist[tlg_antenna];
    return TLG_ERR_SUCCESS;
}


TLGDLL_API int TLG_GetAntennaType(uint32 base_addr, uint16 *antenna_type)
{
    TLG_NULLPTR_CHECK(antenna_type);
    *antenna_type = tlg_antenna;
    return TLG_ERR_SUCCESS;
}



static uint8 TLG_GetDcycle(uint32 freq_cvt, uint16 lna_mode)
{
    uint8 dcycle = 31;

    switch(lna_mode)
    {
        case TLG_LNA_VHF1:
            if(freq_cvt < dcycle_parm[VHF1_DCYCLE4_I])
                dcycle = 4;
            else if(freq_cvt < dcycle_parm[VHF1_DCYCLE6_I])
                dcycle = 6;
            else if(freq_cvt < dcycle_parm[VHF1_DCYCLE8_I])
                dcycle = 8;
            else
                dcycle = 16;
            break;
        case TLG_LNA_VHF3:
            break;
        case TLG_LNA_VHF2:
            if(freq_cvt < dcycle_parm[VHF2_DCYCLE4_I])
                dcycle = 4;
            else if(freq_cvt < dcycle_parm[VHF2_DCYCLE6_I])
                dcycle = 6;
            else if(freq_cvt < dcycle_parm[VHF2_DCYCLE8_I])
                dcycle = 8;
            else
                dcycle = 16;
            break;
        case TLG_LNA_UHF:
                if(freq_cvt < dcycle_parm[UHF_DCYCLE4_I])
                    dcycle = 4;
                else if(freq_cvt < dcycle_parm[UHF_DCYCLE6_I])
                    dcycle = 6;
                else if(freq_cvt < dcycle_parm[UHF_DCYCLE8_I])
                    dcycle = 8;
                else if(freq_cvt < dcycle_parm[UHF_DCYCLE12_I])
                    dcycle = 16;
                else
                    dcycle = 31;
                break;
    }
    return dcycle;
}
#endif 



static void TLG_TFCalibrationSaveReg(uint32 base_addr, uint16 * state)
{
    uint16 cal_injectors      ;
    uint16 sar_freeze         ;
    uint16 atop_trkfil_cal_2  ;

    {
        uint16 cal_override;
        TLG_ReadReg(base_addr, REG_0184  , &cal_override     );
        state[4] = (uint16) cal_override        ;
    }

    TLG_ReadReg(base_addr, REG_018D           , &cal_injectors    );
    TLG_ReadReg(base_addr, REG_0033       , &sar_freeze       );
    TLG_ReadReg(base_addr, REG_0037 , &atop_trkfil_cal_2);
    state[1] = (uint16) cal_injectors       ;
    state[2] = (uint16) sar_freeze          ;
    state[3] = (uint16) atop_trkfil_cal_2   ;
}

static void TLG_TFCalibrationRestoreReg(uint32 base_addr, uint16 * state)
{
    uint16 cal_injectors      ;
    uint16 sar_freeze         ;
    uint16 atop_trkfil_cal_2  ;

    {
        uint16 cal_override;
        cal_override         = (uint16) state[4];
        TLG_ModifyReg(base_addr, REG_0184  , cal_override     );
        state[4] = (uint16) cal_override        ;
    }

    cal_injectors        = (uint16) state[1];
    sar_freeze           = (uint16) state[2];
    atop_trkfil_cal_2    = (uint16) state[3];
    TLGHAL_SET(cal_injectors, 0,
                REG_018D_BIT15_SHIFT,
                REG_018D_BIT15_MASK);

    TLGHAL_SET(cal_injectors, 1, 
                REG_018D_BIT0_SHIFT, 
                REG_018D_BIT0_MASK);

    TLG_ModifyReg(base_addr, REG_018D           , cal_injectors    );
    TLG_ModifyReg(base_addr, REG_0033       , sar_freeze       );
    TLG_ModifyReg(base_addr, REG_0037 , atop_trkfil_cal_2);

}
static void TLG_TFCalibration_UHF_NoSetup(uint32 base_addr, uint32 freq_hz)
{
#define HI_VHF   400
#define LO_LBAND   1000
     uint16  regData;
    uint16  tempData;
    uint32  freq_MHz = freq_hz / 1000000;
    uint8   cgstune;
    uint8   citune;
    uint8   cal_override = 1;
    const tlg_uhf_tbl_t *p_uhf_tbl = uhf_tbl;

    

    
    TLG_SetBits(base_addr, REG_018D, 0,
                    REG_018D_BIT0_SHIFT,
                    REG_018D_BIT0_MASK);
    cgstune = 0;
    citune  = 0;
    while (p_uhf_tbl && (p_uhf_tbl->to_freq != 0))
    {
        if (freq_MHz < p_uhf_tbl->to_freq)
        {
            cgstune = p_uhf_tbl->cgstune;
            citune  = p_uhf_tbl->citune;
            break;
        }
        p_uhf_tbl++;
    }

    // 0x183
    TLG_ReadReg(base_addr, REG_0183, &regData);
    tempData = regData;
    TLGHAL_SET(regData, cgstune,
                REG_0183_BIT15_11_SHIFT,
                REG_0183_BIT15_11_MASK);
    TLGHAL_SET(regData, citune,
                REG_0183_BIT4_0_SHIFT,
                REG_0183_BIT4_0_MASK);
    TLGHAL_SET(regData, cal_override,
                REG_0183_BIT5_SHIFT,
                REG_0183_BIT5_MASK);
    if (tempData != regData)
    {
        TLG_WriteReg(base_addr, REG_0183,regData);
    }
}


void TLG_TFCalibrationASYNC( uint32 base_addr,
                uint32 freq_hz,
                uint16 lna_mode,
                tlg_statep state,
                int *ms)
{
    
    uint32  freq_cvt = HZCVT(freq_hz);
    uint16  regData;
    uint16  data;
    uint16  reg18d  = 0;
    uint16  reg33   = 0;
    uint16  reg30   = 0;
    uint16  reg187  = 0;
    uint16  reg188  = 0;
    uint16  reg18a  = 0;
    if (freq_cvt >= MHZCVT(400))
    {
        
        TLG_TFCalibration_UHF_NoSetup(base_addr, freq_hz);
        *ms = 0;
        state[0] = TLG_STATE_FINAL;
        return;
    }

    
    switch (state[0])
    {
        case TLG_STATE0:
            
            TLG_SetBits(base_addr, REG_018D, 1,
                        REG_018D_BIT0_SHIFT,
                        REG_018D_BIT0_MASK);
            

            
            TLG_ReadReg(base_addr, REG_0187, &reg187);
            TLG_ReadReg(base_addr, REG_0188, &reg188);
            TLG_ReadReg(base_addr, REG_018A, &reg18a);
            TLG_ReadReg(base_addr, REG_018D          , &reg18d);
            TLG_ReadReg(base_addr, REG_0033      , &reg33 );
            TLG_ReadReg(base_addr, REG_0030    , &reg30 );

            
            TLG_ModifyReg(base_addr, REG_018B,   0x292);
            
            TLG_ModifyReg(base_addr, REG_002E,
                            REG_002E_VALUE_1);
            
            TLG_ModifyReg(base_addr, REG_002F,
                            REG_002F_VALUE_1);
            // 0x30
            TLG_ModifyReg(base_addr, REG_0030, 0xc000);

            data = 0x0018;
            regData     = reg18a;
            
            if (freq_hz <= 60000000)
            {
                data = 0x0004;
            }
            else if (freq_hz <= 80000000)
            {
                data = 0x000C;
            }
            TLGHAL_SET(regData, data,
                        REG_018A_BIT8_0_SHIFT,
                        REG_018A_BIT8_0_MASK);
            
            TLGHAL_SET(regData, 1,
                        REG_018A_BIT9_SHIFT,
                        REG_018A_BIT9_MASK);
            TLG_WriteReg(base_addr,REG_018A,regData); 

            TLGHAL_SET(reg18d, 1,
                        REG_018D_BIT15_SHIFT,
                        REG_018D_BIT15_MASK);
            TLG_WriteReg(base_addr,REG_018D,  reg18d);

            
            

            TLG_PowerDownMixer(base_addr);
            TLG_ModifyReg(base_addr, REG_0039, REG_0039_VALUE_1);

            TLG_CHIP_VERS_1_2_BEGIN
 #if defined (TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2) || defined(TLG_CMP_CHIP_1120_1_2)

                    TLG_ModifyReg(base_addr, REG_0033, 0x1d01);

#endif 
            TLG_CHIP_VERS_1_2_ELSE
#ifdef TLG_CMP_CHIP_1120_1_2

                    TLG_ModifyReg(base_addr, REG_0033, 0x1d01);

#endif 
            TLG_CHIP_VERS_1_1_ELSE

#ifdef TLG_CMP_CHIP_1120_1_1
                    regData = reg33;
            TLGHAL_SET(regData, 1,
                        REG_0033_BIT12_SHIFT,
                        REG_0033_BIT12_MASK);

            TLGHAL_SET(regData, 0,
                        REG_0033_BIT0_SHIFT,
                        REG_0033_BIT0_MASK);
            TLG_WriteReg(base_addr, REG_0033, regData);

#endif 

            TLG_CHIP_VERS_END

                    
                    TLG_WriteReg(base_addr, REG_0038, 0);

            
            state[ 4] = 0;
            state[ 3] = reg18d;
            state[ 2] = reg30;
            state[ 1] = reg33;
            state[ 0] = TLG_STATE1;
            *ms = 5;
            break;

        case TLG_STATE1:
            TLG_ReadRegNS(base_addr, REG_0117, &data);
          if ((data & REG_0117_BIT12_MASK) == REG_0117_BIT12_MASK)
            {
                
                
                reg18d = state[3];
                reg30  = state[2];
                reg33  = state[1];

                TLGHAL_SET(reg18d, 0,
                            REG_018D_BIT15_SHIFT,
                            REG_018D_BIT15_MASK);
                TLG_ModifyReg(base_addr, REG_018D          ,   reg18d);
                TLG_ModifyReg(base_addr, REG_0033      ,   reg33 );
                TLG_ModifyReg(base_addr, REG_0030    ,   reg30 );

                
                
                TLG_ReadReg(base_addr, REG_018A,   &reg18a);
                regData = reg18a;
                TLGHAL_SET(regData, 0,
                            REG_018A_BIT9_SHIFT,
                            REG_018A_BIT9_MASK);
                TLG_WriteReg(base_addr,REG_018A,regData);

              state[0] = TLG_STATE2;
                *ms      = 5;
                break;
            }
            else
            {
                
                state[4] ++ ;
                if (state[4]  >= 40) 
                {
                    state[0] = TLG_STATE2;
                    break;
                }
                
                *ms = 5;
                break;
            }
        case TLG_STATE2:
            
            TLG_CHIP_VERS_1_2_BEGIN
#if defined(TLG_CMP_CHIP_1120_1_2) || defined(TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1120_2_3)
            
            TLG_ModifyReg(base_addr, REG_0033, 0x0d01);

#endif 
            TLG_CHIP_VERS_1_1_ELSE
#ifdef  TLG_CMP_CHIP_1120_1_1
#endif 
            TLG_CHIP_VERS_END

            TLG_PowerUpMixer(base_addr);
            
            TLG_SetBits(base_addr, REG_0193, 0,
                        REG_0193_BIT4_0_SHIFT,
                        REG_0193_BIT4_0_MASK);
            state[0] = TLG_STATE_FINAL;
            *ms      = 0;
            break;

        default:
            TLG_REG_LOG((b,"TLG_TFCalibrationASYNC: %d BADSTATE!\n", state[0]));
            break;
    }
}



static int32 getICP(uint32 base_addr, uint16 ndiv, uint32 nr_ref_divide)
{
    
    uint32 vco_freq_cvt;
    int32  vco_freq_mhz;
    uint32 i_fcomp;
    uint32 vco_thresh_lomid_cvt;
    uint32 vco_thresh_midhi_cvt;
    int32  itarget = (TARGET*4);
    int32  icp_denom;
    int32  i_icp;
    uint16 reg1b2, reg1b3;
    
    vco_freq_cvt  = (MHZCVT(27)*ndiv)/nr_ref_divide;
    vco_freq_mhz  = (27*(uint32)(ndiv))/nr_ref_divide;
    i_fcomp       = (27000000 /nr_ref_divide);

    TLG_ReadReg(base_addr, REG_01B2, &reg1b2);
    TLG_ReadReg(base_addr, REG_01B3, &reg1b3);
    reg1b2 &= REG_01B3_BIT13_0_MASK;
    reg1b3 &= REG_01B3_BIT13_0_MASK;

    vco_thresh_lomid_cvt = MHZCVT(reg1b2)/(1000000/i_fcomp);
    vco_thresh_midhi_cvt = MHZCVT(reg1b3)/(1000000/i_fcomp);
    
        TLG1121_VERS_A_2_BEGIN
#if defined(TLG_CMP_CHIP_1121_A_2)
    if (vco_freq_cvt < vco_thresh_lomid_cvt)
        icp_denom = (vco_freq_mhz*LO_SLOPE_3_1 - 4*LO_OFFSET_3_1);
    else if (vco_freq_cvt < vco_thresh_midhi_cvt)
        icp_denom = (vco_freq_mhz*MID_SLOPE_3_1 - 4*MID_OFFSET_3_1);
    else
        icp_denom = (vco_freq_mhz*HI_SLOPE_3_1- 4*HI_OFFSET_3_1);
#endif      
        TLG1121_VERS_A_0_ELSE
#if defined(TLG_CMP_CHIP_1121_A_0)
    if (vco_freq_cvt < vco_thresh_lomid_cvt)
        icp_denom = (vco_freq_mhz*LO_SLOPE_3_1 - 4*LO_OFFSET_3_1);
    else if (vco_freq_cvt < vco_thresh_midhi_cvt)
        icp_denom = (vco_freq_mhz*MID_SLOPE - 4*MID_OFFSET_3_1);
    else
        icp_denom = (vco_freq_mhz*HI_SLOPE_3_1- 4*HI_OFFSET_3_1);
#endif      
        TLG_CHIP_VERS_ELSE
#if defined(TLG_CMP_CHIP_1120_1_2) ||defined(TLG_CMP_CHIP_1120_1_1) ||defined(TLG_CMP_CHIP_1120_2_1) ||defined(TLG_CMP_CHIP_1120_2_3) ||defined(TLG_CMP_CHIP_1120_2_4) 
    if (vco_freq_cvt < vco_thresh_lomid_cvt)
        icp_denom = (vco_freq_mhz*LO_SLOPE - 4*LO_OFFSET);
    else if (vco_freq_cvt < vco_thresh_midhi_cvt)
        icp_denom = (vco_freq_mhz*MID_SLOPE - 4*MID_OFFSET);
    else
        icp_denom = (vco_freq_mhz*HI_SLOPE - 4*HI_OFFSET);
#endif 
    TLG_CHIP_VERS_END

    i_icp = (((4*2*itarget*ndiv)+icp_denom))/(2*icp_denom);

    return max(2, i_icp);
}


void  TLG_SetClkHarmonicNotch(uint32 base_addr, uint32 center_freq_in)
{
    uint32                      center_freq_cvt = HZCVT(center_freq_in);
    int32                       iF;
    uint32                      itF;
    uint16                      bw;
    uint16                      regData         = 0;
    int32                       enable          = 0;
    uint32                      Video_freq_cvt = 0;
    uint32                      Audio_freq_cvt = 0;
    uint32                      Chrom_freq_cvt = 0;
    uint8                       ENABLE_FLG = 0;
    uint16                     notch_bw = 0;
    const tlg_tone_notch_reg_t *p_notch         = &tone_notch_reg[SYS_CLOCK_NOTCH];

    TLG_REG_LOG((b,"TLG_SetClkHarmonicNotch: hz=%d START\n", center_freq_in));
    TLG_GetChnBandwidth(base_addr,  &bw);
    
    iF = center_freq_cvt % HALF_SYS_CLOCK_CVT;
    iF = (iF >= HALF_SYS_CLOCK_CVT/2) ? -iF + HALF_SYS_CLOCK_CVT : -iF;
    itF = (iF < 0) ? -iF : iF;

    if (itF <= MHZCVT(bw/2+1)) 
    {
        switch (bw)
        {
            case TLG_BW_8:
                Video_freq_cvt =   center_freq_cvt - BW_8_VIDEO_CVT;
                Audio_freq_cvt =   center_freq_cvt + BW_8_AUDIO_CVT ;
                Chrom_freq_cvt =   center_freq_cvt + BW_8_CHROM_CVT ;
                break;
            case TLG_BW_7:
                Video_freq_cvt =   center_freq_cvt - BW_7_VIDEO_CVT;
                Audio_freq_cvt =   center_freq_cvt + BW_7_AUDIO_CVT ;
                Chrom_freq_cvt =   center_freq_cvt + BW_7_CHROM_CVT ;
                break;
            case TLG_BW_6:
                Video_freq_cvt =   center_freq_cvt - BW_6_VIDEO_CVT;
                Audio_freq_cvt =   center_freq_cvt + BW_6_AUDIO_CVT ;
                Chrom_freq_cvt =   center_freq_cvt + BW_6_CHROM_CVT ;
                break;
            default: 
                ENABLE_FLG =1;
                break;
        }
        if (Video_freq_cvt % HALF_SYS_CLOCK_CVT == 0) 
        {
            ENABLE_FLG =1;
        }
        if (Audio_freq_cvt % HALF_SYS_CLOCK_CVT == 0) 
        {
            ENABLE_FLG =1;
        }               
        if (Chrom_freq_cvt % HALF_SYS_CLOCK_CVT == 0)              
        {
            ENABLE_FLG =1;
        }

        if(ENABLE_FLG  != 1)
        {
                enable  = 1;
            TLG_SetNotchHz(base_addr, iF*HZCVTUNITS, SYS_CLOCK_NOTCH);
        }
    }
        
        TLG1121_VERS_A_0_BEGIN
    #if defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1121_A_2)
        notch_bw = TLG_NOTCH_MAIN_BW_TLG1121;
    #endif 
    TLG_CHIP_VERS_2_1_ELSE
    #if defined(TLG_CMP_CHIP_1120_1_2) ||defined(TLG_CMP_CHIP_1120_1_1) ||defined(TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1120_2_3)
   
    if (center_freq_in <= TLG_VHF3_CUTOFF_HZ)
        notch_bw = TLG_NOTCH_MAIN_BW_TLG1120_VHF1;
    else
        notch_bw = TLG_NOTCH_MAIN_BW_TLG1120;
    #endif
        TLG_CHIP_VERS_END
    TLG_SetNotchAttrib(base_addr, 
                    0,                               
                    SYS_CLOCK_NOTCH, 
                    notch_bw,                                
                    TLG_OFF,                         
                    0,                               
                    enable,                          
                    TLG_NOTCH_SET_EN|TLG_NOTCH_SET_BYP|TLG_NOTCH_SET_BW
                    );
    TLG_REG_LOG((b,"TLG_SetClkHarmonicNotch: toneHz=%d END\n", regData));
}


TLGDLL_API int TLG_Set_LNA_Mode(uint32 base_addr, int lna_mode)
{
    uint16 uhf_lna_pd     = 1;
    uint16 vhf1_lna_pd    = 1;
    uint16 vhf2_lna_pd    = 1;
    uint16 vhfIII_lna_pd  = 1;
    uint16 regData        = 0;
    uint16 vhf1_cal_overide = 1;
    g_lna_mode = lna_mode;

    TLG_REG_LOG((b,"TLG_Set_LNA_Mode: %d\n", lna_mode));

    switch (lna_mode)
    {
        case TLG_LNA_VHF1:
            vhf1_cal_overide = 0;
            vhf1_lna_pd      = 0;
            break;
        case TLG_LNA_UHF :
            uhf_lna_pd      = 0;
            break;
        case TLG_LNA_VHF3:
            vhfIII_lna_pd   = 0;
            break;
        case TLG_LNA_VHF2:
            vhf2_lna_pd   = 0;
            break;
        default:
            return -1;
    }
        
    TLG_ReadReg(base_addr, REG_0180, &regData);

    
    TLGHAL_CLEAR(regData, REG_0180_BIT12_MASK);

    TLGHAL_SET(regData, uhf_lna_pd,
                REG_0180_BIT15_SHIFT,
                REG_0180_BIT15_MASK);
    TLGHAL_SET(regData, vhf2_lna_pd, 
                REG_0180_BIT14_SHIFT,
                REG_0180_BIT14_MASK);
    TLGHAL_SET(regData, vhf1_lna_pd,
                REG_0180_BIT13_SHIFT,
                REG_0180_BIT13_MASK);
    TLGHAL_SET(regData, vhfIII_lna_pd,
                REG_0180_BIT4_SHIFT,
                REG_0180_BIT4_MASK);
    TLG_ModifyReg(base_addr, REG_0180, regData);

    
    TLG_CHIP_VERS_2_1_BEGIN
 #if defined (TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)
        TLG_SetBits(base_addr, REG_0189, vhf1_cal_overide,
                    REG_0189_BIT4_SHIFT,
                    REG_0189_BIT4_MASK);
#endif 
    TLG_CHIP_VERS_END

    return 0;
}



TLGDLL_API void TLG_TFCalibration(uint32 base_addr, uint32 center_freq_in_hz)
{
    uint32 center_freq         = HZCVT(center_freq_in_hz);
    uint16          reg3c;
    tlg_statep      state;
    int             ms;

    const uint32   UHF_CUTOFF_CVT  = HZCVT(TLG_UHF_CUTOFF_HZ);
    const uint32   VHF3_CUTOFF_CVT = HZCVT(TLG_VHF3_CUTOFF_HZ);
    uint16 lna_mode;

    current_freq_hz = center_freq_in_hz;
    state[0] = TLG_STATE0;


    if ((center_freq > VHF3_CUTOFF_CVT) && (center_freq <= UHF_CUTOFF_CVT))
    {
        lna_mode     = TLG_LNA_VHF3;
        TLG_Set_LNA_Mode(base_addr, lna_mode);
        
        TLG_CHIP_VERS_2_1_BEGIN
 #if defined (TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)
        reg3c        = JUP_2_1_REG_3C;
#endif 
        TLG_CHIP_VERS_1_2_ELSE
#ifdef TLG_CMP_CHIP_1120_1_2
        reg3c        = JUP_1_2_REG_3C;
#endif 
        TLG_CHIP_VERS_1_1_ELSE
#ifdef TLG_CMP_CHIP_1120_1_1
        reg3c        = JUP_1_1_VHF3_UHF_REG_3C;
#endif 
        TLG_CHIP_VERS_END
        TLG_ModifyReg(base_addr, REG_003C, reg3c);
        do
        {
            TLG_TFCalibrationASYNC_VHF3(
                    base_addr           ,
                    center_freq_in_hz   ,
                    TLG_LNA_VHF3       ,
                    0, 
                    1, 
                    1, 
                    &state[0],
                    &ms);
            if (ms)
                    TLG_Delay((uint16)ms);
        } while (state[0] != TLG_STATE_FINAL);
        return;
    }

    if (center_freq > UHF_CUTOFF_CVT)
    {
            TLG_TRACE_LOG((b,"TLG_TFCalibration UHF\n"));
            lna_mode     = TLG_LNA_UHF;
            
            TLG_CHIP_VERS_2_1_BEGIN
 #if defined (TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)
            reg3c        = JUP_2_1_REG_3C;
#endif 
            TLG_CHIP_VERS_1_2_ELSE
#ifdef TLG_CMP_CHIP_1120_1_2
                    reg3c        = JUP_1_2_REG_3C;
#endif 
            TLG_CHIP_VERS_1_1_ELSE
#ifdef TLG_CMP_CHIP_1120_1_1
                    reg3c        = JUP_1_1_VHF3_UHF_REG_3C;
#endif 
            TLG_CHIP_VERS_END
    }
    else
    {

            TLG_TRACE_LOG((b,"TLG_TFCalibration VHF1\n"));
            lna_mode = TLG_LNA_VHF1;
            
            TLG_CHIP_VERS_2_1_BEGIN
 #if defined (TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)
                    reg3c        = JUP_2_1_REG_3C;
#endif 
            TLG_CHIP_VERS_1_2_ELSE
#ifdef TLG_CMP_CHIP_1120_1_2
                    reg3c        = JUP_1_2_REG_3C;
#endif 
            TLG_CHIP_VERS_1_1_ELSE
#ifdef TLG_CMP_CHIP_1120_1_1
                    reg3c        = JUP_1_1_VHF1_REG_3C;
#endif 
            TLG_CHIP_VERS_END
    }
    TLG_ModifyReg(base_addr, REG_003C, reg3c);
    TLG_TRACE_LOG((b,"TLG_TFCalibration::TLG_Set_LNA_Mode start\n"));
    TLG_Set_LNA_Mode(base_addr, lna_mode);
    TLG_TRACE_LOG((b,"TLG_TFCalibration::TLG_Set_LNA_Mode end\n"));
    do
    {
        TLG_TFCalibrationASYNC(
                    base_addr,
                    center_freq_in_hz,
                    lna_mode,
                    &state[0],
                    &ms);
        if (ms)
            TLG_Delay((uint16)ms);
    } while (state[0] != TLG_STATE_FINAL);
}



#if defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1121_A_2)


static void TLG_AltBS_Force_Off(uint32 base_addr)
{
    
    TLG_SetBits(base_addr, REG_01B0_VALUE_1, 0,
                REG_01B0_BIT9_SHIFT,
                REG_01B0_BIT9_MASK);
}


static void TLG_AltBS_Start(uint32 base_addr)
{
    
    TLG_SetBits(base_addr, REG_01B3_VALUE_4, 1,
                REG_01B3_BIT11_SHIFT,
                REG_01B3_BIT11_MASK);
}


static void TLG_AltBS_Finish(uint32 base_addr)
{
    uint16 reg,bank_force;

    
    
    TLG_ReadRegNS(base_addr, REG_01B0, &reg);
    TLGHAL_GET(bank_force, reg, REG_01B0_BIT6_0_SHIFT,
                                REG_01B0_BIT6_0_MASK);
    TLG_REG_LOG((b,"Force Bank idx = %d\n", bank_force));
    
    
    TLG_SetBits(base_addr, REG_01B3_VALUE_4, 0,
                REG_01B3_BIT11_SHIFT,
                REG_01B3_BIT11_MASK);

    
    TLG_ReadReg(base_addr, REG_01B0_VALUE_1, &reg);
    TLGHAL_SET(reg, bank_force, REG_01B0_BIT6_0_SHIFT,
                                REG_01B0_BIT6_0_MASK);
    TLGHAL_SET(reg, 1,          REG_01B0_BIT9_SHIFT,
                                REG_01B0_BIT9_MASK);

    TLG_WriteReg(base_addr, REG_01B0_VALUE_1, reg);
}
#endif      


TLGDLL_API void TLG_SetChnFreqASYNC(uint32 base_addr, uint32 center_freq_in_hz,
                bool isCallCali, uint16 trk_code,
                tlg_statep state, int *ms)
{
    uint32 center_freq         = HZCVT(center_freq_in_hz);
    uint16 reg3c;
    uint32 nr_ref_divide       = NR_REF_DIVIDE;
    
    const uint32   UHF_CUTOFF_CVT  = HZCVT(TLG_UHF_CUTOFF_HZ);
    const uint32   VHF3_CUTOFF_CVT = HZCVT(TLG_VHF3_CUTOFF_HZ);
    uint16 lna_mode, mix_mode, vhf_cap_code, vstd;
#if defined(TLG_CMP_CHIP_1121_A_2)
    
    uint16 vregctl = 1;
#endif 
    
    if (tv_mode != TLG_MODE_FM_RADIO)
    {
        TLG_DisableAudioAuxNotches(base_addr);
    }

    
    TLG_GetChnStandard(base_addr, &vstd);
    TLG_CHIP_VERS_1_2_BEGIN
#if defined(TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1120_1_2) ||defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)
        if (vstd == TLG_VSTD_SECAM_G || vstd == TLG_VSTD_PAL_G)
        {
            uint32 new_center_freq = center_freq - HZCVT(187500); 
            center_freq_in_hz -= 187500;
            TLG_REG_LOG((b,"Video Std=%d, setting Hz from %d to %d\n", 
                         vstd, center_freq, new_center_freq));
            center_freq = new_center_freq;
        }
#endif 
    TLG_CHIP_VERS_END

            current_freq_hz = center_freq_in_hz;

    switch (state[0])
    {
        case TLG_STATE0:
            TLG_AvgFiltInit(&tlg_agc1_filt);
            TLG_AvgFiltInit(&tlg_agc2_filt);
            TLG_AvgFiltInit(&tlg_lock_filt);
            g_tlg_rssi_onetime = 0;             
            

            if (tv_mode == TLG_MODE_ANALOG_TV_UNCOMP)
            {
                uint16 reg;
                
                TLG_ReadReg(base_addr,  REG_006F, &reg);
                TLGHAL_SET(reg, 0, REG_006F_BIT13_12_SHIFT,
                                   REG_006F_BIT13_12_MASK);
                TLG_WriteReg(base_addr, REG_006F, reg);
                g_tlg_vsync_adjust_onetime = 0;

                
                
                
                if (g_tlg_cc_mve == TLG_MVE_NONE)
                {
                    if (TLG_GetMVEMode(base_addr) == TLG_MVE_AUTO)
                    {
                        TLG_REG_LOG((b, "AUTO CC START\n"));
                        g_tlg_cc_mve = TLG_MVE_AUTO;
                        TLG_SetMobileVidEnhancementAux(base_addr, 
                                                       TLG_MVE_MANUAL, 
                                                       40); 
                    }
                }
                
                
                g_tlg_cc_mve_cnt  = 0;

                
                TLG_ChangeSNRmode(base_addr, TLG_MODE_HIGH_SNR, vstd);
                TLG_ChangeREG5Amode(base_addr, TLG_REG5A_MODE_1, vstd);
                TLG_SetSFA(base_addr, vstd, TLG_SFA_HIGH_SNR);
                if (TLG_IsSECAMChnStandard(vstd))
                {
                    TLG_ChangeSecamColorMode(base_addr, TLG_SECAM_COLOR_MODE_1);
                    TLG_ChangeSecamFlickerMode(base_addr, TLG_SECAM_FLICKER_MODE_2);
                }
                
                
                if (!TLG_IsSECAMChnStandard(vstd))
                    TLG_SetBits(base_addr, REG_005A_VALUE_7, 0,
                                           REG_005A_BIT3_2_SHIFT,
                                           REG_005A_BIT3_2_MASK);

                
TLG1121_VERS_A_0_BEGIN
#if defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1121_A_2)
                
                if (center_freq <= VHF3_CUTOFF_CVT || 
                    center_freq > UHF_CUTOFF_CVT)
                {
                    TLG_AltBS_Start(base_addr);
                }
                
                TLG_AltBS_Force_Off(base_addr);
#endif      
TLG_CHIP_VERS_END

TLG1121_VERS_A_2_BEGIN
#if defined(TLG_CMP_CHIP_1121_A_2)
                
#endif    
TLG1121_VERS_A_0_ELSE
#if defined(TLG_CMP_CHIP_1121_A_0)
                
                if (center_freq <= UHF_CUTOFF_CVT || 
                    center_freq >= HZCVT(800000000))
                    TLG_SetBits(base_addr, REG_018F, 0x3a,
                                6, BIT11_6);
                else
                    TLG_SetBits(base_addr, REG_018F, 0x20,
                                6, BIT11_6);
#endif      
TLG_CHIP_VERS_END

                TLG_WriteReg(base_addr, REG_005A_VALUE_2, 0xC771);
            }

            if (center_freq > UHF_CUTOFF_CVT)
            {

                TLG_TRACE_LOG((b,"TLG_SetChnFreqASYNC UHF\n"));

                
                TLG_CHIP_VERS_2_1_BEGIN
 #if defined (TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)
                        reg3c        = JUP_2_1_REG_3C;
#endif 
                TLG_CHIP_VERS_1_2_ELSE
#ifdef TLG_CMP_CHIP_1120_1_2
                        reg3c        = JUP_1_2_REG_3C;
#endif 
                TLG_CHIP_VERS_1_1_ELSE
#ifdef TLG_CMP_CHIP_1120_1_1
                        reg3c        = JUP_1_1_VHF3_UHF_REG_3C;
#endif 
                TLG_CHIP_VERS_END
                        TLG_REG_LOG((b,"TLG_SetChnFreqASYNC lna_mode UHF\n"));

                lna_mode     = TLG_LNA_UHF;
                mix_mode     = 2;
                vhf_cap_code = 0;
                
                TLG_SetBits(base_addr, REG_0191, 0,
                            REG_0191_BIT9_5_SHIFT,
                            REG_0191_BIT9_5_MASK);
                TLG_FixAGC_1_1(base_addr, TLG_LNA_UHF);
                
                TLG1121_VERS_A_2_BEGIN
#if defined(TLG_CMP_CHIP_1121_A_2)
                vregctl = 3;
#endif 
                TLG_CHIP_VERS_END
            }
            else if (center_freq > VHF3_CUTOFF_CVT)
            {
                reg3c = JUP_2_1_REG_3C;
                TLG_REG_LOG((b,"TLG_SetChnFreqASYNC lna_mode VHF3\n"));
                lna_mode     = TLG_LNA_VHF3;
                mix_mode     = 4;
                vhf_cap_code = 1;
                nr_ref_divide = JUPITER_NR_REF_DIVIDE_VHF3;
                TLG_FixAGC_1_1(base_addr, TLG_LNA_VHF3);        
            }
            else
            {

                TLG_TRACE_LOG((b,"TLG_SetChnFreqASYNC VHF1\n"));

                
                TLG_CHIP_VERS_2_1_BEGIN
#if defined (TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)
                        reg3c        = JUP_2_1_REG_3C;
#endif 
                TLG_CHIP_VERS_1_2_ELSE
#ifdef TLG_CMP_CHIP_1120_1_2
                        reg3c        = JUP_1_2_REG_3C;
#endif 
                TLG_CHIP_VERS_1_1_ELSE
#ifdef TLG_CMP_CHIP_1120_1_1
                        reg3c        = JUP_1_1_VHF1_REG_3C;
#endif 
                TLG_CHIP_VERS_END
                TLG_REG_LOG((b,"TLG_SetChnFreqASYNC lna_mode VHF1\n"));

                lna_mode = TLG_LNA_VHF1;
                mix_mode = 1;
                
                TLG_SetBits(base_addr, REG_0036, 0,
                            REG_0036_BIT0_SHIFT,
                            REG_0036_BIT0_MASK);

                
                TLG_SetBits(base_addr, REG_0191, 0,
                            REG_0191_BIT9_5_SHIFT,
                            REG_0191_BIT9_5_MASK);

                
                vhf_cap_code = (uint16)((2*(MHZCVT(684) - 4*center_freq) + center_freq)/(2*center_freq));
                TLG_FixAGC_1_1(base_addr, TLG_LNA_VHF1);
            }
            state[1] = lna_mode; 

            TLG_TRACE_LOG((b,"TLG_SetChnFreqASYNC::TLG_SetSynthesizer start\n"));
            TLG_SetSynthesizer(
                        base_addr,
                        center_freq_in_hz,
                        nr_ref_divide);
            TLG_TRACE_LOG((b,"TLG_SetChnFreqASYNC::TLG_SetSynthesizer end\n"));

            
            
            
            
            

            TLG_TRACE_LOG((b,"TLG_SetChnFreqASYNC::TLG_Set_LNA_Mode start\n"));
            TLG_Set_LNA_Mode(base_addr, lna_mode);
            TLG_TRACE_LOG((b,"TLG_SetChnFreqASYNC::TLG_Set_LNA_Mode end\n"));

            {
                uint16 data;
                TLG_ReadReg(base_addr,      REG_018F, &data);
                
                
                TLG1121_VERS_A_2_BEGIN
#if defined(TLG_CMP_CHIP_1121_A_2)
                TLGHAL_SET(data, vregctl,    
                           REG_018F_BIT7_6_SHIFT,
                           REG_018F_BIT7_6_MASK);
#endif 
                TLG_CHIP_VERS_END
                
                TLGHAL_SET(data, mix_mode,    
                           REG_018F_BIT5_3_SHIFT,
                           REG_018F_BIT5_3_MASK);
                TLG_WriteReg(base_addr,     REG_018F, data);
            }
            
            TLG_ModifyReg(base_addr, REG_003C, reg3c);
            
            
            TLG_SetBits(base_addr, REG_0189, vhf_cap_code,
                        REG_0189_BIT3_0_SHIFT,
                        REG_0189_BIT3_0_MASK);

            

            
            TLG_TRACE_LOG((b,"TLG_SetChnFreqASYNC SetNotch: freq; %01d\n", center_freq_in_hz));
            TLG_SetClkHarmonicNotch(base_addr, center_freq_in_hz);
             
            if (lna_mode != TLG_LNA_VHF3) 
            {
                TLG_BBFilter_Cal(base_addr, center_freq_in_hz);
            }
            state[2] = TLG_STATE0; 

            
            
TLG1121_VERS_A_0_BEGIN
#if defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1121_A_2)
            
            if (tv_mode == TLG_MODE_ANALOG_TV_UNCOMP &&
                (center_freq <= VHF3_CUTOFF_CVT || 
                 center_freq > UHF_CUTOFF_CVT))
            {
                state[0] = TLG_STATE5;
            }
            else
            {
                state[0] = TLG_STATE1;
            }
#endif      
TLG_CHIP_VERS_ELSE
#if defined(TLG_CMP_CHIP_1120_1_2) ||defined(TLG_CMP_CHIP_1120_1_1) ||defined(TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1120_2_3)

            state[0] = TLG_STATE1;
#endif 
TLG_CHIP_VERS_END

            *ms      = 5;       
            break;

#if defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1121_A_2)
        case TLG_STATE5:
            

            
            TLG_AltBS_Finish(base_addr);
            state[0] = TLG_STATE1;
            *ms      = 5;       
            break;
#endif 

        case TLG_STATE1:
            
            
            if (isCallCali && (trk_code == 0))
            {
                if (g_lna_mode == TLG_LNA_VHF3)
                {
                    TLG_REG_LOG((b,"TLG_TFCalibrationASYNC_VHF3 g_lna_mode; %01d\n", g_lna_mode));
                    TLG_TFCalibrationASYNC_VHF3(
                        base_addr,
                        center_freq_in_hz,
                        g_lna_mode,
                        0, 
                        1, 
                        1, 
                        &state[2],
                        ms);        
                }
                else
                {
                    TLG_REG_LOG((b,"TLG_TFCalibrationASYNC_VHF1/UHF g_lna_mode; %01d\n", g_lna_mode));
                    TLG_TFCalibrationASYNC(
                                base_addr,
                                center_freq_in_hz,
                                g_lna_mode,
                                &state[2],
                                ms);
                }
                if (state[2] != TLG_STATE_FINAL)
                {
                    break;
                }
            }
            
            
            state[1] = trk_code;        
            state[0] = TLG_STATE2;      
            state[3] = TLG_STATE0;      

            
            if (trk_code)
            {
                uint16  old_search_limit;

                TLG_ReadReg(base_addr , REG_0039, &old_search_limit);
                TLG_WriteReg(base_addr, REG_0039, 0);
                TLG_WriteReg(base_addr, REG_0038, trk_code);
                state[2] = old_search_limit; 
                *ms      = 5;
                break;
            }

        case TLG_STATE2:
            
            
            TLG_TRACE_LOG((b,"TLG_SetChnFreqASYNC::TLG_STATE2\n"));
            if (g_lna_mode != TLG_LNA_VHF3)
            {
                if (state[1])
                {
                    TLG_WriteReg(base_addr,REG_0039,state[2]);
                    state[1] = 0; 
                }
            }
            TLG_FixAGC_1_1(base_addr, g_lna_mode);
            state[4] = TLG_STATE0;
            state[0] = TLG_STATE3;
            break;
            
        case TLG_STATE3:
            TLG_Fix_LNA_AGC(base_addr, &state[4], ms);
            if (state[4] != TLG_STATE_FINAL)
                break;     
            state[0] = TLG_STATE4;       
            state[3] = TLG_STATE0;   
            break;
            
            
            case TLG_STATE4:
            
            TLG_TRACE_LOG((b,"TLG_SetChnFreqASYNC::TLG_SoftResetASYNC start\n"));
            TLG_SoftResetASYNC(base_addr, &state[3], ms);
            TLG_TRACE_LOG((b,"TLG_SetChnFreqASYNC::TLG_SoftResetASYNC end\n"));
            if (state[3] != TLG_STATE_FINAL)
                break;
            state[0] = TLG_STATE_FINAL;
            *ms      = 0;
            break;

        default: TLG_REG_LOG((b,"TLG_SetChnFreqASYNC: %d BADSTATE!\n", state[0]));
            break;
    }
}


#define NOTCH_DELAY 25

void  TLG_SetNotchHz(uint32 base_addr, int32 center_freq_in, uint32 notch_num)
{
    uint32 abs_freq, regHigh16, regLow12 = 0;
    const tlg_tone_notch_reg_t *p_notch = &tone_notch_reg[notch_num];
    bool add_1687500 = false, add_3375000 = false;
    bool  extend_reg_en = false;
    if (notch_num >= NUM_TONE_NOTCH)
    {
        return;
    }
    abs_freq = (center_freq_in < 0) ? -(center_freq_in) : (center_freq_in);

        TLG1121_VERS_A_0_BEGIN
    #if defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1121_A_2)
        extend_reg_en = (notch_num == TLG_LOWEST_NOTCH_FILTER_WITH_EXTENDED_FREQUENCY) ? true : false;
    #endif  
        TLG_CHIP_VERS_END
    
    if (abs_freq >= 6749794) {
        regHigh16 = 0x7fff;
        regLow12 = 0xfff;
    } else {
        if (abs_freq >= 3375000)
        {
            
            add_3375000 = true;
            abs_freq -= 3375000;
        }
        if (abs_freq >= 1687500)
        {
            
            add_1687500 = true;
            abs_freq -= 1687500;
        }
        if (extend_reg_en)
        {

            
            
                TLG1121_VERS_A_0_BEGIN
            #if defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1121_A_2)
            uint32 remainder_ep;
            regHigh16 = (abs_freq<<11)/HALF_SYS_CLOCK_HZ_DIV_32;
            
            remainder_ep = (abs_freq << TLG_NOTCH_FILTER_EXTENDED_PRECISION_BITS) -
                           (regHigh16 * HALF_SYS_CLOCK_HZ_DIV_32);

            
            regLow12 = (remainder_ep << 12) / HALF_SYS_CLOCK_HZ_DIV_32;
            #endif  
                TLG_CHIP_VERS_END
        }
        else
        {
            
            regHigh16 = ((abs_freq*(1<<11)+HALF_SYS_CLOCK_HZ_DIV_32/2)/HALF_SYS_CLOCK_HZ_DIV_32);
        }
    }

    if (add_3375000)
    {
        regHigh16 += 0x4000;
    }
    if (add_1687500)
    {
        regHigh16 += 0x2000;
    }
    if(center_freq_in <0)
    {
        regHigh16 |= REG_004B_BIT15_MASK;
    } 
    else 
    {
        regHigh16 &= ~REG_004B_BIT15_MASK;
    }

    
    TLG_WriteReg(base_addr, p_notch->tone_notch_reg_2,regHigh16);

    if (extend_reg_en)
    {
        
        TLG1121_VERS_A_0_BEGIN
        #if defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1121_A_2)
        uint16 regDataExtBits11_4, regDataExtBits3_0;
        regDataExtBits11_4 = (regLow12 & REG_00C0_BIT11_4_MASK) >>
                                REG_00C0_BIT11_4_SHIFT;
        regDataExtBits3_0 = (regLow12 & REG_00C0_BIT3_0_MASK) >>
                                REG_00C0_BIT3_0_SHIFT;
        TLG_SetBits(base_addr, p_notch->tone_notch_reg_1, regDataExtBits11_4,
                    REG_00C0_BIT15_8_SHIFT,
                    REG_00C0_BIT15_8_MASK);
        
        TLG_SetBits(base_addr, REG_00D7_VALUE_1,
                    regDataExtBits3_0,
                     p_notch->tone_notch_reg_3_shift,
                     p_notch->tone_notch_reg_3_mask);
        #endif  
        TLG_CHIP_VERS_END
}

}


void  TLG_SetNotchAttrib(
                uint32 base_addr        , 
                int32 freq_hz          , 
                uint32 notch_num        ,
                uint16 bw               ,
                uint16 by_pass          ,
                uint16 hw_avg           ,
                uint16 tone_en,
                uint16 actions)
{
    uint16 data;
    const tlg_tone_notch_reg_t *p_notch_reg  = &tone_notch_reg[notch_num];
    TLG_REG_LOG((b,"TLG_SetNotchAttrib: notch=%01d actions=0x%x  START\n", notch_num, actions));
    TLG_REG_LOG((b,"                   hz=%d, bw=%01d, byp=%01d, hw_avg=%01d, en=%01d\n", 
                    freq_hz, 
                    bw, 
                    by_pass, 
                    hw_avg,
                    tone_en));
#ifdef OPTIMIZE_AUDIO_NOTCH
    
    TLG_ReadReg(base_addr, p_notch_reg->tone_notch_reg_1, &data);
    TLGHAL_SET(data,   bw,
                REG_004A_BIT6_3_SHIFT   ,
                REG_004A_BIT6_3_MASK);
    
    TLGHAL_SET(data, by_pass,
                REG_004A_BIT1_SHIFT   ,
                REG_004A_BIT1_MASK);
    TLGHAL_SET(data, 1,
                REG_004A_BIT0_SHIFT,
                REG_004A_BIT0_MASK);
    TLG_WriteReg(base_addr, p_notch_reg->tone_notch_reg_1, data);
#else 

    if (actions & TLG_NOTCH_SET_HZ)
            TLG_SetNotchHz(base_addr, freq_hz, notch_num);
    
    if (actions & TLG_NOTCH_SET_AVG)
        TLG_SetBits(base_addr, 
                    REG_004A, 
                    hw_avg,
                    REG_004A_BIT11_8_SHIFT   ,
                    REG_004A_BIT11_8_MASK);

    if (actions | (TLG_NOTCH_SET_BW|TLG_NOTCH_SET_BYP|TLG_NOTCH_SET_EN))
    {
        TLG_ReadReg(base_addr, p_notch_reg->tone_notch_reg_1, &data);
        
        if (actions & TLG_NOTCH_SET_BW)
            TLGHAL_SET(data,   bw,
                        REG_004A_BIT6_3_SHIFT   ,
                        REG_004A_BIT6_3_MASK);
        
        if (actions & TLG_NOTCH_SET_BYP)
            TLGHAL_SET(data, by_pass,
                        REG_004A_BIT1_SHIFT   ,
                        REG_004A_BIT1_MASK);

        
        if (actions & TLG_NOTCH_SET_EN)
            TLGHAL_SET(data, tone_en,
                        REG_004A_BIT0_SHIFT,
                        REG_004A_BIT0_MASK);

        TLG_WriteReg(base_addr, p_notch_reg->tone_notch_reg_1, data);
    }
#endif 
    TLG_REG_LOG((b,"TLG_SetNotchAttrib: Reg0x%x=0x%04x END\n", p_notch_reg->tone_notch_reg_1, data));
}

void  TLG_GetAudioNotch(
                uint32 base_addr        , 
                uint32 notch_num        ,
                uint32 *p_tone_avg_len  ,
                uint32 *p_tone_bw       ,
                uint32 *p_tone_hold     ,
                uint32 *p_tone_byp      ,
                uint32 *p_tone_en
                )
{
    uint16 data;
    uint16 value;
    const tlg_tone_notch_reg_t *p_notch  = &tone_notch_reg[notch_num];
    TLG_ReadReg(base_addr, p_notch->tone_notch_reg_1, &data);
    TLGHAL_GET( value, data,
                REG_004A_BIT11_8_SHIFT,
                REG_004A_BIT11_8_MASK);
    *p_tone_avg_len = (uint32) value;
    TLGHAL_GET( value, data,
                REG_004A_BIT6_3_SHIFT   ,
                REG_004A_BIT6_3_MASK);
    *p_tone_bw = (uint32) value;
    TLGHAL_GET( value, data,
                REG_004A_BIT2_SHIFT,
                REG_004A_BIT2_MASK);
    *p_tone_hold = (uint32) value;
    TLGHAL_GET( value, data,
                REG_004A_BIT1_SHIFT   ,
                REG_004A_BIT1_MASK);
    *p_tone_byp = (uint32) value;
    TLGHAL_GET( value, data,
                REG_004A_BIT0_SHIFT,
                REG_004A_BIT0_MASK);
    *p_tone_en = (uint32) value;
}

TLGDLL_API int TLG_Set_FM_StepSize(
                uint32 base_addr        , 
                uint32 step_size_hz
                )
{
    uint32 center_freq_hz = 2750000; 
    uint32 effective_step_size_hz = step_size_hz / FM_AUDIO_STEP_SIZE_DIV;

    g_step_size_hz = step_size_hz;
    TLG_REG_LOG((b, "   TLG_Set_FM_StepSize: %d\n", g_step_size_hz));
    TLG_SetNotchHz(base_addr, center_freq_hz - effective_step_size_hz, AUDIO_ADJ_1_NOTCH);
    TLG_SetNotchHz(base_addr, center_freq_hz + effective_step_size_hz, AUDIO_ADJ_2_NOTCH);
    return(TLG_ERR_SUCCESS);
}

int TLG_DisableNotch(uint32 base_addr, int notch_num)
{
    
    TLG_REG_LOG((b, "TLG_DisableNotches: "));
    TLG_SetNotchAttrib(base_addr,
                    0,                
                    notch_num,
                    0,                
                    0,                
                    0,                
                    TLG_OFF,          
                    TLG_NOTCH_SET_EN);
    return(TLG_ERR_SUCCESS);
}

int TLG_DisableAudioAuxNotches(uint32 base_addr)
{
    TLG_DisableNotch(base_addr, AUDIO_ADJ_1_NOTCH);
    TLG_DisableNotch(base_addr, AUDIO_ADJ_2_NOTCH);
    return(TLG_ERR_SUCCESS);
}


int TLG_SetAudioNotches(
                uint32 base_addr, 
                uint32 freq_hz          , 
                uint32 step_size_hz     , 
                uint32 is_read_RSSI_Mode)
{
    uint16 fm_bw;
    uint16 adj_bw;
    uint32 effective_step_size_hz = step_size_hz;
    uint16 bypass = 0;  

    
    TLG_CHIP_VERS_2_1_BEGIN
#if defined(TLG_CMP_CHIP_1120_2_1) ||defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)
            bypass = 1;
#endif 
    TLG_CHIP_VERS_END

            effective_step_size_hz = step_size_hz;

    TLG_REG_LOG((b, "step_size %01d \n", step_size_hz));

    if (is_read_RSSI_Mode)
    {
        
        effective_step_size_hz = 50000;  
        fm_bw  = (uint16) TLG_NOTCH_ADJ_1_2_BW;
        adj_bw = (uint16) TLG_NOTCH_ADJ_3_4_BW;
    }
    else
    {
        if (step_size_hz > 100000)
        {
                
                fm_bw = 1;
        }
        else if (step_size_hz > 75000)
        {
                
                fm_bw = 2;
        }
        else
        {
                
                fm_bw = 3;
        }
        adj_bw = fm_bw;
    }
#ifndef OPTIMIZE_AUDIO_NOTCH
    TLG_SetNotchAttrib(
                base_addr,            
                freq_hz,              
                AUDIO_MAIN_NOTCH,    
                (uint16) TLG_NOTCH_AUD_MAIN_BW,                 
                1,                                          
                7,                                          
                1,                                           
                TLG_NOTCH_SET_ALL);
#endif 
    TLG_SetNotchAttrib(
                base_addr, 
                freq_hz  - effective_step_size_hz, 
                AUDIO_ADJ_1_NOTCH,
                fm_bw,                                      
                bypass,                                     
                7,                                          
                1,                                           
                TLG_NOTCH_SET_ALL);
    TLG_SetNotchAttrib(
                    base_addr,            
                freq_hz  + effective_step_size_hz,        
                AUDIO_ADJ_2_NOTCH,
                fm_bw,                                      
                bypass,                                     
                7,                                          
                1,                                           
                TLG_NOTCH_SET_ALL
                );
    return(TLG_ERR_SUCCESS);
}

void TLG_SetFMNotches(uint32 base_addr, 
                     uint32 freq_hz)
{
    const uint16 fm_bw = (uint16) TLG_NOTCH_ADJ_1_2_BW;
    const uint16 adj_bw = (uint16) TLG_NOTCH_ADJ_3_4_BW;
    const uint32 effective_step_size_hz = 50000;
    uint16 bypass = 0;  

    
    TLG_CHIP_VERS_2_1_BEGIN
#if defined(TLG_CMP_CHIP_1120_2_1) ||defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)
            bypass = 1;
#endif 
    TLG_CHIP_VERS_END


    TLG_SetNotchAttrib(
                base_addr,            
                freq_hz,              
                AUDIO_MAIN_NOTCH,    
                (uint16) TLG_NOTCH_AUD_MAIN_BW,                 
                1,                                          
                0xf,                                          
                1,                                           
                TLG_NOTCH_SET_ALL);

    TLG_SetNotchAttrib(
                base_addr, 
                freq_hz  - effective_step_size_hz, 
                AUDIO_ADJ_1_NOTCH,
                fm_bw,                                      
                bypass,                                     
                0xf,                                          
                1,                                           
                TLG_NOTCH_SET_ALL);
    TLG_SetNotchAttrib(
                    base_addr,            
                freq_hz  + effective_step_size_hz,        
                AUDIO_ADJ_2_NOTCH,
                fm_bw,                                      
                bypass,                                     
                0xf,                                          
                1,                                           
                TLG_NOTCH_SET_ALL
                );
    return;
}

int TLG_GetNotchPwr(uint32 base_addr, int notch_num, int read_num)
{
    const tlg_tone_notch_reg_t *p_notch = &tone_notch_reg[notch_num];
    int32  tmp_ssi = 0;
    int32  avg_ssi = 0;
    int32  pwr;
    uint16 notchStat2;
    uint16 agcStat3;
    int    i;

    TLG_REG_LOG((b,"TLG_GetNotchPwr: notch=%d, read=%d START\n", notch_num, read_num));

    for(i = 0; i < read_num ; i++)
    {
        TLG_ReadRegNS(base_addr, p_notch->notch_status2_addr, &notchStat2);
        TLG_ReadRegNS(base_addr, REG_0123, &agcStat3);
        tmp_ssi = notchStat2 * agcStat3;
        avg_ssi += tmp_ssi;
    }
    pwr = (avg_ssi>>10)/read_num;

    TLG_REG_LOG((b,"TLG_GetNotchPwr: pwr=%d END\n", pwr));

    return (pwr);
}

int TLG_GetAudPllLockStatHz(uint32 base_addr, int32 freq_hz)
{
    uint16  old55, 
            luma_offset, 
            reg;
    int32   luma_shift, 
            lock = 0;
    int     i;

    TLG_REG_LOG((b,"TLG_GetAudPllLockStatHz: hz=%d START\n", freq_hz));

    
    TLG_ReadReg(base_addr, 0x55, &old55);

    
    luma_shift = 4500000 - freq_hz;
    luma_offset = (uint16)((((luma_shift/1000)<<16) + 13499)/13500);
    TLG_WriteReg(base_addr, 0x55, luma_offset);

    
    TLG_ModifyBits(base_addr, 
                    0x2, 
                    1,
                    7   ,
                    BIT7);

    
    for (i=0; i< 5; i++)
    {
            TLG_ReadRegNS(base_addr, REG_013B, &reg);
            lock += (reg & REG_013B_BIT3_MASK) ? 1 : 0;
    }

    
    TLG_ModifyBits(base_addr, 
                    0x2, 
                    0,
                    7   ,
                    BIT7);

    
    TLG_WriteReg(base_addr, 0x55, old55);

    TLG_REG_LOG((b,"TLG_GetAudPllLockStatHz: lock=%d END\n", lock));
    return lock;
}


int TLG_ReadNotchPwrAvgNotches(
                uint32 base_addr, 
                uint32 freq_hz          , 
                uint32 step_size_hz     , 
                int read_num ,
                uint32* status)
{
    int pwr_m, pwr_a1, pwr_a2=0;
    int result = CHANNEL_NOT_EXIST;

    TLG_REG_LOG((b, "TLG_ReadNotchPwrAvgNotches: bw=4 freq=%d, step=%d, read=%d\n",
                            freq_hz, step_size_hz, read_num));

    TLG_SetAudioNotches(
                base_addr, 
                freq_hz          , 
                step_size_hz     , 
                1     
                    );
    pwr_m  = TLG_GetNotchPwr(base_addr, AUDIO_MAIN_NOTCH, read_num);
    tlg_cur_fm_ssi = pwr_m;
    if(pwr_m >= TLG_SSI_THRESHOLD)
    {
        pwr_a1 = TLG_GetNotchPwr(base_addr, AUDIO_ADJ_1_NOTCH, read_num);
    if (pwr_a1 < pwr_m) 
    {
            pwr_a2  = TLG_GetNotchPwr(base_addr, AUDIO_ADJ_2_NOTCH, read_num);
        if (pwr_a2 < pwr_m) 
                            result = pwr_m;
                        }
                    } 
    return result;
}

int TLG_CheckFMPower(
    uint32 base_addr, 
    uint32 freq_hz, 
    uint32* status)
{
    uint16 pwr_m, pwr_a1, pwr_a2;
    uint16 notch_m, notch_a1, notch_a2;
    
    int result = CHANNEL_NOT_EXIST;
    const tlg_tone_notch_reg_t *p_notch_m = &tone_notch_reg[AUDIO_MAIN_NOTCH];
    const tlg_tone_notch_reg_t *p_notch_a1 = &tone_notch_reg[AUDIO_ADJ_1_NOTCH];
    const tlg_tone_notch_reg_t *p_notch_a2 = &tone_notch_reg[AUDIO_ADJ_2_NOTCH];    
    
    TLG_ReadRegNS(base_addr, p_notch_m->notch_status2_addr, &notch_m);
    
    TLGHAL_GET(pwr_m,notch_m,REG_015D_BIT11_0_SHIFT,REG_015D_BIT11_0_MASK);
     tlg_cur_fm_ssi = pwr_m; 
    if(pwr_m >= TLG_FMSSI_THRESHOLD)
    {
        TLG_ReadRegNS(base_addr, p_notch_a1->notch_status2_addr, &notch_a1);
        TLGHAL_GET(pwr_a1,notch_a1,REG_015D_BIT11_0_SHIFT,REG_015D_BIT11_0_MASK);

        if (pwr_a1 < pwr_m) 
        {
            TLG_ReadRegNS(base_addr, p_notch_a2->notch_status2_addr, &notch_a2);
            TLGHAL_GET(pwr_a2,notch_a2,REG_015D_BIT11_0_SHIFT,REG_015D_BIT11_0_MASK);            
            if (pwr_a2 < pwr_m)
            { 
                result = pwr_m;
            }
        }
    } 
    return result;    
}

int TLG_ReadLumaNotchPwr(uint32 base_addr, int read_num)
{
    int32  freq_hz=0;
    uint16 vstd;

    TLG_GetChnStandard(base_addr, &vstd);

    switch(vstd)
    {
        case TLG_VSTD_PAL_SECAM_B:
        case TLG_VSTD_SECAM_B:
        case TLG_VSTD_PAL_B  : freq_hz = -2250000; break;
        case TLG_VSTD_NTSC_M :
        case TLG_VSTD_PAL_M  :
        case TLG_VSTD_PAL_NC :
        case TLG_VSTD_PAL_N  : freq_hz = -1750000; break;
        case TLG_VSTD_PAL_G  :
        case TLG_VSTD_SECAM_G: freq_hz = -2562500; break; 
        default              : freq_hz = -2750000; break;
    }
    TLG_REG_LOG((b,"TLG_ReadLumaNotchPwr: freq=%d\n", freq_hz));
    TLG_SetNotchAttrib(
                base_addr,            
                freq_hz,              
                MAIN_NOTCH,    
                4,                                          
                1,                                          
                7,                                          
                1,                                           
                TLG_NOTCH_SET_ALL);

    return TLG_GetNotchPwr(base_addr, MAIN_NOTCH, read_num);
}

int TLG_GetAudioCarrierPwr(uint32 base_addr, int read_num)
{
    int32  freq_hz;
    uint16 vstd;
    uint32 status = 0;

    TLG_GetChnStandard(base_addr, &vstd);

    switch(vstd)
    {
        case TLG_VSTD_PAL_B     :
        case TLG_VSTD_PAL_I     : freq_hz = 3250000; break;
        case TLG_VSTD_PAL_D     :
        case TLG_VSTD_PAL_D1    :
        case TLG_VSTD_PAL_K     : freq_hz = 3750000; break;
        default                 : freq_hz = 2750000; break;
    }
    TLG_REG_LOG((b, "\n\n\nTLG_ReadAudioNotchPwr: g_step_size: %01d, read_num %d\n\n\n", g_step_size_hz, read_num));
    return TLG_ReadNotchPwrAvgNotches( base_addr, freq_hz, g_step_size_hz, read_num, &status);
}

TLGDLL_API int TLG_SetTVChnFreqASYNC(uint32 base_addr, uint32 center_freq_hz,
                tlg_statep state, int *ms)
{
    TLG_ModeSetup(base_addr, TLG_MODE_SETUP_TV, 0);
    TLG_SetChnFreqASYNC(base_addr, center_freq_hz, true, 0, state, ms);
    return(TLG_ERR_SUCCESS);
}





TLGDLL_API int TLG_SetChnFreq(uint32 base_addr, uint32 center_freq_hz)
{
#ifdef TLG_EMBEDDED_TIMERS
    tlg_statep state;
    int        ms;

    state[0] = TLG_STATE0;
    do 
    {
        TLG_SetChnFreqASYNC(base_addr, center_freq_hz, true, 0, state, &ms);
        if (ms)
        TLG_Delay((uint16)ms);
    } while (state[0] != TLG_STATE_FINAL);
#endif 
    return TLG_ERR_SUCCESS;
}



TLGDLL_API int TLG_SetTVChnFreq(uint32 base_addr, uint32 center_freq_hz)
{
    int res = TLG_ERR_NOT_SUPPORTED;
#ifdef TLG_EMBEDDED_TIMERS
    tlg_statep state;
    int        ms;

    state[0] = TLG_STATE0;
    do 
    {
        res = TLG_SetTVChnFreqASYNC(base_addr, center_freq_hz, state, &ms);
        if (res != TLG_ERR_SUCCESS) break;
        if (ms)
        TLG_Delay((uint16)ms);
    } while (state[0] != TLG_STATE_FINAL);
#endif 
    return res;
}







    



TLGDLL_API int TLG_SetFMChnFreqASYNC(uint32 base_addr, uint32 center_freq_hz,
                int32 slope, int32 offset, tlg_statep state, int *ms)
{
    TLG_ModeSetup(base_addr, TLG_MODE_SETUP_FM, 0);
    switch (state[0])
    {
        case TLG_STATE0:
            
            state[1] = 0;            
            state[2] = TLG_STATE0;   

        case TLG_STATE1:
            
            
            if (slope && offset)
            {
                state[1] = (uint16)((HZCVT(center_freq_hz)*slope + offset  + 500000)/(1000000));
            }
            else
            {
                TLG_SetChnFreqASYNC(base_addr, center_freq_hz, true, 0, &state[2], ms);
                if (state[2] != TLG_STATE_FINAL)
                {
                    state[0] = TLG_STATE1;
                    break;
                }
            }
            state[2] = TLG_STATE0;   

        case TLG_STATE2:
            
            
            TLG_SetChnFreqASYNC(base_addr, center_freq_hz - CHANNEL_FREQ_OFFSET_HZ, false, state[1], &state[2], ms);
            if (state[2] != TLG_STATE_FINAL)
            {
                state[0] = TLG_STATE2;
            } 
            else 
            {
                state[0] = TLG_STATE_FINAL;
            }
            break;

        default: TLG_REG_LOG((b,"TLG_SetFMChnFreqASYNC: %d BADSTATE!\n", state[0]));
             break;
    }
    return(TLG_ERR_SUCCESS);
}



TLGDLL_API int TLG_SetFMChnFreq(uint32 base_addr, uint32 center_freq_hz,
                int32 slope, int32 offset)
{
    int res = TLG_ERR_NOT_SUPPORTED;

#ifdef TLG_EMBEDDED_TIMERS
    tlg_statep state;
    int        ms;

    state[0] = TLG_STATE0;
    do 
    {
        res = TLG_SetFMChnFreqASYNC(base_addr, center_freq_hz, slope, offset, state, &ms);
        if (res != TLG_ERR_SUCCESS) break;
        if (ms)
            TLG_Delay((uint16)ms);
    } while (state[0] != TLG_STATE_FINAL);
#endif 
    return(res);
}






TLGDLL_API int TLG_ReleaseLnaAgc_Gain(uint32 base_addr)
{
    TLG_REG_LOG((b, "TLG_ReleaseLnaAgc_Gain\n"));
    TLG_WriteReg(base_addr, REG_0030, 0x0000);
    return(TLG_ERR_SUCCESS);
}

TLGDLL_API int TLG_ForceLnaAgc_Gain(uint32 base_addr)
{
    
    
    TLG_WriteReg(base_addr,REG_002E,  0x3A0D);
    
    TLG_WriteReg(base_addr,REG_002F, 0x7F0D);
    TLG_REG_LOG((b, "TLG_ForceLnaAgc_Gain\n"));
    TLG_WriteReg(base_addr, REG_0030, 0xC000);
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_FastScanTVChnASYNC(uint32 base_addr,
                uint32 center_freq_hz,
                tlg_statep state, int *ms)
{
    uint16 regData;
    int    pwr;
    int    res = CHANNEL_NOT_EXIST;

    
    TLG_NULLPTR_CHECK(state);
    TLG_NULLPTR_CHECK(ms);

    switch (state[0])
{
        case TLG_STATE0:
            
            state[1] = TLG_STATE0; 

        case TLG_STATE1:
            
            TLG_SetTVChnFreqASYNC(base_addr, center_freq_hz, &state[1], ms);
            if (state[1] != TLG_STATE_FINAL)
            {
                    state[0] = TLG_STATE1;
                    break;
            }
            TLG_ForceLnaAgc_Gain(base_addr);
            TLG_ReleaseLnaAgc_Gain(base_addr);

            
            pwr = TLG_ReadLumaNotchPwr(base_addr, 5);
            if (pwr < tlg_tv_ssi_threshold )
            {
                TLG_REG_LOG((b, "\n\nTLG_FastScanTVChnASYNC\nnot enough luma energy\n\n\n"));
                state[0] = TLG_STATE_FINAL;
                *ms      = 0;
                TLG_ReadRegNS(base_addr, REG_013B,&regData);
                log_printf(("not enough luma energy, VSYNC %s, LUMA %s, CHROMA %s\n", 
                        regData & REG_013B_BIT5_MASK  ? "Locked" : "Not locked",
                        regData & REG_013B_BIT1_MASK   ? "Locked" : "Not locked",
                        regData & REG_013B_BIT2_MASK ? "Locked" : "Not locked"
                        ));
                
                break;
            }
            state[1] = 0; 

            

        case TLG_STATE2:
            
            
            TLG_ReadRegNS(base_addr, REG_013B,&regData);
            if (regData & (REG_013B_BIT5_MASK | REG_013B_BIT1_MASK | REG_013B_BIT2_MASK))
            {
                state[0] = TLG_STATE_FINAL;
                *ms      = 0;
                res      = regData; 
                log_printf(("is locked\n"));

            }
            else
            {
                if ((int)state[1] < FAST_TVSCAN_WAITFOR_LOCK_TIMEOUT1)
                {
                    TLG_REG_LOG((b, "\n\n\nWATING FOR LOCK\n\n\n"));
                    state[1]++;
                    state[0] = TLG_STATE2;
                    *ms      = 30;
                } 
                else 
                {
                    state[0] = TLG_STATE_FINAL;
                    *ms      = 0;
                    log_printf(("is NOT locked 0x%04X, VSYNC %s, LUMA %s, CHROMA %s\n", regData,
                            regData & REG_013B_BIT5_MASK  ? "Locked" : "Not locked",
                            regData & REG_013B_BIT1_MASK   ? "Locked" : "Not locked",
                            regData & REG_013B_BIT2_MASK ? "Locked" : "Not locked"
                            ));
                }
            }
            break;

        default: TLG_REG_LOG((b,"TLG_ScanTVChnASYNC: %d BADSTATE!\n", state[0]));
                 break;
    }
    return res;
}

#define TVSCAN_WAITFOR_LOCK_TIMEOUT1    3  
#define TVSCAN_WAITFOR_LOCK_TIMEOUT2    6  


TLGDLL_API int TLG_FastScanTVChn(uint32 base_addr, uint32 center_freq_hz)
{
    int        res = CHANNEL_NOT_EXIST;

#ifdef TLG_EMBEDDED_TIMERS
    tlg_statep state;
    int        ms=0;

#ifdef TLG_VSCREEN_LOCK
    TLG_UnlockVScreen(base_addr);
#endif

    state[0] = TLG_STATE0;
    do 
    {
        res = TLG_FastScanTVChnASYNC(base_addr, center_freq_hz, state, &ms);
        if (res != TLG_ERR_SUCCESS) break;
        if (ms)
            TLG_Delay((uint16)ms);
    } while (state[0] != TLG_STATE_FINAL);

#ifdef TLG_NO_SSI_READOUT
    TLG_LockVScreen(base_addr);
#endif

#endif 

    return res;
}


TLGDLL_API int TLG_ScanTVChnASYNC(uint32 base_addr, uint32 center_freq_hz,
                tlg_statep state, int *ms)
{
    uint16 regData;

    TLG_NULLPTR_CHECK(state);
    TLG_NULLPTR_CHECK(ms);

    

    switch (state[0])
    {
        case TLG_STATE0:
            state[1] = TLG_STATE0; 

        case TLG_STATE1:
            TLG_REG_LOG((b, "TLG_ScanTVChnASYNC::TLG_SetTVChnFreqASYNC\n\n\n"));
            TLG_SetTVChnFreqASYNC(base_addr, center_freq_hz, &state[1], ms);
            if (state[1] != TLG_STATE_FINAL)
            {
                    state[0] = TLG_STATE1;
                    break;
            }
            TLG_ForceLnaAgc_Gain(base_addr);
            TLG_ReleaseLnaAgc_Gain(base_addr);
            state[1] = 0 ;

        case TLG_STATE2:
            
            TLG_ReadRegNS(base_addr, REG_013B,&regData);
            if(!(regData & (REG_013B_BIT5_MASK | REG_013B_BIT1_MASK | REG_013B_BIT2_MASK)))
            {
                    if (state[1]++ >= TVSCAN_WAITFOR_LOCK_TIMEOUT1)
                    {
                            state[0] = TLG_STATE_FINAL;
                            *ms      = 0;
                    } else {
                            state[0] = TLG_STATE2; 
                            *ms      = 100;
                    }
                    return CHANNEL_NOT_EXIST;
            }
            TLG_REG_LOG((b,"First Lock Loop=%d ms\n", (state[1]-1)*100));
            state[1] = 0;  

        case TLG_STATE3:
             
            TLG_ReadRegNS(base_addr, REG_013B,&regData);
            if(regData & REG_013B_BIT5_MASK)
            {
#ifdef FIX_AGC_PROBLEM
                TLG_FixAGC(base_addr);
#endif 

                state[0] = TLG_STATE_FINAL;
                *ms      = 0;
                TLG_REG_LOG((b,"Second Lock Loop=%d ms\n", (state[1]-1)*100));

                return regData; 
            } 
            else 
            {
                if (state[1]++ >= TVSCAN_WAITFOR_LOCK_TIMEOUT2)
                {
                    state[0] = TLG_STATE_FINAL;
                    *ms      = 0;
                } 
                else 
               {
                    state[0] = TLG_STATE3; 
                    *ms      = 100;
                }
            }
            break;

        default: TLG_REG_LOG((b,"TLG_ScanTVChnASYNC: %d BADSTATE!\n", state[0]));
                 break;
    }
    return CHANNEL_NOT_EXIST;
}


TLGDLL_API int TLG_ScanTVChn(uint32 base_addr, uint32 center_freq_hz)
{
    int        res = CHANNEL_NOT_EXIST;

#ifdef TLG_EMBEDDED_TIMERS
    tlg_statep state;
    int        ms=0;

#ifdef TLG_VSCREEN_LOCK
    TLG_UnlockVScreen(base_addr);
#endif
    TLG_REG_LOG((b, "TLG_ScanTVChn center freq %d start\n", center_freq_hz));


#ifdef TLG_SUPPORT_FM_AVC 
    TLG_AutoVolumeControlReset(base_addr);
#endif 

    state[0] = TLG_STATE0;
    do 
    {
        res = TLG_ScanTVChnASYNC(base_addr, center_freq_hz, state, &ms);
        if (res != TLG_ERR_SUCCESS) break;
        if (ms)
            TLG_Delay((uint16)ms);
    } while (state[0] != TLG_STATE_FINAL);

#ifdef FIX_AGC_PROBLEM
    TLG_FixAGC(base_addr);
#endif 

#ifdef TLG_NO_SSI_READOUT
    TLG_LockVScreen(base_addr);
#endif
    TLG_REG_LOG((b, "TLG_ScanTVChn freq %d end\n", center_freq_hz));

#endif 
    return res;
}



TLGDLL_API int TLG_InitScanFMChnASYNC(uint32 base_addr, uint32 min_hz, uint32 max_hz,
                int32 *slope, int32 *offset, tlg_statep state, int *ms)
{
    uint32 maxhz_cvt, minhz_cvt, last_cvt, first_cvt;
    uint16 last_code, first_code;

    TLG_NULLPTR_CHECK(slope);
    TLG_NULLPTR_CHECK(offset);
    TLG_BOUNDS_CHECK(min_hz, max_hz);

    if (min_hz == max_hz)
    {
        *slope = 0;
        *offset = 0;
        state[0] = TLG_STATE_FINAL;
        return TLG_ERR_SUCCESS;
    }
    switch (state[0])
    {
        case TLG_STATE0:
            state[4] = TLG_STATE0;    

        case TLG_STATE1:
            
            TLG_SetFMChnFreqASYNC(base_addr, max_hz, 0, 0, &state[4], ms);
            if (state[4] != TLG_STATE_FINAL)
            {
                    state[0] = TLG_STATE1;
                    break;
            }
            TLG_ReadRegNS(base_addr, 0x11c, &last_code);
            state[1] = last_code;     
            state[4] = TLG_STATE0;    

        case TLG_STATE2:
            TLG_SetFMChnFreqASYNC(base_addr, min_hz, 0, 0, &state[4], ms);
            if (state[4] != TLG_STATE_FINAL)
            {
                state[0] = TLG_STATE2;
                break;
            }
            else 
            {
                state[0] = TLG_STATE_FINAL;
            }
            TLG_ReadRegNS(base_addr, 0x11c, &first_code);
            last_code = state[1];

            maxhz_cvt = HZCVT(max_hz);
            minhz_cvt = HZCVT(min_hz);
            last_cvt  = MHZCVT(last_code);
            first_cvt = MHZCVT(first_code);
            
            *slope = 100*(int32)((last_cvt) -(first_cvt))/(int32)(maxhz_cvt - minhz_cvt);
            
            *offset = (100*(int32)(first_cvt) - (*slope) * (int32)(minhz_cvt));
            TLG_REG_LOG((b,"min=%d, max=%d, first=%d, last=%d, slope=%d, offset=%d\n",
                            minhz_cvt, maxhz_cvt, first_cvt, last_cvt, *slope, *offset));
            break;

        default: TLG_REG_LOG((b,"TLG_InitScanFMChnASYNC: %d BADSTATE!\n", state[0]));
            break;
    }

    return(TLG_ERR_SUCCESS);
}

typedef struct VHF3_input_cal_s
{
    uint32  max_freq;
    uint32  min_freq;
    uint32  cgs_code;
} VHF3_input_cal_t, *p_VHF3_input_cal_t;

#define VHF3_TABLE_SIZE     15

VHF3_input_cal_t vhf3_input_cal_tbl [VHF3_TABLE_SIZE] = {
        {   400, 380, 3   },
        {   380, 360, 4   },
        {   360, 340, 5   },
        {   340, 320, 6   },
        {   320, 300, 7   },
        {   300, 280, 8   },
        {   280, 260, 9   },
        {   260, 240, 12  },
        {   240, 220, 14  },
        {   220, 200, 18  },
        {   200, 190, 20  },
        {   190, 180, 22  },
        {   180, 170, 25  },
        {   170, 160, 28  },
        {   160, 150, 31  }
};

#if defined(TLG_CMP_CHIP_1121_A_0)
VHF3_input_cal_t vhf3_input_cal_tbl_3_1 [VHF3_TABLE_SIZE] = {
        {   400, 380, 3   },
        {   380, 360, 4   },
        {   360, 340, 5   },
        {   340, 320, 6   },
        {   320, 300, 7   },
        {   300, 280, 7   },
        {   280, 260, 7   },
        {   260, 240, 7   },
        {   240, 220, 9   },
        {   220, 200, 13  },
        {   200, 190, 16  },
        {   190, 180, 19  },
        {   180, 170, 21  },
        {   170, 160, 24  },
        {   160, 150, 31  }
};
#endif 

static void TLG_PowerDownMixer(uint32 base_addr)
{
    TLG_CHIP_VERS_1_2_BEGIN
#if defined(TLG_CMP_CHIP_1120_1_2) || defined(TLG_CMP_CHIP_1120_2_1) ||(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)
    
    TLG_SetBits(base_addr, REG_0180, 1,
                    REG_0180_BIT12_SHIFT,
                    REG_0180_BIT12_MASK);

#endif 

    TLG_CHIP_VERS_1_1_ELSE
#ifdef TLG_CMP_CHIP_1120_1_1
    
    TLG_SetBits(base_addr, REG_0180, 0,
                    REG_0180_BIT12_SHIFT,
                    REG_0180_BIT12_MASK);
#endif 
    TLG_CHIP_VERS_END
}
static void TLG_PowerUpMixer(uint32 base_addr)
{
    TLG_SetBits(base_addr, REG_0180, 0,
                    REG_0180_BIT12_SHIFT,
                    REG_0180_BIT12_MASK);
}


static void TLG_TFCalibrationASYNC_VHF3_Setup(
                uint32 base_addr    ,
                uint16 seed       ,
                uint16 avg1       ,
                uint16 avg0       ,
                uint16 is_harmonic_canceller)
{
    uint16 regData;
    uint16 dcycle = 24;
    uint16 jitter = 1; 

    if (is_harmonic_canceller == 0)
    {
        
#ifndef OPTIMIZE_VHF3
        
        TLG_ModifyReg(base_addr, REG_0185, 0x0104);

        
        TLG_SetBits(base_addr, REG_0183, 0,
                    REG_0183_BIT8_SHIFT,
                    REG_0183_BIT8_MASK);

        
        TLG_SetBits(base_addr, REG_0187, 0,
                    REG_0187_BIT15_SHIFT,
                    REG_0187_BIT15_MASK);
#endif 
        
        
        TLG_WriteReg(base_addr,
                    REG_0039,
                    0x003E
                    );
        
        TLG_SetBits(base_addr, REG_0036, is_harmonic_canceller,
                    REG_0036_BIT0_SHIFT,
                    REG_0036_BIT0_MASK);
        

        
        TLG_ModifyReg(base_addr, REG_0184, 0x8000);

#ifndef OPTIMIZE_VHF3
        
        TLG_ModifyReg(base_addr, REG_003A, 0x0);
        TLG_ModifyReg(base_addr, REG_003B, 0x0);
#endif 
        
        TLG_ModifyReg(base_addr, REG_018D, 0x80A1);

        
        TLG_ModifyReg(base_addr, REG_0033, 0x1d00);

        TLG_CHIP_VERS_1_2_BEGIN
#if defined(TLG_CMP_CHIP_1120_1_2) || defined(TLG_CMP_CHIP_1120_2_1) ||defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)
#ifndef OPTIMIZE_VHF3
                TLG_ReadReg (base_addr, REG_0037, &regData );
        TLGHAL_SET(regData, avg1, 7, BIT7);
        TLGHAL_SET(regData, avg0, 6, BIT6);
        TLG_WriteReg(base_addr, REG_0037, regData);
#endif 
#endif 

        TLG_CHIP_VERS_1_1_ELSE
#ifdef TLG_CMP_CHIP_1120_1_1
                
        TLG_ReadReg (base_addr, REG_0036, &regData );
        TLGHAL_SET(regData, dcycle,
                    REG_0036_BIT15_9_SHIFT              ,
                    REG_0036_BIT15_9_MASK);
        TLGHAL_SET(regData, jitter,
                    REG_0036_BIT8_SHIFT      ,
                    REG_0036_BIT8_MASK);
        TLGHAL_SET(regData, 1,
                    REG_0036_BIT7_6_SHIFT      ,
                    REG_0036_BIT7_6_MASK);
        TLG_WriteReg(base_addr, REG_0036, regData);
        

        TLG_ReadReg (base_addr, REG_0037, &regData );
        TLGHAL_SET(regData, avg1, 7, BIT7);
        TLGHAL_SET(regData, avg0, 6, BIT6);
        TLGHAL_SET(regData, 7,
                    REG_0037_BIT15_13_SHIFT    ,
                    REG_0037_BIT15_13_MASK);
        TLGHAL_SET(regData, 7,
                    REG_0037_BIT12_10_SHIFT    ,
                    REG_0037_BIT12_10_MASK);
        TLG_WriteReg(base_addr, REG_0037, regData);

        
        TLG_WriteReg(base_addr, REG_0194, 0x0000);
#endif 
        TLG_CHIP_VERS_END
    }
    else
    {

        
        TLG_WriteReg(base_addr,
                    REG_0039,
                    0x007E
                    );
        
        TLG_SetBits(base_addr, REG_0036, is_harmonic_canceller,
                    REG_0036_BIT0_SHIFT,
                    REG_0036_BIT0_MASK);
    }

}


void TLG_TFCalibrationASYNC_VHF3( uint32        base_addr,
                uint32        freq_hz,
                uint16        lna_mode,
                uint16        seed,
                uint16        avg1,
                uint16        avg0,
                tlg_statep    state,
                int           *ms)
{
    uint16 cgs_code     = 0;
    uint32 done         = 0;
    uint32 i            ;
    uint16 regData;

    uint32  freq_cvt = HZCVT(freq_hz);
    uint32  min_freq;
    uint32  max_freq;

    uint16  atop_trkfil_cal_2;
    p_VHF3_input_cal_t vhf3_table = vhf3_input_cal_tbl;

    TLG_REG_LOG((b,"TLG_TFCalibrationASYNC_VHF3: STATE %d \n", state[0]));

    
    switch (state[0])
    {
        case TLG_STATE0:
            TLG_AvgFiltInit(&tlg_agc1_filt);
            TLG_AvgFiltInit(&tlg_agc2_filt);
            TLG_AvgFiltInit(&tlg_lock_filt);
TLG1121_VERS_A_0_BEGIN            
#if defined(TLG_CMP_CHIP_1121_A_0)
            
            vhf3_table = vhf3_input_cal_tbl_3_1;
#endif 
TLG_CHIP_VERS_END

            for (i = 0; i < VHF3_TABLE_SIZE; i++)
            {
                min_freq = MHZCVT(vhf3_table[i].min_freq);
                max_freq = MHZCVT(vhf3_table[i].max_freq);
                if ((freq_cvt >= min_freq) && (freq_cvt < max_freq))
                {
                    cgs_code = (uint16) vhf3_table[i].cgs_code;
                    break;
                }
            }
            
            TLG_TFCalibrationSaveReg(base_addr, &state[1]);

            
            
            TLG_SetBits(base_addr, REG_0191, cgs_code,
                        REG_0191_BIT9_5_SHIFT,
                        REG_0191_BIT9_5_MASK);

            TLG_CHIP_VERS_2_1_BEGIN
#ifdef TLG_CMP_CHIP_1120_2_1
#endif 
                    TLG_CHIP_VERS_1_2_ELSE
#ifdef TLG_CMP_CHIP_1120_1_2
#endif 
                    TLG_CHIP_VERS_1_1_ELSE

#ifdef TLG_CMP_CHIP_1120_1_1
                    
            TLG_ReadReg(base_addr, REG_0037  , &atop_trkfil_cal_2     );
            regData = atop_trkfil_cal_2;
            TLGHAL_SET(regData,
                        0x00,
                        REG_0037_BIT7_6_SHIFT,
                        REG_0037_BIT7_6_MASK);
            TLG_ModifyReg(base_addr, REG_0037  , regData     );
#endif 
            TLG_CHIP_VERS_END

                    
                    TLG_PowerDownMixer(base_addr);
            TLG_TFCalibrationASYNC_VHF3_Setup(  base_addr, 0, 1, 1, 0);
            TLG_WriteReg(base_addr, REG_0038, seed);
            
            
            state[1] = 0;
            state[0] = TLG_STATE1;
            *ms = 5;
            break;
        case TLG_STATE1:
                
            TLG_ReadRegNS(base_addr, REG_0117, &regData);
            TLGHAL_GET(done, regData,
                        REG_0117_BIT12_SHIFT,
                        REG_0117_BIT12_MASK);
            if (done == 1)
            {
                    state[0] = TLG_STATE2;
            }
            else
            {
                
                state[1] ++ ;
                if (state[1]  >= 40) 
               {
                    state[0] = TLG_STATE2;
                    break;
               }
                *ms      = 5;
                break;
            }
           
        case TLG_STATE2:
            TLG_PowerUpMixer( base_addr);
            

            
            TLG_SetSynthesizer(
                        base_addr,
                        freq_hz * 3,
                        JUPITER_NR_REF_DIVIDE_VHF3);

            state[0] = TLG_STATE3;
            *ms = 5;
            break;
        case TLG_STATE3:
            
            TLG_ModifyReg(base_addr, REG_002E   , // 0x3A0D
                        REG_002E_VALUE_1         );
            TLG_ModifyReg(base_addr, REG_002F   , // 0x560D
                        REG_002F_VALUE_1         );
            

            
            

            

            TLG_PowerDownMixer(base_addr);
            TLG_TFCalibrationASYNC_VHF3_Setup(base_addr, seed, avg1, avg0,1);
            TLG_WriteReg(base_addr, REG_0038, seed);
             
            state[1] = 0;
            state[0] = TLG_STATE4;
            *ms = 5;
            break;
        case TLG_STATE4:
            
            TLG_ReadRegNS(base_addr, REG_0117, &regData);
            TLGHAL_GET(done, regData,
                        REG_0117_BIT12_SHIFT,
                        REG_0117_BIT12_MASK);
            if (done == 1)
            {
                    state[0] = TLG_STATE5;
            }            
            else
            {
            
                state[1] ++ ;
                if (state[1]  >= 40) 
               {
                    state[0] = TLG_STATE5;
                    break;
               }
                *ms      = 5;
                break;
            }
           
        case TLG_STATE5:
            TLG_PowerUpMixer( base_addr);
            TLG_TFCalibrationRestoreReg(base_addr, &state[1]);
            TLG_SetBits(base_addr, REG_0184, 0,
                        REG_0184_BIT15_SHIFT,
                        REG_0184_BIT15_MASK);
            TLG_ModifyReg(base_addr, REG_002E   , // 0x3A0D
                        REG_002E_VALUE_1         );
            TLG_ModifyReg(base_addr, REG_002F   , // 0x560D
                        REG_002F_VALUE_1         );
            
            TLG_SetBits(base_addr, REG_0180, 0,
                        REG_0180_BIT12_SHIFT,
                        REG_0180_BIT12_MASK);

            
            
TLG1121_VERS_A_0_BEGIN
#if defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1121_A_2)
            TLG_AltBS_Start(base_addr);
#endif      
TLG_CHIP_VERS_END

            TLG_SetSynthesizer(
                base_addr,
                freq_hz,
                JUPITER_NR_REF_DIVIDE_VHF3);

TLG1121_VERS_A_0_BEGIN
#if defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1121_A_2)

            state[0] = TLG_STATE6;
            *ms      = 5;
#endif      
TLG_CHIP_VERS_ELSE
#if defined(TLG_CMP_CHIP_1120_1_2) ||defined(TLG_CMP_CHIP_1120_1_1) ||defined(TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1120_2_3)

            state[0] = TLG_STATE_FINAL;
            *ms      = 0;
#endif 
TLG_CHIP_VERS_END
            break;

#if defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1121_A_2)
        case TLG_STATE6:
            
            TLG_AltBS_Finish(base_addr);
            state[0] = TLG_STATE_FINAL;
            *ms      = 5;
            break;
#endif 

        default:
            TLG_REG_LOG((b,"TLG_TFCalibrationASYNC_VHF3: %d BADSTATE!\n", state[0]));
            break;
    }

    
}
#define xxUSE_ICP_TABLE
#ifdef USE_ICP_TABLE
typedef struct _tlg_icp_tbl_s
{
    uint32 center_freq_100_khz;
    uint16 lodiv;
    uint16 reg1a6;
} tlg_icp_tbl;
#if 1
tlg_icp_tbl icp_tbl[] =
{
    {  10 , 32 , 0x4D6C}, 
    {  13 , 32 , 0x4C6C}, 
    {  15 , 32 , 0x4D6C}, 
    {  16 , 32 , 0x446C}, 
    {  17 , 32 , 0x456C}, 
    {  19 , 32 , 0x446C}, 
    {  20 , 32 , 0x456C}, 
    {  21 , 32 , 0x446C}, 
    {  22 , 32 , 0x456C}, 
    {  25 , 32 , 0x446C}, 
    {  29 , 32 , 0x456C}, 
    {  32 , 32 , 0x3C6C}, 
    {  34 , 32 , 0x3D6C}, 
    {  38 , 32 , 0x3C6C}, 
    {  40 , 32 , 0x3D6C}, 
    {  41 , 32 , 0x3C6C}, 
    {  44 , 32 , 0x3D6C}, 
    {  50 , 32 , 0x3C6C}, 
    {  53 , 32 , 0x3D6C}, 
    {  54 , 32 , 0x3C6C}, 
    {  57 , 32 , 0x3D6C}, 
    {  63 , 32 , 0x356C}, 
    {  64 , 32 , 0x346C}, 
    {  68 , 32 , 0x356C}, 
    {  76 , 32 , 0x346C}, 
    {  79 , 32 , 0x356C}, 
    {  82 , 32 , 0x346C}, 
    {  88 , 32 , 0x356C}, 
    {  99 , 32 , 0x346C}, 
    { 105 , 32 , 0x356C}, 
    { 107 , 32 , 0x346C}, 
    { 114 , 32 , 0x356C}, 
    { 125 , 32 , 0x2D6C}, 
    { 127 , 32 , 0x2C6C}, 
    { 136 , 32 , 0x2D6C}, 
    { 152 , 32 , 0x2C6C}, 
    { 158 , 32 , 0x2D6C}, 
    { 164 , 32 , 0x2C6C}, 
    { 176 , 32 , 0x2D6C}, 
    { 197 , 32 , 0x2C6C}, 
    { 210 , 32 , 0x2D6C}, 
    { 213 , 32 , 0x2C6C}, 
    { 227 , 32 , 0x2D6C}, 
    { 250 , 32 , 0x256C}, 
    { 253 , 32 , 0x246C}, 
    { 271 , 32 , 0x256C}, 
    { 303 , 32 , 0x246C}, 
    { 316 , 32 , 0x256C}, 
    { 327 , 32 , 0x246C}, 
    { 351 , 32 , 0x256C}, 
    { 394 , 32 , 0x246C}, 
    { 420 , 32 , 0x256C}, 
    { 425 , 32 , 0x246C}, 
    { 454 , 32 , 0x256C}, 
    { 500 , 32 , 0x1D6C}, 
    { 505 , 32 , 0x1C6C}, 
    { 542 , 32 , 0x1D6C}, 
    { 606 , 32 , 0x1C6C}, 
    { 632 , 32 , 0x1D6C}, 
    { 653 , 32 , 0x1C6C}, 
    { 702 , 32 , 0x1D6C}, 
    { 787 , 32 , 0x1C6C}, 
    { 840 , 32 , 0x1D6C}, 
    { 849 , 32 , 0x1C6C}, 
    { 907 , 32 , 0x1D6C}, 
    {1000 , 16 , 0x156C}, 
    {1009 , 16 , 0x146C}, 
    {1083 , 16 , 0x156C}, 
    {1212 , 16 , 0x146C}, 
    {1263 , 16 , 0x156C}, 
    {1306 , 16 , 0x146C}, 
    {1403 , 16 , 0x156C}, 
    {1573 , 16 , 0x146C}, 
    {1679 , 16 , 0x156C}, 
    {1698 , 16 , 0x146C}, 
    {1814 , 16 , 0x156C}, 
    {2000 ,  8 , 0x0D6C}, 
    {2017 ,  8 , 0x0C6C}, 
    {2165 ,  8 , 0x0D6C}, 
    {2423 ,  8 , 0x0C6C}, 
    {2526 ,  8 , 0x0D6C}, 
    {2611 ,  8 , 0x0C6C}, 
    {2806 ,  8 , 0x0D6C}, 
    {3145 ,  8 , 0x0C6C}, 
    {3358 ,  8 , 0x0D6C}, 
    {3395 ,  8 , 0x0C6C}, 
    {3627 ,  8 , 0x0D6C}, 
    {4000 ,  4 , 0x056C}, 
    {4033 ,  4 , 0x046C}, 
    {4330 ,  4 , 0x056C}, 
    {4846 ,  4 , 0x046C}, 
    {5051 ,  4 , 0x056C}, 
    {5221 ,  4 , 0x046C}, 
    {5611 ,  4 , 0x056C}, 
    {6290 ,  4 , 0x046C}, 
    {6715 ,  4 , 0x056C}, 
    {6790 ,  4 , 0x046C}, 
    {7254 ,  4 , 0x056C}, 
    { 0   , 0, 0}
};

#else 
tlg_icp_tbl icp_tbl[] =
{
    { 57  , 32 , 0x1D6C}, 
    { 61  , 32 , 0x1C6C}, 
    { 64  , 32 , 0x1D6C}, 
    { 66  , 32 , 0x1C6C}, 
    { 71  , 32 , 0x1D6C}, 
    { 79  , 32 , 0x1C6C}, 
    { 84  , 32 , 0x1D6C}, 
    { 85  , 32 , 0x1C6C}, 
    { 91  , 32 , 0x1D6C}, 
    { 100 , 16 , 0x156C}, 
    { 101 , 16 , 0x146C}, 
    { 109 , 16 , 0x156C}, 
    { 122 , 16 , 0x146C}, 
    { 127 , 16 , 0x156C}, 
    { 131 , 16 , 0x146C}, 
    { 141 , 16 , 0x156C}, 
    { 158 , 16 , 0x146C}, 
    { 168 , 16 , 0x156C}, 
    { 170 , 16 , 0x146C}, 
    { 182 , 16 , 0x156C}, 
    { 200 , 8  , 0x0D6C}, 
    { 202 , 8  , 0x0C6C}, 
    { 217 , 8  , 0x0D6C}, 
    { 243 , 8  , 0x0C6C}, 
    { 253 , 8  , 0x0D6C}, 
    { 262 , 8  , 0x0C6C}, 
    { 281 , 8  , 0x0D6C}, 
    { 315 , 8  , 0x0C6C}, 
    { 336 , 8  , 0x0D6C}, 
    { 340 , 8  , 0x0C6C}, 
    { 363 , 8  , 0x0D6C}, 
    { 400 , 4  , 0x056C}, 
    { 404 , 4  , 0x046C}, 
    { 433 , 4  , 0x056C}, 
    { 485 , 4  , 0x046C}, 
    { 506 , 4  , 0x056C}, 
    { 523 , 4  , 0x046C}, 
    { 562 , 4  , 0x056C}, 
    { 629 , 4  , 0x046C}, 
    { 672 , 4  , 0x056C}, 
    { 679 , 4  , 0x046C}, 
    { 726 , 4  , 0x056C}, 
    { 0   , 0, 0}
};
#endif 
#endif 


TLGDLL_API void TLG_SetSynthesizer(
                uint32 base_addr,
                uint32 center_freq_in_hz,
                uint32 nr_ref_divide)
{
    uint32          center_freq         = HZCVT(center_freq_in_hz);

    
    uint16          lodiv;
    int32           icp;
    uint16          reg1a5, reg1a6;
    uint16          log2lodiv;

    

TLG_CHIP_VERS_2_3_BEGIN
#if defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)
    lodiv = (uint16)min(64, max(4, pow2(ceil_log2((BASE_VALUE_CVT_3_1+(center_freq-1))/center_freq, 1))));
#endif 
TLG_CHIP_VERS_ELSE
#if defined(TLG_CMP_CHIP_1120_1_2) || defined(TLG_CMP_CHIP_1120_1_1) || defined(TLG_CMP_CHIP_1120_2_1) 
    lodiv = (uint16)max(4, pow2(ceil_log2((BASE_VALUE_CVT+(center_freq-1))/center_freq, 1)));
#endif 
TLG_CHIP_VERS_END

    log2lodiv = (uint16) ceil_log2(lodiv, 0);
    reg1a5    = (uint16)((lodiv * center_freq * nr_ref_divide + MHZCVT(27)/2)/MHZCVT(27));
    
    
    if (TLG_SYS_CLK_SOURCE_XTAL == g_sys_clk_source)
    {
    icp       = getICP(base_addr, reg1a5, nr_ref_divide);
        icp -= 2;
    }
    else
    {
        icp  = 0;
    }

    TLG_REG_LOG((b,"hz=%d, lodiv=%d, icp=%d\n", center_freq_in_hz, lodiv, icp));
    reg1a6 = (uint16)  ( ( (uint16)(log2lodiv - 2 ) << 11) |
                ( (uint16)(icp) << 8  ) | nr_ref_divide);
#ifdef USE_ICP_TABLE
    {
        uint32 center_freq_100_khz = center_freq_in_hz / 100000;
        tlg_icp_tbl *p_icp_tbl = icp_tbl;
        reg1a6 = p_icp_tbl->reg1a6;
        while (p_icp_tbl->center_freq_100_khz != 0)
        {
                if (center_freq_100_khz < p_icp_tbl->center_freq_100_khz)
                {
                    break;
                }
                reg1a6 = p_icp_tbl->reg1a6;
                p_icp_tbl++;
        }
    }
#else 
#endif 
    TLG_REG_LOG((b,"hz=%d, lodiv=%d, icp=%d, 0x01a5 = 0x%04X, 0x01a6 = 0x%04X\n",
                            center_freq_in_hz, lodiv, icp, reg1a5, reg1a6));
    TLG_ModifyReg(base_addr, REG_01A6, reg1a6);
    TLG_ModifyReg(base_addr, REG_01A5, reg1a5);
}


TLGDLL_API int TLG_InitScanFMChn(uint32 base_addr, uint32 min_hz, uint32 max_hz,
                int32 *slope, int32 *offset)
{
    int res = TLG_ERR_NOT_SUPPORTED;

#ifdef TLG_EMBEDDED_TIMERS
    tlg_statep state;
    int        ms=0;

    state[0] = TLG_STATE0;
    do 
    {
        res = TLG_InitScanFMChnASYNC(base_addr, min_hz, max_hz, slope, offset, state, &ms);
        if (res != TLG_ERR_SUCCESS) break;
        if (ms != 0)
            TLG_Delay((uint16)ms);
    } while (state[0] != TLG_STATE_FINAL);
#endif 

    return res;
}


TLGDLL_API int TLG_ScanFMChnASYNC(uint32 base_addr, uint32 center_freq_hz,
                int32 slope, int32 offset, tlg_statep state, int *ms)
{
    
    TLG_SetFMChnFreqASYNC(base_addr, center_freq_hz, slope, offset, state, ms);
    if (state[0] != TLG_STATE_FINAL)
            return CHANNEL_NOT_EXIST;

    TLG_ForceLnaAgc_Gain(base_addr);
    TLG_ReleaseLnaAgc_Gain(base_addr);
    tlg_cur_fm_ssi = TLG_GetAudioCarrierPwr(base_addr, MAX_AUDIO_READ_NOTCH_NUM);
    if (tlg_cur_fm_ssi > TLG_SSI_THRESHOLD)
    {
        log_printf(("Hz=%d\t %d, %d\n", center_freq_hz, tlg_cur_fm_ssi, TLG_SSI_THRESHOLD));
        TLG_REG_LOG((b,"\nFM STRENGTH for %d = %d (pass thresh=%d)\n\n", center_freq_hz, tlg_cur_fm_ssi,TLG_SSI_THRESHOLD));
        return CHANNEL_EXIST;
    }
    log_printf(("%d\t %d\t", center_freq_hz, tlg_cur_fm_ssi));
    tlg_cur_fm_ssi = 0;
    log_printf(("%d\t %d channel not exist\n", center_freq_hz, tlg_cur_fm_ssi));
    TLG_REG_LOG((b,"\nFM STRENGTH for %d = %d (fail thresh=%d)\n", center_freq_hz, tlg_cur_fm_ssi,TLG_SSI_THRESHOLD));
    return CHANNEL_NOT_EXIST;
}



TLGDLL_API int TLG_ScanFMChn(uint32 base_addr, uint32 center_freq_hz,
                int32 slope, int32 offset)
{
    int res = TLG_ERR_NOT_SUPPORTED;

#ifdef TLG_EMBEDDED_TIMERS
    tlg_statep state;
    int        ms=0;
    state[0] = TLG_STATE0;


#ifdef TLG_SUPPORT_FM_AVC 
    TLG_AutoVolumeControlReset(base_addr);
#endif 

    do 
    {
        res = TLG_ScanFMChnASYNC(base_addr, center_freq_hz, slope, offset, state, &ms);
        if (res != TLG_ERR_SUCCESS) break;
        if (ms != 0)
            TLG_Delay((uint16)ms);
    } while (state[0] != TLG_STATE_FINAL);
#endif 

    return res;
}



TLGDLL_API int TLG_GetChnStatus(uint32 base_addr, bool *isLock)
{
    uint16 regData;

    TLG_NULLPTR_CHECK(isLock);

    
    TLG_ReadRegNS(base_addr, REG_013B,&regData);
    
    *isLock = (regData >> REG_013B_BIT5_SHIFT ) & 1;

    return(TLG_ERR_SUCCESS);
}


#ifdef TLG_RESTART_THRESH_DEV
TLGDLL_API int TLG_RESTART_THRESH = TLG_RESTART_THRESH_DEF_VAL;
#endif 

TLGDLL_API int TLG_GetChnRestartStatus(uint32 base_addr, bool *lockStat)
{
    uint16  reg;
    uint16  lumaStat;
    uint16  c3;
    int     thresh;

    
    TLG_NULLPTR_CHECK(lockStat);
    
    TLG_ReadRegNS(base_addr, REG_013B, &reg);
    *lockStat = !(reg & REG_013B_BIT1_MASK);
    if (*lockStat)
    {
        
        TLG_ReadRegNS(base_addr, REG_0129, (uint16*) &lumaStat);
        if (lumaStat & 0x8000) lumaStat = ~(lumaStat-1) & 0xffff;
        
        TLG_ReadReg(base_addr, REG_0053, &c3);
        TLGHAL_GET(c3, c3, REG_0053_BIT14_12_SHIFT,
                    REG_0053_BIT14_12_MASK);
        thresh    = (TLG_RESTART_THRESH*(1<<(25-c3)))/13500;
        *lockStat = (lumaStat > thresh);
    }

    return(TLG_ERR_SUCCESS);

}


TLGDLL_API int TLG_GetLockStatus(uint32 base_addr, uint16 lockTypes, uint16 *locks)
{
    uint16  bsCode;
    TLG_NULLPTR_CHECK(locks);
    *locks = 0;

    
    
    
    TLG_CHIP_VERS_2_1_BEGIN
 #if defined (TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)
            if (lockTypes & (TLG_LOCK_STATE_VSYNC|TLG_LOCK_STATE_VIDEO|
                            TLG_LOCK_STATE_CHROMA | TLG_LOCK_STATE_LUMA |
                            TLG_LOCK_STATE_FMPLL | TLG_LOCK_STATE_FMSNR |
                            TLG_LOCK_STATE_AUDIO | TLG_LOCK_STATE_STEREO))
            {
                TLG_ReadRegNS(base_addr,    REG_013B,         locks);
TLG_REG_LOG((b,"\nTLG_GetLockStatus: locks=%d)\n\n", locks));
        
                if (lockTypes & TLG_LOCK_STATE_STEREO)
                {
                    
                    uint16 fm_aud_type;
                    uint16 stereo_checks = TLG_LOCK_STATE_FMSNR|TLG_LOCK_STATE_AUDIO|TLG_LOCK_STATE_FMPLL;
                    TLG_Get_FM_AudioStereoMode(base_addr, &fm_aud_type);
                    if ((fm_aud_type==TLG_FM_STEREO  && (*locks & stereo_checks))
                                || ((fm_aud_type==TLG_FM_AUTO_DETECT) 
                                && (*locks & stereo_checks)==stereo_checks))
                        *locks |= TLG_LOCK_STATE_STEREO;
                    else
                        *locks &= ~TLG_LOCK_STATE_STEREO;
                }
            }
#endif 
    TLG_CHIP_VERS_1_1_ELSE
#if defined(TLG_CMP_CHIP_1120_1_1) || defined(TLG_CMP_CHIP_1120_1_2)

            if (lockTypes & (TLG_LOCK_STATE_VSYNC|TLG_LOCK_STATE_VIDEO|
                                    TLG_LOCK_STATE_CHROMA | TLG_LOCK_STATE_LUMA))
            {
                TLG_ReadRegNS(base_addr,    REG_013B,         locks);
            }
    
    if (lockTypes & TLG_LOCK_STATE_AUDIO)
    {
        TLGHAL_SET(*locks, (TLG_GetAudioCarrierPwr(base_addr, MAX_AUDIO_READ_NOTCH_NUM) > (int)TLG_SSI_THRESHOLD),
                    REG_013B_BIT3_SHIFT,
                    REG_013B_BIT3_MASK);
        log_printf(("\n"));
    }
    
    *locks &= ~(TLG_LOCK_STATE_FMSNR|TLG_LOCK_STATE_FMPLL|TLG_LOCK_STATE_STEREO);
#endif 
    TLG_CHIP_VERS_END

            
        if (lockTypes & TLG_LOCK_STATE_SYNTH)
        {
            TLG_ReadRegNS(base_addr,    REG_01B0,           &bsCode);
            TLGHAL_GET(bsCode, bsCode,  REG_01B0_BIT12_SHIFT,
                        REG_01B0_BIT12_MASK);
            TLGHAL_SET(*locks, bsCode,  REG_01B0_BIT12_SHIFT,
                        REG_01B0_BIT12_MASK);
        }

    return TLG_ERR_SUCCESS;
}



int gVbiSize = 0; 


TLGDLL_API void TLG_VbiSetup(
                uint32 base_addr           ,
                uint32 field_1_start_line  ,
                uint32 filed_1_stop_line   ,
                uint32 field_2_start_line  ,
                uint32 filed_2_stop_line   ,
                uint32 start_pixel         ,
                uint32 end_pixel           ,
                uint32 interpolater_disable)
{
    
    TLG_WriteReg(base_addr, 0xdb, (uint16) field_1_start_line  );
    TLG_WriteReg(base_addr, 0xdc, (uint16) filed_1_stop_line   );
    TLG_WriteReg(base_addr, 0xdd, (uint16) field_2_start_line  );
    TLG_WriteReg(base_addr, 0xde, (uint16) filed_2_stop_line   );
    
    TLG_WriteReg(base_addr, 0xdf,
                    ( ((uint16) start_pixel - 1) << 8)   
                    | (870 - ((uint16) end_pixel + 1))); 

    
    gVbiSize = (filed_1_stop_line - field_1_start_line + 1)
            * (end_pixel - start_pixel + 1) * 2;  
}


TLGDLL_API int TLG_VbiGetSize(
                uint32 base_addr)
{
    return gVbiSize;
}



TLGDLL_API void TLG_AlternativeMode(
                uint32                  base_addr               ,
                uint32                  atop_bypass_mode_cseldel,
                uint32                  hera_mode_iqdel         ,
                TLG_ALTERNATIVE_MODE    alternative_mode)
{
    
    TLG_WriteReg(
                base_addr,
                0x98,
                ((uint16) atop_bypass_mode_cseldel << 13)
                | ((uint16) hera_mode_iqdel << 12)
                | ((uint16) alternative_mode )
                );

}

TLGDLL_API void TLG_VideoADC_Calibration(
                uint32                  base_addr               ,
                uint32                  phase)
{
    uint16 reg16;
    uint16 data1;
    uint16 data2;
    
    switch (phase)
    {
        case 0:
            
            TLG_SetBits(base_addr, REG_0180, 0,
                        REG_0180_BIT6_SHIFT,
                        REG_0180_BIT6_MASK);
            
            TLG_ReadReg(base_addr, REG_0198, &reg16);
            TLGHAL_SET(reg16, TLG_ON,
                        REG_0198_BIT8_SHIFT,
                        REG_0198_BIT8_MASK);
            
            TLGHAL_SET(reg16, 0x6,
                        REG_0198_BIT6_4_SHIFT,
                        REG_0198_BIT6_4_MASK);

            TLGHAL_SET(reg16, 0xd,
                        REG_0198_BIT3_0_SHIFT,
                        REG_0198_BIT3_0_MASK);
            TLG_ModifyReg(base_addr, REG_0198, reg16);


            
            TLG_SetBits(base_addr, REG_0197, REG_0197_VALUE_4,
                        REG_0197_BIT9_8_SHIFT,
                        REG_0197_BIT9_8_MASK);
            break;
        case 1:
            
            
            TLG_ReadReg (base_addr, REG_0004, &data1);
            TLGHAL_SET(data1, 0,
                        REG_0004_BIT13_SHIFT,
                        REG_0004_BIT13_MASK);
            
            
            TLGHAL_SET(data1, 0,
                        REG_0004_BIT14_SHIFT,
                        REG_0004_BIT14_MASK);
            TLG_WriteReg(base_addr, REG_0004, data1);

            TLG_ReadReg(base_addr, REG_0003, &data2);
            
            
            TLGHAL_SET(data2, 0,
                        REG_0003_BIT14_SHIFT,
                        REG_0003_BIT14_MASK);

            
            TLGHAL_SET(data2, 0xF,
                        REG_0003_BIT13_10_SHIFT,
                        REG_0003_BIT13_10_MASK);
            
            TLGHAL_SET(data2, 0x6,
                        REG_0003_BIT3_0_SHIFT,
                        REG_0003_BIT3_0_MASK);

            TLG_WriteReg(base_addr, REG_0003, data2);

            
            // TLG_ReadReg (base_addr, REG_0004, &data1);
            
            TLGHAL_SET(data1, 0,
                        REG_0004_BIT15_SHIFT,
                        REG_0004_BIT15_MASK);
            TLG_WriteReg(base_addr, REG_0004, data1);

            
            TLG_WriteReg(base_addr, REG_0004, data1 | REG_0004_BIT15_MASK);

            
            TLG_WriteReg(base_addr, REG_0003, data2 | REG_0003_BIT14_MASK);

            
            TLG_WriteReg(base_addr, REG_0004, data1);
            
            TLG_WriteReg(base_addr, REG_0004, data1 | REG_0004_BIT15_MASK);
            break;
        case 2:
            
            TLG_SetBits(base_addr, REG_0197, REG_0197_VALUE_1,
                        REG_0197_BIT9_8_SHIFT,
                        REG_0197_BIT9_8_MASK);

            
            TLG_SetBits(base_addr, REG_0198, 0,
                        REG_0198_BIT8_SHIFT,
                        REG_0198_BIT8_MASK);

            
            TLG_SetBits(base_addr, REG_0180, 1,
                        REG_0180_BIT6_SHIFT,
                        REG_0180_BIT6_MASK);
            
            
            TLG_ReadReg (base_addr, REG_0004, &data1);
            TLGHAL_SET(data1, 1,
                        REG_0004_BIT14_SHIFT,
                        REG_0004_BIT14_MASK);
            TLG_WriteReg(base_addr, REG_0004, data1);

            
            
            TLGHAL_SET(data1, 0,
                        REG_0004_BIT15_SHIFT,
                        REG_0004_BIT15_MASK);
            TLG_WriteReg(base_addr, REG_0004, data1);

            
            TLG_WriteReg(base_addr, REG_0004, data1 | REG_0004_BIT15_MASK);
            break;
    }
}


TLGDLL_API void TLG_ReadStrobe(
                uint32                  base_addr)
{
    uint16 data;

    
    TLG_ReadReg(base_addr, REG_0004,&data);

    TLGHAL_SET(data,
                0x0002,
                REG_0004_BIT15_13_SHIFT,
                REG_0004_BIT15_13_MASK);
    TLG_WriteReg(base_addr, REG_0004,data);

    TLGHAL_SET(data,
                0x0006,
                REG_0004_BIT15_13_SHIFT,
                REG_0004_BIT15_13_MASK);
    TLG_WriteReg(base_addr, REG_0004,data);
}


TLGDLL_API void TLG_WriteStrobe(
                uint32                  base_addr)
{
    uint16 data;

    
    TLG_ReadReg(base_addr, REG_0004,&data);

    TLGHAL_SET(data,
                0x0000,
                REG_0004_BIT15_13_SHIFT,
                REG_0004_BIT15_13_MASK);
    TLG_WriteReg(base_addr, REG_0004,data);

    TLGHAL_SET(data,
                0x0004,
                REG_0004_BIT15_13_SHIFT,
                REG_0004_BIT15_13_MASK);
    TLG_WriteReg(base_addr, REG_0004,data);
}

TLGDLL_API void TLG_ReadRegADC(
                uint32                  base_addr,
                uint32                  reg_addr,
                uint16                  *p_reg_val)
{
    TLG_ReadStrobe(base_addr);
    TLG_ReadReg(base_addr, reg_addr, p_reg_val);
}


TLGDLL_API void TLG_WriteRegADC(
                uint32                  base_addr,
                uint32                  reg_addr,
                uint16                  reg_val)
{

    TLG_WriteReg(base_addr, reg_addr, reg_val);
    TLG_WriteStrobe(base_addr);
}



TLGDLL_API int TLG_GetCurrentFreq(uint32 base_addr, uint32 *p_current_freq_hz)
{
    *p_current_freq_hz = current_freq_hz;
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_CC_GetAttrib(
                uint32                  base_addr,
                tlg_cc_attrib_t        *p_cc_attrib,
                tlg_cc_field_t         *p_cc_field0,
                tlg_cc_field_t         *p_cc_field1)
{

    uint16 data;
    TLG_ReadRegNS(base_addr, REG_00D0, &data);
    TLGHAL_GET (p_cc_attrib->cc_hsync_sel, data,
                REG_00D0_BIT8_SHIFT,
                REG_00D0_BIT8_MASK);

    TLGHAL_GET (p_cc_attrib->cc_flip_vsync, data,
                REG_00D0_BIT5_SHIFT,
                REG_00D0_BIT5_MASK);
    TLGHAL_GET (p_cc_attrib->cc_flip_hsync, data,
                REG_00D0_BIT4_SHIFT,
                REG_00D0_BIT4_MASK);
    TLGHAL_GET (p_cc_attrib->cc_output_sel, data,
                REG_00D0_BIT3_2_SHIFT,
                REG_00D0_BIT3_2_MASK);

    TLG_ReadRegNS(base_addr, REG_00D5, &data);
    TLGHAL_GET (p_cc_attrib->cc_data_dly, data,
                REG_00D5_BIT15_8_SHIFT,
                REG_00D5_BIT15_8_MASK);
    TLGHAL_GET (p_cc_attrib->cc_center_dly, data,
                REG_00D5_BIT7_0_SHIFT,
                REG_00D5_BIT7_0_MASK);
    TLG_ReadRegNS(base_addr, REG_00D6, &data);
    TLGHAL_GET (p_cc_attrib->cc_slice_lpf, data,
                REG_00D6_BIT13_12_SHIFT,
                REG_00D6_BIT13_12_MASK);
    TLGHAL_GET (p_cc_attrib->cc_parity_win, data,
                REG_00D6_BIT11_10_SHIFT,
                REG_00D6_BIT11_10_MASK);
    TLGHAL_GET (p_cc_attrib->cc_pass_parity, data,
                REG_00D6_BIT8_SHIFT,
                REG_00D6_BIT8_MASK);
    TLGHAL_GET (p_cc_attrib->cc_correct_errs, data,
                REG_00D6_BIT7_SHIFT,
                REG_00D6_BIT7_MASK);
    TLGHAL_GET (p_cc_attrib->cc_drop_errs, data,
                REG_00D6_BIT6_SHIFT,
                REG_00D6_BIT6_MASK);
    TLGHAL_GET (p_cc_attrib->cc_drop_nulls, data,
                REG_00D6_BIT5_SHIFT,
                REG_00D6_BIT5_MASK);
    TLGHAL_GET (p_cc_attrib->cc_insert_nulls, data,
                REG_00D6_BIT4_SHIFT,
                REG_00D6_BIT4_MASK);
    TLGHAL_GET (p_cc_attrib->cc_slice_lpf, data,
                REG_00D6_BIT13_12_SHIFT,
                REG_00D6_BIT13_12_MASK);
    TLGHAL_GET (p_cc_attrib->cc_avg_sel, data,
                REG_00D6_BIT2_SHIFT,
                REG_00D6_BIT2_MASK);
    TLGHAL_GET (p_cc_attrib->cc_lpf_sel, data,
                REG_00D6_BIT1_0_SHIFT,
                REG_00D6_BIT1_0_MASK);

    TLG_ReadRegNS(base_addr, REG_00D7, &data);
    TLGHAL_GET (p_cc_attrib->cc_start_thresh, data,
                REG_00D7_BIT15_8_SHIFT,
                REG_00D7_BIT15_8_MASK);

    TLGHAL_GET (p_cc_attrib->cc_freq_adj, data,
                REG_00D7_BIT7_0_SHIFT,
                REG_00D7_BIT7_0_MASK);

    TLG_ReadRegNS(base_addr, REG_00D1, &data);
    TLGHAL_GET (p_cc_field0->cc_line_start , data,
                REG_00D1_BIT9_0_SHIFT,
                REG_00D1_BIT9_0_MASK);

    TLG_ReadRegNS(base_addr, REG_00D2, &data);
    TLGHAL_GET (p_cc_field0->cc_line_stop , data,
                REG_00D2_BIT9_0_SHIFT,
                REG_00D2_BIT9_0_MASK);

    TLG_ReadRegNS(base_addr, REG_00D3, &data);
    TLGHAL_GET (p_cc_field1->cc_line_start , data,
                REG_00D1_BIT9_0_SHIFT,
                REG_00D1_BIT9_0_MASK);

    TLG_ReadRegNS(base_addr, REG_00D4, &data);
    TLGHAL_GET (p_cc_field1->cc_line_stop , data,
                REG_00D2_BIT9_0_SHIFT,
                REG_00D2_BIT9_0_MASK);

    return(TLG_ERR_SUCCESS);
}

TLGDLL_API int TLG_CC_SetAttrib(
                uint32                  base_addr,
                tlg_cc_attrib_t        *p_cc_attrib,
                tlg_cc_field_t         *p_cc_field0,
                tlg_cc_field_t         *p_cc_field1)
{
    uint16 data;
    TLG_ReadRegNS(base_addr, REG_00D0, &data);
    TLGHAL_SET ( data, p_cc_attrib->cc_hsync_sel,
                REG_00D0_BIT8_SHIFT,
                REG_00D0_BIT8_MASK);
    TLGHAL_SET ( data, p_cc_attrib->cc_flip_vsync,
                REG_00D0_BIT5_SHIFT,
                REG_00D0_BIT5_MASK);
    TLGHAL_SET ( data, p_cc_attrib->cc_flip_hsync,
                REG_00D0_BIT4_SHIFT,
                REG_00D0_BIT4_MASK);
    TLGHAL_SET ( data, p_cc_attrib->cc_output_sel,
                REG_00D0_BIT3_2_SHIFT,
                REG_00D0_BIT3_2_MASK);
    TLG_WriteReg(base_addr, REG_00D0, data);

    TLG_ReadRegNS(base_addr, REG_00D5, &data);
    TLGHAL_SET ( data, p_cc_attrib->cc_data_dly,
                REG_00D5_BIT15_8_SHIFT,
                REG_00D5_BIT15_8_MASK);
    TLGHAL_SET ( data, p_cc_attrib->cc_center_dly,
                REG_00D5_BIT7_0_SHIFT,
                REG_00D5_BIT7_0_MASK);

    TLG_ReadRegNS(base_addr, REG_00D6, &data);
    TLGHAL_SET ( data, p_cc_attrib->cc_slice_lpf,
                REG_00D6_BIT13_12_SHIFT,
                REG_00D6_BIT13_12_MASK);
    TLGHAL_SET ( data, p_cc_attrib->cc_parity_win,
                REG_00D6_BIT11_10_SHIFT,
                REG_00D6_BIT11_10_MASK);
    TLGHAL_SET ( data, p_cc_attrib->cc_pass_parity,
                REG_00D6_BIT8_SHIFT,
                REG_00D6_BIT8_MASK);
    TLGHAL_SET ( data, p_cc_attrib->cc_correct_errs,
                REG_00D6_BIT7_SHIFT,
                REG_00D6_BIT7_MASK);
    TLGHAL_SET ( data, p_cc_attrib->cc_drop_errs,
                REG_00D6_BIT6_SHIFT,
                REG_00D6_BIT6_MASK);
    TLGHAL_SET ( data, p_cc_attrib->cc_drop_nulls,
                REG_00D6_BIT5_SHIFT,
                REG_00D6_BIT5_MASK);
    TLGHAL_SET ( data, p_cc_attrib->cc_insert_nulls,
                REG_00D6_BIT13_12_SHIFT,
                REG_00D6_BIT4_MASK);
    TLGHAL_SET ( data, p_cc_attrib->cc_slice_lpf,
                REG_00D6_BIT4_SHIFT,
                REG_00D6_BIT13_12_MASK);
    TLGHAL_SET ( data, p_cc_attrib->cc_avg_sel,
                REG_00D6_BIT2_SHIFT,
                REG_00D6_BIT2_MASK);
    TLGHAL_SET ( data, p_cc_attrib->cc_lpf_sel,
                REG_00D6_BIT1_0_SHIFT,
                REG_00D6_BIT1_0_MASK);
    TLG_WriteReg(base_addr, REG_00D6, data);

    TLG_ReadRegNS(base_addr, REG_00D7, &data);
    TLGHAL_SET ( data, p_cc_attrib->cc_start_thresh,
                REG_00D7_BIT15_8_SHIFT,
                REG_00D7_BIT15_8_MASK);
    TLGHAL_SET ( data, p_cc_attrib->cc_freq_adj,
                REG_00D7_BIT7_0_SHIFT,
                REG_00D7_BIT7_0_MASK);
    TLG_WriteReg(base_addr, REG_00D7, data);

    TLG_ReadRegNS(base_addr, REG_00D1, &data);
    TLGHAL_SET ( data, p_cc_field0->cc_line_start ,
                REG_00D1_BIT9_0_SHIFT,
                REG_00D1_BIT9_0_MASK);
    TLG_WriteReg(base_addr, REG_00D1, data);

    TLG_ReadRegNS(base_addr, REG_00D2, &data);
    TLGHAL_SET ( data, p_cc_field0->cc_line_stop ,
                REG_00D2_BIT9_0_SHIFT,
                REG_00D2_BIT9_0_MASK);
    TLG_WriteReg(base_addr, REG_00D2, data);

    TLG_ReadRegNS(base_addr, REG_00D3, &data);
    TLGHAL_SET ( data, p_cc_field1->cc_line_start ,
                REG_00D1_BIT9_0_SHIFT,
                REG_00D1_BIT9_0_MASK);
    TLG_WriteReg(base_addr, REG_00D3, data);

    TLG_ReadRegNS(base_addr, REG_00D4, &data);
    TLGHAL_SET ( data, p_cc_field1->cc_line_stop ,
                REG_00D2_BIT9_0_SHIFT,
                REG_00D2_BIT9_0_MASK);
    TLG_WriteReg(base_addr, REG_00D4, data);

    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_CC_Control(
                uint32                  base_addr,
                uint16                  field0_en,
                uint16                  field1_en)
{
    uint16 data;

    TLG_ReadRegNS(base_addr, REG_00D0, &data);
    TLGHAL_SET ( data, field1_en,
                REG_00D0_BIT1_SHIFT,
                REG_00D0_BIT1_MASK);
    TLGHAL_SET ( data, field0_en,
                REG_00D0_BIT0_SHIFT,
                REG_00D0_BIT0_MASK);
    TLG_WriteReg(base_addr, REG_00D0, data);
    return(TLG_ERR_SUCCESS);
}

TLGDLL_API int TLG_CC_GetRawData(
                uint32                  base_addr,
                uint16                  field_number,
                uint16                 *p_length ,
                uint8                  *p_data)
{
    uint16 data;
    uint16 level;
    uint16 num_byte = 0;
    uint16 i;
    uint8  byte1, byte2;
    uint16 reg;

    
    TLG_ReadRegNS(base_addr, REG_0160, &data);
    if (field_number == 0)
    {
        TLGHAL_GET (level, data,
                    REG_0160_BIT4_0_SHIFT,
                    REG_0160_BIT4_0_MASK);
        reg   = REG_0162;
    }
    else
    {
        TLGHAL_GET (level, data,
                    REG_0160_BIT12_8_SHIFT,
                    REG_0160_BIT12_8_MASK);
        reg   = REG_0164;
    }
    
    if (level == 0)
    {
        
        *p_length = 0;
        return(0);
    }
    
    for (i = 0; i < level; i++)
    {
        
        
        TLG_ReadRegNS(base_addr, reg, &data);
        
        byte1   = (uint8) ((data     ) & 0xFF);
        byte2   = (uint8) ((data >> 8) & 0xFF);
        *p_data++ = byte1;
        *p_data++ = byte2;
        num_byte += 2;
    } 
    *p_length = num_byte;
    return(1);

}


TLGDLL_API int TLG_WritePair(uint32 base_addr, const tlg_reg_pair_t *p_tbl)
{
        
    while (p_tbl && p_tbl->reg != 0xFFFF)
    {
        TLG_WriteReg(base_addr, p_tbl->reg, p_tbl->value);
        p_tbl++;
    }
    return(0);
}


TLGDLL_API int TLG_Init_AGC_Reg(
                uint32                  base_addr,
                int                     TV_mode  )
{

    
    uint32 version;
    const tlg_reg_pair_t *p_tbl;
    TLG_GetChipVersion(tlg_i2c_addr, &version);

    TLG_REG_LOG((b, "TLG_Init_AGC_Reg START\n"));

    TLG_CHIP_VERS_1_2_BEGIN
#if defined(TLG_CMP_CHIP_1120_1_2) || defined(TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)

            switch (TV_mode)
            {
                case TLG_MODE_ANALOG_TV_UNCOMP   :  
                case TLG_MODE_ANALOG_TV_COMP     :  
                    
                    TLG_REG_LOG((b, "tlg_init_reg = AGC_in_VBI\n"));
                    
                    p_tbl = p_init_reg_1_2_AGC_in_VBI;
                    break;
                case TLG_MODE_FM_RADIO           :  
                        
                case TLG_MODE_NONE               :  
                case TLG_MODE_DVB_T              :  
                case TLG_MODE_DVB_H              :  
                case TLG_MODE_ALL                :  
                default:
                    TLG_REG_LOG((b, "tlg_init_reg = AGC_Always\n"));
                    p_tbl = p_init_reg_1_2_AGC_Always;
                    break;
            }

    TLG_WritePair(base_addr, p_tbl);

#endif 
    TLG_CHIP_VERS_1_1_ELSE

#ifdef TLG_CMP_CHIP_1120_1_1
#endif 
            TLG_CHIP_VERS_END

            TLG_REG_LOG((b, "TLG_Init_AGC_Reg DONE\n"));

    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_Set_AGC_Behavior(
                uint32                  base_addr,
                uint32                  new_agc_mode,
                uint32                  update_agc_reg)
{
    switch (new_agc_mode)
    {
        case TLG_AGC_NORMAL_MODE:
            agc_mode = new_agc_mode;
            p_init_reg_1_2_AGC_in_VBI = default_init_reg_1_2_AGC_in_VBI;
            p_init_reg_1_2_AGC_Always = default_init_reg_1_2_AGC_Always;
            break;
        case TLG_AGC_ALWAYS_MODE:
            agc_mode = new_agc_mode;
            p_init_reg_1_2_AGC_in_VBI = default_init_reg_1_2_AGC_Always;
            p_init_reg_1_2_AGC_Always = default_init_reg_1_2_AGC_Always;
            break;
        case TLG_AGC_CUSTOM_MODE:
            agc_mode = new_agc_mode;
            p_init_reg_1_2_AGC_in_VBI = custom_init_reg_1_2_AGC_in_VBI;
            p_init_reg_1_2_AGC_Always = custom_init_reg_1_2_AGC_Always;
            break;
        case TLG_AGC_SCAN_MODE:
            saved_init_reg_1_2_AGC_in_VBI  = p_init_reg_1_2_AGC_in_VBI;
            p_init_reg_1_2_AGC_in_VBI = p_init_reg_1_2_AGC_Always;
            break;
        case TLG_AGC_RESTORE_MODE:
            p_init_reg_1_2_AGC_in_VBI = saved_init_reg_1_2_AGC_in_VBI;
            break;
    }
    if (update_agc_reg)
    {
        TLG_Init_AGC_Reg(base_addr, tv_mode);
    }
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_Get_AGC_Behavior(
                uint32                  base_addr,
                uint32                  *p_agc_mode )
{
    *p_agc_mode = agc_mode;
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_Set_Table(
                uint32                  base_addr,
                uint32                  table_destination,
                tlg_reg_pair_t            *p_tbl)
{
    switch (table_destination)
    {
        case TLG_AGC_IN_VBI_TBL:
            custom_init_reg_1_2_AGC_in_VBI = p_tbl;
            TLG_Set_AGC_Behavior(base_addr, TLG_AGC_CUSTOM_MODE, 0);
            break;
        case TLG_AGC_ALWAYS_TBL:
            custom_init_reg_1_2_AGC_Always = p_tbl;
            TLG_Set_AGC_Behavior(base_addr, TLG_AGC_CUSTOM_MODE, 0);
            break;
    }
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLG_Get_Table(
                uint32                  base_addr,
                uint32                  table_destination,
                tlg_reg_pair_t            **p_tbl)
{
    switch (table_destination)
    {
        case TLG_AGC_IN_VBI_TBL:
            *p_tbl = (tlg_reg_pair_t *) p_init_reg_1_2_AGC_in_VBI;
            break;
        case TLG_AGC_ALWAYS_TBL:
            *p_tbl = (tlg_reg_pair_t *) p_init_reg_1_2_AGC_Always;
            break;
        default:
            return(TLG_ERR_FAIL);
    }
    return(TLG_ERR_SUCCESS);
}

TLGDLL_API int TLG_SetAudioNotchesNormal(uint32 base_addr)
{

    int32  freq_hz;
    uint16 vstd;

    TLG_GetChnStandard(base_addr, &vstd);

    switch(vstd)
    {
        case TLG_VSTD_PAL_B     :
        case TLG_VSTD_PAL_I     : freq_hz = 3250000; break;
        case TLG_VSTD_PAL_D     :
        case TLG_VSTD_PAL_D1    :
        case TLG_VSTD_PAL_K     : freq_hz = 3750000; break;
        default                 : freq_hz = 2750000; break;
    }

    TLG_REG_LOG((b, "After channel change, set the bw according to the freq stepsize\n"));
    TLG_SetAudioNotches(
                base_addr, 
                freq_hz          , 
                g_step_size_hz, 
                0     
                );
    return(TLG_ERR_SUCCESS);
}
TLGDLL_API int TLG_StartChannelScan(uint32 base_addr)
{
    TLG_Set_AGC_Behavior(base_addr, TLG_AGC_SCAN_MODE, 1);
    return(TLG_ERR_SUCCESS);
}
TLGDLL_API int TLG_StopChannelScan(uint32 base_addr)
{
    TLG_Set_AGC_Behavior(base_addr, TLG_AGC_RESTORE_MODE, 1);
    return(TLG_ERR_SUCCESS);
}

TLGDLL_API int TLG_SetTV_SSI_ThreshHold(uint32 base_addr, uint16 TV_SSI_Threshold)
{
    tlg_tv_ssi_threshold = TV_SSI_Threshold; 
    TLG_REG_LOG((b, "Set TV SSI threshold to: %d\n", tlg_tv_ssi_threshold));
    return(TLG_ERR_SUCCESS);
}

#ifdef ENABLE_REGISTER_SETTING_TEST
TLGDLL_API int TLG_SetTestCallbackFunc(
                uint32              base_addr, 
                fp_test_call_back   test_call_back,
                void                *test_param)
{
    test_call_back_func = test_call_back;
    g_test_param        = test_param;
    return(TLG_ERR_SUCCESS);
}
#endif 


void TLG_ModeSetup(uint32 base_addr, uint16 mode, uint16 force)
{
    uint16 data;

    TLG_REG_LOG((b,"TLG_ModeSetup: mode=%d\n", mode));
    TLG_ReadReg(base_addr, REG_0054,&data);

    TLGHAL_SET(data, 
                mode, 
                REG_0054_BIT15_SHIFT, 
                REG_0054_BIT15_MASK);
    
    TLGHAL_SET(data, 
                mode, 
                REG_0054_BIT12_SHIFT, 
                REG_0054_BIT12_MASK);
    TLG_WriteReg(base_addr,REG_0054,data);

    TLG_CHIP_VERS_2_1_BEGIN
 #if defined (TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)

        TLG_SetBits(base_addr, REG_00B1, 
                    mode, 
                    REG_00B1_BIT2_SHIFT,
                    REG_00B1_BIT2_MASK);

    if (mode == TLG_MODE_SETUP_TV)
    {
        
        TLG_ReadReg(base_addr, REG_00B4, &data);
        TLGHAL_SET(data, 1, REG_00B4_BIT1_SHIFT,
                    REG_00B4_BIT1_MASK);
        TLGHAL_CLEAR(data, REG_00B4_BIT4_MASK);
        TLG_WriteReg(base_addr, REG_00B4, data);
    }

#endif 
    TLG_CHIP_VERS_1_2_ELSE
#ifdef TLG_CMP_CHIP_1120_1_2

            TLG_ReadReg(base_addr, REG_008D, &data);
    TLGHAL_SET(data, 
                !mode, 
                REG_008D_BIT8_SHIFT,
                REG_008D_BIT8_MASK);
    TLG_WriteReg(base_addr, REG_008D, data);

#endif 
    TLG_CHIP_VERS_1_1_ELSE

#ifdef TLG_CMP_CHIP_1120_1_1

            TLG_ReadReg(base_addr, REG_008D, &data);
    TLGHAL_SET(data, 
                !mode, 
                REG_008D_BIT8_SHIFT,
                REG_008D_BIT8_MASK);
    TLG_WriteReg(base_addr, REG_008D, data);

#endif 
    TLG_CHIP_VERS_END

            TLG_REG_LOG((b,"TLG_ModeSetup:EXIT\n"));

}

static int tlg_1_2_region = TLG_REGION_WW; 

TLGDLL_API int TLG_SetRegion(uint32 base_addr, uint16 region)
{
    int res = TLG_ERR_SUCCESS;

    TLG_CHIP_VERS_2_1_BEGIN
 #if defined (TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)
        switch (region)
        {
            case TLG_REGION_US:
            case TLG_REGION_WW:
                TLG_SetBits(base_addr, 
                            REG_00B4, 
                            region,
                            REG_00B4_BIT5_SHIFT,
                            REG_00B4_BIT5_MASK);
                break;
            default: res = TLG_ERR_PARAM;
        }
#endif 
    TLG_CHIP_VERS_1_1_ELSE
#ifdef TLG_CMP_CHIP_1120_1_1
            tlg_1_2_region = region;
#endif 
    TLG_CHIP_VERS_END

            return res;
}

TLGDLL_API int TLG_GetRegion(uint32 base_addr, uint16 *region)
{
    uint16 reg;

    TLG_NULLPTR_CHECK(region);

    TLG_CHIP_VERS_2_1_BEGIN
 #if defined (TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)
    TLG_ReadReg(base_addr, REG_00B4, &reg);
    TLGHAL_GET(*region, reg,
                REG_00B4_BIT5_SHIFT,
                REG_00B4_BIT5_MASK);
#endif 
    TLG_CHIP_VERS_1_1_ELSE
#ifdef TLG_CMP_CHIP_1120_1_1
    *region = tlg_1_2_region;
#endif 
    TLG_CHIP_VERS_END

    return TLG_ERR_SUCCESS;
}

#ifdef TLG_ENABLE_I2C_QUAL_TESTS


uint16 *tlg_defreg_val;
uint16 tlg_defreg1_2_val[] = {
    
    
     0x0000, 0x0000, 0x4000, 0x2093, 0x0001, 0x0002, 0x0004, 0x0008,
     0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0001,
     0x26C8, 0x4610, 0xC820, 0x0000, 0x0000, 0x1009, 0x600A, 0x9109,
     0x9109, 0x0001, 0x0000, 0x8000, 0x0000, 0x0000, 0x4018, 0x4A40,
     0x8000, 0x4006, 0x1364, 0x0000, 0x0000, 0x0010, 0x000A, 0x000A,
     0x0000, 0x0000, 0x0000, 0x0010, 0x0008, 0x0010, 0x0000, 0x0000,
     0x0000, 0x012A, 0x0008, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x00A8, 0x0005, 0x0000, 0x0000,
     0x0007, 0x000C, 0x00F9, 0x0064, 0x000C, 0x0033, 0x0064, 0x000C,
     0x0062, 0x0064, 0x0040, 0xA5ED, 0x0800, 0x0000, 0x0400, 0x060C,
     0x0014, 0x008A, 0x0FFF, 0x7100, 0x90C8, 0x2130, 0x0490, 0x8000,
     0x4000, 0x804A, 0x1488, 0x1444, 0x7808, 0xC7A3, 0x810F, 0x0000,
     0x5864, 0x4504, 0xFC04, 0x2041, 0x43E1, 0x300F, 0x4096, 0x840C,
     0x9EEB, 0x0016, 0x0160, 0x77C6, 0x0380, 0x160C, 0x00D8, 0x40C2,
     0x8634, 0xC810, 0x4000, 0x8160, 0x0452, 0x0200, 0x0080, 0x0030,
     0x0002, 0x0108, 0x020D, 0x020D, 0x020D, 0x020D, 0x020D, 0x0089,
     0x035A, 0x020E, 0x020E, 0x020E, 0x020E, 0x0001, 0x0019, 0x0108,
     0x011F, 0x011F, 0x011F, 0x0101, 0x8440, 0x4944, 0x0000, 0x0000,
     0x031C, 0x0593, 0x1555, 0x5400, 0x0000, 0x0000, 0x0840, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x8830, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0404, 0x0408, 0x050E, 0x2001, 0x2108, 0x0000, 0x0000,
     0x1000, 0x0400, 0x0004, 0x0000, 0x49A0, 0x0000, 0x031C, 0x0593,
     0xFF22, 0x0100, 0x0000, 0x005C, 0x3C00, 0x08C5, 0x0000, 0x03C4,
     0x0000, 0x0000, 0x0000, 0x0000, 0x1040, 0x0800, 0x0400, 0x00D0,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0001, 0x0002, 0x0004, 0x0008,
     0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0000, 0x0400,
     0x0000, 0x0015, 0x0015, 0x011C, 0x011C, 0xB486, 0x0000, 0x200E,
     0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x4000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x4000, 0x0000, 0x8300, 0x0321, 0x5098, 0x538E, 0x297A, 0xF010,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0001, 0x4002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080,
     0x0100, 0x0200, 0x0000, 0x0400, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0100, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0400, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0400, 0x4000, 0x0000, 0x0001,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x4000, 0x0000, 0x0000, 0x0000, 0x0001, 0x0001,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0001, 0x0000, 0x4A55, 0x0400,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000,
     0x0001, 0x4002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080,
     0x0100, 0x0200, 0x0000, 0x0400, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x8080, 0x0000, 0x8080, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0052, 0x0000, 0x0000,
     0x0052, 0x0000, 0x0000, 0x0052, 0x0000, 0x0000, 0x0000, 0xAA80,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0063, 0x0000, 0x0000, 0x9400, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0202, 0x0390, 0x1A6C, 0x0044,
     0x048A, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0040, 0x1F90, 0x27B0, 0x0061, 0x0244, 0x0000, 0x06C1,
     0x0000, 0x0000, 0x4000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};

uint16 tlg_defreg2_1_val[] = {
    
    
     0x0000, 0x0000, 0x4000, 0x2093, 0x0001, 0x0002, 0x0004, 0x0008,
     0x0771, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0001,
     0x26C8, 0x4610, 0xC820, 0x0000, 0x0000, 0x1009, 0x600A, 0x9109,
     0x9109, 0x0001, 0x0000, 0x8000, 0x0000, 0x0000, 0x4018, 0x4A40,
     0x8000, 0x4006, 0x1364, 0x0000, 0x0000, 0x0010, 0x000A, 0x000A,
     0x0000, 0x0000, 0x0000, 0x0010, 0x0008, 0x0010, 0x0000, 0x0000,
     0x0000, 0x012A, 0x0008, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x00A8, 0x0005, 0x0000, 0x0000,
     0x0007, 0x000C, 0x00F9, 0x0064, 0x000C, 0x0033, 0x0064, 0x000C,
     0x0062, 0x0064, 0x0040, 0xA5ED, 0x0800, 0x0000, 0x0400, 0x060C,
     0x0014, 0x008A, 0x0FFF, 0x7100, 0x90C8, 0x2130, 0x0490, 0x8000,
     0x4000, 0x804A, 0x1488, 0x1444, 0x7808, 0xC7A3, 0x810F, 0x0000,
     0x5864, 0x4504, 0xFC04, 0x2041, 0x43E1, 0x300F, 0x4096, 0x840C,
     0x9EEB, 0x0016, 0x0160, 0x77C6, 0x0380, 0x160C, 0x00D8, 0x40C2,
     0x8634, 0xC810, 0x4000, 0x8160, 0x0452, 0x0200, 0x0080, 0x0030,
     0x0002, 0x0108, 0x020D, 0x020D, 0x020D, 0x020D, 0x020D, 0x0089,
     0x035A, 0x020E, 0x020E, 0x020E, 0x020E, 0x0001, 0x0019, 0x0108,
     0x011F, 0x011F, 0x011F, 0x0101, 0x8400, 0x4944, 0x0000, 0x5600,
     0x031C, 0x0593, 0x1555, 0x5702, 0x0000, 0x4000, 0x0840, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x24f8, 0x0000, 0x0000, 0x1170,
     0x0000, 0x0404, 0x0408, 0x050E, 0x2001, 0x2108, 0x0000, 0x0000,
     0x0771, 0x0400, 0x7751, 0x0000, 0x49A0, 0x5100, 0x01c2, 0x0190,
     0xFFb6, 0x0104, 0x0000, 0x005C, 0x3C00, 0x08C5, 0x7445, 0x03C4,
     0x0000, 0x0000, 0x0000, 0x0000, 0x1040, 0x0800, 0x0400, 0x00D0,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0001, 0x0002, 0x0004, 0x0008,
     0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0000, 0x0400,
     0x0000, 0x0015, 0x0015, 0x011C, 0x011C, 0xB486, 0x0000, 0x200E,
     0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x4000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x4000, 0x0000, 0x8300, 0x0321, 0x5098, 0x538E, 0x297A, 0xF010,
     0xec11, 0x0490, 0x8000, 0x0004, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0001, 0x4002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080,
     0x0100, 0x0200, 0x0000, 0x0400, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0100, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0400, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0400, 0x4000, 0x0000, 0x0001,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x4000, 0x0000, 0x0000, 0x0000, 0x0001, 0x0001,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0001, 0x0000, 0x4A55, 0x0400,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000,
     0x0001, 0x4002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080,
     0x0100, 0x0200, 0x0000, 0x0400, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x8080, 0x0000, 0x8080, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0052, 0x0000, 0x0000,
     0x0052, 0x0000, 0x0000, 0x0052, 0x0000, 0x0000, 0x0000, 0xAA80,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0063, 0x0000, 0x0000, 0x9400, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0202, 0x0390, 0x1A6C, 0x0044,
     0x048A, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0040, 0x1F90, 0x27B0, 0x0061, 0x0244, 0x0000, 0x06C1,
     0x0000, 0x0000, 0x4000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};

#define TLG_VALID_DEF_REG(a) (((0x0000 <= (a) <= 0x00ac) || \
                    (0x0100 <= (a) <= 0x0148) || \
                    (0x0180 <= (a) <= 0x01b8)) && \
            ((a) != 0x00ac && (a) != 0x0001 && \
             (a) != 0x0135 && (a) != 0x4000 && \
             (a) != 0x0139 && (a) != 0x0600 && \
             (a) != 0x010e && (a) != 0x0110 && \
             (a) != 0x0114 && (a) != 0x011b && \
             (a) != 0x0124 && (a) != 0x0125 && \
             (a) != 0x0128 && (a) != 0x0129 && \
             (a) != 0x012a && (a) != 0x012c && \
             (a) != 0x0132 && (a) != 0x010d && \
             (a) != 0x011e && (a) != 0x0121 && \
             (a) != 0x012b && (a) != 0x0000 && \
             (a) != 0x010c && (a) != 0x018e))

TLGDLL_API int TLG_TestRegVal_Init(uint32 base_addr)
{
#ifdef TLG_SUPPORT_RUNTIME_CHECK
    {uint32 version;
            TLG_GetChipVersion(base_addr, &version);};
#endif 
            TLG_CHIP_VERS_2_1_BEGIN
 #if defined (TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)

            tlg_defreg_val = tlg_defreg2_1_val;

#endif 
            TLG_CHIP_VERS_1_2_ELSE
#ifdef TLG_CMP_CHIP_1120_1_2

            tlg_defreg_val = tlg_defreg1_2_val;

#endif 
            TLG_CHIP_VERS_1_1_ELSE
#ifdef TLG_CMP_CHIP_1120_1_1

            TLG_REG_LOG((b,"setting defreg to 1.2\n"));
            tlg_defreg_val = tlg_defreg1_2_val;
#endif 
            TLG_CHIP_VERS_END

            return 0;
}

TLGDLL_API int TLG_TestRegVal(uint32 base_addr, uint16 reg, uint16 val)
{
    uint16 regval;

    TLG_ReadRegNS(base_addr, reg, &regval);

    if (regval != val)
    {
        TLG_REG_LOG((b,"ERROR: Val for Register 0x%x: Expected 0x%x, Received 0x%x\n",
                        reg, val, regval));
        return 1;
    }
    return 0;
}


TLGDLL_API void TLG_TestDefRegs(uint32 base_addr)
{
    uint16  reg;
    int     test_errors = 0;

    TLG_TestRegVal_Init(base_addr);
    TLG_REG_LOG((b, "START TLG DEFAULT REGISTER TESTS\n"));
    for (reg=0; reg <= 0x01b8; reg++)
            if (TLG_VALID_DEF_REG((short)reg))
                test_errors += TLG_TestRegVal(base_addr, reg, tlg_defreg_val[reg]);
    TLG_REG_LOG((b, "TOTAL DEFAULT REG VAL TEST ERRORS: %d\n", test_errors));
    TLG_REG_LOG((b, "END TLG DEFAULT REGISTER TESTS\n"));
}

TLGDLL_API void TLG_TestWriteRegs(uint32 base_addr)
{
    int            test_errors = 0;
    uint16         reg;
    unsigned short regval = 0;

    TLG_TestRegVal_Init(base_addr);
    TLG_REG_LOG((b, "START TLG WRITE REGISTER TESTS\n"));
    for (reg = 0x5; reg <= 0xe; reg++)
    {
        
        for (regval = 1; 1; regval = 2*regval)
        {
            TLG_WriteReg(base_addr, reg, regval);
            test_errors += TLG_TestRegVal(base_addr, reg, regval);
            if (regval == 0x8000) break;
        }
        
        for (regval = 3; 1; regval = 2*regval)
        {
            TLG_WriteReg(base_addr, reg, regval);
            test_errors += TLG_TestRegVal(base_addr, reg, regval);
            if (regval == 0xc000) break;
        }
        
        for (regval = 1; 1; regval = 2*regval + 1)
        {
            TLG_WriteReg(base_addr, reg, regval);
            test_errors += TLG_TestRegVal(base_addr, reg, regval);
            if (regval == 0xffff) break;
        }
        
        for (regval = 1; 1; regval = 4*regval + 1)
        {
            TLG_WriteReg(base_addr, reg, regval);
            test_errors += TLG_TestRegVal(base_addr, reg, regval);
            if (regval == 0x5555) break;
        }
        
        for (regval = 2; 1; regval = 4*regval + 2)
        {
            TLG_WriteReg(base_addr, reg, regval);
            test_errors += TLG_TestRegVal(base_addr, reg, regval);
            if (regval == 0xaaaa) break;
        }
        
        TLG_WriteReg(base_addr, reg, tlg_defreg_val[reg]);
    }
    TLG_REG_LOG((b, "TOTAL WRITE REG VAL TEST ERRORS: %d\n", test_errors));
    TLG_REG_LOG((b, "END TLG WRITE REGISTER TESTS\n"));
}
#endif 

TLGDLL_API int TLG_Set601DriveStrength(uint32 base_addr, uint16 bus_lines, uint16 driver_strength)
{
    uint16 tlg_data;
    TLG_BOUNDS_CHECK(driver_strength,TLG_601_BUS_DRIVE_MAX);
    if (bus_lines & TLG_601_BUS_SELECT_ALL)
        TLG_ReadReg(base_addr, REG_0000, &tlg_data);

        TLG1121_VERS_A_0_BEGIN
#if defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1121_A_2)
    if ((bus_lines & TLG_601_BUS_SELECT_DATA) == TLG_601_BUS_SELECT_DATA)
    {
    TLGHAL_SET(tlg_data,
            driver_strength,
        REG_0000_BIT9_8_SHIFT,
        REG_0000_BIT9_8_MASK);  
    TLGHAL_SET(tlg_data,
             driver_strength>>2,
            REG_0000_BIT11_SHIFT,
            REG_0000_BIT11_MASK);
    }
    if ((bus_lines & TLG_601_BUS_SELECT_CLK) == TLG_601_BUS_SELECT_CLK)
    {
        TLGHAL_SET(tlg_data,
            driver_strength,
        REG_0000_BIT2_1_SHIFT,
        REG_0000_BIT2_1_MASK);
    TLGHAL_SET(tlg_data,
            driver_strength>>2,
        REG_0000_BIT4_SHIFT,
        REG_0000_BIT4_MASK);
    }
    #endif  
        TLG_CHIP_VERS_2_1_ELSE
    #if defined (TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1120_2_3)
    driver_strength = ((driver_strength > 0) ? 1 : 0);
    if ((bus_lines & TLG_601_BUS_SELECT_DATA) == TLG_601_BUS_SELECT_DATA)
    {
        TLGHAL_SET(tlg_data,
            driver_strength,
            REG_0000_BIT8_SHIFT,
            REG_0000_BIT8_MASK);
    }
    if ((bus_lines & TLG_601_BUS_SELECT_CLK) == TLG_601_BUS_SELECT_CLK)
    {
        TLGHAL_SET(tlg_data,
            driver_strength,
            REG_0000_BIT9_SHIFT,
            REG_0000_BIT9_MASK);
    }
    #endif 
        TLG_CHIP_VERS_END
    if (bus_lines & TLG_601_BUS_SELECT_ALL)
    {
        TLG_WriteReg(base_addr, REG_0000, tlg_data);  
    }
        return  TLG_ERR_SUCCESS;   
}


TLGDLL_API int TLG_Get601DriveStrength(uint32 base_addr,uint16 bus_lines, uint16 *driver_strength)
{
    uint16 tlg_data;
    uint16 driver_strength_h;
          
    TLG_ReadReg(base_addr, REG_0000, &tlg_data);
        
    TLG1121_VERS_A_0_BEGIN
    #if defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1121_A_2)
    if ((bus_lines & TLG_601_BUS_SELECT_DATA) == TLG_601_BUS_SELECT_DATA)
    {
            TLGHAL_GET( *driver_strength, tlg_data,
                REG_0000_BIT9_8_SHIFT   ,
                REG_0000_BIT9_8_MASK);
            TLGHAL_GET(driver_strength_h,tlg_data,
                REG_0000_BIT11_SHIFT,
                REG_0000_BIT11_MASK);
            *driver_strength |= (driver_strength_h<<2);
    }
    if ((bus_lines & TLG_601_BUS_SELECT_CLK) == TLG_601_BUS_SELECT_CLK)
    {
            TLGHAL_GET( *driver_strength, tlg_data,
                REG_0000_BIT2_1_SHIFT,
                REG_0000_BIT2_1_MASK);
            TLGHAL_GET(driver_strength_h,tlg_data,
                REG_0000_BIT4_SHIFT,
                REG_0000_BIT4_MASK);
            *driver_strength |= (driver_strength_h<<2);  
    }
    #endif  
        TLG_CHIP_VERS_2_1_ELSE
    #if defined (TLG_CMP_CHIP_1120_2_1) || defined(TLG_CMP_CHIP_1120_2_3)
    if ((bus_lines & TLG_601_BUS_SELECT_DATA) == TLG_601_BUS_SELECT_DATA)
    {
        TLGHAL_GET( *driver_strength, tlg_data,
                    REG_0000_BIT8_SHIFT,
                    REG_0000_BIT8_MASK);
    }
    if ((bus_lines & TLG_601_BUS_SELECT_CLK) == TLG_601_BUS_SELECT_CLK)
    {
        TLGHAL_GET( *driver_strength, tlg_data,
                REG_0000_BIT9_SHIFT,
                REG_0000_BIT9_MASK);
    }
    #endif 
        TLG_CHIP_VERS_END
    return  TLG_ERR_SUCCESS;
}

TLGDLL_API int TLG_SetI2sBusStrength(uint32 base_addr, uint16 SM_strength, uint16 SL_strength)
{
    uint16 tlg_data;
    TLG_BOUNDS_CHECK(SM_strength,TLG_MAX_I2S_BUS_STRENGTH_SM);
    TLG_BOUNDS_CHECK(SL_strength,TLG_MAX_I2S_BUS_STRENGTH_SL);

#if defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1121_A_2)
    
    TLG_ReadReg(base_addr, REG_008E, &tlg_data);
    TLGHAL_SET(tlg_data,
                SM_strength,
                REG_008E_BIT14_12_SHIFT,
                REG_008E_BIT14_12_MASK);
    
    TLGHAL_SET(tlg_data,
                SL_strength,
                REG_008E_BIT10_8_SHIFT,
                REG_008E_BIT10_8_MASK);
    TLG_WriteReg(base_addr, REG_008E, tlg_data);

    return  TLG_ERR_SUCCESS;
#else
     return  TLG_ERR_NOT_SUPPORTED;
#endif 
}

TLGDLL_API int TLG_GetI2sBusStrength(uint32 base_addr, uint16 *SM_strength, uint16 *SL_strength)
{
    uint16 tlg_data;

#if defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1121_A_2)
    
    TLG_ReadReg(base_addr, REG_008E, &tlg_data);
    TLGHAL_GET(
                *SM_strength,tlg_data,
                REG_008E_BIT14_12_SHIFT,
                REG_008E_BIT14_12_MASK);
    
    TLGHAL_GET(
                *SL_strength,tlg_data,
                REG_008E_BIT10_8_SHIFT,
                REG_008E_BIT10_8_MASK);

    return  TLG_ERR_SUCCESS;
#else
     return  TLG_ERR_NOT_SUPPORTED;
#endif 
}
 #ifdef TLG_SUPPORT_FM_AVC 
 
 int TLG_AutoVolumeControlReset(uint32 base_addr)
{
    uint16 regData;
    int i;
    
    TLG_ReadReg(base_addr, REG_00B4, &regData);
    TLGHAL_SET(regData, fm_demph_bypass,
            REG_00B4_BIT6_SHIFT, 
            REG_00B4_BIT6_MASK);
    TLGHAL_SET(regData, fm_mu_sel,
            REG_00B4_BIT5_SHIFT,
            REG_00B4_BIT5_MASK);
    TLG_WriteReg(base_addr, REG_00B4, regData);
    TLG_ReadReg(base_addr, REG_00F0, &regData);
     
    TLGHAL_SET(regData,  fm_scale_sel, 
                REG_00F0_BIT12_10_SHIFT,
                REG_00F0_BIT12_10_MASK);
     TLG_WriteReg(base_addr, REG_00F0, regData);
    for (i = 0; i < TLG_FM_AVC_LEVEL_TOTAL; i++)
       fm_SignalCounter[i] = 0;
    fm_Level_cur = 0;
        return(TLG_ERR_SUCCESS);
 }

 int TLG_AutoVolumeControlInit(uint32 base_addr, uint16 mode)
{
    uint16 regData;
    int i;
    
    TLGHAL_SET(regData, 4, 
        REG_00AA_BIT14_12_SHIFT,
        REG_00AA_BIT14_12_MASK);
    TLGHAL_SET(regData, 6, 
        REG_00AA_BIT10_8_SHIFT,
        REG_00AA_BIT10_8_MASK);
    TLGHAL_SET(regData, 4, 
        REG_00AA_BIT6_4_SHIFT,
        REG_00AA_BIT6_4_MASK);
    TLGHAL_SET(regData, 1, 
        REG_00AA_BIT1_SHIFT,
        REG_00AA_BIT1_MASK);
    TLGHAL_SET(regData, 1, 
        REG_00AA_BIT0_SHIFT,
        REG_00AA_BIT0_MASK);
    TLG_WriteReg(base_addr, REG_00AA, regData);
    

    
    TLGHAL_SET(regData, 5, 
        REG_00A8_BIT10_8_SHIFT,
        REG_00A8_BIT10_8_MASK);
    TLGHAL_SET(regData, 5, 
        REG_00AA_BIT6_4_SHIFT,
        REG_00AA_BIT6_4_MASK);
    TLGHAL_SET(regData, 1, 
        REG_00A8_BIT0_SHIFT,
        REG_00A8_BIT0_MASK);
    TLG_WriteReg(base_addr, REG_00A8, regData);
    
    if (mode  == TLG_MODE_FM_RADIO )
    {
        TLG_WriteReg(base_addr, REG_00B3, 0x005c);
        TLG_ReadRegNS(base_addr, REG_00B4, &regData);
        regData = regData & 0x00FF | 0x3c00;
        TLG_WriteReg(base_addr, REG_00B4, regData);
        TLG_SetBits(base_addr, REG_00B0, 0, 
            REG_00B0_BIT0_SHIFT, 
            REG_00B0_BIT0_MASK);
    }
    else
    {
        TLG_WriteReg(base_addr, REG_00B3, 0x0043);
        TLG_ReadRegNS(base_addr, REG_00B4, &regData);
        regData = regData & 0x00FF | 0xF700;
        TLG_WriteReg(base_addr, REG_00B4, regData);
        TLG_SetBits(base_addr, REG_00B0, 1, 
            REG_00B0_BIT0_SHIFT, 
            REG_00B0_BIT0_MASK);
        
        TLG_ReadReg(base_addr, REG_00B4, &regData);
        TLGHAL_GET(fm_demph_bypass, regData,
                REG_00B4_BIT6_SHIFT, 
                REG_00B4_BIT6_MASK);
        TLGHAL_GET(fm_mu_sel, regData,
                REG_00B4_BIT5_SHIFT,
                REG_00B4_BIT5_MASK);
    }
    for (i = 0; i < TLG_FM_AVC_LEVEL_TOTAL; i++)
       fm_SignalCounter[i] = 0;
    fm_Level_cur = 0;
    	return(TLG_ERR_SUCCESS);
}


 int TLG_AutoVolumeControlCheck(uint32 base_addr)
{
    static uint16 Reg_F0_Data;
    static int fm_Level_new, gain;
    int i;

    int SignalStrong;
    uint16 regData;
    uint32 audio_noise_level;
    g_tlg_reg_f0_min = fm_scale_sel;

    TLG_SetBits(base_addr, REG_00B1, 1,
        REG_00B1_BIT5_SHIFT, 
        REG_00B1_BIT5_MASK);

    TLG_ReadRegNS(base_addr, REG_016E_VALUE_2, &regData);
    audio_noise_level = regData;
    TLG_ReadRegNS(base_addr, REG_016E_VALUE_3, &regData);
    audio_noise_level |= regData << 16;
    TLG_SetBits(base_addr, REG_00B1, 0,
        REG_00B1_BIT5_SHIFT, 
        REG_00B1_BIT5_MASK);
    if (audio_noise_level < TLG_FM_AVC_BASE_THRESHOLD)
       SignalStrong = TLG_FM_AVC_LEVEL_0;
    else if (audio_noise_level < TLG_FM_AVC_BASE_THRESHOLD*2)
       SignalStrong = TLG_FM_AVC_LEVEL_1;
    else if (audio_noise_level < TLG_FM_AVC_BASE_THRESHOLD*4)
       SignalStrong = TLG_FM_AVC_LEVEL_2;
    else if (audio_noise_level < TLG_FM_AVC_BASE_THRESHOLD*8)
       SignalStrong = TLG_FM_AVC_LEVEL_3;
    else
       SignalStrong = TLG_FM_AVC_LEVEL_4;
    if (SignalStrong != -1)
    {
        fm_SignalCounter[SignalStrong]++;
        for (i = 1, fm_Level_new = 0; i < TLG_FM_AVC_LEVEL_TOTAL; i++)
        {
            if (fm_SignalCounter[i] > fm_SignalCounter[fm_Level_new])
            fm_Level_new = i;
        }
        if (fm_SignalCounter[fm_Level_new] > TLG_FM_AVC_COUNT_MAX)
        {
            for (i = 0; i < TLG_FM_AVC_LEVEL_TOTAL; i++)
            {
                if (fm_SignalCounter[i] != 0)
                fm_SignalCounter[i]--;
            }
        if ((fm_SignalCounter[fm_Level_new] - fm_SignalCounter[fm_Level_cur]) >= TLG_FM_AVC_LEAD_AHEAD)
        {
            fm_Level_cur = fm_Level_new;
            switch(fm_Level_cur)
            {
                case TLG_FM_AVC_LEVEL_0:
                    break;

                case TLG_FM_AVC_LEVEL_1:
                    TLG_SetBits(base_addr, REG_00B4, 0, 
                        REG_00B4_BIT6_SHIFT, 
                        REG_00B4_BIT6_MASK);
                    TLG_SetBits(base_addr, REG_00B4, 0, 
                        REG_00B4_BIT5_SHIFT, 
                        REG_00B4_BIT5_MASK);
                    break;

                case TLG_FM_AVC_LEVEL_2:
                    TLG_SetBits(base_addr, REG_00B4, 0, 
                        REG_00B4_BIT6_SHIFT, 
                        REG_00B4_BIT6_MASK);
                    TLG_SetBits(base_addr, REG_00B4, 1, 
                        REG_00B4_BIT5_SHIFT, 
                        REG_00B4_BIT5_MASK);
                    break;

                case TLG_FM_AVC_LEVEL_3:
                    TLG_SetBits(base_addr, REG_00B4, 0, 
                        REG_00B4_BIT6_SHIFT, 
                        REG_00B4_BIT6_MASK);
                    TLG_SetBits(base_addr, REG_00B4, 1, 
                        REG_00B4_BIT5_SHIFT, 
                        REG_00B4_BIT5_MASK);
                    break;

                case TLG_FM_AVC_LEVEL_4:  
                    TLG_SetBits(base_addr, REG_00B4, 0, 
                        REG_00B4_BIT6_SHIFT, 
                        REG_00B4_BIT6_MASK);
                    TLG_SetBits(base_addr, REG_00B4, 1, 
                        REG_00B4_BIT5_SHIFT, 
                        REG_00B4_BIT5_MASK);
                    break;
                }
            }
       }
    }

    TLG_ReadRegNS(base_addr, REG_00F0, &Reg_F0_Data);
    gain = (Reg_F0_Data >> 10)&7;
    if (fm_Level_cur == TLG_FM_AVC_LEVEL_4)
    {
        if (gain < TLG_FM_AVC_REG_F0_MAX)
        {
            gain += TLG_FM_AVC_REG_F0_INC_STEP;
            if (gain > TLG_FM_AVC_REG_F0_MAX)
                gain = TLG_FM_AVC_REG_F0_MAX;
            Reg_F0_Data = (Reg_F0_Data & ~0x1C00) |(gain << 10);
            TLG_WriteReg(base_addr, REG_00F0, Reg_F0_Data);
        }
    }
    else
    {
        if (gain > g_tlg_reg_f0_min)
        {
            gain -= TLG_FM_AVC_REG_F0_DEC_STEP;
            if (gain < g_tlg_reg_f0_min)
                gain = g_tlg_reg_f0_min;
            Reg_F0_Data = (Reg_F0_Data & ~0x1C00) |(gain << 10);
            TLG_WriteReg(base_addr, REG_00F0, Reg_F0_Data);
        }
    }
        return(TLG_ERR_SUCCESS);
}
#endif 
#ifdef TLG_SUPPORT_AUTO_AGC_CHANGE
int TLG_CheckAudioPerf(uint32 base_addr)
{
    static uint16  Raise = 0;
    static uint16 Fall = 0;
    static uint16  Mode_cur, Mode_new;
    uint16 RegData;
    uint16 SignalStrong;

    TLG_ReadRegNS(base_addr, REG_013B, &RegData);


    switch(RegData&(REG_013B_BIT5_MASK + REG_013B_BIT2_MASK))
    {
        case 0:
        case REG_013B_BIT2_MASK:
        case REG_013B_BIT5_MASK:
            SignalStrong = 0;
            break;
        case REG_013B_BIT5_MASK + REG_013B_BIT2_MASK:
            SignalStrong = 1;
            break;
        default:
            break;
    }
    
    if (SignalStrong == 0)
    {
        Raise = 0;
        if (++Fall > 6)
        {
            Fall--;
            Mode_new = 0;
        }
    }
    else
    {
        Fall= 0;
        if (++Raise > 6)
        {
            Raise--;
            Mode_new = 1;
        }
    }

    if (Mode_cur != Mode_new)
    {
        Mode_cur = Mode_new;
        TLG_Set_AGC_Behavior(base_addr, (Mode_cur ? TLG_AGC_NORMAL_MODE : TLG_AGC_ALWAYS_MODE), 1);
    }
    return(TLG_ERR_SUCCESS);
}
#endif 


static void TLG_ChangeSNRmode(uint32 base_addr, int snrmode, uint16 vstd)
{
    uint16 reg65, val, vdemod;

    snr_mode = snrmode;

    if(snrmode == TLG_MODE_LOW_SNR)
    {
        TLG_REG_LOG((b,"LOW SNR MODE\n"));

        if (!TLG_IsSECAMChnStandard(vstd))
        {
            
            TLG_ReadReg(base_addr, REG_0065, &reg65);
            
            
            TLGHAL_SET(reg65, 0,
                        REG_0065_BIT3_SHIFT,
                        REG_0065_BIT3_MASK);
            
            TLGHAL_SET(reg65, 0,
                        REG_0065_BIT1_SHIFT,
                        REG_0065_BIT1_MASK);
            
            TLGHAL_SET(reg65, 0,
                        REG_0065_BIT0_SHIFT,
                        REG_0065_BIT0_MASK);
            TLG_WriteReg(base_addr, REG_0065,reg65);    

            TLG_WriteReg(base_addr, REG_00BB_VALUE_2,                      
                                    TLGAPI_CHROMA_COEFF_2_LO_SNR_NTSC_PAL);
            
            
            TLG_SetBits(base_addr, REG_005A_VALUE_1, 
                                   TLGAPI_HSYNC_3_DELAY1_LOW_SNR,
                                   REG_005A_BIT7_0_SHIFT,
                                   REG_005A_BIT7_0_MASK);

            if (TLG_IsNTSCorPALMChnStandard(vstd))
            {
                val    = TLG_NTSC_LUMA_SCALE + 5;
                vdemod = 0x2020;
            }
            else
            {
                val    = TLG_PAL_LUMA_SCALE + 5;
                if (vstd == TLG_VSTD_PAL_NC)
                    vdemod = 0x2020;
                else
                    vdemod = 0xc020;
            }
            TLG_SetBits(base_addr, REG_006D, val,
                        REG_006D_BIT14_8_SHIFT,
                        REG_006D_BIT14_8_MASK);

            TLG_WriteReg(base_addr, REG_00A7, vdemod);
        }
        
        TLG_SetBits(base_addr, REG_005A_VALUE_4,   0x0d, 
                    REG_005A_BIT11_8_SHIFT,
                    REG_005A_BIT11_8_MASK);
        TLG_SetBits(base_addr, REG_0011,   0x28, 
                    REG_0011_BIT15_8_SHIFT,
                    REG_0011_BIT15_8_MASK);
        TLG_SetBits(base_addr, REG_0019,   0x01,
                    REG_0019_BIT12_SHIFT,
                    REG_0019_BIT12_MASK);
    }
    else
    {
        TLG_REG_LOG((b,"HIGH SNR MODE\n"));

        if (!TLG_IsSECAMChnStandard(vstd))
        {
            
            TLG_ReadReg(base_addr, REG_0065, &reg65);
            
            
            TLGHAL_SET(reg65, 1,
                        REG_0065_BIT3_SHIFT,
                        REG_0065_BIT3_MASK);
            
            TLGHAL_SET(reg65, 1,
                        REG_0065_BIT1_SHIFT,
                        REG_0065_BIT1_MASK);
            
            TLGHAL_SET(reg65, 1,
                        REG_0065_BIT0_SHIFT,
                        REG_0065_BIT0_MASK);
            TLG_WriteReg(base_addr, REG_0065,reg65);   
             
             
            TLG_SetBits(base_addr, REG_005A_VALUE_1, 
                                   TLGAPI_HSYNC_3_DELAY1_DEFAULT,
                                   REG_005A_BIT7_0_SHIFT,
                                   REG_005A_BIT7_0_MASK);

            TLG_WriteReg(base_addr, REG_00BB_VALUE_2,
                                    TLGAPI_CHROMA_COEFF_2_HI_SNR_NTSC_PAL);
 
            if (TLG_IsNTSCorPALMChnStandard(vstd))
                val = TLG_NTSC_LUMA_SCALE;
            else
                val = TLG_PAL_LUMA_SCALE;
            TLG_SetBits(base_addr, REG_006D, val,
                        REG_006D_BIT14_8_SHIFT,
                        REG_006D_BIT14_8_MASK);

            TLG_WriteReg(base_addr, REG_00A7, 0x0);
        }
        
        TLG_SetBits(base_addr, REG_005A_VALUE_4,   0x08, 
                    REG_005A_BIT11_8_SHIFT,
                    REG_005A_BIT11_8_MASK);
        TLG_SetBits(base_addr, REG_0011,   0x46, 
                    REG_0011_BIT15_8_SHIFT,
                    REG_0011_BIT15_8_MASK);
        TLG_SetBits(base_addr, REG_0019,   0x00,
                    REG_0019_BIT12_SHIFT,
                    REG_0019_BIT12_MASK);
    }
}

static void TLG_ChangeREG5Amode(uint32 base_addr, int reg5a_mode, uint16 vstd)
{
    uint16 reg5a;

    g_reg5a_mode = reg5a_mode;
    TLG_REG_LOG((b,"REG5A mode = %d\n", reg5a_mode));
   
    
    if (TLG_IsSECAMChnStandard(vstd))
        reg5a_mode = TLG_REG5A_MODE_1;

    TLG_ReadReg(base_addr, REG_005A, &reg5a);
    if(reg5a_mode == TLG_REG5A_MODE_1)
    {
        TLGHAL_SET(reg5a, 0x4, REG_005A_BIT11_8_SHIFT,
                               REG_005A_BIT11_8_MASK);
    }
    else if(reg5a_mode == TLG_REG5A_MODE_2)
    {
        TLGHAL_SET(reg5a, 0x5, REG_005A_BIT11_8_SHIFT,
                               REG_005A_BIT11_8_MASK);
    }
    else if(reg5a_mode == TLG_REG5A_MODE_3)
    {
        TLGHAL_SET(reg5a, 0x6, REG_005A_BIT11_8_SHIFT,
                               REG_005A_BIT11_8_MASK);
    }

    TLG_WriteReg(base_addr, REG_005A, reg5a); 
}

static void TLG_ChangeSecamColorMode(uint32 base_addr, int secam_color_mode)
{
    g_secam_color_mode = secam_color_mode;
    TLG_REG_LOG((b,"secam color mode = %d\n", secam_color_mode));
   
    if(secam_color_mode == TLG_SECAM_COLOR_MODE_1)
    {      
        
        TLG_WriteReg(base_addr, REG_00E9_VALUE_1, 0xf010);
        
        TLG_SetBits(base_addr,
            REG_005A_VALUE_7,
            0x20,
            REG_005A_BIT15_8_SHIFT,
            REG_005A_BIT15_8_MASK);
    }
    else if(secam_color_mode == TLG_SECAM_COLOR_MODE_2)
    { 
        
        TLG_WriteReg(base_addr, REG_00E9_VALUE_1, 0xf40c);
        
        TLG_SetBits(base_addr,
            REG_005A_VALUE_7,
            0x30,
            REG_005A_BIT15_8_SHIFT,
            REG_005A_BIT15_8_MASK);
    }
    else if(secam_color_mode == TLG_SECAM_COLOR_MODE_3)
    {
        
        TLG_WriteReg(base_addr, REG_00E9_VALUE_1, 0xf808);
        
        TLG_SetBits(base_addr,
            REG_005A_VALUE_7,
            0x38,
            REG_005A_BIT15_8_SHIFT,
            REG_005A_BIT15_8_MASK);
    }
    else if(secam_color_mode == TLG_SECAM_COLOR_MODE_4)
    {
        
        TLG_WriteReg(base_addr, REG_00E9_VALUE_1, 0xfc04);
        
        TLG_SetBits(base_addr,
            REG_005A_VALUE_7,
            0x40,
            REG_005A_BIT15_8_SHIFT,
            REG_005A_BIT15_8_MASK);
    }

}

static void TLG_ChangeSecamFlickerMode(uint32 base_addr, int secam_flicker_mode)
{
    g_secam_flicker_mode = secam_flicker_mode;
    if (secam_flicker_mode == TLG_SECAM_FLICKER_MODE_1)
    {
        
        TLG_SetBits(base_addr,
                    REG_0065,
                    0x2,
                    REG_0065_BIT9_8_SHIFT,
                    REG_0065_BIT9_8_MASK);
    }
    else if (secam_flicker_mode == TLG_SECAM_FLICKER_MODE_2)
    {
        
        TLG_SetBits(base_addr,
                    REG_0065,
                    0x1,
                    REG_0065_BIT9_8_SHIFT,
                    REG_0065_BIT9_8_MASK);    
    }
}

TLGDLL_API int TLG_SetSysClkSource(
    uint32                base_addr,
    tlg_sys_clk_source_t  clock_source)
{
    TLG_BOUNDS_CHECK(clock_source, TLG_SYS_CLK_SOURCE_MAX);
    g_sys_clk_source = clock_source;
    
    if (TLG_SYS_CLK_SOURCE_EXT == g_sys_clk_source)
    {
        TLG_WriteReg(base_addr, REG_004B_VALUE_5, 0x2fff);
    } 
    else if (TLG_SYS_CLK_SOURCE_XTAL == g_sys_clk_source) 
    {
        TLG_WriteReg(base_addr, REG_004B_VALUE_5, 0xfff);            
    }
    return TLG_ERR_SUCCESS;
    
}



typedef struct tlg_notch_pair_s
{

    uint16  notch  ;
    uint16  bw   ;
} tlg_notch_pair_t, *p_tlg_notch_pair_t;


/*******************in tlg1120api.c*******************************/
uint32 BB_System_Clk[NUM_TONE_NOTCH -1]=
{
        TLG_BB_SYS_CLK_1,
        TLG_BB_SYS_CLK_2,
        TLG_BB_SYS_CLK_3,
        TLG_BB_SYS_CLK_4,
        TLG_BB_SYS_CLK_5
};
tlg_notch_pair_t tlg_notch_table[NUM_TONE_NOTCH -1]=
{
    {AUDIO_ADJ_4_NOTCH,     TLG_NOTCH_BW_1},
    {AUDIO_ADJ_3_NOTCH,     TLG_NOTCH_BW_2},
    {AUDIO_ADJ_2_NOTCH,     TLG_NOTCH_BW_3},
    {AUDIO_MAIN_NOTCH,       TLG_NOTCH_BW_4},
    {AUDIO_ADJ_1_NOTCH ,    TLG_NOTCH_BW_5}

};
void  TLG_SetBBClkHarmonicNotch(uint32 base_addr, int32 center_freq_in)
{

    uint32       center_freq_cvt = HZCVT(center_freq_in);
    int32          iF;
    uint32        itF;
    uint16       i , bw;
    uint32       base_clk;

  
    TLG_REG_LOG((b,"TLGAPP_SetClkHarmonicNotch: hz=%d START\n", center_freq_in));

   
    TLG_GetChnBandwidth(base_addr,  &bw);

    for(i = 0; i <(NUM_TONE_NOTCH -1) ;i ++ )
    {


        if( (BB_System_Clk[i]!=0))
        {

            base_clk = HZCVT(BB_System_Clk[i]);

            iF = center_freq_cvt % base_clk;
            iF = (iF >=  base_clk/2) ? -iF +  base_clk : -iF;
            itF = (iF < 0) ? -iF : iF;

            if (itF <= MHZCVT(bw/2+1)) 
            {
                TLG_SetNotchAttrib(
                    base_addr, 
                    iF*HZCVTUNITS,                               
                    tlg_notch_table[i].notch, 
                    tlg_notch_table[i].bw, 
                    TLG_OFF,                         
                    0,                               
                    1,                          
                    TLG_NOTCH_SET_HZ|TLG_NOTCH_SET_EN|TLG_NOTCH_SET_BYP|TLG_NOTCH_SET_BW
                    );
            }
            else/*disable notch*/
            {
               TLG_SetNotchAttrib(
                    base_addr, 
                    0,                               
                    tlg_notch_table[i].notch, 
                    0, 
                    0,                         
                    0,                               
                    0,                          
                    TLG_NOTCH_SET_EN
                    );

            }

         }


   }

    
}
/************************************************************************************************/

/*in tlg1120app.c  which is used for FAE to debug*/
/*
center_freq_in = harmonic freq  - channel center freq (hz)
notch = 0~5  
bw = 0~8
enable  1: enable  0 : disable
*/
void  TLGAPP_SetBBClkHarmonicNotch_debug(uint32 base_addr, int32 center_freq_in,uint16 notch,uint16 bw,uint16 enable)
{
                TLG_SetNotchAttrib(base_addr, 
                    center_freq_in,                               
                    notch, 
                    bw, 
                    TLG_OFF,                         
                    0,                               
                    enable,                          
                    TLG_NOTCH_SET_HZ|TLG_NOTCH_SET_EN|TLG_NOTCH_SET_BYP|TLG_NOTCH_SET_BW
                    );

}
/************************************************************************************************/
#endif 
