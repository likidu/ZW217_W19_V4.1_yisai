//##!(important notice: do not remove this line)
/*****************************************************************************
* FILENAME
* tlg1120App.c
*
*
* ABSTRACT
* This file contains example procedures for initializing and managing the
* TLG1120. Note: This example code assumes that TLG_EMBEDDED_TIMERS is
* enabled.
*
* $Revision: #4 $
*
* (c) 2008 Telegent Systems
*****************************************************************************/


#include "tlgInclude.h"
#include "spload_main.h"
#include "arm_reg.h"
#include "os_api.h"
#include "tb_comm.h"
#include "tb_drv.h"
#include "iram_mgr.h"
#include "lcd_cfg.h"
//#include "TV_Driver.h"

#if TLG_CHIP_TYPE == TLG_CHIP_TYPE_TLG1120_1

int         tv_mode         = TLG_MODE_ANALOG_TV_UNCOMP;
#ifdef TLG_USE_FAST_LOCK_DETECT
uint16      g_fast_scan_mode = 1;
#else 
uint16      g_fast_scan_mode = 0;
#endif 

#ifdef DEBUG_FILE_LOG
#define dbg_printf(a)     tlg_dbg_printf a 
#define log_printf(a)        tlg_log_printf a
void log_printf((char * format, ...));
void dbg_printf((int num, char * format, ...));
#else 
#define dbg_printf(a)
#define log_printf(a)
#endif 

#define FM_MIN_FREQ 87500000
#define FM_MAX_FREQ 108000000


tlg_channel_t(tlg_fm_chn_map) = {
    {201,    87500000,  6,    TLG_VSTD_NTSC_M  },
    {202,    87600000,  6,    TLG_VSTD_NTSC_M  },
    {203,    87700000,  6,    TLG_VSTD_NTSC_M  },
    {204,    87800000,  6,    TLG_VSTD_NTSC_M  },
    {205,    87900000,  6,    TLG_VSTD_NTSC_M  },
    {206,    88000000,  6,    TLG_VSTD_NTSC_M  },
    {207,    88100000,  6,    TLG_VSTD_NTSC_M  },
    {208,    88200000,  6,    TLG_VSTD_NTSC_M  },
    {209,    88300000,  6,    TLG_VSTD_NTSC_M  },
    {210,    88400000,  6,    TLG_VSTD_NTSC_M  },
    {211,    88500000,  6,    TLG_VSTD_NTSC_M  },
    {212,    88600000,  6,    TLG_VSTD_NTSC_M  },
    {213,    88700000,  6,    TLG_VSTD_NTSC_M  },
    {214,    88800000,  6,    TLG_VSTD_NTSC_M  },
    {215,    88900000,  6,    TLG_VSTD_NTSC_M  },
    {216,    89000000,  6,    TLG_VSTD_NTSC_M  },
    {217,    89100000,  6,    TLG_VSTD_NTSC_M  },
    {218,    89200000,  6,    TLG_VSTD_NTSC_M  },
    {219,    89300000,  6,    TLG_VSTD_NTSC_M  },
    {220,    89400000,  6,    TLG_VSTD_NTSC_M  },
    {221,    89500000,  6,    TLG_VSTD_NTSC_M  },
    {222,    89600000,  6,    TLG_VSTD_NTSC_M  },
    {223,    89700000,  6,    TLG_VSTD_NTSC_M  },
    {224,    89800000,  6,    TLG_VSTD_NTSC_M  },
    {225,    89900000,  6,    TLG_VSTD_NTSC_M  },
    {226,    90000000,  6,    TLG_VSTD_NTSC_M  },
    {227,    90100000,  6,    TLG_VSTD_NTSC_M  },
    {228,    90200000,  6,    TLG_VSTD_NTSC_M  },
    {229,    90300000,  6,    TLG_VSTD_NTSC_M  },
    {230,    90400000,  6,    TLG_VSTD_NTSC_M  },
    {231,    90500000,  6,    TLG_VSTD_NTSC_M  },
    {232,    90600000,  6,    TLG_VSTD_NTSC_M  },
    {233,    90700000,  6,    TLG_VSTD_NTSC_M  },
    {234,    90800000,  6,    TLG_VSTD_NTSC_M  },
    {235,    90900000,  6,    TLG_VSTD_NTSC_M  },
    {236,    91000000,  6,    TLG_VSTD_NTSC_M  },
    {237,    91100000,  6,    TLG_VSTD_NTSC_M  },
    {238,    91200000,  6,    TLG_VSTD_NTSC_M  },
    {239,    91300000,  6,    TLG_VSTD_NTSC_M  },
    {240,    91400000,  6,    TLG_VSTD_NTSC_M  },
    {241,    91500000,  6,    TLG_VSTD_NTSC_M  },
    {242,    91600000,  6,    TLG_VSTD_NTSC_M  },
    {243,    91700000,  6,    TLG_VSTD_NTSC_M  },
    {244,    91800000,  6,    TLG_VSTD_NTSC_M  },
    {245,    91900000,  6,    TLG_VSTD_NTSC_M  },
    {246,    92000000,  6,    TLG_VSTD_NTSC_M  },
    {247,    92100000,  6,    TLG_VSTD_NTSC_M  },
    {248,    92200000,  6,    TLG_VSTD_NTSC_M  },
    {249,    92300000,  6,    TLG_VSTD_NTSC_M  },
    {250,    92400000,  6,    TLG_VSTD_NTSC_M  },
    {251,    92500000,  6,    TLG_VSTD_NTSC_M  },
    {252,    92600000,  6,    TLG_VSTD_NTSC_M  },
    {253,    92700000,  6,    TLG_VSTD_NTSC_M  },
    {254,    92800000,  6,    TLG_VSTD_NTSC_M  },
    {255,    92900000,  6,    TLG_VSTD_NTSC_M  },
    {256,    93000000,  6,    TLG_VSTD_NTSC_M  },
    {257,    93100000,  6,    TLG_VSTD_NTSC_M  },
    {258,    93200000,  6,    TLG_VSTD_NTSC_M  },
    {259,    93300000,  6,    TLG_VSTD_NTSC_M  },
    {260,    93400000,  6,    TLG_VSTD_NTSC_M  },
    {261,    93500000,  6,    TLG_VSTD_NTSC_M  },
    {262,    93600000,  6,    TLG_VSTD_NTSC_M  },
    {263,    93700000,  6,    TLG_VSTD_NTSC_M  },
    {264,    93800000,  6,    TLG_VSTD_NTSC_M  },
    {265,    93900000,  6,    TLG_VSTD_NTSC_M  },
    {266,    94000000,  6,    TLG_VSTD_NTSC_M  },
    {267,    94100000,  6,    TLG_VSTD_NTSC_M  },
    {268,    94200000,  6,    TLG_VSTD_NTSC_M  },
    {269,    94300000,  6,    TLG_VSTD_NTSC_M  },
    {270,    94400000,  6,    TLG_VSTD_NTSC_M  },
    {271,    94500000,  6,    TLG_VSTD_NTSC_M  },
    {272,    94600000,  6,    TLG_VSTD_NTSC_M  },
    {273,    94700000,  6,    TLG_VSTD_NTSC_M  },
    {274,    94800000,  6,    TLG_VSTD_NTSC_M  },
    {275,    94900000,  6,    TLG_VSTD_NTSC_M  },
    {276,    95000000,  6,    TLG_VSTD_NTSC_M  },
    {277,    95100000,  6,    TLG_VSTD_NTSC_M  },
    {278,    95200000,  6,    TLG_VSTD_NTSC_M  },
    {279,    95300000,  6,    TLG_VSTD_NTSC_M  },
    {280,    95400000,  6,    TLG_VSTD_NTSC_M  },
    {281,    95500000,  6,    TLG_VSTD_NTSC_M  },
    {282,    95600000,  6,    TLG_VSTD_NTSC_M  },
    {283,    95700000,  6,    TLG_VSTD_NTSC_M  },
    {284,    95800000,  6,    TLG_VSTD_NTSC_M  },
    {285,    95900000,  6,    TLG_VSTD_NTSC_M  },
    {286,    96000000,  6,    TLG_VSTD_NTSC_M  },
    {287,    96100000,  6,    TLG_VSTD_NTSC_M  },
    {288,    96200000,  6,    TLG_VSTD_NTSC_M  },
    {289,    96300000,  6,    TLG_VSTD_NTSC_M  },
    {290,    96400000,  6,    TLG_VSTD_NTSC_M  },
    {291,    96500000,  6,    TLG_VSTD_NTSC_M  },
    {292,    96600000,  6,    TLG_VSTD_NTSC_M  },
    {293,    96700000,  6,    TLG_VSTD_NTSC_M  },
    {294,    96800000,  6,    TLG_VSTD_NTSC_M  },
    {295,    96900000,  6,    TLG_VSTD_NTSC_M  },
    {296,    97000000,  6,    TLG_VSTD_NTSC_M  },
    {297,    97100000,  6,    TLG_VSTD_NTSC_M  },
    {298,    97200000,  6,    TLG_VSTD_NTSC_M  },
    {299,    97300000,  6,    TLG_VSTD_NTSC_M  },
    {300,    97400000,  6,    TLG_VSTD_NTSC_M  },
    {301,    97500000,  6,    TLG_VSTD_NTSC_M  },
    {302,    97600000,  6,    TLG_VSTD_NTSC_M  },
    {303,    97700000,  6,    TLG_VSTD_NTSC_M  },
    {304,    97800000,  6,    TLG_VSTD_NTSC_M  },
    {305,    97900000,  6,    TLG_VSTD_NTSC_M  },
    {306,    98000000,  6,    TLG_VSTD_NTSC_M  },
    {307,    98100000,  6,    TLG_VSTD_NTSC_M  },
    {308,    98200000,  6,    TLG_VSTD_NTSC_M  },
    {309,    98300000,  6,    TLG_VSTD_NTSC_M  },
    {310,    98400000,  6,    TLG_VSTD_NTSC_M  },
    {311,    98500000,  6,    TLG_VSTD_NTSC_M  },
    {312,    98600000,  6,    TLG_VSTD_NTSC_M  },
    {313,    98700000,  6,    TLG_VSTD_NTSC_M  },
    {314,    98800000,  6,    TLG_VSTD_NTSC_M  },
    {315,    98900000,  6,    TLG_VSTD_NTSC_M  },
    {316,    99000000,  6,    TLG_VSTD_NTSC_M  },
    {317,    99100000,  6,    TLG_VSTD_NTSC_M  },
    {318,    99200000,  6,    TLG_VSTD_NTSC_M  },
    {319,    99300000,  6,    TLG_VSTD_NTSC_M  },
    {320,    99400000,  6,    TLG_VSTD_NTSC_M  },
    {321,    99500000,  6,    TLG_VSTD_NTSC_M  },
    {322,    99600000,  6,    TLG_VSTD_NTSC_M  },
    {323,    99700000,  6,    TLG_VSTD_NTSC_M  },
    {324,    99800000,  6,    TLG_VSTD_NTSC_M  },
    {325,    99900000,  6,    TLG_VSTD_NTSC_M  },
    {326,   100000000,  6,    TLG_VSTD_NTSC_M  },
    {327,   100100000,  6,    TLG_VSTD_NTSC_M  },
    {328,   100200000,  6,    TLG_VSTD_NTSC_M  },
    {329,   100300000,  6,    TLG_VSTD_NTSC_M  },
    {330,   100400000,  6,    TLG_VSTD_NTSC_M  },
    {331,   100500000,  6,    TLG_VSTD_NTSC_M  },
    {332,   100600000,  6,    TLG_VSTD_NTSC_M  },
    {333,   100700000,  6,    TLG_VSTD_NTSC_M  },
    {334,   100800000,  6,    TLG_VSTD_NTSC_M  },
    {335,   100900000,  6,    TLG_VSTD_NTSC_M  },
    {336,   101000000,  6,    TLG_VSTD_NTSC_M  },
    {337,   101100000,  6,    TLG_VSTD_NTSC_M  },
    {338,   101200000,  6,    TLG_VSTD_NTSC_M  },
    {339,   101300000,  6,    TLG_VSTD_NTSC_M  },
    {340,   101400000,  6,    TLG_VSTD_NTSC_M  },
    {341,   101500000,  6,    TLG_VSTD_NTSC_M  },
    {342,   101600000,  6,    TLG_VSTD_NTSC_M  },
    {343,   101700000,  6,    TLG_VSTD_NTSC_M  },
    {344,   101800000,  6,    TLG_VSTD_NTSC_M  },
    {345,   101900000,  6,    TLG_VSTD_NTSC_M  },
    {346,   102000000,  6,    TLG_VSTD_NTSC_M  },
    {347,   102100000,  6,    TLG_VSTD_NTSC_M  },
    {348,   102200000,  6,    TLG_VSTD_NTSC_M  },
    {349,   102300000,  6,    TLG_VSTD_NTSC_M  },
    {350,   102400000,  6,    TLG_VSTD_NTSC_M  },
    {351,   102500000,  6,    TLG_VSTD_NTSC_M  },
    {352,   102600000,  6,    TLG_VSTD_NTSC_M  },
    {353,   102700000,  6,    TLG_VSTD_NTSC_M  },
    {354,   102800000,  6,    TLG_VSTD_NTSC_M  },
    {355,   102900000,  6,    TLG_VSTD_NTSC_M  },
    {356,   103000000,  6,    TLG_VSTD_NTSC_M  },
    {357,   103100000,  6,    TLG_VSTD_NTSC_M  },
    {358,   103200000,  6,    TLG_VSTD_NTSC_M  },
    {359,   103300000,  6,    TLG_VSTD_NTSC_M  },
    {360,   103400000,  6,    TLG_VSTD_NTSC_M  },
    {361,   103500000,  6,    TLG_VSTD_NTSC_M  },
    {362,   103600000,  6,    TLG_VSTD_NTSC_M  },
    {363,   103700000,  6,    TLG_VSTD_NTSC_M  },
    {364,   103800000,  6,    TLG_VSTD_NTSC_M  },
    {365,   103900000,  6,    TLG_VSTD_NTSC_M  },
    {366,   104000000,  6,    TLG_VSTD_NTSC_M  },
    {367,   104100000,  6,    TLG_VSTD_NTSC_M  },
    {368,   104200000,  6,    TLG_VSTD_NTSC_M  },
    {369,   104300000,  6,    TLG_VSTD_NTSC_M  },
    {370,   104400000,  6,    TLG_VSTD_NTSC_M  },
    {371,   104500000,  6,    TLG_VSTD_NTSC_M  },
    {372,   104600000,  6,    TLG_VSTD_NTSC_M  },
    {373,   104700000,  6,    TLG_VSTD_NTSC_M  },
    {374,   104800000,  6,    TLG_VSTD_NTSC_M  },
    {375,   104900000,  6,    TLG_VSTD_NTSC_M  },
    {376,   105000000,  6,    TLG_VSTD_NTSC_M  },
    {377,   105100000,  6,    TLG_VSTD_NTSC_M  },
    {378,   105200000,  6,    TLG_VSTD_NTSC_M  },
    {379,   105300000,  6,    TLG_VSTD_NTSC_M  },
    {380,   105400000,  6,    TLG_VSTD_NTSC_M  },
    {381,   105500000,  6,    TLG_VSTD_NTSC_M  },
    {382,   105600000,  6,    TLG_VSTD_NTSC_M  },
    {383,   105700000,  6,    TLG_VSTD_NTSC_M  },
    {384,   105800000,  6,    TLG_VSTD_NTSC_M  },
    {385,   105900000,  6,    TLG_VSTD_NTSC_M  },
    {386,   106000000,  6,    TLG_VSTD_NTSC_M  },
    {387,   106100000,  6,    TLG_VSTD_NTSC_M  },
    {388,   106200000,  6,    TLG_VSTD_NTSC_M  },
    {389,   106300000,  6,    TLG_VSTD_NTSC_M  },
    {390,   106400000,  6,    TLG_VSTD_NTSC_M  },
    {391,   106500000,  6,    TLG_VSTD_NTSC_M  },
    {392,   106600000,  6,    TLG_VSTD_NTSC_M  },
    {393,   106700000,  6,    TLG_VSTD_NTSC_M  },
    {394,   106800000,  6,    TLG_VSTD_NTSC_M  },
    {395,   106900000,  6,    TLG_VSTD_NTSC_M  },
    {396,   107000000,  6,    TLG_VSTD_NTSC_M  },
    {397,   107100000,  6,    TLG_VSTD_NTSC_M  },
    {398,   107200000,  6,    TLG_VSTD_NTSC_M  },
    {399,   107300000,  6,    TLG_VSTD_NTSC_M  },
    {400,   107400000,  6,    TLG_VSTD_NTSC_M  },
    {401,   107500000,  6,    TLG_VSTD_NTSC_M  },
    {402,   107600000,  6,    TLG_VSTD_NTSC_M  },
    {403,   107700000,  6,    TLG_VSTD_NTSC_M  },
    {404,   107800000,  6,    TLG_VSTD_NTSC_M  },
    {405,   107900000,  6,    TLG_VSTD_NTSC_M  },
    {406,   108000000,  6,    TLG_VSTD_NTSC_M  },
    {0, 0, 0, 0}
};


