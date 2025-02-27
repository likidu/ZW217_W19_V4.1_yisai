#ifndef _TGT_AP_PSRAM_CFG_H_
#define _TGT_AP_PSRAM_CFG_H_
//#include "tgt_ap_clock_config.h"
#include "uix8910_clk_cfg.h"

#if !defined(CONFIG_RDA_FPGA)

/*
 * EVB board
 */
#if (_TGT_AP_DDR_CLOCK == 200)
#include "ddr_timing/8910_psram_16x2_200m.h"
#else
#error "Invalid _TGT_AP_DDR_CLOCK value for PSRAM !"
#endif

/*
 * FPGA board
 */
#else
#include "ddr_timing/8910_psram_16x2_60m_fpga.h"
#endif

#endif