tlg_channel_t(tlg_fm_usa_chn_map) = {
    {201,    88100000 ,  6,    TLG_VSTD_NTSC_M  },
    {202,    88300000 ,  6,    TLG_VSTD_NTSC_M  },
    {203,    88500000 ,  6,    TLG_VSTD_NTSC_M  },
    {204,    88700000 ,  6,    TLG_VSTD_NTSC_M  },
    {205,    88900000 ,  6,    TLG_VSTD_NTSC_M  },
    {206,    89100000 ,  6,    TLG_VSTD_NTSC_M  },
    {207,    89300000 ,  6,    TLG_VSTD_NTSC_M  },
    {208,    89500000 ,  6,    TLG_VSTD_NTSC_M  },
    {209,    89700000 ,  6,    TLG_VSTD_NTSC_M  },
    {210,    89900000 ,  6,    TLG_VSTD_NTSC_M  },
    {211,    90100000 ,  6,    TLG_VSTD_NTSC_M  },
    {212,    90300000 ,  6,    TLG_VSTD_NTSC_M  },
    {213,    90500000 ,  6,    TLG_VSTD_NTSC_M  },
    {214,    90700000 ,  6,    TLG_VSTD_NTSC_M  },
    {215,    90900000 ,  6,    TLG_VSTD_NTSC_M  },
    {216,    91100000 ,  6,    TLG_VSTD_NTSC_M  },
    {217,    91300000 ,  6,    TLG_VSTD_NTSC_M  },
    {218,    91500000 ,  6,    TLG_VSTD_NTSC_M  },
    {219,    91700000 ,  6,    TLG_VSTD_NTSC_M  },
    {220,    91900000 ,  6,    TLG_VSTD_NTSC_M  },
    {221,    92100000 ,  6,    TLG_VSTD_NTSC_M  },
    {222,    92300000 ,  6,    TLG_VSTD_NTSC_M  },
    {223,    92500000 ,  6,    TLG_VSTD_NTSC_M  },
    {224,    92700000 ,  6,    TLG_VSTD_NTSC_M  },
    {225,    92900000 ,  6,    TLG_VSTD_NTSC_M  },
    {226,    93100000 ,  6,    TLG_VSTD_NTSC_M  },
    {227,    93300000 ,  6,    TLG_VSTD_NTSC_M  },
    {228,    93500000 ,  6,    TLG_VSTD_NTSC_M  },
    {229,    93700000 ,  6,    TLG_VSTD_NTSC_M  },
    {230,    93900000 ,  6,    TLG_VSTD_NTSC_M  },
    {231,    94100000 ,  6,    TLG_VSTD_NTSC_M  },
    {232,    94300000 ,  6,    TLG_VSTD_NTSC_M  },
    {233,    94500000 ,  6,    TLG_VSTD_NTSC_M  },
    {234,    94700000 ,  6,    TLG_VSTD_NTSC_M  },
    {235,    94900000 ,  6,    TLG_VSTD_NTSC_M  },
    {236,    95100000 ,  6,    TLG_VSTD_NTSC_M  },
    {237,    95300000 ,  6,    TLG_VSTD_NTSC_M  },
    {238,    95500000 ,  6,    TLG_VSTD_NTSC_M  },
    {239,    95700000 ,  6,    TLG_VSTD_NTSC_M  },
    {240,    95900000 ,  6,    TLG_VSTD_NTSC_M  },
    {241,    96100000 ,  6,    TLG_VSTD_NTSC_M  },
    {242,    96300000 ,  6,    TLG_VSTD_NTSC_M  },
    {243,    96500000 ,  6,    TLG_VSTD_NTSC_M  },
    {244,    96700000 ,  6,    TLG_VSTD_NTSC_M  },
    {245,    96900000 ,  6,    TLG_VSTD_NTSC_M  },
    {246,    97100000 ,  6,    TLG_VSTD_NTSC_M  },
    {247,    97300000 ,  6,    TLG_VSTD_NTSC_M  },
    {248,    97500000 ,  6,    TLG_VSTD_NTSC_M  },
    {249,    97700000 ,  6,    TLG_VSTD_NTSC_M  },
    {250,    97900000 ,  6,    TLG_VSTD_NTSC_M  },
    {251,    98100000 ,  6,    TLG_VSTD_NTSC_M  },
    {252,    98300000 ,  6,    TLG_VSTD_NTSC_M  },
    {253,    98500000 ,  6,    TLG_VSTD_NTSC_M  },
    {254,    98700000 ,  6,    TLG_VSTD_NTSC_M  },
    {255,    98900000 ,  6,    TLG_VSTD_NTSC_M  },
    {256,    99100000 ,  6,    TLG_VSTD_NTSC_M  },
    {257,    99300000 ,  6,    TLG_VSTD_NTSC_M  },
    {258,    99500000 ,  6,    TLG_VSTD_NTSC_M  },
    {259,    99700000 ,  6,    TLG_VSTD_NTSC_M  },
    {260,    99900000 ,  6,    TLG_VSTD_NTSC_M  },
    {261,    100100000,  6,    TLG_VSTD_NTSC_M  },
    {262,    100300000,  6,    TLG_VSTD_NTSC_M  },
    {263,    100500000,  6,    TLG_VSTD_NTSC_M  },
    {264,    100700000,  6,    TLG_VSTD_NTSC_M  },
    {265,    100900000,  6,    TLG_VSTD_NTSC_M  },
    {266,    101100000,  6,    TLG_VSTD_NTSC_M  },
    {267,    101300000,  6,    TLG_VSTD_NTSC_M  },
    {268,    101500000,  6,    TLG_VSTD_NTSC_M  },
    {269,    101700000,  6,    TLG_VSTD_NTSC_M  },
    {270,    101900000,  6,    TLG_VSTD_NTSC_M  },
    {271,    102100000,  6,    TLG_VSTD_NTSC_M  },
    {272,    102300000,  6,    TLG_VSTD_NTSC_M  },
    {273,    102500000,  6,    TLG_VSTD_NTSC_M  },
    {274,    102700000,  6,    TLG_VSTD_NTSC_M  },
    {275,    102900000,  6,    TLG_VSTD_NTSC_M  },
    {276,    103100000,  6,    TLG_VSTD_NTSC_M  },
    {277,    103300000,  6,    TLG_VSTD_NTSC_M  },
    {278,    103500000,  6,    TLG_VSTD_NTSC_M  },
    {279,    103700000,  6,    TLG_VSTD_NTSC_M  },
    {280,    103900000,  6,    TLG_VSTD_NTSC_M  },
    {281,    104100000,  6,    TLG_VSTD_NTSC_M  },
    {282,    104300000,  6,    TLG_VSTD_NTSC_M  },
    {283,    104500000,  6,    TLG_VSTD_NTSC_M  },
    {284,    104700000,  6,    TLG_VSTD_NTSC_M  },
    {285,    104900000,  6,    TLG_VSTD_NTSC_M  },
    {286,    105100000,  6,    TLG_VSTD_NTSC_M  },
    {287,    105300000,  6,    TLG_VSTD_NTSC_M  },
    {288,    105500000,  6,    TLG_VSTD_NTSC_M  },
    {289,    105700000,  6,    TLG_VSTD_NTSC_M  },
    {290,    105900000,  6,    TLG_VSTD_NTSC_M  },
    {291,    106100000,  6,    TLG_VSTD_NTSC_M  },
    {292,    106300000,  6,    TLG_VSTD_NTSC_M  },
    {293,    106500000,  6,    TLG_VSTD_NTSC_M  },
    {294,    106700000,  6,    TLG_VSTD_NTSC_M  },
    {295,    106900000,  6,    TLG_VSTD_NTSC_M  },
    {296,    107100000,  6,    TLG_VSTD_NTSC_M  },
    {297,    107300000,  6,    TLG_VSTD_NTSC_M  },
    {298,    107500000,  6,    TLG_VSTD_NTSC_M  },
    {299,    107700000,  6,    TLG_VSTD_NTSC_M  },
    {300,    107900000,  6,    TLG_VSTD_NTSC_M  },
    {0, 0, 0, 0}
};

#ifdef SUPPORT_CHANNEL_MAP_NTSC


tlg_channel_t(tlg_ntsc_chn_map) = {
    {2,      57000000,  6,    TLG_VSTD_NTSC_M  },
    {3,      63000000,  6,    TLG_VSTD_NTSC_M  },
    {4,      69000000,  6,    TLG_VSTD_NTSC_M  },
    {5,      79000000,  6,    TLG_VSTD_NTSC_M  },
    {6,      85000000,  6,    TLG_VSTD_NTSC_M  },
    {7,     177000000,  6,    TLG_VSTD_NTSC_M  },
    {8,     183000000,  6,    TLG_VSTD_NTSC_M  },
    {9,     189000000,  6,    TLG_VSTD_NTSC_M  },
    {10,    195000000,  6,    TLG_VSTD_NTSC_M  },
    {11,    201000000,  6,    TLG_VSTD_NTSC_M  },
    {12,    207000000,  6,    TLG_VSTD_NTSC_M  },
    {13,    213000000,  6,    TLG_VSTD_NTSC_M  },
    {14,    473000000,  6,    TLG_VSTD_NTSC_M  },
    {15,    479000000,  6,    TLG_VSTD_NTSC_M  },
    {16,    485000000,  6,    TLG_VSTD_NTSC_M  },
    {17,    491000000,  6,    TLG_VSTD_NTSC_M  },
    {18,    497000000,  6,    TLG_VSTD_NTSC_M  },
    {19,    503000000,  6,    TLG_VSTD_NTSC_M  },
    {20,    509000000,  6,    TLG_VSTD_NTSC_M  },
    {21,    515000000,  6,    TLG_VSTD_NTSC_M  },
    {22,    521000000,  6,    TLG_VSTD_NTSC_M  },
    {23,    527000000,  6,    TLG_VSTD_NTSC_M  },
    {24,    533000000,  6,    TLG_VSTD_NTSC_M  },
    {25,    539000000,  6,    TLG_VSTD_NTSC_M  },
    {26,    545000000,  6,    TLG_VSTD_NTSC_M  },
    {27,    551000000,  6,    TLG_VSTD_NTSC_M  },
    {28,    557000000,  6,    TLG_VSTD_NTSC_M  },
    {29,    563000000,  6,    TLG_VSTD_NTSC_M  },
    {30,    569000000,  6,    TLG_VSTD_NTSC_M  },
    {31,    575000000,  6,    TLG_VSTD_NTSC_M  },
    {32,    581000000,  6,    TLG_VSTD_NTSC_M  },
    {33,    587000000,  6,    TLG_VSTD_NTSC_M  },
    {34,    593000000,  6,    TLG_VSTD_NTSC_M  },
    {35,    599000000,  6,    TLG_VSTD_NTSC_M  },
    {36,    605000000,  6,    TLG_VSTD_NTSC_M  },
    {37,    611000000,  6,    TLG_VSTD_NTSC_M  },
    {38,    617000000,  6,    TLG_VSTD_NTSC_M  },
    {39,    623000000,  6,    TLG_VSTD_NTSC_M  },
    {40,    629000000,  6,    TLG_VSTD_NTSC_M  },
    {41,    635000000,  6,    TLG_VSTD_NTSC_M  },
    {42,    641000000,  6,    TLG_VSTD_NTSC_M  },
    {43,    647000000,  6,    TLG_VSTD_NTSC_M  },
    {44,    653000000,  6,    TLG_VSTD_NTSC_M  },
    {45,    659000000,  6,    TLG_VSTD_NTSC_M  },
    {46,    665000000,  6,    TLG_VSTD_NTSC_M  },
    {47,    671000000,  6,    TLG_VSTD_NTSC_M  },
    {48,    677000000,  6,    TLG_VSTD_NTSC_M  },
    {49,    683000000,  6,    TLG_VSTD_NTSC_M  },
    {50,    689000000,  6,    TLG_VSTD_NTSC_M  },
    {51,    695000000,  6,    TLG_VSTD_NTSC_M  },
    {52,    701000000,  6,    TLG_VSTD_NTSC_M  },
    {53,    707000000,  6,    TLG_VSTD_NTSC_M  },
    {54,    713000000,  6,    TLG_VSTD_NTSC_M  },
    {55,    719000000,  6,    TLG_VSTD_NTSC_M  },
    {56,    725000000,  6,    TLG_VSTD_NTSC_M  },
    {57,    731000000,  6,    TLG_VSTD_NTSC_M  },
    {58,    737000000,  6,    TLG_VSTD_NTSC_M  },
    {59,    743000000,  6,    TLG_VSTD_NTSC_M  },
    {60,    749000000,  6,    TLG_VSTD_NTSC_M  },
    {61,    755000000,  6,    TLG_VSTD_NTSC_M  },
    {62,    761000000,  6,    TLG_VSTD_NTSC_M  },
    {63,    767000000,  6,    TLG_VSTD_NTSC_M  },
    {64,    773000000,  6,    TLG_VSTD_NTSC_M  },
    {65,    779000000,  6,    TLG_VSTD_NTSC_M  },
    {66,    785000000,  6,    TLG_VSTD_NTSC_M  },
    {67,    791000000,  6,    TLG_VSTD_NTSC_M  },
    {68,    797000000,  6,    TLG_VSTD_NTSC_M  },
    {69,    803000000,  6,    TLG_VSTD_NTSC_M  },
    {0, 0, 0, 0}
};

#endif 

#ifdef SUPPORT_CHANNEL_MAP_CHINA

tlg_channel_t(tlg_china_pald_chn_map) = {
    {1,      52500000,  8,    TLG_VSTD_PAL_D  },
    {2,      60500000,  8,    TLG_VSTD_PAL_D  },
    {3,      68500000,  8,    TLG_VSTD_PAL_D  },
    {4,      80000000,  8,    TLG_VSTD_PAL_D  },
    {5,      88000000,  8,    TLG_VSTD_PAL_D  },
    {6,     171000000,  8,    TLG_VSTD_PAL_D  },
    {7,     179000000,  8,    TLG_VSTD_PAL_D  },
    {8,     187000000,  8,    TLG_VSTD_PAL_D  },
    {9,     195000000,  8,    TLG_VSTD_PAL_D  },
    {10,    203000000,  8,    TLG_VSTD_PAL_D  },
    {11,    211000000,  8,    TLG_VSTD_PAL_D  },
    {12,    219000000,  8,    TLG_VSTD_PAL_D  },
    {13,    474000000,  8,    TLG_VSTD_PAL_D  },
    {14,    482000000,  8,    TLG_VSTD_PAL_D  },
    {15,    490000000,  8,    TLG_VSTD_PAL_D  },
    {16,    498000000,  8,    TLG_VSTD_PAL_D  },
    {17,    506000000,  8,    TLG_VSTD_PAL_D  },
    {18,    514000000,  8,    TLG_VSTD_PAL_D  },
    {19,    522000000,  8,    TLG_VSTD_PAL_D  },
    {20,    530000000,  8,    TLG_VSTD_PAL_D  },
    {21,    538000000,  8,    TLG_VSTD_PAL_D  },
    {22,    546000000,  8,    TLG_VSTD_PAL_D  },
    {23,    554000000,  8,    TLG_VSTD_PAL_D  },
    {24,    562000000,  8,    TLG_VSTD_PAL_D  },
    {25,    610000000,  8,    TLG_VSTD_PAL_D  },
    {26,    618000000,  8,    TLG_VSTD_PAL_D  },
    {27,    626000000,  8,    TLG_VSTD_PAL_D  },
    {28,    634000000,  8,    TLG_VSTD_PAL_D  },
    {29,    642000000,  8,    TLG_VSTD_PAL_D  },
    {30,    650000000,  8,    TLG_VSTD_PAL_D  },
    {31,    658000000,  8,    TLG_VSTD_PAL_D  },
    {32,    666000000,  8,    TLG_VSTD_PAL_D  },
    {33,    674000000,  8,    TLG_VSTD_PAL_D  },
    {34,    682000000,  8,    TLG_VSTD_PAL_D  },
    {35,    690000000,  8,    TLG_VSTD_PAL_D  },
    {36,    698000000,  8,    TLG_VSTD_PAL_D  },
    {37,    706000000,  8,    TLG_VSTD_PAL_D  },
    {38,    714000000,  8,    TLG_VSTD_PAL_D  },
    {39,    722000000,  8,    TLG_VSTD_PAL_D  },
    {40,    730000000,  8,    TLG_VSTD_PAL_D  },
    {41,    738000000,  8,    TLG_VSTD_PAL_D  },
    {42,    746000000,  8,    TLG_VSTD_PAL_D  },
    {43,    754000000,  8,    TLG_VSTD_PAL_D  },
    {44,    762000000,  8,    TLG_VSTD_PAL_D  },
    {45,    770000000,  8,    TLG_VSTD_PAL_D  },
    {46,    778000000,  8,    TLG_VSTD_PAL_D  },
    {47,    786000000,  8,    TLG_VSTD_PAL_D  },
    {48,    794000000,  8,    TLG_VSTD_PAL_D  },
    {49,    802000000,  8,    TLG_VSTD_PAL_D  },
    {50,    810000000,  8,    TLG_VSTD_PAL_D  },
    {51,    818000000,  8,    TLG_VSTD_PAL_D  },
    {52,    826000000,  8,    TLG_VSTD_PAL_D  },
    {53,    834000000,  8,    TLG_VSTD_PAL_D  },
    {54,    842000000,  8,    TLG_VSTD_PAL_D  },
    {55,    850000000,  8,    TLG_VSTD_PAL_D  },
    {56,    858000000,  8,    TLG_VSTD_PAL_D  },
    {57,    866000000,  8,    TLG_VSTD_PAL_D  },
    {58,    874000000,  8,    TLG_VSTD_PAL_D  },
    {59,    882000000,  8,    TLG_VSTD_PAL_D  },
    {60,    890000000,  8,    TLG_VSTD_PAL_D  },
    {61,    898000000,  8,    TLG_VSTD_PAL_D  },
    {62,    906000000,  8,    TLG_VSTD_PAL_D  },
    {63,    914000000,  8,    TLG_VSTD_PAL_D  },
    {64,    922000000,  8,    TLG_VSTD_PAL_D  },
    {65,    930000000,  8,    TLG_VSTD_PAL_D  },
    {66,    938000000,  8,    TLG_VSTD_PAL_D  },
    {67,    946000000,  8,    TLG_VSTD_PAL_D  },
    {68,    954000000,  8,    TLG_VSTD_PAL_D  },
    {69,    962000000,  8,    TLG_VSTD_PAL_D  },
    {0, 0, 0, 0}
};
#endif

#ifdef SUPPORT_CHANNEL_MAP_SHENZHEN

tlg_channel_t(tlg_palI_sz_chn_map) = {
    {1,      52500000,  8,    TLG_VSTD_PAL_I  },
    {2,      60500000,  8,    TLG_VSTD_PAL_I   },
    {3,      68500000,  8,    TLG_VSTD_PAL_I   },
    {4,      80000000,  8,    TLG_VSTD_PAL_I   },
    {5,      88000000,  8,    TLG_VSTD_PAL_I   },
    {6,     171000000,  8,    TLG_VSTD_PAL_I   },
    {7,     179000000,  8,    TLG_VSTD_PAL_I   },
    {8,     187000000,  8,    TLG_VSTD_PAL_I   },
    {9,     195000000,  8,    TLG_VSTD_PAL_I   },
    {10,    203000000,  8,    TLG_VSTD_PAL_I   },
    {11,    211000000,  8,    TLG_VSTD_PAL_I   },
    {12,    219000000,  8,    TLG_VSTD_PAL_I   },
    {13,    474000000,  8,    TLG_VSTD_PAL_I   },
    {14,    482000000,  8,    TLG_VSTD_PAL_I   },
    {15,    490000000,  8,    TLG_VSTD_PAL_I   },
    {16,    498000000,  8,    TLG_VSTD_PAL_I   },
    {17,    506000000,  8,    TLG_VSTD_PAL_I   },
    {18,    514000000,  8,    TLG_VSTD_PAL_I  },
    {19,    522000000,  8,    TLG_VSTD_PAL_I  },
    {20,    530000000,  8,    TLG_VSTD_PAL_I  },
    {21,    538000000,  8,    TLG_VSTD_PAL_I  },
    {22,    546000000,  8,    TLG_VSTD_PAL_I  },
    {23,    554000000,  8,    TLG_VSTD_PAL_I  },
    {24,    562000000,  8,    TLG_VSTD_PAL_I  },
    {25,    610000000,  8,    TLG_VSTD_PAL_I  },
    {26,    618000000,  8,    TLG_VSTD_PAL_I  },
    {27,    626000000,  8,    TLG_VSTD_PAL_I  },
    {28,    634000000,  8,    TLG_VSTD_PAL_I  },
    {29,    642000000,  8,    TLG_VSTD_PAL_I  },
    {30,    650000000,  8,    TLG_VSTD_PAL_I  },
    {31,    658000000,  8,    TLG_VSTD_PAL_I  },
    {32,    666000000,  8,    TLG_VSTD_PAL_I  },
    {33,    674000000,  8,    TLG_VSTD_PAL_I  },
    {34,    682000000,  8,    TLG_VSTD_PAL_I  },
    {35,    690000000,  8,    TLG_VSTD_PAL_I  },
    {36,    698000000,  8,    TLG_VSTD_PAL_I  },
    {37,    706000000,  8,    TLG_VSTD_PAL_I  },
    {38,    714000000,  8,    TLG_VSTD_PAL_I  },
    {39,    722000000,  8,    TLG_VSTD_PAL_I  },
    {40,    730000000,  8,    TLG_VSTD_PAL_I  },
    {41,    738000000,  8,    TLG_VSTD_PAL_I  },
    {42,    746000000,  8,    TLG_VSTD_PAL_I  },
    {43,    754000000,  8,    TLG_VSTD_PAL_I  },
    {44,    762000000,  8,    TLG_VSTD_PAL_I  },
    {45,    770000000,  8,    TLG_VSTD_PAL_I  },
    {46,    778000000,  8,    TLG_VSTD_PAL_I  },
    {47,    786000000,  8,    TLG_VSTD_PAL_I  },
    {48,    794000000,  8,    TLG_VSTD_PAL_I  },
    {49,    802000000,  8,    TLG_VSTD_PAL_I  },
    {50,    810000000,  8,    TLG_VSTD_PAL_I  },
    {51,    818000000,  8,    TLG_VSTD_PAL_I  },
    {52,    826000000,  8,    TLG_VSTD_PAL_I  },
    {53,    834000000,  8,    TLG_VSTD_PAL_I  },
    {54,    842000000,  8,    TLG_VSTD_PAL_I  },
    {55,    850000000,  8,    TLG_VSTD_PAL_I  },
    {56,    858000000,  8,    TLG_VSTD_PAL_I  },
    {57,    866000000,  8,    TLG_VSTD_PAL_I  },
    {58,    874000000,  8,    TLG_VSTD_PAL_I  },
    {59,    882000000,  8,    TLG_VSTD_PAL_I  },
    {60,    890000000,  8,    TLG_VSTD_PAL_I  },
    {61,    898000000,  8,    TLG_VSTD_PAL_I  },
    {62,    906000000,  8,    TLG_VSTD_PAL_I  },
    {63,    914000000,  8,    TLG_VSTD_PAL_I  },
    {64,    922000000,  8,    TLG_VSTD_PAL_I  },
    {65,    930000000,  8,    TLG_VSTD_PAL_I  },
    {66,    938000000,  8,    TLG_VSTD_PAL_I  },
    {67,    946000000,  8,    TLG_VSTD_PAL_I  },
    {68,    954000000,  8,    TLG_VSTD_PAL_I  },
    {69,    962000000,  8,    TLG_VSTD_PAL_I  },
    {0, 0, 0, 0}
};

#endif 

#ifdef SUPPORT_CHANNEL_MAP_VIETNAM


tlg_channel_t(tlg_vietnam_pal_d_k_chn_map) = {
    {2,      52500000,  8,    TLG_VSTD_PAL_D  },
    {3,      62000000,  8,    TLG_VSTD_PAL_D  },
    {4,      80000000,  8,    TLG_VSTD_PAL_D  },
    {5,      96000000,  8,    TLG_VSTD_PAL_D  },
    {6,     178000000,  8,    TLG_VSTD_PAL_D  },
    {7,     186000000,  8,    TLG_VSTD_PAL_D  },
    {8,     194000000,  8,    TLG_VSTD_PAL_D  },
    {9,     202000000,  8,    TLG_VSTD_PAL_D  },
    {10,    210000000,  8,    TLG_VSTD_PAL_D  },
    {11,    218000000,  8,    TLG_VSTD_PAL_D  },
    {12,    226000000,  8,    TLG_VSTD_PAL_D  },
    {21,    474000000,  8,    TLG_VSTD_PAL_K  },
    {22,    482000000,  8,    TLG_VSTD_PAL_K  },
    {23,    490000000,  8,    TLG_VSTD_PAL_K  },
    {24,    498000000,  8,    TLG_VSTD_PAL_K  },
    {25,    506000000,  8,    TLG_VSTD_PAL_K  },
    {26,    514000000,  8,    TLG_VSTD_PAL_K  },
    {27,    522000000,  8,    TLG_VSTD_PAL_K  },
    {28,    530000000,  8,    TLG_VSTD_PAL_K  },
    {29,    538000000,  8,    TLG_VSTD_PAL_K  },
    {30,    546000000,  8,    TLG_VSTD_PAL_K  },
    {31,    554000000,  8,    TLG_VSTD_PAL_K  },
    {32,    562000000,  8,    TLG_VSTD_PAL_K  },
    {33,    570000000,  8,    TLG_VSTD_PAL_K  },
    {34,    578000000,  8,    TLG_VSTD_PAL_K  },
    {35,    586000000,  8,    TLG_VSTD_PAL_K  },
    {36,    594000000,  8,    TLG_VSTD_PAL_K  },
    {37,    602000000,  8,    TLG_VSTD_PAL_K  },
    {38,    610000000,  8,    TLG_VSTD_PAL_K  },
    {39,    618000000,  8,    TLG_VSTD_PAL_K  },
    {40,    626000000,  8,    TLG_VSTD_PAL_K  },
    {41,    634000000,  8,    TLG_VSTD_PAL_K  },
    {42,    642000000,  8,    TLG_VSTD_PAL_K  },
    {43,    650000000,  8,    TLG_VSTD_PAL_K  },
    {44,    658000000,  8,    TLG_VSTD_PAL_K  },
    {45,    666000000,  8,    TLG_VSTD_PAL_K  },
    {46,    674000000,  8,    TLG_VSTD_PAL_K  },
    {47,    682000000,  8,    TLG_VSTD_PAL_K  },
    {48,    690000000,  8,    TLG_VSTD_PAL_K  },
    {49,    698000000,  8,    TLG_VSTD_PAL_K  },
    {50,    706000000,  8,    TLG_VSTD_PAL_K  },
    {51,    714000000,  8,    TLG_VSTD_PAL_K  },
    {52,    722000000,  8,    TLG_VSTD_PAL_K  },
    {53,    730000000,  8,    TLG_VSTD_PAL_K  },
    {54,    738000000,  8,    TLG_VSTD_PAL_K  },
    {55,    746000000,  8,    TLG_VSTD_PAL_K  },
    {56,    754000000,  8,    TLG_VSTD_PAL_K  },
    {57,    762000000,  8,    TLG_VSTD_PAL_K  },
    {58,    770000000,  8,    TLG_VSTD_PAL_K  },
    {59,    778000000,  8,    TLG_VSTD_PAL_K  },
    {60,    786000000,  8,    TLG_VSTD_PAL_K  },
    {61,    794000000,  8,    TLG_VSTD_PAL_K  },
    {62,    802000000,  8,    TLG_VSTD_PAL_K  },
    {63,    810000000,  8,    TLG_VSTD_PAL_K  },
    {64,    818000000,  8,    TLG_VSTD_PAL_K  },
    {65,    826000000,  8,    TLG_VSTD_PAL_K  },
    {66,    834000000,  8,    TLG_VSTD_PAL_K  },
    {67,    842000000,  8,    TLG_VSTD_PAL_K  },
    {68,    850000000,  8,    TLG_VSTD_PAL_K  },
    {69,    858000000,  8,    TLG_VSTD_PAL_K  },
    {0, 0, 0, 0}
};
#endif 

#ifdef SUPPORT_CHANNEL_MAP_PALWE

tlg_channel_t(tlg_pal_b_g_chn_map) = {
    {2,      50500000,  7,    TLG_VSTD_PAL_B  },
    {3,      57500000,  7,    TLG_VSTD_PAL_B  },
    {4,      64500000,  7,    TLG_VSTD_PAL_B  },
    {5,     177500000,  7,    TLG_VSTD_PAL_B  },
    {6,     184500000,  7,    TLG_VSTD_PAL_B  },
    {7,     191500000,  7,    TLG_VSTD_PAL_B  },
    {8,     198500000,  7,    TLG_VSTD_PAL_B  },
    {9,     205500000,  7,    TLG_VSTD_PAL_B  },
    {10,    212500000,  7,    TLG_VSTD_PAL_B  },
    {11,    219500000,  7,    TLG_VSTD_PAL_B  },
    {12,    226500000,  7,    TLG_VSTD_PAL_B  },
    {21,    474000000,  8,    TLG_VSTD_PAL_G  },
    {22,    482000000,  8,    TLG_VSTD_PAL_G  },
    {23,    490000000,  8,    TLG_VSTD_PAL_G  },
    {24,    498000000,  8,    TLG_VSTD_PAL_G  },
    {25,    506000000,  8,    TLG_VSTD_PAL_G  },
    {26,    514000000,  8,    TLG_VSTD_PAL_G  },
    {27,    522000000,  8,    TLG_VSTD_PAL_G  },
    {28,    530000000,  8,    TLG_VSTD_PAL_G  },
    {29,    538000000,  8,    TLG_VSTD_PAL_G  },
    {30,    546000000,  8,    TLG_VSTD_PAL_G  },
    {31,    554000000,  8,    TLG_VSTD_PAL_G  },
    {32,    562000000,  8,    TLG_VSTD_PAL_G  },
    {33,    570000000,  8,    TLG_VSTD_PAL_G  },
    {34,    578000000,  8,    TLG_VSTD_PAL_G  },
    {35,    586000000,  8,    TLG_VSTD_PAL_G  },
    {36,    594000000,  8,    TLG_VSTD_PAL_G  },
    {37,    602000000,  8,    TLG_VSTD_PAL_G  },
    {38,    610000000,  8,    TLG_VSTD_PAL_G  },
    {39,    618000000,  8,    TLG_VSTD_PAL_G  },
    {40,    626000000,  8,    TLG_VSTD_PAL_G  },
    {41,    634000000,  8,    TLG_VSTD_PAL_G  },
    {42,    642000000,  8,    TLG_VSTD_PAL_G  },
    {43,    650000000,  8,    TLG_VSTD_PAL_G  },
    {44,    658000000,  8,    TLG_VSTD_PAL_G  },
    {45,    666000000,  8,    TLG_VSTD_PAL_G  },
    {46,    674000000,  8,    TLG_VSTD_PAL_G  },
    {47,    682000000,  8,    TLG_VSTD_PAL_G  },
    {48,    690000000,  8,    TLG_VSTD_PAL_G  },
    {49,    698000000,  8,    TLG_VSTD_PAL_G  },
    {50,    706000000,  8,    TLG_VSTD_PAL_G  },
    {51,    714000000,  8,    TLG_VSTD_PAL_G  },
    {52,    722000000,  8,    TLG_VSTD_PAL_G  },
    {53,    730000000,  8,    TLG_VSTD_PAL_G  },
    {54,    738000000,  8,    TLG_VSTD_PAL_G  },
    {55,    746000000,  8,    TLG_VSTD_PAL_G  },
    {56,    754000000,  8,    TLG_VSTD_PAL_G  },
    {57,    762000000,  8,    TLG_VSTD_PAL_G  },
    {58,    770000000,  8,    TLG_VSTD_PAL_G  },
    {59,    778000000,  8,    TLG_VSTD_PAL_G  },
    {60,    786000000,  8,    TLG_VSTD_PAL_G  },
    {61,    794000000,  8,    TLG_VSTD_PAL_G  },
    {62,    802000000,  8,    TLG_VSTD_PAL_G  },
    {63,    810000000,  8,    TLG_VSTD_PAL_G  },
    {64,    818000000,  8,    TLG_VSTD_PAL_G  },
    {65,    826000000,  8,    TLG_VSTD_PAL_G  },
    {66,    834000000,  8,    TLG_VSTD_PAL_G  },
    {67,    842000000,  8,    TLG_VSTD_PAL_G  },
    {68,    850000000,  8,    TLG_VSTD_PAL_G  },
    {69,    858000000,  8,    TLG_VSTD_PAL_G  },
    {0, 0, 0, 0}
};

tlg_channel_t(tlg_pal_b_g_hybrid_chn_map) = {
    {2,      50500000,  7,    TLG_VSTD_PAL_SECAM_B  },
    {3,      57500000,  7,    TLG_VSTD_PAL_SECAM_B  },
    {4,      64500000,  7,    TLG_VSTD_PAL_SECAM_B  },
    {5,     177500000,  7,    TLG_VSTD_PAL_SECAM_B  },
    {6,     184500000,  7,    TLG_VSTD_PAL_SECAM_B  },
    {7,     191500000,  7,    TLG_VSTD_PAL_SECAM_B  },
    {8,     198500000,  7,    TLG_VSTD_PAL_SECAM_B  },
    {9,     205500000,  7,    TLG_VSTD_PAL_SECAM_B  },
    {10,    212500000,  7,    TLG_VSTD_PAL_SECAM_B  },
    {11,    219500000,  7,    TLG_VSTD_PAL_SECAM_B  },
    {12,    226500000,  7,    TLG_VSTD_PAL_SECAM_B  },
    {21,    474000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {22,    482000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {23,    490000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {24,    498000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {25,    506000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {26,    514000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {27,    522000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {28,    530000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {29,    538000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {30,    546000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {31,    554000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {32,    562000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {33,    570000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {34,    578000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {35,    586000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {36,    594000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {37,    602000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {38,    610000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {39,    618000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {40,    626000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {41,    634000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {42,    642000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {43,    650000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {44,    658000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {45,    666000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {46,    674000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {47,    682000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {48,    690000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {49,    698000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {50,    706000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {51,    714000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {52,    722000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {53,    730000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {54,    738000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {55,    746000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {56,    754000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {57,    762000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {58,    770000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {59,    778000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {60,    786000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {61,    794000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {62,    802000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {63,    810000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {64,    818000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {65,    826000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {66,    834000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {67,    842000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {68,    850000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {69,    858000000,  8,    TLG_VSTD_PAL_SECAM_G  },
    {0, 0, 0, 0}
};


#endif 

#ifdef SUPPORT_CHANNEL_MAP_JAPAN
tlg_channel_t(tlg_ntsc_japan_chn_map) = {
    {1,      93000000,  6,    TLG_VSTD_NTSC_M  },
    {2,      99000000,  6,    TLG_VSTD_NTSC_M  },
    {3,     105000000,  6,    TLG_VSTD_NTSC_M  },
    {4,     173000000,  6,    TLG_VSTD_NTSC_M  },
    {5,     179000000,  6,    TLG_VSTD_NTSC_M  },
    {6,     185000000,  6,    TLG_VSTD_NTSC_M  },
    {7,     191000000,  6,    TLG_VSTD_NTSC_M  },
    {8,     195000000,  6,    TLG_VSTD_NTSC_M  },
    {9,     201000000,  6,    TLG_VSTD_NTSC_M  },
    {10,    207000000,  6,    TLG_VSTD_NTSC_M  },
    {11,    213000000,  6,    TLG_VSTD_NTSC_M  },
    {12,    219000000,  6,    TLG_VSTD_NTSC_M  },
    {13,    473000000,  6,    TLG_VSTD_NTSC_M  },
    {14,    479000000,  6,    TLG_VSTD_NTSC_M  },
    {15,    485000000,  6,    TLG_VSTD_NTSC_M  },
    {16,    491000000,  6,    TLG_VSTD_NTSC_M  },
    {17,    497000000,  6,    TLG_VSTD_NTSC_M  },
    {18,    503000000,  6,    TLG_VSTD_NTSC_M  },
    {19,    509000000,  6,    TLG_VSTD_NTSC_M  },
    {20,    515000000,  6,    TLG_VSTD_NTSC_M  },
    {21,    521000000,  6,    TLG_VSTD_NTSC_M  },
    {22,    527000000,  6,    TLG_VSTD_NTSC_M  },
    {23,    533000000,  6,    TLG_VSTD_NTSC_M  },
    {24,    539000000,  6,    TLG_VSTD_NTSC_M  },
    {25,    545000000,  6,    TLG_VSTD_NTSC_M  },
    {26,    551000000,  6,    TLG_VSTD_NTSC_M  },
    {27,    557000000,  6,    TLG_VSTD_NTSC_M  },
    {28,    563000000,  6,    TLG_VSTD_NTSC_M  },
    {29,    569000000,  6,    TLG_VSTD_NTSC_M  },
    {30,    575000000,  6,    TLG_VSTD_NTSC_M  },
    {31,    581000000,  6,    TLG_VSTD_NTSC_M  },
    {32,    587000000,  6,    TLG_VSTD_NTSC_M  },
    {33,    593000000,  6,    TLG_VSTD_NTSC_M  },
    {34,    599000000,  6,    TLG_VSTD_NTSC_M  },
    {35,    605000000,  6,    TLG_VSTD_NTSC_M  },
    {36,    611000000,  6,    TLG_VSTD_NTSC_M  },
    {37,    617000000,  6,    TLG_VSTD_NTSC_M  },
    {38,    623000000,  6,    TLG_VSTD_NTSC_M  },
    {39,    629000000,  6,    TLG_VSTD_NTSC_M  },
    {40,    635000000,  6,    TLG_VSTD_NTSC_M  },
    {41,    641000000,  6,    TLG_VSTD_NTSC_M  },
    {42,    647000000,  6,    TLG_VSTD_NTSC_M  },
    {43,    653000000,  6,    TLG_VSTD_NTSC_M  },
    {44,    659000000,  6,    TLG_VSTD_NTSC_M  },
    {45,    665000000,  6,    TLG_VSTD_NTSC_M  },
    {46,    671000000,  6,    TLG_VSTD_NTSC_M  },
    {47,    677000000,  6,    TLG_VSTD_NTSC_M  },
    {48,    683000000,  6,    TLG_VSTD_NTSC_M  },
    {49,    689000000,  6,    TLG_VSTD_NTSC_M  },
    {50,    695000000,  6,    TLG_VSTD_NTSC_M  },
    {51,    701000000,  6,    TLG_VSTD_NTSC_M  },
    {52,    707000000,  6,    TLG_VSTD_NTSC_M  },
    {53,    713000000,  6,    TLG_VSTD_NTSC_M  },
    {54,    719000000,  6,    TLG_VSTD_NTSC_M  },
    {55,    725000000,  6,    TLG_VSTD_NTSC_M  },
    {56,    731000000,  6,    TLG_VSTD_NTSC_M  },
    {57,    737000000,  6,    TLG_VSTD_NTSC_M  },
    {58,    743000000,  6,    TLG_VSTD_NTSC_M  },
    {59,    749000000,  6,    TLG_VSTD_NTSC_M  },
    {60,    755000000,  6,    TLG_VSTD_NTSC_M  },
    {61,    761000000,  6,    TLG_VSTD_NTSC_M  },
    {62,    767000000,  6,    TLG_VSTD_NTSC_M  },
    {0, 0, 0, 0}
};
#endif 

#ifdef SUPPORT_CHANNEL_MAP_UK

tlg_channel_t(tlg_pal_i_chn_map) = {
    {2,      48500000,  8,    TLG_VSTD_PAL_I  },
    {3,      56500000,  8,    TLG_VSTD_PAL_I  },
    {4,      64500000,  8,    TLG_VSTD_PAL_I  },
    {5,     178000000,  8,    TLG_VSTD_PAL_I  },
    {6,     186000000,  8,    TLG_VSTD_PAL_I  },
    {7,     194000000,  8,    TLG_VSTD_PAL_I  },
    {8,     202000000,  8,    TLG_VSTD_PAL_I  },
    {9,     210000000,  8,    TLG_VSTD_PAL_I  },
    {10,    218000000,  8,    TLG_VSTD_PAL_I  },
    {11,    226000000,  8,    TLG_VSTD_PAL_I  },
    {12,    234000000,  8,    TLG_VSTD_PAL_I  },
    {13,    242000000,  8,    TLG_VSTD_PAL_I  },
    {21,    474000000,  8,    TLG_VSTD_PAL_I  },
    {22,    482000000,  8,    TLG_VSTD_PAL_I  },
    {23,    490000000,  8,    TLG_VSTD_PAL_I  },
    {24,    498000000,  8,    TLG_VSTD_PAL_I  },
    {25,    506000000,  8,    TLG_VSTD_PAL_I  },
    {26,    514000000,  8,    TLG_VSTD_PAL_I  },
    {27,    522000000,  8,    TLG_VSTD_PAL_I  },
    {28,    530000000,  8,    TLG_VSTD_PAL_I  },
    {29,    538000000,  8,    TLG_VSTD_PAL_I  },
    {30,    546000000,  8,    TLG_VSTD_PAL_I  },
    {31,    554000000,  8,    TLG_VSTD_PAL_I  },
    {32,    562000000,  8,    TLG_VSTD_PAL_I  },
    {33,    570000000,  8,    TLG_VSTD_PAL_I  },
    {34,    578000000,  8,    TLG_VSTD_PAL_I  },
    {35,    586000000,  8,    TLG_VSTD_PAL_I  },
    {36,    594000000,  8,    TLG_VSTD_PAL_I  },
    {37,    602000000,  8,    TLG_VSTD_PAL_I  },
    {38,    610000000,  8,    TLG_VSTD_PAL_I  },
    {39,    618000000,  8,    TLG_VSTD_PAL_I  },
    {40,    626000000,  8,    TLG_VSTD_PAL_I  },
    {41,    634000000,  8,    TLG_VSTD_PAL_I  },
    {42,    642000000,  8,    TLG_VSTD_PAL_I  },
    {43,    650000000,  8,    TLG_VSTD_PAL_I  },
    {44,    658000000,  8,    TLG_VSTD_PAL_I  },
    {45,    666000000,  8,    TLG_VSTD_PAL_I  },
    {46,    674000000,  8,    TLG_VSTD_PAL_I  },
    {47,    682000000,  8,    TLG_VSTD_PAL_I  },
    {48,    690000000,  8,    TLG_VSTD_PAL_I  },
    {49,    698000000,  8,    TLG_VSTD_PAL_I  },
    {50,    706000000,  8,    TLG_VSTD_PAL_I  },
    {51,    714000000,  8,    TLG_VSTD_PAL_I  },
    {52,    722000000,  8,    TLG_VSTD_PAL_I  },
    {53,    730000000,  8,    TLG_VSTD_PAL_I  },
    {54,    738000000,  8,    TLG_VSTD_PAL_I  },
    {55,    746000000,  8,    TLG_VSTD_PAL_I  },
    {56,    754000000,  8,    TLG_VSTD_PAL_I  },
    {57,    762000000,  8,    TLG_VSTD_PAL_I  },
    {58,    770000000,  8,    TLG_VSTD_PAL_I  },
    {59,    778000000,  8,    TLG_VSTD_PAL_I  },
    {60,    786000000,  8,    TLG_VSTD_PAL_I  },
    {61,    794000000,  8,    TLG_VSTD_PAL_I  },
    {62,    802000000,  8,    TLG_VSTD_PAL_I  },
    {63,    810000000,  8,    TLG_VSTD_PAL_I  },
    {64,    818000000,  8,    TLG_VSTD_PAL_I  },
    {65,    826000000,  8,    TLG_VSTD_PAL_I  },
    {66,    834000000,  8,    TLG_VSTD_PAL_I  },
    {67,    842000000,  8,    TLG_VSTD_PAL_I  },
    {68,    850000000,  8,    TLG_VSTD_PAL_I  },
    {69,    858000000,  8,    TLG_VSTD_PAL_I  },
    {0, 0, 0, 0}
};

#endif 


#ifdef SUPPORT_CHANNEL_MAP_BRAZIL
tlg_channel_t(tlg_brazil_pal_m_chn_map) = {
    {2,      57000000,  6,    TLG_VSTD_PAL_M  },
    {3,      63000000,  6,    TLG_VSTD_PAL_M  },
    {4,      69000000,  6,    TLG_VSTD_PAL_M  },
    {5,      79000000,  6,    TLG_VSTD_PAL_M  },
    {6,      85000000,  6,    TLG_VSTD_PAL_M  },
    {7,     177000000,  6,    TLG_VSTD_PAL_M  },
    {8,     183000000,  6,    TLG_VSTD_PAL_M  },
    {9,     189000000,  6,    TLG_VSTD_PAL_M  },
    {10,    195000000,  6,    TLG_VSTD_PAL_M  },
    {11,    201000000,  6,    TLG_VSTD_PAL_M  },
    {12,    207000000,  6,    TLG_VSTD_PAL_M  },
    {13,    213000000,  6,    TLG_VSTD_PAL_M  },
    {14,    473000000,  6,    TLG_VSTD_PAL_M  },
    {15,    479000000,  6,    TLG_VSTD_PAL_M  },
    {16,    485000000,  6,    TLG_VSTD_PAL_M  },
    {17,    491000000,  6,    TLG_VSTD_PAL_M  },
    {18,    497000000,  6,    TLG_VSTD_PAL_M  },
    {19,    503000000,  6,    TLG_VSTD_PAL_M  },
    {20,    509000000,  6,    TLG_VSTD_PAL_M  },
    {21,    515000000,  6,    TLG_VSTD_PAL_M  },
    {22,    521000000,  6,    TLG_VSTD_PAL_M  },
    {23,    527000000,  6,    TLG_VSTD_PAL_M  },
    {24,    533000000,  6,    TLG_VSTD_PAL_M  },
    {25,    539000000,  6,    TLG_VSTD_PAL_M  },
    {26,    545000000,  6,    TLG_VSTD_PAL_M  },
    {27,    551000000,  6,    TLG_VSTD_PAL_M  },
    {28,    557000000,  6,    TLG_VSTD_PAL_M  },
    {29,    563000000,  6,    TLG_VSTD_PAL_M  },
    {30,    569000000,  6,    TLG_VSTD_PAL_M  },
    {31,    575000000,  6,    TLG_VSTD_PAL_M  },
    {32,    581000000,  6,    TLG_VSTD_PAL_M  },
    {33,    587000000,  6,    TLG_VSTD_PAL_M  },
    {34,    593000000,  6,    TLG_VSTD_PAL_M  },
    {35,    599000000,  6,    TLG_VSTD_PAL_M  },
    {36,    605000000,  6,    TLG_VSTD_PAL_M  },
    {37,    611000000,  6,    TLG_VSTD_PAL_M  },
    {38,    617000000,  6,    TLG_VSTD_PAL_M  },
    {39,    623000000,  6,    TLG_VSTD_PAL_M  },
    {40,    629000000,  6,    TLG_VSTD_PAL_M  },
    {41,    635000000,  6,    TLG_VSTD_PAL_M  },
    {42,    641000000,  6,    TLG_VSTD_PAL_M  },
    {43,    647000000,  6,    TLG_VSTD_PAL_M  },
    {44,    653000000,  6,    TLG_VSTD_PAL_M  },
    {45,    659000000,  6,    TLG_VSTD_PAL_M  },
    {46,    665000000,  6,    TLG_VSTD_PAL_M  },
    {47,    671000000,  6,    TLG_VSTD_PAL_M  },
    {48,    677000000,  6,    TLG_VSTD_PAL_M  },
    {49,    683000000,  6,    TLG_VSTD_PAL_M  },
    {50,    689000000,  6,    TLG_VSTD_PAL_M  },
    {51,    695000000,  6,    TLG_VSTD_PAL_M  },
    {52,    701000000,  6,    TLG_VSTD_PAL_M  },
    {53,    707000000,  6,    TLG_VSTD_PAL_M  },
    {54,    713000000,  6,    TLG_VSTD_PAL_M  },
    {55,    719000000,  6,    TLG_VSTD_PAL_M  },
    {56,    725000000,  6,    TLG_VSTD_PAL_M  },
    {57,    731000000,  6,    TLG_VSTD_PAL_M  },
    {58,    737000000,  6,    TLG_VSTD_PAL_M  },
    {59,    743000000,  6,    TLG_VSTD_PAL_M  },
    {60,    749000000,  6,    TLG_VSTD_PAL_M  },
    {61,    755000000,  6,    TLG_VSTD_PAL_M  },
    {62,    761000000,  6,    TLG_VSTD_PAL_M  },
    {63,    767000000,  6,    TLG_VSTD_PAL_M  },
    {64,    773000000,  6,    TLG_VSTD_PAL_M  },
    {65,    779000000,  6,    TLG_VSTD_PAL_M  },
    {66,    785000000,  6,    TLG_VSTD_PAL_M  },
    {67,    791000000,  6,    TLG_VSTD_PAL_M  },
    {68,    797000000,  6,    TLG_VSTD_PAL_M  },
    {69,    803000000,  6,    TLG_VSTD_PAL_M  },
    {0, 0, 0, 0}
};
#endif 


#ifdef SUPPORT_CHANNEL_MAP_ARGENTINA
tlg_channel_t(tlg_argentina_pal_nc_chn_map) = {
    {2,      57000000,  6,    TLG_VSTD_PAL_NC  },
    {3,      63000000,  6,    TLG_VSTD_PAL_NC  },
    {4,      69000000,  6,    TLG_VSTD_PAL_NC  },
    {5,      79000000,  6,    TLG_VSTD_PAL_NC  },
    {6,      85000000,  6,    TLG_VSTD_PAL_NC  },
    {7,     177000000,  6,    TLG_VSTD_PAL_NC  },
    {8,     183000000,  6,    TLG_VSTD_PAL_NC  },
    {9,     189000000,  6,    TLG_VSTD_PAL_NC  },
    {10,    195000000,  6,    TLG_VSTD_PAL_NC  },
    {11,    201000000,  6,    TLG_VSTD_PAL_NC  },
    {12,    207000000,  6,    TLG_VSTD_PAL_NC  },
    {13,    213000000,  6,    TLG_VSTD_PAL_NC  },
    {14,    473000000,  6,    TLG_VSTD_PAL_NC  },
    {15,    479000000,  6,    TLG_VSTD_PAL_NC  },
    {16,    485000000,  6,    TLG_VSTD_PAL_NC  },
    {17,    491000000,  6,    TLG_VSTD_PAL_NC  },
    {18,    497000000,  6,    TLG_VSTD_PAL_NC  },
    {19,    503000000,  6,    TLG_VSTD_PAL_NC  },
    {20,    509000000,  6,    TLG_VSTD_PAL_NC  },
    {21,    515000000,  6,    TLG_VSTD_PAL_NC  },
    {22,    521000000,  6,    TLG_VSTD_PAL_NC  },
    {23,    527000000,  6,    TLG_VSTD_PAL_NC  },
    {24,    533000000,  6,    TLG_VSTD_PAL_NC  },
    {25,    539000000,  6,    TLG_VSTD_PAL_NC  },
    {26,    545000000,  6,    TLG_VSTD_PAL_NC  },
    {27,    551000000,  6,    TLG_VSTD_PAL_NC  },
    {28,    557000000,  6,    TLG_VSTD_PAL_NC  },
    {29,    563000000,  6,    TLG_VSTD_PAL_NC  },
    {30,    569000000,  6,    TLG_VSTD_PAL_NC  },
    {31,    575000000,  6,    TLG_VSTD_PAL_NC  },
    {32,    581000000,  6,    TLG_VSTD_PAL_NC  },
    {33,    587000000,  6,    TLG_VSTD_PAL_NC  },
    {34,    593000000,  6,    TLG_VSTD_PAL_NC  },
    {35,    599000000,  6,    TLG_VSTD_PAL_NC  },
    {36,    605000000,  6,    TLG_VSTD_PAL_NC  },
    {37,    611000000,  6,    TLG_VSTD_PAL_NC  },
    {38,    617000000,  6,    TLG_VSTD_PAL_NC  },
    {39,    623000000,  6,    TLG_VSTD_PAL_NC  },
    {40,    629000000,  6,    TLG_VSTD_PAL_NC  },
    {41,    635000000,  6,    TLG_VSTD_PAL_NC  },
    {42,    641000000,  6,    TLG_VSTD_PAL_NC  },
    {43,    647000000,  6,    TLG_VSTD_PAL_NC  },
    {44,    653000000,  6,    TLG_VSTD_PAL_NC  },
    {45,    659000000,  6,    TLG_VSTD_PAL_NC  },
    {46,    665000000,  6,    TLG_VSTD_PAL_NC  },
    {47,    671000000,  6,    TLG_VSTD_PAL_NC  },
    {48,    677000000,  6,    TLG_VSTD_PAL_NC  },
    {49,    683000000,  6,    TLG_VSTD_PAL_NC  },
    {50,    689000000,  6,    TLG_VSTD_PAL_NC  },
    {51,    695000000,  6,    TLG_VSTD_PAL_NC  },
    {52,    701000000,  6,    TLG_VSTD_PAL_NC  },
    {53,    707000000,  6,    TLG_VSTD_PAL_NC  },
    {54,    713000000,  6,    TLG_VSTD_PAL_NC  },
    {55,    719000000,  6,    TLG_VSTD_PAL_NC  },
    {56,    725000000,  6,    TLG_VSTD_PAL_NC  },
    {57,    731000000,  6,    TLG_VSTD_PAL_NC  },
    {58,    737000000,  6,    TLG_VSTD_PAL_NC  },
    {59,    743000000,  6,    TLG_VSTD_PAL_NC  },
    {60,    749000000,  6,    TLG_VSTD_PAL_NC  },
    {61,    755000000,  6,    TLG_VSTD_PAL_NC  },
    {62,    761000000,  6,    TLG_VSTD_PAL_NC  },
    {63,    767000000,  6,    TLG_VSTD_PAL_NC  },
    {64,    773000000,  6,    TLG_VSTD_PAL_NC  },
    {65,    779000000,  6,    TLG_VSTD_PAL_NC  },
    {66,    785000000,  6,    TLG_VSTD_PAL_NC  },
    {67,    791000000,  6,    TLG_VSTD_PAL_NC  },
    {68,    797000000,  6,    TLG_VSTD_PAL_NC  },
    {69,    803000000,  6,    TLG_VSTD_PAL_NC  },
    {0, 0, 0, 0}
};
#endif 

#ifdef SUPPORT_CHANNEL_MAP_SECAM_BG

tlg_channel_t(tlg_secam_b_g_chn_map) = {
    {2,      50500000,  7,    TLG_VSTD_SECAM_B  },
    {3,      57500000,  7,    TLG_VSTD_SECAM_B  },
    {4,      64500000,  7,    TLG_VSTD_SECAM_B  },
    {5,     177500000,  7,    TLG_VSTD_SECAM_B  },
    {6,     184500000,  7,    TLG_VSTD_SECAM_B  },
    {7,     191500000,  7,    TLG_VSTD_SECAM_B  },
    {8,     198500000,  7,    TLG_VSTD_SECAM_B  },
    {9,     205500000,  7,    TLG_VSTD_SECAM_B  },
    {10,    212500000,  7,    TLG_VSTD_SECAM_B  },
    {11,    219500000,  7,    TLG_VSTD_SECAM_B  },
    {12,    226500000,  7,    TLG_VSTD_SECAM_B  },
    {21,    474000000,  8,    TLG_VSTD_SECAM_G  },
    {22,    482000000,  8,    TLG_VSTD_SECAM_G  },
    {23,    490000000,  8,    TLG_VSTD_SECAM_G  },
    {24,    498000000,  8,    TLG_VSTD_SECAM_G  },
    {25,    506000000,  8,    TLG_VSTD_SECAM_G  },
    {26,    514000000,  8,    TLG_VSTD_SECAM_G  },
    {27,    522000000,  8,    TLG_VSTD_SECAM_G  },
    {28,    530000000,  8,    TLG_VSTD_SECAM_G  },
    {29,    538000000,  8,    TLG_VSTD_SECAM_G  },
    {30,    546000000,  8,    TLG_VSTD_SECAM_G  },
    {31,    554000000,  8,    TLG_VSTD_SECAM_G  },
    {32,    562000000,  8,    TLG_VSTD_SECAM_G  },
    {33,    570000000,  8,    TLG_VSTD_SECAM_G  },
    {34,    578000000,  8,    TLG_VSTD_SECAM_G  },
    {35,    586000000,  8,    TLG_VSTD_SECAM_G  },
    {36,    594000000,  8,    TLG_VSTD_SECAM_G  },
    {37,    602000000,  8,    TLG_VSTD_SECAM_G  },
    {38,    610000000,  8,    TLG_VSTD_SECAM_G  },
    {39,    618000000,  8,    TLG_VSTD_SECAM_G  },
    {40,    626000000,  8,    TLG_VSTD_SECAM_G  },
    {41,    634000000,  8,    TLG_VSTD_SECAM_G  },
    {42,    642000000,  8,    TLG_VSTD_SECAM_G  },
    {43,    650000000,  8,    TLG_VSTD_SECAM_G  },
    {44,    658000000,  8,    TLG_VSTD_SECAM_G  },
    {45,    666000000,  8,    TLG_VSTD_SECAM_G  },
    {46,    674000000,  8,    TLG_VSTD_SECAM_G  },
    {47,    682000000,  8,    TLG_VSTD_SECAM_G  },
    {48,    690000000,  8,    TLG_VSTD_SECAM_G  },
    {49,    698000000,  8,    TLG_VSTD_SECAM_G  },
    {50,    706000000,  8,    TLG_VSTD_SECAM_G  },
    {51,    714000000,  8,    TLG_VSTD_SECAM_G  },
    {52,    722000000,  8,    TLG_VSTD_SECAM_G  },
    {53,    730000000,  8,    TLG_VSTD_SECAM_G  },
    {54,    738000000,  8,    TLG_VSTD_SECAM_G  },
    {55,    746000000,  8,    TLG_VSTD_SECAM_G  },
    {56,    754000000,  8,    TLG_VSTD_SECAM_G  },
    {57,    762000000,  8,    TLG_VSTD_SECAM_G  },
    {58,    770000000,  8,    TLG_VSTD_SECAM_G  },
    {59,    778000000,  8,    TLG_VSTD_SECAM_G  },
    {60,    786000000,  8,    TLG_VSTD_SECAM_G  },
    {61,    794000000,  8,    TLG_VSTD_SECAM_G  },
    {62,    802000000,  8,    TLG_VSTD_SECAM_G  },
    {63,    810000000,  8,    TLG_VSTD_SECAM_G  },
    {64,    818000000,  8,    TLG_VSTD_SECAM_G  },
    {65,    826000000,  8,    TLG_VSTD_SECAM_G  },
    {66,    834000000,  8,    TLG_VSTD_SECAM_G  },
    {67,    842000000,  8,    TLG_VSTD_SECAM_G  },
    {68,    850000000,  8,    TLG_VSTD_SECAM_G  },
    {69,    858000000,  8,    TLG_VSTD_SECAM_G  },
    {0, 0, 0, 0}
};
#endif 

#ifdef SUPPORT_CHANNEL_MAP_SECAM_DK

tlg_channel_t(tlg_secam_d_k_chn_map) = {
    {1,      52500000,  8,    TLG_VSTD_SECAM_D  },
    {2,      62000000,  8,    TLG_VSTD_SECAM_D  },
    {3,      80000000,  8,    TLG_VSTD_SECAM_D  },
    {4,      88000000,  8,    TLG_VSTD_SECAM_D  },
    {5,      96000000,  8,    TLG_VSTD_SECAM_D  },
    {6,     178000000,  8,    TLG_VSTD_SECAM_D  },
    {7,     186000000,  8,    TLG_VSTD_SECAM_D  },
    {8,     194000000,  8,    TLG_VSTD_SECAM_D  },
    {9,     202000000,  8,    TLG_VSTD_SECAM_D  },
    {10,    210000000,  8,    TLG_VSTD_SECAM_D  },
    {11,    218000000,  8,    TLG_VSTD_SECAM_D  },
    {12,    226000000,  8,    TLG_VSTD_SECAM_D  },
    {21,    474000000,  8,    TLG_VSTD_SECAM_K  },
    {22,    482000000,  8,    TLG_VSTD_SECAM_K  },
    {23,    490000000,  8,    TLG_VSTD_SECAM_K  },
    {24,    498000000,  8,    TLG_VSTD_SECAM_K  },
    {25,    506000000,  8,    TLG_VSTD_SECAM_K  },
    {26,    514000000,  8,    TLG_VSTD_SECAM_K  },
    {27,    522000000,  8,    TLG_VSTD_SECAM_K  },
    {28,    530000000,  8,    TLG_VSTD_SECAM_K  },
    {29,    538000000,  8,    TLG_VSTD_SECAM_K  },
    {30,    546000000,  8,    TLG_VSTD_SECAM_K  },
    {31,    554000000,  8,    TLG_VSTD_SECAM_K  },
    {32,    562000000,  8,    TLG_VSTD_SECAM_K  },
    {33,    570000000,  8,    TLG_VSTD_SECAM_K  },
    {34,    578000000,  8,    TLG_VSTD_SECAM_K  },
    {35,    586000000,  8,    TLG_VSTD_SECAM_K  },
    {36,    594000000,  8,    TLG_VSTD_SECAM_K  },
    {37,    602000000,  8,    TLG_VSTD_SECAM_K  },
    {38,    610000000,  8,    TLG_VSTD_SECAM_K  },
    {39,    618000000,  8,    TLG_VSTD_SECAM_K  },
    {40,    626000000,  8,    TLG_VSTD_SECAM_K  },
    {41,    634000000,  8,    TLG_VSTD_SECAM_K  },
    {42,    642000000,  8,    TLG_VSTD_SECAM_K  },
    {43,    650000000,  8,    TLG_VSTD_SECAM_K  },
    {44,    658000000,  8,    TLG_VSTD_SECAM_K  },
    {45,    666000000,  8,    TLG_VSTD_SECAM_K  },
    {46,    674000000,  8,    TLG_VSTD_SECAM_K  },
    {47,    682000000,  8,    TLG_VSTD_SECAM_K  },
    {48,    690000000,  8,    TLG_VSTD_SECAM_K  },
    {49,    698000000,  8,    TLG_VSTD_SECAM_K  },
    {50,    706000000,  8,    TLG_VSTD_SECAM_K  },
    {51,    714000000,  8,    TLG_VSTD_SECAM_K  },
    {52,    722000000,  8,    TLG_VSTD_SECAM_K  },
    {53,    730000000,  8,    TLG_VSTD_SECAM_K  },
    {54,    738000000,  8,    TLG_VSTD_SECAM_K  },
    {55,    746000000,  8,    TLG_VSTD_SECAM_K  },
    {56,    754000000,  8,    TLG_VSTD_SECAM_K  },
    {57,    762000000,  8,    TLG_VSTD_SECAM_K  },
    {58,    770000000,  8,    TLG_VSTD_SECAM_K  },
    {59,    778000000,  8,    TLG_VSTD_SECAM_K  },
    {60,    786000000,  8,    TLG_VSTD_SECAM_K  },
    {61,    794000000,  8,    TLG_VSTD_SECAM_K  },
    {62,    802000000,  8,    TLG_VSTD_SECAM_K  },
    {63,    810000000,  8,    TLG_VSTD_SECAM_K  },
    {64,    818000000,  8,    TLG_VSTD_SECAM_K  },
    {65,    826000000,  8,    TLG_VSTD_SECAM_K  },
    {66,    834000000,  8,    TLG_VSTD_SECAM_K  },
    {67,    842000000,  8,    TLG_VSTD_SECAM_K  },
    {68,    850000000,  8,    TLG_VSTD_SECAM_K  },
    {69,    858000000,  8,    TLG_VSTD_SECAM_K  },
    {0, 0, 0, 0}
};
tlg_channel_t(tlg_secam_d_k_hybrid_chn_map) = {
    {1,      52500000,  8,    TLG_VSTD_PAL_SECAM_D  },
    {2,      62000000,  8,    TLG_VSTD_PAL_SECAM_D  },
    {3,      80000000,  8,    TLG_VSTD_PAL_SECAM_D  },
    {4,      88000000,  8,    TLG_VSTD_PAL_SECAM_D  },
    {5,      96000000,  8,    TLG_VSTD_PAL_SECAM_D  },
    {6,     178000000,  8,    TLG_VSTD_PAL_SECAM_D  },
    {7,     186000000,  8,    TLG_VSTD_PAL_SECAM_D  },
    {8,     194000000,  8,    TLG_VSTD_PAL_SECAM_D  },
    {9,     202000000,  8,    TLG_VSTD_PAL_SECAM_D  },
    {10,    210000000,  8,    TLG_VSTD_PAL_SECAM_D  },
    {11,    218000000,  8,    TLG_VSTD_PAL_SECAM_D  },
    {12,    226000000,  8,    TLG_VSTD_PAL_SECAM_D  },
    {21,    474000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {22,    482000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {23,    490000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {24,    498000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {25,    506000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {26,    514000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {27,    522000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {28,    530000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {29,    538000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {30,    546000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {31,    554000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {32,    562000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {33,    570000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {34,    578000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {35,    586000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {36,    594000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {37,    602000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {38,    610000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {39,    618000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {40,    626000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {41,    634000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {42,    642000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {43,    650000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {44,    658000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {45,    666000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {46,    674000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {47,    682000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {48,    690000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {49,    698000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {50,    706000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {51,    714000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {52,    722000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {53,    730000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {54,    738000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {55,    746000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {56,    754000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {57,    762000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {58,    770000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {59,    778000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {60,    786000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {61,    794000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {62,    802000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {63,    810000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {64,    818000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {65,    826000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {66,    834000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {67,    842000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {68,    850000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {69,    858000000,  8,    TLG_VSTD_PAL_SECAM_K  },
    {0, 0, 0, 0}
};

#endif 
tlg_channel_rw_t(tlg_search_chn_map) = {
    {1,     0,  0,    0  },
    {2,     0,  0,    0  },
    {3,     0,  0,    0  },
    {4,     0,  0,    0  },
    {5,     0,  0,    0  },
    {6,     0,  0,    0  },
    {7,     0,  0,    0  },
    {8,     0,  0,    0  },
    {9,     0,  0,    0  },
    {10,    0,  0,    0  },
    {11,    0,  0,    0  },
    {12,    0,  0,    0  },
    {13,    0,  0,    0  },
    {14,    0,  0,    0  },
    {15,    0,  0,    0  },
    {16,    0,  0,    0  },
    {17,    0,  0,    0  },
    {18,    0,  0,    0  },
    {19,    0,  0,    0  },
    {20,    0,  0,    0  },
    {21,    0,  0,    0  },
    {22,    0,  0,    0  },
    {23,    0,  0,    0  },
    {24,    0,  0,    0  },
    {25,    0,  0,    0  },
    {26,    0,  0,    0  },
    {27,    0,  0,    0  },
    {28,    0,  0,    0  },
    {29,    0,  0,    0  },
    {30,    0,  0,    0  },
    {31,    0,  0,    0  },
    {32,    0,  0,    0  },
    {33,    0,  0,    0  },
    {34,    0,  0,    0  },
    {35,    0,  0,    0  },
    {36,    0,  0,    0  },
    {37,    0,  0,    0  },
    {38,    0,  0,    0  },
    {39,    0,  0,    0  },
    {40,    0,  0,    0  },
    {41,    0,  0,    0  },
    {42,    0,  0,    0  },
    {43,    0,  0,    0  },
    {44,    0,  0,    0  },
    {45,    0,  0,    0  },
    {46,    0,  0,    0  },
    {47,    0,  0,    0  },
    {48,    0,  0,    0  },
    {49,    0,  0,    0  },
    {50,    0,  0,    0  },
    {51,    0,  0,    0  },
    {52,    0,  0,    0  },
    {53,    0,  0,    0  },
    {54,    0,  0,    0  },
    {55,    0,  0,    0  },
    {56,    0,  0,    0  },
    {57,    0,  0,    0  },
    {58,    0,  0,    0  },
    {59,    0,  0,    0  },
    {60,    0,  0,    0  },
    {61,    0,  0,    0  },
    {62,    0,  0,    0  },
    {63,    0,  0,    0  },
    {64,    0,  0,    0  },
    {65,    0,  0,    0  },
    {66,    0,  0,    0  },
    {67,    0,  0,    0  },
    {68,    0,  0,    0  },
    {69,    0,  0,    0  },
    {0, 0, 0, 0}
};

tlg_channel_t(tlg_test_chn_map) = {
    {1,      50000000,  8,    TLG_VSTD_PAL_D  },
    {2,      80000000,  8,    TLG_VSTD_PAL_D  },
    {3,     110000000,  8,    TLG_VSTD_PAL_D  },
    {4,     170000000,  8,    TLG_VSTD_PAL_D  },
    {5,     200000000,  8,    TLG_VSTD_PAL_D  },
    {6,     230000000,  8,    TLG_VSTD_PAL_D  },
    {7,     480000000,  8,    TLG_VSTD_PAL_D  },
    {8,     640000000,  8,    TLG_VSTD_PAL_D  },
    {9,     800000000,  8,    TLG_VSTD_PAL_D  },
    {0, 0, 0, 0}
};

tlg_channel_t(tlg_test1_chn_map) = {
    {1,      68500000,  8,    TLG_VSTD_PAL_D  },
    {2,     203000000,  8,    TLG_VSTD_PAL_D  },
    {3,     650000000,  8,    TLG_VSTD_PAL_D  },
    {0, 0, 0, 0}
};

tlg_channel_t(tlg_test2_chn_map) = {
    {1,       80000000,  8,    TLG_VSTD_PAL_D  },
    {2,     203000000,  8,    TLG_VSTD_PAL_D  },
    {3,     786000000,  8,    TLG_VSTD_PAL_D  },
    {0, 0, 0, 0}
};
p_tlg_channel_t(p_tlg_cur_map)      = NULL; 
int              tlg_cur_bw         = TLG_BW_NONE;   
int              tlg_cur_mode       = TLG_MODE_NONE; 
int              tlg_cur_std        = TLG_VSTD_NONE; 
int              tlg_cur_chn_cnt    = 0;             
int              tlg_slope          = 0;             
int              tlg_offset         = 0;             
int              tlg_init_done      = 0;             
int              tlg_cur_lna        = TLG_LNA_NONE;  
uint16           tlg_cur_gain       = 0;             
uint16           tlg_cur_aud_mode   = 0;             
uint32           tlg_i2c_addr       = TLGI2C_DEFAULT_BASE_ADDR; 
tlg_statep       tlg_power_state    = {TLG_STATE0};  
uint32           tlg_prev_fm_ssi    = 0;             
uint32           tlg_prev_fm_chn    = 0;             
bool             tlg_scan_state     = false;         


#ifdef TLG_FM_NEW_INTERFACE

int              tlg_fm_min_hz = 0;
int              tlg_fm_max_hz = 0;
int              tlg_fm_scan = 0;
int              tlg_fm_scan_set_freq = 0;

#define FM_SCAN_HALF_BANDWIDTH      (3000000) 
static int TLGAPP_FMScanFrequency(int32 center_freq_hz);

#endif 
extern TLGDLL_API uint32 tlg_cur_fm_ssi;



TLGDLL_API void TLGAPP_Restart(void)
{
    p_tlg_cur_map = NULL;                       
    tlg_cur_bw    = TLG_BW_NONE;                
    tlg_cur_mode  = TLG_MODE_NONE;              
    tlg_cur_std   = TLG_VSTD_NONE;              
    tlg_slope     = 0;                          
    tlg_offset    = 0;                          
    tlg_init_done = 0;                          
    tlg_cur_lna   = TLG_LNA_NONE;               
    tlg_cur_gain  = 0;                          
    tlg_cur_aud_mode = 0;                       
    TLG_ClearRegShadow(tlg_i2c_addr);           
    tlg_scan_state = false;                     
}

int TLGAPP_Ini_WriteReg(int start, uint32 i2c_addr, uint16 reg_addr, uint16 val)
{
    if (start == TLG_INIT_DEEP_SLEEP && (reg_addr >= 0x180))
        return TLG_ERR_SUCCESS;
    return TLG_WriteReg(i2c_addr, reg_addr, val);
}




const tlg_reg_pair_t tlg_init_reg_1_1 []=
{

    
    {0x0002, 0x9000},

    
    {0x0055, 0x3098},

    
    {0x0064, 0x5413},

    
    {0x009b, 0x0040},

    {0x0010, 0xc8c8},  
    {0x0012, 0xC80a},  
    {0x0016, 0x400A},  
    {0x001D, 0xbc00},  
    {0x001e, 0x4011},  
    {0x001f, 0x9a40},  
    {0x0021, 0x4001},  
    {0x0025, 0x002f},  
    {0x0026, 0x0001},  
    {0x002B, 0x005a},  
    {0x002C, 0x0002},  
    {0x002D, 0x000A},  
    {0x002E, 0x3a0d},  
    {0x002F, 0x560d},  
    {0x0031, 0x41c0},  

    {0x006B, 0x73c6},

    {0x006f, 0x40c3},  

    {0x0070, 0x8610},
    {0x0072, 0x4060},
    {0x0061, 0xd504},
    {0x0062, 0xfe04},
    {0x0063, 0x204d},
    {0x006c, 0xa229},
    {0x0077, 0x1034},
    {0x0078, 0x0000},


    {0x008b, 0x0001},
    {0x0097, 0x0003},


    {0x009c, 0xC040},


    {0x00b4, 0x3c02},

    {0x0185, 0x0104},
    {0x0188, 0x0292},
    {0x018b, 0x0292},

    {0x0197, 0x0841},  
    {0x0198, 0x006d},  
    {0x01a4, 0x0282},  
    {0x01b3, 0x29F8},  
    {0x01b4, 0x0261},  
    {0x01b7, 0x0641},  

    {0x0076, 0x0080},  
    {0x0051, 0x008e},  
    {0x008c, 0xd808},

    {0xFFFF, 0xFFFF}

};


const tlg_reg_pair_t tlg_init_reg_1_2_common []=
{


    
    {0x0002, 0x9000},

    
    {0x0055, 0x3098},

    
    {0x0064, 0x5413},

    
    {0x009b, 0x0040},


    
    {0x0010, 0x64c8}, 

    {0x0011, 0x460f},

    
    {0x0012, 0xC80a}, 

    
    {0x0019, 0x2001}, 


    {0x001E, 0x4011}, 

    
    {0x0025, 0x002f},

    
    {0x002b, 0x001e},

    {0x002C, 0x8002},

    {0x002D, 0x0005},


    {0x006f, 0x40c3},
    {0x0185, 0x0104},
    {0x0188, 0x0292},
    {0x018b, 0x0292},

    
    {0x018f, 0xae90},


    {0x0197, 0x0841},


    {0x0198, 0x006D},


    {0x01A4, 0x0282},
    {0x01B3, 0x29F8},



    {0x01B4, 0x4261},


    {0x01B7, 0x0641},

    {0x0070, 0x8610},
    {0x0072, 0x4060},
    {0x0061, 0xd504},
    {0x0062, 0xfe04},
    {0x0063, 0x204d},
    {0x006c, 0xa229},
    {0x0077, 0x1034},
    {0x0078, 0x0000},


    {0x006d, 0x260c},
    {0x006b, 0x73c6},


    {0x00b4, 0x3c02},
    {0x008b, 0x0001},

    {0x0097, 0x0113},

    {0x009c, 0xc040},


    {0x00bc, 0x0040},
    {0x0067, 0x400c},
    {0x00bd, 0x1000},

    {0x0076, 0x0080},  
    {0x0051, 0x008e},  
    {0x008c, 0xd808},


    {0xFFFF, 0xFFFF}
};


const tlg_reg_pair_t tlg_init_reg_2_1_common []=
{


    
    {0x0002, 0x9000},

    
    {0x0055, 0x3098},

    
    {0x0064, 0x5413},

    
    {0x009b, 0x0040},


    
    {0x0010, 0x64c8}, 

    {0x0011, 0x460f},

    
    {0x0012, 0xC80a}, 

    
    {0x0019, 0x2001}, 


    {0x001E, 0x4011}, 

    
    {0x0025, 0x002f},

    
    {0x002b, 0x001e},

    {0x002C, 0x8002},

    {0x002D, 0x0005},


    {0x006f, 0x40c3},
    {0x0185, 0x0104},
    {0x0188, 0x0292},
    {0x018b, 0x0292},


    {0x0189, 0x0400},

    
    {0x018f, 0xae90},


    {0x0197, 0x0841},


    {0x0198, 0x006D},


    {0x01A4, 0x0282},
    {0x01B3, 0x29F8},



    {0x01B4, 0x4261},


    {0x01B7, 0x0641},

    {0x0070, 0x8610},

    {0x0071, 0x8d1a },
    {0x0072, 0x4060},
    {0x0061, 0xd504},
    {0x0062, 0xfe04},
    {0x0063, 0x204d},
    {0x0066, 0x2096},
    {0x006c, 0xa229},
    {0x0077, 0x1034},
    {0x0078, 0x0000},


    {0x006d, 0x260c},
    {0x006b, 0x73c6},


//    {0x00b4, 0x3c02},
//    {0x008b, 0x0001},


    {0x0097, 0x0103},

    {0x009c, 0xc040},


    {0x00bc, 0x0040},
    {0x0067, 0x400c},
    {0x00bd, 0x1000},

    {0x0076, 0x0080},  
    {0x0051, 0x008e},  
    {0x008c, 0xd808},


    {0x009c, 0x93e0},
    {0x009f, 0xa980},   




    {0x008b, 0x0109}, 
    {0x00b6, 0x7445}, 
    {0x00f2, 0x1000}, 
    {0x00f1, 0x0495}, 

    {0x00b4, 0x3cc0}, 


    {0x00f0, 0xb4fc},
    {0x00b0, 0xffb6}, 

    {0x0095, 0x0000},
    {0x00ad, 0x7100}, 

    {0x00ae, 0x0384}, 
    {0x00af, 0x0320}, 

    {0x00aa, 0x0703},

    //{0x0098, 0x0000}, 
    //{0x009e, 0x0000}, 


    {0x00a8, 0x0341},
    
    {0x00b5, 0x0095},
    {0x00b7, 0x000f},

    {0xFFFF, 0xFFFF}
};


const tlg_reg_pair_t tlg_init_reg_2_3_common []=
{


    
    {0x0002, 0x9000},

    
    {0x0055, 0x3098},

    
    {0x0064, 0x5413},

    
    {0x009b, 0x0040},


    
    {0x0010, 0x64c8}, 

    {0x0011, 0x460f},

    
    {0x0012, 0xC80a}, 

    
    {0x0019, 0x2001}, 


    {0x001E, 0x4011}, 

    
    {0x0025, 0x002f},

    
    {0x002b, 0x001e},

    {0x002C, 0x8002},

    {0x002D, 0x0005},


    {0x006f, 0x40c3},
    {0x0185, 0x0104},
    {0x0188, 0x0292},
    {0x018b, 0x0292},


    {0x0189, 0x0400},

    
    {0x018f, 0xae90},


    {0x0197, 0x0841},


    {0x0198, 0x006D},


    {0x01A4, 0x0282},
    {0x01B2, 0x222C},
    {0x01B3, 0x2CC9},



    {0x01B4, 0x4261},


    {0x01B7, 0x0641},

    {0x0070, 0x8610},

    {0x0071, 0x8d1a },
    {0x0072, 0x4060},
    {0x0061, 0xd504},
    {0x0062, 0xfe04},
    {0x0063, 0x204d},
    {0x0066, 0x2096},
    {0x006c, 0xa229},
    {0x0077, 0x1034},
    {0x0078, 0x0000},


    {0x006d, 0x260c},
    {0x006b, 0x73c6},


//    {0x00b4, 0x3c02},
//    {0x008b, 0x0001},


    {0x0097, 0x0103},

    {0x009c, 0xc040},


    {0x00bc, 0x0040},
    {0x0067, 0x400c},
    {0x00bd, 0x1000},

    {0x0076, 0x0080},  
    {0x0051, 0x008e},  
    {0x008c, 0xd808},


    {0x009c, 0x93e0},
    {0x009f, 0xa980},   




    {0x008b, 0x0109}, 
    {0x00b6, 0x7445}, 
    {0x00f2, 0x1000}, 
    {0x00f1, 0x0495}, 

    {0x00b4, 0x3cc0}, 


    {0x00f0, 0xb4fc},
    {0x00b0, 0xffb6}, 

    {0x0095, 0x0000},
    {0x00ad, 0x7100}, 

    {0x00ae, 0x0384}, 
    {0x00af, 0x0320}, 

    {0x00aa, 0x0703},

    //{0x0098, 0x0000}, 
    //{0x009e, 0x0000}, 


    {0x00a8, 0x0341},
    
    {0x00b5, 0x0095},
    {0x00b7, 0x000f},

    {0xFFFF, 0xFFFF}
};



const tlg_reg_pair_t tlg_init_reg_3_1_common []=
{


    
    {0x0002, 0x9000},

    
    {0x0055, 0x3098},

    
    {0x0064, 0x5413},

    
    {0x009b, 0x0040},


    
    {0x0010, 0x64c8}, 

    {0x0011, 0x460f},

    
    {0x0012, 0xC80a}, 

    
    {0x0019, 0x2001}, 


    {0x001E, 0x4011}, 

    
    {0x0025, 0x002f},

    
    {0x002b, 0x001e},

    {0x002C, 0x8002},

    {0x002D, 0x0005},


    {0x006f, 0x40c3},
    {0x0185, 0x0104},
    {0x0188, 0x0292},
    {0x018b, 0x0292},


    {0x0189, 0x0400},

    
    {0x018f, 0xae90},


    {0x0197, 0x0841},


    {0x0198, 0x006D},


    {0x01A4, 0x0282},
    {0x01B2, 0x20D8},
    {0x01B3, 0x2b1c},



    {0x01B4, 0x4261},


    {0x01B7, 0x0641},

    {0x0180, 0xffff},
    {0x0181, 0xffff},
    {0x0182, 0x0000},
    {0x0183, 0x0000},
    {0x0184, 0x0000},
    {0x0186, 0x0000},
    {0x0187, 0x0000},
    {0x018a, 0x0000},
    {0x018c, 0x0000},
    {0x018d, 0x0000},
    {0x018e, 0x0000},
    {0x0190, 0x0000},
    {0x0191, 0x0000},
    {0x0192, 0x0000},
    {0x0195, 0x0000},
    {0x0196, 0x0000},
    {0x0199, 0x0000},
    {0x019a, 0x0000},
    {0x019b, 0x9400},
    {0x019c, 0x0000},
    {0x019d, 0x0000},
    {0x019e, 0x0000},
    {0x019f, 0x0000},
    {0x01a0, 0x0000},
    {0x01a1, 0x0000},
    {0x01a2, 0x0000},
    {0x01a3, 0x0000},
    {0x01a5, 0x0390},
    {0x01a6, 0x1a6c},
    {0x01a7, 0x0044},
    {0x01a8, 0x048a},
    {0x01a9, 0x0000},
    {0x01aa, 0x0000},
    {0x01ab, 0x0000},
    {0x01ac, 0x0000},
    {0x01ad, 0x0000},
    {0x01ae, 0x0000},
    {0x01af, 0x0000},
    {0x01b0, 0x0000},
    {0x01b1, 0x0040},
    {0x01b5, 0x0704},
    {0x01b6, 0x0000},
    {0x01b8, 0x0000},

    {0x0070, 0x8610},

    {0x0071, 0x8d1a },
    {0x0072, 0x4060},
    {0x0061, 0xd504},
    {0x0062, 0xfe04},
    {0x0063, 0x204d},
    {0x0066, 0x2096},
    {0x006c, 0xa229},
    {0x0077, 0x1034},
    {0x0078, 0x0000},


    {0x006d, 0x260c},
    {0x006b, 0x73c6},


//    {0x00b4, 0x3c02},
//    {0x008b, 0x0001},


    {0x0097, 0x0103},

    {0x009c, 0xc040},


    {0x00bc, 0x0040},
    {0x0067, 0x400c},
    {0x00bd, 0x1000},

    {0x0076, 0x0080},  
    {0x0051, 0x008e},  
    {0x008c, 0xd808},


    {0x009c, 0x93e0},
    {0x009f, 0xa980},   




    {0x008b, 0x0109}, 
    {0x00b6, 0x7445}, 
    {0x00f2, 0x1000}, 
    {0x00f1, 0x0495}, 

    {0x00b4, 0x3cc0}, 


    {0x00f0, 0xb4fc},
    {0x00b0, 0xffb6}, 

    {0x0095, 0x0000},
    {0x00ad, 0x7100}, 

    {0x00ae, 0x0384}, 
    {0x00af, 0x0320}, 

    {0x00aa, 0x0703},

    //{0x0098, 0x0000}, 
    //{0x009e, 0x0000}, 


    {0x00a8, 0x0341},
    
    {0x00b5, 0x0095},
    {0x00b7, 0x000f},

    
    {0x0193, 0x0060},
    {0x0194, 0x2000},

    {0xFFFF, 0xFFFF}
};


const tlg_reg_pair_t tlg_init_reg_3_3_common []=
{


    
    {0x0002, 0x9000},

    
    {0x0055, 0x3098},

    
    {0x0064, 0x5413},

    
    {0x009b, 0x0040},


    
    {0x0010, 0x64c8}, 

    {0x0011, 0x460f},

    
    {0x0012, 0xC80a}, 

    
    {0x0019, 0x2001}, 


    {0x001E, 0x4011}, 

    
    {0x0025, 0x002f},

    
    {0x002b, 0x001e},

    {0x002C, 0x8002},

    {0x002D, 0x0005},


    {0x006f, 0x40c3},
    {0x0185, 0x0104},
    {0x0188, 0x0292},
    {0x018b, 0x0292},


    {0x0189, 0x0400},

    
    {0x018f, 0xae90},


    {0x0197, 0x0841},


    {0x0198, 0x006D},


    {0x01A4, 0x0282},
    {0x01B2, 0x20D8},
    {0x01B3, 0x2b1c},



    {0x01B4, 0x4261},


    {0x01B7, 0x0641},

    {0x0070, 0x8610},

    {0x0071, 0x8d1a },
    {0x0072, 0x4060},
    {0x0061, 0xd504},
    {0x0062, 0xfe04},
    {0x0063, 0x204d},
    {0x0066, 0x2096},
    {0x006c, 0xa229},
    {0x0077, 0x1034},
    {0x0078, 0x0000},


    {0x006d, 0x260c},
    {0x006b, 0x73c6},


//    {0x00b4, 0x3c02},
//    {0x008b, 0x0001},


    {0x0097, 0x0103},

    {0x009c, 0xc040},


    {0x00bc, 0x0040},
    {0x0067, 0x400c},
    {0x00bd, 0x1000},

    {0x0076, 0x0080},  
    {0x0051, 0x008e},  
    {0x008c, 0xd808},


    {0x009c, 0x93e0},
    {0x009f, 0xa980},   




    {0x008b, 0x0109}, 
    {0x00b6, 0x7445}, 
    {0x00f2, 0x1000}, 
    {0x00f1, 0x0495}, 

    {0x00b4, 0x3cc0}, 


    {0x00f0, 0xb4fc},
    {0x00b0, 0xffb6}, 

    {0x0095, 0x0000},
    {0x00ad, 0x7100}, 

    {0x00ae, 0x0384}, 
    {0x00af, 0x0320}, 

    {0x00aa, 0x0703},

    //{0x0098, 0x0000}, 
    //{0x009e, 0x0000}, 


    {0x00a8, 0x0341},
    
    {0x00b5, 0x0095},
    {0x00b7, 0x000f},

    
    {0x0193, 0x0060},
    {0x0194, 0x2000},
    
    
    {0x01b5, 0x0704},
    
    {0xFFFF, 0xFFFF}
};


const tlg_reg_pair_t *getTlgInitReg()
{
    uint32 version;
    TLG_GetChipVersion(tlg_i2c_addr, &version);

    TLG1121_VERS_A_2_BEGIN
    #ifdef TLG_CMP_CHIP_1121_A_2
    TLG_REG_LOG((b, "tlg_init_reg = tlg_init_reg_3_3_common\n"));
    return (tlg_init_reg_3_3_common);
    #endif 

    TLG1121_VERS_A_0_ELSE
    #ifdef TLG_CMP_CHIP_1121_A_0
    TLG_REG_LOG((b, "tlg_init_reg = tlg_init_reg_3_1_common\n"));
    return (tlg_init_reg_3_1_common);
    #endif 

    TLG_CHIP_VERS_2_3_ELSE
    #if defined(TLG_CMP_CHIP_1120_2_3)
    TLG_REG_LOG((b, "tlg_init_reg = tlg_init_reg_2_3_common\n"));
    return (tlg_init_reg_2_3_common);
    #endif 

    TLG_CHIP_VERS_2_1_ELSE
    #ifdef TLG_CMP_CHIP_1120_2_1
    TLG_REG_LOG((b, "tlg_init_reg = tlg_init_reg_2_1_common\n"));
    return (tlg_init_reg_2_1_common);

    #endif 
    TLG_CHIP_VERS_1_2_ELSE
    #ifdef TLG_CMP_CHIP_1120_1_2
    TLG_REG_LOG((b, "tlg_init_reg = tlg_init_reg_1_2_common\n"));
    return (tlg_init_reg_1_2_common);

    #endif 
    TLG_CHIP_VERS_1_1_ELSE

    #ifdef TLG_CMP_CHIP_1120_1_1
    TLG_REG_LOG((b, "tlg_init_reg = tlg_init_reg_1_1\n"));
    return tlg_init_reg_1_1;
    #endif 
    TLG_CHIP_VERS_END
    return tlg_init_reg_1_1;
}

int TLG_Ini_WritePair(uint32 base_addr, int start, const tlg_reg_pair_t *p_tbl)
{
    
#define TEST_START
#ifdef TEST_START
    if (start == TLG_INIT_DEEP_SLEEP)
    {
        while (p_tbl && p_tbl->reg != 0xFFFF)
        {
            if (p_tbl->reg < 0x180)
            {
                TLG_WriteReg(base_addr, p_tbl->reg, p_tbl->value);
            }
            p_tbl++;
        }
    }
    else
    {
        while (p_tbl && p_tbl->reg != 0xFFFF)
        {
            TLG_WriteReg(base_addr, p_tbl->reg, p_tbl->value);
            p_tbl++;
        }
    }

#else 
    while (p_tbl && (p_tbl->reg != 0xFFFF))
    {
        if (!((start == TLG_INIT_DEEP_SLEEP) && (p_tbl->reg >= 0x180)))
        {
            TLG_WriteReg(base_addr, p_tbl->reg, p_tbl->value);
        }
        p_tbl++;
    }

#endif 

    return(0);
}

TLGDLL_API void TLGAPP_Ini_Reg(int start)
{
    
    const tlg_reg_pair_t *tlg_init_reg ;

    tlg_init_reg = getTlgInitReg();
    TLG_Ini_WritePair(tlg_i2c_addr, start, tlg_init_reg);
    TLG_Init_AGC_Reg(
        tlg_i2c_addr,
        tv_mode     
    );


    TLG_REG_LOG((b, "TLGAPP_Ini_Reg DONE\n"));
}



TLGDLL_API int TLGAPP_Init_Aux(uint32 base_addr, int start)
{
    tlg_i2c_addr = base_addr;

    TLGAPP_Ini_Reg(start);

    if (start == TLG_NEW_INIT)
        TLG_Init(tlg_i2c_addr);
    else
        TLG_ReInitFromDeepSleep(tlg_i2c_addr);

    TLGAPP_InitVideoAndAudio();
    tlg_init_done = 1;
    return TLG_ERR_SUCCESS;
}


TLGDLL_API int TLGAPP_Init(uint32 base_addr)
{
    TLGAPP_Restart();
    return TLGAPP_Init_Aux(base_addr, TLG_NEW_INIT);
}


TLGDLL_API int TLGAPP_PowerDown(void)
{
    tlg_power_state[0] = TLG_STATE0;
    TLG_DeepSleep(tlg_i2c_addr, tlg_power_state);
    return TLG_ERR_SUCCESS;
}


TLGDLL_API int TLGAPP_ReInitFromPowerDown(void)
{
    if (TLG_ReturnFromDeepSleep(tlg_i2c_addr, tlg_power_state) == TLG_ERR_SUCCESS) 
    {
        TLGAPP_Restart();
        return TLGAPP_Init_Aux(tlg_i2c_addr, TLG_INIT_DEEP_SLEEP);
    } else
        return TLG_ERR_FAIL;
}


TLGDLL_API int TLGAPP_ChannelSetup(int mode, int standard, int bw)
{
    if (!tlg_init_done) TLGAPP_Init(tlg_i2c_addr);

    if ((mode != TLG_MODE_NONE) && (mode != tlg_cur_mode))
    {
        TLG_SetChnMode(tlg_i2c_addr, (uint16) mode);
        tlg_cur_mode = mode;
    }
    
    TLG_SetHybrid((uint8)((uint16)(standard & TLG_VSTD_HYBRID_BIT)!=0));

    if (((standard != TLG_VSTD_NONE) && (standard != tlg_cur_std)) || 
        (standard & TLG_VSTD_HYBRID_BIT))
    {
        TLG_SetChnStandard(tlg_i2c_addr, ((uint16) standard) & ~TLG_VSTD_HYBRID_BIT);
        tlg_cur_std = standard;
    }

    if ((bw != TLG_BW_NONE) && (bw != tlg_cur_bw))
    {
        TLG_CHIP_VERS_2_1_BEGIN
        TLG_CHIP_VERS_1_2_ELSE
        TLG_CHIP_VERS_1_1_ELSE
        #ifdef TLG_CMP_CHIP_1120_1_1
        bw = TLG_BW_12;
        #endif 
        TLG_CHIP_VERS_END
        TLG_SetChnBandwidth(tlg_i2c_addr, (uint16) bw);
        tlg_cur_bw = bw;
    }
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLGAPP_SetChannelMap(p_tlg_channel_t(map), int mode, int vidstd, int bw)
{
    int i;
    
    TLG_CHIP_VERS_1_2_BEGIN
    #if defined(TLG_CMP_CHIP_1120_1_2) || defined(TLG_CMP_CHIP_1120_2_1) ||defined(TLG_CMP_CHIP_1121_A_0) || defined(TLG_CMP_CHIP_1120_2_3) || defined(TLG_CMP_CHIP_1121_A_2)
    TLGAPP_ChannelSetup(mode, vidstd, bw);
    #endif 
    TLG_CHIP_VERS_1_1_ELSE

    #ifdef TLG_CMP_CHIP_1120_1_1
    TLGAPP_ChannelSetup(mode, vidstd, TLG_BW_12);
    #endif 
    TLG_CHIP_VERS_END

    p_tlg_cur_map   = map;
    tlg_slope       = 0;
    tlg_offset      = 0;
    tlg_cur_chn_cnt = 0;
    for(i=0; p_tlg_cur_map[i] CHN_S != 0; i++)
    {
        tlg_cur_chn_cnt++;
    }
    return(TLG_ERR_SUCCESS);
}


#ifdef  SUPPORT_CHANNEL_MAP_CHINA
TLGDLL_API int TLGAPP_SetChannelMapPALD(void)
{
    return TLGAPP_SetChannelMap(tlg_china_pald_chn_map, TLG_MODE_ANALOG_TV_UNCOMP,
                         TLG_VSTD_PAL_D, TLG_BW_8);
}
#endif 


#ifdef SUPPORT_CHANNEL_MAP_NTSC
TLGDLL_API int TLGAPP_SetChannelMapNTSC(void)
{
    return TLGAPP_SetChannelMap(tlg_ntsc_chn_map, TLG_MODE_ANALOG_TV_UNCOMP,
                         TLG_VSTD_NTSC_M, TLG_BW_6);
}
#endif 


TLGDLL_API int TLGAPP_SetChannelMapFM(void)
{
    int    i;
    uint32 minhz = 999999999;
    uint32 maxhz = 0;
    uint32 step_size_hz;

    
    p_tlg_cur_map = tlg_fm_chn_map;

    tv_mode = TLG_MODE_FM_RADIO;

    TLGAPP_SetChannelMap(p_tlg_cur_map, TLG_MODE_FM_RADIO, 
                    TLG_VSTD_NTSC_M, TLG_BW_6); 

    step_size_hz = p_tlg_cur_map[1] HZ_S - p_tlg_cur_map[0] HZ_S;
    
    for (i=0; p_tlg_cur_map[i] CHN_S != 0; i++)
    {
        if (p_tlg_cur_map[i] HZ_S < minhz) minhz = p_tlg_cur_map[i] HZ_S;
        if (p_tlg_cur_map[i] HZ_S > maxhz) maxhz = p_tlg_cur_map[i] HZ_S;
    }
    TLG_REG_LOG((b, "TLGAPP_SetChannelMapFM TLG_Set_FM_StepSize %01d\n", step_size_hz));
    TLG_Set_FM_StepSize(tlg_i2c_addr, step_size_hz);
    TLG_SetRegion(tlg_i2c_addr, TLG_REGION_WW);
    return TLG_InitScanFMChn(tlg_i2c_addr, minhz, maxhz, &tlg_slope, &tlg_offset);
}



TLGDLL_API int TLGAPP_SetChannelMapFM_USA(void)
{
    int    i;
    uint32 minhz = 999999999;
    uint32 maxhz = 0;
    uint32 step_size_hz;

    
    p_tlg_cur_map = tlg_fm_usa_chn_map;
    tv_mode = TLG_MODE_FM_RADIO;


    TLGAPP_SetChannelMap(p_tlg_cur_map, TLG_MODE_FM_RADIO, 
                    TLG_VSTD_NTSC_M, TLG_BW_6); 

    step_size_hz = p_tlg_cur_map[1] HZ_S - p_tlg_cur_map[0] HZ_S;
    
    for (i=0; p_tlg_cur_map[i] CHN_S != 0; i++)
    {
        if (p_tlg_cur_map[i] HZ_S < minhz) minhz = p_tlg_cur_map[i] HZ_S;
        if (p_tlg_cur_map[i] HZ_S > maxhz) maxhz = p_tlg_cur_map[i] HZ_S;
    }
    TLG_REG_LOG((b, "TLGAPP_SetChannelMapFM TLG_Set_FM_StepSize %01d\n", step_size_hz));
    TLG_Set_FM_StepSize(tlg_i2c_addr, step_size_hz);
    TLG_SetRegion(tlg_i2c_addr, TLG_REGION_US);
    return TLG_InitScanFMChn(tlg_i2c_addr, minhz, maxhz, &tlg_slope, &tlg_offset);
}

TLGDLL_API int TLGAPP_SwitchChannelMap(p_tlg_channel_t(map))
{
    int i;
    TLG_NULLPTR_CHECK(map);
    p_tlg_cur_map   = map;
    tlg_cur_chn_cnt = 0;
    for(i=0; p_tlg_cur_map[i] CHN_S != 0; i++)
    {
        tlg_cur_chn_cnt++;
    }
    return(TLG_ERR_SUCCESS);
}


TLGDLL_API int TLGAPP_SetChannelAux(int chn, int useFast)
{
    int         hybridMode;
    int i       = 0;
    int result  = 0;
    uint32 step_size_hz;

    if (chn == 0) return 0;
    if (p_tlg_cur_map == NULL) return 0;

    //if (!tlg_init_done) TLGAPP_Init(tlg_i2c_addr);
    TLG_REG_LOG((b, "SetChn # %d\n", chn));
    while (p_tlg_cur_map[i] CHN_S != 0)
    {
        TLG_REG_LOG((b, "Search chn %d\n", p_tlg_cur_map[i] CHN_S));
        
        if (p_tlg_cur_map[i] CHN_S == chn)
        {
            TLG_REG_LOG((b, "FOUND channel %d\n", chn));
            
            TLGAPP_SetLnaGpio(p_tlg_cur_map[i] HZ_S);

            
            hybridMode = p_tlg_cur_map[i] STD_S & TLG_VSTD_HYBRID_BIT;
            TLGAPP_ChannelSetup(tlg_cur_mode, p_tlg_cur_map[i] STD_S, p_tlg_cur_map[i] BW_S);
            if (tlg_cur_mode == TLG_MODE_ANALOG_TV_UNCOMP)
            {
                if (useFast)
                    result = TLG_FastScanTVChn(tlg_i2c_addr, p_tlg_cur_map[i] HZ_S);
                else
                    result = TLG_ScanTVChn(tlg_i2c_addr, p_tlg_cur_map[i] HZ_S);
                
            #ifdef HYBRID_DK_I
                if(p_tlg_cur_map[i] STD_S == TLG_VSTD_PAL_D)
                {
                    tlg_cur_std = TLG_setAlternateAudioOffset(tlg_i2c_addr);
                }
                #endif 
                #ifndef TLG_SUPPORT_HYBRID_POLL
                if (!tlg_scan_state && hybridMode && (result != 0))
                {
                    result = TLG_SetAlternateStandard(tlg_i2c_addr);
                }
                #endif 
                // for HarmonicNotch 2010.10.13
		  TLG_SetBBClkHarmonicNotch(tlg_i2c_addr, p_tlg_cur_map[i] HZ_S);
            } 
            else
            {
                if (i > 0)
                {
                    step_size_hz = p_tlg_cur_map[i] HZ_S - p_tlg_cur_map[i - 1] HZ_S;
                }
                else
                {
                    step_size_hz = p_tlg_cur_map[1] HZ_S - p_tlg_cur_map[0] HZ_S;
                }
                TLG_Set_FM_StepSize(tlg_i2c_addr, step_size_hz);
                result = TLG_ScanFMChn(tlg_i2c_addr, p_tlg_cur_map[i] HZ_S,
                                     tlg_slope, tlg_offset);
            }
            TLG_SetAudioNotchesNormal(tlg_i2c_addr);
            return((result > 0) ? 1 : 0); 
        }
        i++;
    }
    return((result > 0) ? 1 : 0);
}


TLGDLL_API int TLGAPP_SetChannel(int chn)
{
    TLG_REG_LOG((b, "TLGAPP_SetChn # %d\n", chn));
    return TLGAPP_SetChannelAux(chn, false );
}


TLGDLL_API int TLGAPP_FastSetChannel(int chn)
{
    return TLGAPP_SetChannelAux(chn, true );
}


p_tlg_channel_t(tlg_scan_map)           = NULL;
int             tlg_scan_map_max        = 0;
int             tlg_scan_map_i          = 0;

int             tlg_fm_step_size_hz     = 0;


TLGDLL_API int TLGAPP_StartChannelScan(p_tlg_channel_t(scan_map), int max_scan)
{
    int result = TLG_ERR_SUCCESS;

    
    if (max_scan < 0) return TLG_ERR_PARAM;
    tlg_scan_map            = scan_map;
    tlg_scan_map_max        = max_scan;
    tlg_scan_map_i          = 0;

    
    tlg_prev_fm_ssi         = 0;
    tlg_prev_fm_chn         = 0;
    if (tlg_cur_mode == TLG_MODE_FM_RADIO)
    {
        
        if (max_scan < 2) return TLG_ERR_PARAM;
        tlg_fm_step_size_hz = p_tlg_cur_map[1] HZ_S - p_tlg_cur_map[0] HZ_S;
        TLG_Set_FM_StepSize(tlg_i2c_addr, tlg_fm_step_size_hz);
    }

    tlg_scan_state = true;
    TLG_StartChannelScan(tlg_i2c_addr);
    return (result);
}


TLGDLL_API int TLGAPP_StopChannelScan(void)
{
    int result = TLG_ERR_SUCCESS;
    TLG_StopChannelScan(tlg_i2c_addr);
    tlg_scan_state = false;

    if (tlg_scan_map && tlg_scan_map_i < tlg_scan_map_max)
    {
        tlg_scan_map[tlg_scan_map_i] CHN_S   = 0;
        tlg_scan_map[tlg_scan_map_i] HZ_S    = 0;
        tlg_scan_map[tlg_scan_map_i] BW_S    = 0;
        tlg_scan_map[tlg_scan_map_i] STD_S   = 0;
    }
    return(result);
}




TLGDLL_API int TLGAPP_ScanNextChannel(int cur_chn_i, int *ret_locked, int *ret_chn_i)
{
    int result = TLG_ERR_SUCCESS;
    int locked = 0;

    if (ret_locked)   *ret_locked = false;
    if (ret_chn_i)    *ret_chn_i  = cur_chn_i;

    
    if (p_tlg_cur_map[cur_chn_i] CHN_S == 0)
    {
        if (ret_chn_i) *ret_chn_i = 0;              
        return result;
    }

    if (tlg_cur_mode == TLG_MODE_FM_RADIO)
    {
        int new_step_size_hz = 0;
        if (cur_chn_i > 0)
        {
            new_step_size_hz = p_tlg_cur_map[cur_chn_i] HZ_S - p_tlg_cur_map[cur_chn_i - 1] HZ_S;
        }
        if (new_step_size_hz != tlg_fm_step_size_hz)
        {
            tlg_fm_step_size_hz = new_step_size_hz;
            TLG_Set_FM_StepSize(tlg_i2c_addr, tlg_fm_step_size_hz);
        }
    }
    TLGAPP_PreScanChan(cur_chn_i);
    locked = TLGAPP_SetChannelAux(p_tlg_cur_map[cur_chn_i] CHN_S, g_fast_scan_mode);
    if (locked || tlg_prev_fm_ssi)
    {
        int schn_i = cur_chn_i;
        
        if (!locked)
        {
            schn_i = tlg_prev_fm_chn;
            tlg_prev_fm_chn = 0;
            tlg_prev_fm_ssi = 0;
        } 
        else if (tlg_cur_mode == TLG_MODE_FM_RADIO) 
        {
            
            if (!tlg_prev_fm_ssi || tlg_prev_fm_ssi < tlg_cur_fm_ssi)
            {
                tlg_prev_fm_ssi = tlg_cur_fm_ssi;
                tlg_prev_fm_chn = cur_chn_i;
                return(result);
            } else { 
                schn_i          = tlg_prev_fm_chn;
                tlg_prev_fm_chn = 0;
                tlg_prev_fm_ssi = 0;
            }
        }
        if (ret_locked) *ret_locked = true;
        if (ret_chn_i)  *ret_chn_i  = schn_i;


        if (tlg_scan_map)
        {
            
            if (tlg_scan_map_i >= (tlg_scan_map_max-1)) return TLG_ERR_NO_MEM;

            tlg_scan_map[tlg_scan_map_i] CHN_S = p_tlg_cur_map[schn_i] CHN_S;
            tlg_scan_map[tlg_scan_map_i] HZ_S  = p_tlg_cur_map[schn_i] HZ_S;
            tlg_scan_map[tlg_scan_map_i] BW_S  = p_tlg_cur_map[schn_i] BW_S;
            tlg_scan_map[tlg_scan_map_i] STD_S = p_tlg_cur_map[schn_i] STD_S;
        }
        
        TLGAPP_ScanChanFound(schn_i);

        tlg_scan_map_i++;
    }
    return result;
}


TLGDLL_API void TLGAPP_ScanChannels(p_tlg_channel_t(scan_map), int max_scan)
{
    int tlg_cur_map_i;

    TLGAPP_StartChannelScan(scan_map, max_scan);

    for(tlg_cur_map_i=0; p_tlg_cur_map[tlg_cur_map_i] CHN_S != 0; tlg_cur_map_i++)    
    {
        TLGAPP_ScanNextChannel(tlg_cur_map_i, NULL, NULL); 
    }

    TLGAPP_StopChannelScan();
}


TLGDLL_API void TLGAPP_SeekNextChn(int chn)
{
    int chni;

    int cnt=0;
    tlg_prev_fm_ssi = 0;
    tlg_prev_fm_chn = 0;
    if (p_tlg_cur_map == NULL) return;
    chni = chn;
    while (cnt < 2) {
        int locked;
        chni++;
        if (p_tlg_cur_map[chni] CHN_S == 0) {
            cnt++;
            chni = 0;
        }
        locked = TLGAPP_SetChannelAux(p_tlg_cur_map[chni] CHN_S, g_fast_scan_mode);
        if (locked || tlg_prev_fm_ssi) {
            int schn = chni;
            
            if (!locked) {
                schn = tlg_prev_fm_chn;
                tlg_prev_fm_chn = 0;
                tlg_prev_fm_ssi = 0;
            } else if (tlg_cur_mode == TLG_MODE_FM_RADIO) {
                
                if (!tlg_prev_fm_ssi || tlg_prev_fm_ssi < tlg_cur_fm_ssi) {
                    tlg_prev_fm_ssi = tlg_cur_fm_ssi;
                    tlg_prev_fm_chn = chni;
                    continue;
                } else { 
                    schn = tlg_prev_fm_chn;
                    tlg_prev_fm_chn = 0;
                    tlg_prev_fm_ssi = 0;
                }
            }
            if (schn != chni) 
            {
                locked = TLGAPP_SetChannelAux(p_tlg_cur_map[chni] CHN_S, g_fast_scan_mode);
            }
            break;
        }
    }
}


TLGDLL_API void TLGAPP_SeekPrevChn(int chn)
{
    int chni;

    int cnt=0;
    tlg_prev_fm_ssi = 0;
    tlg_prev_fm_chn = 0;
    if (p_tlg_cur_map == NULL) return;
    chni = chn;
    while (cnt < 2) {
        int locked;
        chni--;
        if (chni < 0) {
            cnt++;
            chni = tlg_cur_chn_cnt - 1;
        }
        locked = TLGAPP_SetChannelAux(p_tlg_cur_map[chni] CHN_S, g_fast_scan_mode);
        if (locked || tlg_prev_fm_ssi) {
            int schn = chni;
            
            if (!locked) {
                schn = tlg_prev_fm_chn;
                tlg_prev_fm_chn = 0;
                tlg_prev_fm_ssi = 0;
            } else if (tlg_cur_mode == TLG_MODE_FM_RADIO) {
                
                if (!tlg_prev_fm_ssi || tlg_prev_fm_ssi < tlg_cur_fm_ssi) {
                    tlg_prev_fm_ssi = tlg_cur_fm_ssi;
                    tlg_prev_fm_chn = chni;
                    continue;
                } else { 
                    schn = tlg_prev_fm_chn;
                    tlg_prev_fm_chn = 0;
                    tlg_prev_fm_ssi = 0;
                }
            }
            if (schn != chni) 
            {
                locked = TLGAPP_SetChannelAux(p_tlg_cur_map[chni] CHN_S, g_fast_scan_mode);
            }
            break;
        }
    }
}


TLGDLL_API void TLGAPP_TurnOnTestPattern(void)
{
    if (!tlg_init_done) TLGAPP_Init(tlg_i2c_addr);
    TLG_SetVideoTestPatternState(tlg_i2c_addr, TLG_ON);
}


TLGDLL_API void TLGAPP_TurnOffTestPattern(void)
{
    if (!tlg_init_done) TLGAPP_Init(tlg_i2c_addr);
    TLG_SetVideoTestPatternState(tlg_i2c_addr, TLG_OFF);
}


TLGDLL_API void TLGAPP_TurnOnTestToneMode(void)
{
        
        TLG_SetAudioGain(tlg_i2c_addr, tlg_cur_aud_mode, TLG_MIN_AUD_GAIN);
        TLG_SetAudioTestToneMode(tlg_i2c_addr, TLG_ON);
}


TLGDLL_API void TLGAPP_TurnOffTestToneMode(void)
{
        
        TLG_SetAudioGain(tlg_i2c_addr, tlg_cur_aud_mode, tlg_cur_gain);
        TLG_SetAudioTestToneMode(tlg_i2c_addr, TLG_OFF);
}

/*********************************************************/
//ADDED BY spt
p_tlg_channel_t(p_tlg_scan_map) = NULL;
static uint16 max_scan_chn = 0;
static uint16 total_scan_chn = 0;
static uint16 scan_chn_index = 0;
static uint16 found_chn_index = 0;
static uint16 found_fm_freq = 0;

void TLGAPP_InitScanParamUser(p_tlg_channel_t(tlg_scan_map), int max_scan)
{/*lint !e578*/
	uint16 i = 0;
	
	p_tlg_scan_map = tlg_scan_map;
	max_scan_chn = max_scan;
	scan_chn_index = 0;
	found_chn_index = 0;
	tlg_prev_fm_ssi = 0;
	tlg_prev_fm_chn = 0;
    total_scan_chn = 0;

	for(i = 0;i<=125;i++)
		{
		if(p_tlg_cur_map[i] CHN_S == 0)
			break;
		else
			total_scan_chn++;
		}
}

uint16 TLGAPP_GetTotalSearchNum(void)
{
	return total_scan_chn;
}

uint16 TLGAPP_GetCurrSearchNum(void)
{
	return scan_chn_index;
}

//TRUE:confirm FALSE:not found
TLGDLL_API BOOLEAN TLGAPP_ScanOneChnUser(uint32* chn_id, uint32* is_valid)
{
	 int locked;

	 if(max_scan_chn == 0)//parameter is out of range
	 	return TRUE;

#ifdef TLG_USE_FAST_LOCK_DETECT
	locked = TLGAPP_FastSetChannel(p_tlg_cur_map[scan_chn_index] CHN_S);
#else
	locked = TLGAPP_SetChannel(p_tlg_cur_map[scan_chn_index] CHN_S);
#endif /* TLG_USE_FAST_LOCK_DETECT */

	if (locked)
	{
		int schn = scan_chn_index;

		/* leave room for 0-valued entry to terminate channel map */
		(*(p_tlg_scan_map + found_chn_index)) CHN_S = p_tlg_cur_map[schn] CHN_S;
		(*(p_tlg_scan_map + found_chn_index)) HZ_S  = p_tlg_cur_map[schn] HZ_S;
		(*(p_tlg_scan_map + found_chn_index)) BW_S  = p_tlg_cur_map[schn] BW_S;
		(*(p_tlg_scan_map + found_chn_index)) STD_S = p_tlg_cur_map[schn] CHN_S;

        *chn_id=p_tlg_cur_map[schn] CHN_S;
        *is_valid=TRUE;

		/* customizable callback routine                          */
		TLGAPP_ScanChanFound(p_tlg_cur_map[schn] CHN_S);
		found_chn_index++;
		return TRUE;
	}
	return FALSE;
}

//TRUE:complete FALSE:need to continue
TLGDLL_API BOOLEAN TLGAPP_ScanChnUser(uint32* chn_id, uint32* is_valid)
{
	BOOLEAN ret = FALSE;

	 if(max_scan_chn == 0)//parameter is out of range
	 	return TRUE;

	 if(p_tlg_cur_map[scan_chn_index] CHN_S == 0)
		return TRUE;

	tlg_prev_fm_ssi = 0;
	tlg_prev_fm_chn = 0;

	ret = TLGAPP_ScanOneChnUser(chn_id, is_valid);

	if(ret)//one channel has been found
		{
		if (found_chn_index >= (max_scan_chn-1))
			{
			p_tlg_scan_map[found_chn_index+1] CHN_S = 0;
			p_tlg_scan_map[found_chn_index+1] HZ_S = 0;
			p_tlg_scan_map[found_chn_index+1] BW_S = 0;
			p_tlg_scan_map[found_chn_index+1] STD_S = 0;
		  	return TRUE;
			}
		}
	
	scan_chn_index++;

	return FALSE;
}

TLGDLL_API uint32 TLGAPP_TestGetFreq(uint8 index)
{
	if(index > 8)
		return FALSE;

	return tlg_test_chn_map[index] HZ_S;
}

TLGDLL_API BOOLEAN TLGAPP_TestLockFreq(uint8 index)
{
	 int locked;
	 
	if(index > 8)
		return FALSE;

#ifdef TLG_USE_FAST_LOCK_DETECT
	locked = TLGAPP_FastSetChannel(tlg_test_chn_map[index] CHN_S);
#else
	locked = TLGAPP_SetChannel(tlg_test_chn_map[index] CHN_S);
#endif /* TLG_USE_FAST_LOCK_DETECT */

	if ( locked )
		return TRUE;
	else
		return FALSE;
}

//TRUE:confirm FALSE:not found
TLGDLL_API int16 TLGAPP_FindFmChn(int16 ch)
{
	 int locked;

#ifdef TLG_USE_FAST_LOCK_DETECT
	locked = TLGAPP_FastSetChannel(ch);
#else
	locked = TLGAPP_SetChannel(ch);
#endif /* TLG_USE_FAST_LOCK_DETECT */

	if ( locked )
		return TRUE;
	else
		return FALSE;
}

//dir:true up;false down   currFreq:current valid frequeny;return: ch number
TLGDLL_API uint32 TLGAPP_GetNextValidFreq(BOOLEAN dir,uint32 currFreq)
{

#ifdef TLG_FM_NEW_INTERFACE

	if((currFreq == FM_MAX_FREQ)&&dir)
	{
		return FM_MIN_FREQ;
	}
	else if((currFreq == FM_MIN_FREQ)&&(!dir))
	{
		return FM_MAX_FREQ;
	}
	else
	{
		if(dir)
			{
			return currFreq+TLG_FM_DEFAULT_STEP;
			}
		else
			{
			return currFreq-TLG_FM_DEFAULT_STEP;
			}
	}
#else
	int16 index = 0;
	BOOLEAN ret = FALSE;
	int16 limit;
	uint32 dif;

	if((currFreq<FM_MIN_FREQ) || (currFreq>FM_MAX_FREQ))
		return 0;
	
	limit = sizeof(tlg_fm_chn_map)/(sizeof(uint32)*4);
	limit--;//the last one is a invalid member

	if(currFreq == FM_MIN_FREQ)
	{
		if(dir)
			index = 1;
		else
			index = (limit-1);
	}
	else if(currFreq == FM_MAX_FREQ)
	{
		if(dir)
			index = 0;
		else
			index = (limit-2);
	}
	else
	{
	while(p_tlg_cur_map[index] CHN_S != 0)
		{
		if(index >= (limit-1))
			break;
		
		if(dir)
			{
			if((currFreq >= p_tlg_cur_map[index] HZ_S)&&(currFreq < p_tlg_cur_map[index+1] HZ_S))
				{
				index += 1;
				break;
				}
			}
		else
			{
			if((currFreq > p_tlg_cur_map[index] HZ_S)&&(currFreq <= p_tlg_cur_map[index+1] HZ_S))
				{
				break;
				}
			}
		
		index++;
		}
	}

	return p_tlg_cur_map[index] HZ_S;
#endif
}

//dir:true up;false down   currFreq:current valid frequeny;return: lock result
TLGDLL_API BOOLEAN TLGAPP_FMScanChnUser(uint32 currFreq)
{
	int16 index = 0;
	BOOLEAN ret = FALSE;
	int16 limit;

	if((currFreq<FM_MIN_FREQ) || (currFreq>FM_MAX_FREQ))
		return 0;
#ifdef TLG_FM_NEW_INTERFACE
	ret =TLGAPP_FMSetFrequency(currFreq);
#else
	while(p_tlg_cur_map[index] CHN_S != 0)
		{
		if(currFreq == p_tlg_cur_map[index] HZ_S)
			break;

		index++;
		}
	
	if(p_tlg_cur_map[index] CHN_S == 0)
		return 0;//param error
	
	ret = TLGAPP_FindFmChn( p_tlg_cur_map[index] CHN_S );
#endif
	if(ret)//one channel has been found
		//return (p_tlg_cur_map[index] HZ_S);
		return TRUE;
	else
		return FALSE;
}

//return value:0(failed)
TLGDLL_API BOOLEAN TLGAPP_SetChnByFreq(uint32 freq)
{
	int result;

	if((freq<FM_MIN_FREQ) || (freq>FM_MAX_FREQ))
		return FALSE;
	
	/* change LNA if required */
	TLGAPP_SetLnaGpio(freq);

	/* update bw if needed */
	TLGAPP_ChannelSetup(tlg_cur_mode, TLG_VSTD_NTSC_M, 6);

	result = TLG_ScanFMChn(tlg_i2c_addr, freq, tlg_slope, tlg_offset);

	return TRUE;
}
/*********************************************************/

#ifdef  SUPPORT_CHANNEL_MAP_SHENZHEN
TLGDLL_API int TLGAPP_SetChannelMapShenzhen(void)
{
    return TLGAPP_SetChannelMap(tlg_palI_sz_chn_map, TLG_MODE_ANALOG_TV_UNCOMP, 
                         TLG_VSTD_PAL_I, TLG_BW_8);
}
#endif 

#ifdef SUPPORT_CHANNEL_MAP_ARGENTINA

TLGDLL_API int TLGAPP_SetChannelMapArgentina(void)
{
    return TLGAPP_SetChannelMap(tlg_argentina_pal_nc_chn_map, TLG_MODE_ANALOG_TV_UNCOMP,
                         TLG_VSTD_PAL_NC,TLG_BW_6);
}
#endif 

#ifdef SUPPORT_CHANNEL_MAP_BRAZIL

TLGDLL_API int TLGAPP_SetChannelMapBrazil(void)
{
    return TLGAPP_SetChannelMap(tlg_brazil_pal_m_chn_map, TLG_MODE_ANALOG_TV_UNCOMP,
                         TLG_VSTD_PAL_M,TLG_BW_6);
}
#endif 

#ifdef SUPPORT_CHANNEL_MAP_UK

TLGDLL_API int TLGAPP_SetChannelMapUK(void)
{
    return TLGAPP_SetChannelMap(tlg_pal_i_chn_map, TLG_MODE_ANALOG_TV_UNCOMP,
                         TLG_VSTD_PAL_I,TLG_BW_8);
}
#endif 

#ifdef SUPPORT_CHANNEL_MAP_JAPAN

TLGDLL_API int TLGAPP_SetChannelMapJapan(void)
{
     return TLGAPP_SetChannelMap(tlg_ntsc_japan_chn_map, TLG_MODE_ANALOG_TV_UNCOMP,
                         TLG_VSTD_NTSC_M,TLG_BW_6);
}
#endif 

#ifdef SUPPORT_CHANNEL_MAP_PALWE

TLGDLL_API int TLGAPP_SetChannelMapPALWE(void)
{
    return TLGAPP_SetChannelMap(tlg_pal_b_g_chn_map, TLG_MODE_ANALOG_TV_UNCOMP,
                         TLG_VSTD_PAL_B,TLG_BW_7);
}

TLGDLL_API int TLGAPP_SetChannelMapPAL_SECAM_b_g_Hybrid(void)
{
    return TLGAPP_SetChannelMap(tlg_pal_b_g_hybrid_chn_map, TLG_MODE_ANALOG_TV_UNCOMP,
                         TLG_VSTD_PAL_SECAM_B,TLG_BW_7);
}
#endif 

#ifdef SUPPORT_CHANNEL_MAP_VIETNAM

TLGDLL_API int TLGAPP_SetChannelMapVietnam(void)
{
    return TLGAPP_SetChannelMap(tlg_vietnam_pal_d_k_chn_map, TLG_MODE_ANALOG_TV_UNCOMP,
                         TLG_VSTD_PAL_D,TLG_BW_6);
}
#endif 

#ifdef SUPPORT_CHANNEL_MAP_SECAM_BG

TLGDLL_API int TLGAPP_SetChannelMap_SECAM_b_g(void)
{
    return TLGAPP_SetChannelMap(tlg_secam_b_g_chn_map, TLG_MODE_ANALOG_TV_UNCOMP,
                         TLG_VSTD_SECAM_B, TLG_BW_7);
}
#endif 

#ifdef SUPPORT_CHANNEL_MAP_SECAM_DK

TLGDLL_API int TLGAPP_SetChannelMap_SECAM_d_k(void)
{
    return TLGAPP_SetChannelMap(tlg_secam_d_k_chn_map, TLG_MODE_ANALOG_TV_UNCOMP,
                         TLG_VSTD_SECAM_D, TLG_BW_8);
}


TLGDLL_API int TLGAPP_SetChannelMapPAL_SECAM_d_k_Hybrid(void)
{
    return TLGAPP_SetChannelMap(tlg_secam_d_k_hybrid_chn_map, TLG_MODE_ANALOG_TV_UNCOMP,
                         TLG_VSTD_PAL_SECAM_D, TLG_BW_8);
}

#endif 


TLGDLL_API int TLGAPP_CC_Control   (
    uint16 start_stop,
    uint16 drop_nulls ,
    uint16 pass_parity)
{
    tlg_cc_attrib_t cc_attrib;
    tlg_cc_field_t  field_0;
    tlg_cc_field_t  field_1;
    tlg_vidstd_t    video_std;

    TLG_GetChnStandard(tlg_i2c_addr, (uint16 *) &video_std);
    if (start_stop)
    {
        TLG_CC_GetAttrib(tlg_i2c_addr, &cc_attrib, &field_0, &field_1);
        switch (video_std)
        {
            case TLG_VSTD_NTSC_M    :
                field_0.cc_line_start = 21;
                field_0.cc_line_stop  = 21;
                field_1.cc_line_start = 284;
                field_1.cc_line_stop  = 284;
                break;
            case TLG_VSTD_PAL_M     :
            case TLG_VSTD_PAL_B     :
            case TLG_VSTD_PAL_B1    :
            case TLG_VSTD_PAL_D     :
            case TLG_VSTD_PAL_D1    :
            case TLG_VSTD_PAL_G     :
            case TLG_VSTD_PAL_H     :
            case TLG_VSTD_PAL_K     :
            case TLG_VSTD_PAL_N     :
            case TLG_VSTD_PAL_I     :
            case TLG_VSTD_PAL_NC    :
            case TLG_VSTD_SECAM_M   :
            case TLG_VSTD_SECAM_B   :
            case TLG_VSTD_SECAM_B1  :
            case TLG_VSTD_SECAM_D   :
            case TLG_VSTD_SECAM_D1  :
            case TLG_VSTD_SECAM_G   :
            case TLG_VSTD_SECAM_H   :
            case TLG_VSTD_SECAM_K   :
            case TLG_VSTD_SECAM_N   :
            case TLG_VSTD_SECAM_I   :
            case TLG_VSTD_SECAM_NC  :
            case TLG_VSTD_SECAM_L   :
            case TLG_VSTD_PAL_SECAM_M  :
            case TLG_VSTD_PAL_SECAM_B  :
            case TLG_VSTD_PAL_SECAM_B1 :
            case TLG_VSTD_PAL_SECAM_D  :
            case TLG_VSTD_PAL_SECAM_D1 :
            case TLG_VSTD_PAL_SECAM_G  :
            case TLG_VSTD_PAL_SECAM_H  :
            case TLG_VSTD_PAL_SECAM_K  :
            case TLG_VSTD_PAL_SECAM_N  :
            case TLG_VSTD_PAL_SECAM_I  :
            case TLG_VSTD_PAL_SECAM_NC :
                field_0.cc_line_start = 22;
                field_0.cc_line_stop  = 22;
                field_1.cc_line_start = 335;
                field_1.cc_line_stop  = 335;
                break;
            default:
                return TLG_ERR_PARAM;
        }
        cc_attrib.cc_drop_nulls     = drop_nulls ;
        cc_attrib.cc_pass_parity    = pass_parity;
        TLG_CC_SetAttrib(tlg_i2c_addr, &cc_attrib, &field_0, &field_1);
    }
    TLG_CC_Control(tlg_i2c_addr, start_stop, start_stop);
    return(TLG_ERR_SUCCESS);
}

TLGDLL_API int TLGAPP_CC_GetRawData(
    uint16                  field_number,
    uint16                 *p_length ,
    uint8                  *p_data
)
{
    return TLG_CC_GetRawData(tlg_i2c_addr, field_number, p_length, p_data);
}


TLGDLL_API int TLGAPP_SetAppInitStatus(int init_done)
{
    tlg_init_done = init_done;
    return(TLG_ERR_SUCCESS);
}

TLGDLL_API void *TLGAPP_GetCurChannelMap(uint16 *p_chn_map_size)
{
    uint16 chni = 0;
    while (p_tlg_cur_map[chni] CHN_S != 0)
    {
        chni++;
    }
    *p_chn_map_size = chni + 1;
    return(p_tlg_cur_map);
}

TLGDLL_API void TLGAPP_SetFastScanMode(uint16 fast_scan_mode)
{
    g_fast_scan_mode = fast_scan_mode;
}
#ifdef TLG_FM_NEW_INTERFACE

extern uint32 tlg_fm_step_size;


TLGDLL_API int TLGAPP_FMSetParam(uint32 step_size, uint32 min_hz, uint32 max_hz)
{
    
    tlg_fm_step_size = step_size;
#ifndef CHECK_FM_POWER_TLG1100_WAY
    
    TLG_Set_FM_StepSize(tlg_i2c_addr, step_size);
#endif 
    tlg_fm_min_hz = min_hz;
    tlg_fm_max_hz = max_hz;

    if((step_size < TLG_FM_MIN_STEP) || (step_size > TLG_FM_MAX_STEP))
        tlg_fm_step_size = TLG_FM_DEFAULT_STEP;
    
    if((tlg_fm_min_hz < TLG_FM_MIN_HZ) || (tlg_fm_min_hz > TLG_FM_MAX_HZ))
        tlg_fm_min_hz = TLG_FM_MIN_HZ;

    if((tlg_fm_max_hz < TLG_FM_MIN_HZ) || (tlg_fm_max_hz > TLG_FM_MAX_HZ))
        tlg_fm_max_hz = TLG_FM_MAX_HZ;

    tlg_slope       = 0;
    tlg_offset      = 0;
    tlg_cur_chn_cnt = 0;

    TLG_REG_LOG((b,"TLGAPP_FMSetParm (%d khz, %d khz, %d khz )\n", tlg_fm_step_size/1000, min_hz/1000, max_hz/1000));
    TLGAPP_ChannelSetup(TLG_MODE_FM_RADIO, TLG_VSTD_NTSC_M, TLG_BW_6);

    return TLG_InitScanFMChn(tlg_i2c_addr, tlg_fm_min_hz, tlg_fm_max_hz, &tlg_slope, &tlg_offset);
}


TLGDLL_API int TLGAPP_FMSetFrequency(uint32 center_freq_hz)
{
    int result = 0;

    if(tlg_fm_scan)
    {
        result = TLGAPP_FMScanFrequency(center_freq_hz);
    }
    else
    {
        
        TLGAPP_SetLnaGpio(center_freq_hz);

        
        TLGAPP_ChannelSetup(TLG_MODE_FM_RADIO, TLG_VSTD_NTSC_M, TLG_BW_6);


        TLG_REG_LOG((b,"TLGAPP_FMSetFrequency (%d khz ) (%x, %x)\n", center_freq_hz/1000, tlg_slope, tlg_offset));
        result = TLG_ScanFMChn(tlg_i2c_addr, center_freq_hz, tlg_slope, tlg_offset);
    }

    return result;
}

void TLG_SetFMNotches(uint32 base_addr, 
                     uint32 freq_hz);
                     
int TLG_CheckFMPower(
    uint32 base_addr, 
    uint32 freq_hz, 
    uint32* status);

#define TLGAPP_FM_SCAN_WAIT_TIME_MSEC (40)
#define TLGAPP_FM_SCAN_AFE_SETTLING_TIME_MSEC (400)


static int TLGAPP_FMScanFrequency(int32 center_freq_hz)
{
    int32   freqOffset;
    int     result = 0;
    uint32 status = 0;
    int32   searchOffset =  100000; 
    int32   halfMargin   = 275000; 
    int32   halfScanBandwidth = FM_SCAN_HALF_BANDWIDTH - halfMargin - searchOffset;
    
    TLG_REG_LOG((b,"TLGAPP_FMScanFrequency hz=%d START\n", center_freq_hz));
    
    if((center_freq_hz < tlg_fm_min_hz) || (center_freq_hz > tlg_fm_max_hz))
        return 0;
        
    if((center_freq_hz < tlg_fm_scan_set_freq - halfScanBandwidth) 
        || (center_freq_hz > tlg_fm_scan_set_freq + halfScanBandwidth))
    {
        int i;
        tlg_fm_scan_set_freq = 0;

        for(i=1; i<10; i++)
        {
            if(center_freq_hz < tlg_fm_min_hz + i*halfScanBandwidth*2)
            {
                tlg_fm_scan_set_freq = tlg_fm_min_hz + i*halfScanBandwidth*2 - halfScanBandwidth;
                break;
            }
        }

        
        TLGAPP_SetLnaGpio(tlg_fm_scan_set_freq);

        
        TLGAPP_ChannelSetup(TLG_MODE_FM_RADIO, TLG_VSTD_NTSC_M, TLG_BW_6);
        TLG_REG_LOG((b,"TLGAPP_FMScanFrequency (%d khz ) (%x, %x)\n", tlg_fm_scan_set_freq/1000, tlg_slope, tlg_offset));

        
        TLG_SetFMChnFreq(tlg_i2c_addr, tlg_fm_scan_set_freq+2750000, tlg_slope, tlg_offset);
        TLG_Delay(TLGAPP_FM_SCAN_AFE_SETTLING_TIME_MSEC);
    }

    freqOffset = center_freq_hz - tlg_fm_scan_set_freq;
    TLG_SetFMNotches( tlg_i2c_addr, freqOffset);
    TLG_SoftReset(tlg_i2c_addr);
    TLG_Delay(TLGAPP_FM_SCAN_WAIT_TIME_MSEC);
    result = TLG_CheckFMPower(tlg_i2c_addr, freqOffset, &status);
    log_printf(("\thz=%d\n", center_freq_hz));
    TLG_REG_LOG((b,"TLGAPP_FMScanFrequency: %d - freq=%d khz, set_freq=%d khz, freqOffset=%d khz\n", 
                result, center_freq_hz/1000, tlg_fm_scan_set_freq/1000, freqOffset/1000));
    return ((result > 0) ? 1: 0);
}


TLGDLL_API void TLGAPP_FMStartScan()
{
    TLG_REG_LOG((b,"TLGAPP_FMStartScan \n"));
    tlg_fm_scan= 1;
}


TLGDLL_API void TLGAPP_FMStopScan()
{
    TLG_REG_LOG((b,"TLGAPP_FMStopScan \n"));
    tlg_fm_scan= 0;
    tlg_fm_scan_set_freq = 0;
}


TLGDLL_API int TLGAPP_FMGetCurrentSSI()
{
    TLG_REG_LOG((b,"TLGAPP_FMGetCurrentSSI: %d \n", tlg_cur_fm_ssi));
    return tlg_cur_fm_ssi;
}


#endif 



TLGDLL_API tlg_errcode_t TLGAPP_SetFrequency(uint32_t frequency)
{
    tlg_errcode_t ret_val = TLG_ERR_SUCCESS;

    
    TLGAPP_SetLnaGpio(frequency);

    if (tlg_cur_mode == TLG_MODE_ANALOG_TV_UNCOMP)
    {
        if (tlg_scan_state)
        {
            ret_val = TLG_FastScanTVChn(tlg_i2c_addr, frequency);
#ifdef HYBRID_DK_I
            if(tlg_cur_std == TLG_VSTD_PAL_D)
            {
                tlg_cur_std = TLG_setAlternateAudioOffset(tlg_i2c_addr);
            }
#endif 
        }
        else
        {
            int hybridMode = tlg_cur_std & TLG_VSTD_HYBRID_BIT;
                        
            TLGAPP_ChannelSetup(tlg_cur_mode, tlg_cur_std, tlg_cur_bw);            
            ret_val = TLG_ScanTVChn(tlg_i2c_addr, frequency);
#ifdef HYBRID_DK_I
            if(tlg_cur_std == TLG_VSTD_PAL_D)
            {
                tlg_cur_std = TLG_setAlternateAudioOffset(tlg_i2c_addr);
            }
#endif 
#ifndef TLG_SUPPORT_HYBRID_POLL
            if (hybridMode && (ret_val != 0))
            {
                ret_val = TLG_SetAlternateStandard(tlg_i2c_addr);
            }
#endif  
        }
    }
    else if (tlg_cur_mode == TLG_MODE_FM_RADIO)
    {
        if(tlg_fm_scan)
        {
            ret_val = TLGAPP_FMScanFrequency(frequency);
        }
        else
        {
            
            TLGAPP_ChannelSetup(TLG_MODE_FM_RADIO, TLG_VSTD_NTSC_M, TLG_BW_6);

            ret_val = TLG_ScanFMChn(tlg_i2c_addr, frequency, tlg_slope, tlg_offset);
        }
    }
    return ret_val;
}



TLGDLL_API tlg_errcode_t TLGAPP_SetBandwidth(uint32_t bw)
{
    return TLGAPP_ChannelSetup(tlg_cur_mode, tlg_cur_std, bw);
}



TLGDLL_API tlg_errcode_t TLGAPP_SetStd(uint32_t std)
{
    int standard;
    switch (std)
    {
        case (TLG_TUNE_VSTD_PAL_B):
            standard = TLG_VSTD_PAL_B;
            break;
        case (TLG_TUNE_VSTD_PAL_D):      
            standard = TLG_VSTD_PAL_D;
            break;
        case (TLG_TUNE_VSTD_PAL_G):
            standard = TLG_VSTD_PAL_G;
            break;
        case (TLG_TUNE_VSTD_PAL_H):
            standard = TLG_VSTD_PAL_H;
            break;
        case (TLG_TUNE_VSTD_PAL_I):
            standard = TLG_VSTD_PAL_I;
            break;
        case (TLG_TUNE_VSTD_PAL_K):
            standard = TLG_VSTD_PAL_K;
            break;
        case (TLG_TUNE_VSTD_PAL_M):
            standard = TLG_VSTD_PAL_M;
            break;
        case (TLG_TUNE_VSTD_PAL_N):
        case (TLG_TUNE_VSTD_PAL_NC):        
            standard = TLG_VSTD_PAL_NC;
            break;
        case (TLG_TUNE_VSTD_SECAM_B):
            standard = TLG_VSTD_SECAM_B;
            break;
        case (TLG_TUNE_VSTD_SECAM_D):       
            standard = TLG_VSTD_SECAM_D;
            break;
        case (TLG_TUNE_VSTD_SECAM_G):
            standard = TLG_VSTD_SECAM_G;
            break;
        case (TLG_TUNE_VSTD_SECAM_H):
            standard = TLG_VSTD_SECAM_H;
            break;
        case (TLG_TUNE_VSTD_SECAM_K):
        case (TLG_TUNE_VSTD_SECAM_K1):
            standard = TLG_VSTD_SECAM_K;
            break;
        case (TLG_TUNE_VSTD_PAL_B | TLG_TUNE_VSTD_SECAM_B):
            standard = TLG_VSTD_PAL_SECAM_B;
            break;
        case (TLG_TUNE_VSTD_PAL_G | TLG_TUNE_VSTD_SECAM_G):
            standard = TLG_VSTD_PAL_SECAM_G;
            break;
        case (TLG_TUNE_VSTD_PAL_D | TLG_TUNE_VSTD_SECAM_D):
            standard = TLG_VSTD_PAL_SECAM_D;
            break;
        case (TLG_TUNE_VSTD_PAL_K | TLG_TUNE_VSTD_SECAM_K):
            standard = TLG_VSTD_PAL_SECAM_K;
            break;            
        case (TLG_TUNE_VSTD_NTSC_M):
        default:
            standard = TLG_VSTD_NTSC_M;
            break;
    }

    return TLGAPP_ChannelSetup(tlg_cur_mode, standard, tlg_cur_bw);
}



TLGDLL_API tlg_errcode_t TLGAPP_SetMode(uint32_t mode)
{
    return TLGAPP_ChannelSetup(mode, tlg_cur_std, tlg_cur_bw);
}



TLGDLL_API tlg_errcode_t TLGAPP_ScanStart(void)
{
    tlg_errcode_t ret_value;

    tlg_fm_scan= 1;
    tlg_scan_state = true;
    ret_value = TLG_StartChannelScan(tlg_i2c_addr);

    return ret_value;
}



TLGDLL_API tlg_errcode_t TLGAPP_ScanStop(void)
{
    tlg_errcode_t ret_value;

    tlg_scan_state = false;
    tlg_fm_scan= 0;
    tlg_fm_scan_set_freq = 0;

    ret_value = TLG_StopChannelScan(tlg_i2c_addr);
        
    return ret_value; 
}


#endif 
