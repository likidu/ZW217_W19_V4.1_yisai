/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/


#ifndef _SPI_REG_UWS6121E_H_
#define _SPI_REG_UWS6121E_H_

#undef uint32_t
#define uint32_t uint32

#define REG_AP_SPI_BASE (0x04008000)
#define REG_AON_SPI_BASE	(0x5150c0000)

typedef volatile struct
{
    uint32_t spi_txd;          // 0x00000000
    uint32_t spi_clkd;         // 0x00000004
    uint32_t spi_ctl0;         // 0x00000008
    uint32_t spi_ctl1;         // 0x0000000c
    uint32_t spi_ctl2;         // 0x00000010
    uint32_t spi_ctl3;         // 0x00000014
    uint32_t spi_ctl4;         // 0x00000018
    uint32_t spi_ctl5;         // 0x0000001c
    uint32_t spi_int_en;       // 0x00000020
    uint32_t spi_int_clr;      // 0x00000024
    uint32_t spi_int_raw_sts;  // 0x00000028
    uint32_t spi_int_mask_sts; // 0x0000002c
    uint32_t spi_sts1;         // 0x00000030
    uint32_t spi_sts2;         // 0x00000034
    uint32_t spi_dspwait;      // 0x00000038
    uint32_t spi_sts3;         // 0x0000003c
    uint32_t spi_ctl6;         // 0x00000040
    uint32_t spi_sts4;         // 0x00000044
    uint32_t spi_fifo_rst;     // 0x00000048
    uint32_t spi_ctl7;         // 0x0000004c
    uint32_t spi_sts5;         // 0x00000050
    uint32_t spi_ctl8;         // 0x00000054
    uint32_t spi_ctl9;         // 0x00000058
    uint32_t spi_ctl10;        // 0x0000005c
    uint32_t spi_ctl11;        // 0x00000060
    uint32_t spi_ctl12;        // 0x00000064
    uint32_t spi_sts6;         // 0x00000068
    uint32_t spi_sts7;         // 0x0000006c
    uint32_t spi_sts8;         // 0x00000070
    uint32_t spi_sts9;         // 0x00000074
    uint32_t spi_version;      // 0x00000078
} HWP_SPI_T;

#define hwp_apSpi ((HWP_SPI_T *)REG_ACCESS_ADDRESS(REG_AP_SPI_BASE))
#define hwp_aonSpi	((HWP_SPI_T *)REG_ACCESS_ADDRESS(REG_AON_SPI_BASE))
// spi_clkd
typedef union {
    uint32_t v;
    struct
    {
        uint32_t spi_clkd : 16; // [15:0]
        uint32_t __31_16 : 16;  // [31:16]
    } b;
} REG_AP_SPI_SPI_CLKD_T;

// spi_ctl0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t ng_rx : 1;         // [0]
        uint32_t ng_tx : 1;         // [1]
        uint32_t chnl_len : 5;      // [6:2]
        uint32_t lsb : 1;           // [7]
        uint32_t spi_csn_pre : 4;   // [11:8]
        uint32_t __12_12 : 1;       // [12]
        uint32_t is_sck_rev : 1;    // [13]
        uint32_t sync_md : 1;       // [14]
        uint32_t sync_3wrd_pol : 1; // [15]
        uint32_t __31_16 : 16;      // [31:16]
    } b;
} REG_AP_SPI_SPI_CTL0_T;

// spi_ctl1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t s3w_pos : 5;      // [4:0]
        uint32_t s3w_md : 1;       // [5]
        uint32_t cs_h_md : 1;      // [6]
        uint32_t s8_md : 1;        // [7]
        uint32_t sync_csn_sel : 4; // [11:8]
        uint32_t is_rxmd : 1;      // [12]
        uint32_t is_txmd : 1;      // [13]
        uint32_t do_hold_en : 2;   // [15:14]
        uint32_t __31_16 : 16;     // [31:16]
    } b;
} REG_AP_SPI_SPI_CTL1_T;

// spi_ctl2
typedef union {
    uint32_t v;
    struct
    {
        uint32_t s3w_rd_strt : 5;     // [4:0]
        uint32_t is_slvd : 1;         // [5]
        uint32_t dma_en : 1;          // [6]
        uint32_t rx_only_nhd : 1;     // [7]
        uint32_t rx_dma_sel : 1;      // [8]
        uint32_t tx_dma_sel : 1;      // [9]
        uint32_t dma_req_seq_sel : 1; // [10]
        uint32_t __31_11 : 21;        // [31:11]
    } b;
} REG_AP_SPI_SPI_CTL2_T;

// spi_ctl3
typedef union {
    uint32_t v;
    struct
    {
        uint32_t rxf_full_thrhld : 5;  // [4:0]
        uint32_t __7_5 : 3;            // [7:5]
        uint32_t rxf_empty_thrhld : 5; // [12:8]
        uint32_t __31_13 : 19;         // [31:13]
    } b;
} REG_AP_SPI_SPI_CTL3_T;

// spi_ctl4
typedef union {
    uint32_t v;
    struct
    {
        uint32_t block_num : 9;       // [8:0]
        uint32_t is_rx_only : 1;      // [9]
        uint32_t sync_half : 1;       // [10]
        uint32_t sync_clkmask_en : 1; // [11]
        uint32_t phs_dly : 2;         // [13:12]
        uint32_t is_fst : 1;          // [14]
        uint32_t rx_only_do : 1;      // [15]
        uint32_t __31_16 : 16;        // [31:16]
    } b;
} REG_AP_SPI_SPI_CTL4_T;

// spi_ctl5
typedef union {
    uint32_t v;
    struct
    {
        uint32_t itvl_num_sam : 16; // [15:0]
        uint32_t __31_16 : 16;      // [31:16]
    } b;
} REG_AP_SPI_SPI_CTL5_T;

// spi_int_en
typedef union {
    uint32_t v;
    struct
    {
        uint32_t rxf_full_int_en : 1;    // [0]
        uint32_t rxf_empty_int_en : 1;   // [1]
        uint32_t txf_full_int_en : 1;    // [2]
        uint32_t txf_empty_en : 1;       // [3]
        uint32_t rx_ovf_int_en : 1;      // [4]
        uint32_t time_out_int_en : 1;    // [5]
        uint32_t rxf_r_full_int_en : 1;  // [6]
        uint32_t txf_w_empty_int_en : 1; // [7]
        uint32_t tx_end_int_en : 1;      // [8]
        uint32_t rx_end_int_en : 1;      // [9]
        uint32_t __31_10 : 22;           // [31:10]
    } b;
} REG_AP_SPI_SPI_INT_EN_T;

// spi_int_clr
typedef union {
    uint32_t v;
    struct
    {
        uint32_t rxf_full_int_clr : 1;  // [0]
        uint32_t rxf_empty_int_clr : 1; // [1]
        uint32_t txf_full_int_clr : 1;  // [2]
        uint32_t txf_empty_int_clr : 1; // [3]
        uint32_t rx_ovf_int_clr : 1;    // [4]
        uint32_t time_out_int_clr : 1;  // [5]
        uint32_t __7_6 : 2;             // [7:6]
        uint32_t tx_end_int_clr : 1;    // [8]
        uint32_t rx_end_int_clr : 1;    // [9]
        uint32_t __31_10 : 22;          // [31:10]
    } b;
} REG_AP_SPI_SPI_INT_CLR_T;

// spi_int_raw_sts
typedef union {
    uint32_t v;
    struct
    {
        uint32_t rx_full_raw_sts : 1;   // [0], read only
        uint32_t rxf_empty_raw_sts : 1; // [1], read only
        uint32_t txf_full_raw_sts : 1;  // [2], read only
        uint32_t tx_fifo_empty_w : 1;   // [3], read only
        uint32_t rx_ovf_raw_sts : 1;    // [4], read only
        uint32_t time_out_raw_sts : 1;  // [5], read only
        uint32_t rxf_full_r : 1;        // [6], read only
        uint32_t txf_empty_w : 1;       // [7], read only
        uint32_t tx_end_irq : 1;        // [8], read only
        uint32_t rx_end_irq : 1;        // [9], read only
        uint32_t __31_10 : 22;          // [31:10]
    } b;
} REG_AP_SPI_SPI_INT_RAW_STS_T;

// spi_int_mask_sts
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __0_0 : 1;               // [0]
        uint32_t rxf_empty_mask_sts : 1;  // [1], read only
        uint32_t txf_full_mask_sts : 1;   // [2], read only
        uint32_t __3_3 : 1;               // [3]
        uint32_t rx_ovf_mask_sts : 1;     // [4], read only
        uint32_t time_out_mask_sts : 1;   // [5], read only
        uint32_t rxf_full_mask_sts : 1;   // [6], read only
        uint32_t txf_empty_mask_sts : 1;  // [7], read only
        uint32_t tx_end_irq_mask_sts : 1; // [8], read only
        uint32_t rx_end_irq_mask_sts : 1; // [9], read only
        uint32_t __31_10 : 22;            // [31:10]
    } b;
} REG_AP_SPI_SPI_INT_MASK_STS_T;

// spi_sts1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t rxf_raddr : 5; // [4:0], read only
        uint32_t __7_5 : 3;     // [7:5]
        uint32_t rxf_waddr : 5; // [12:8], read only
        uint32_t __31_13 : 19;  // [31:13]
    } b;
} REG_AP_SPI_SPI_STS1_T;

// spi_sts2
typedef union {
    uint32_t v;
    struct
    {
        uint32_t rxf_full : 1;       // [0], read only
        uint32_t rxf_empty : 1;      // [1], read only
        uint32_t txf_full : 1;       // [2], read only
        uint32_t txf_empty : 1;      // [3], read only
        uint32_t rxf_real_full : 1;  // [4], read only
        uint32_t rxf_real_empty : 1; // [5], read only
        uint32_t txf_real_full : 1;  // [6], read only
        uint32_t txf_real_empty : 1; // [7], read only
        uint32_t busy : 1;           // [8], read only
        uint32_t spi_rxd : 1;        // [9], read only
        uint32_t spi_txd : 1;        // [10], read only
        uint32_t spi_sck : 1;        // [11], read only
        uint32_t spi_cs : 1;         // [12], read only
        uint32_t __31_13 : 19;       // [31:13]
    } b;
} REG_AP_SPI_SPI_STS2_T;

// spi_dspwait
typedef union {
    uint32_t v;
    struct
    {
        uint32_t spi_dspwait : 4; // [3:0]
        uint32_t rx_data_swt : 2; // [5:4]
        uint32_t tx_data_swt : 2; // [7:6]
        uint32_t __31_8 : 24;     // [31:8]
    } b;
} REG_AP_SPI_SPI_DSPWAIT_T;

// spi_sts3
typedef union {
    uint32_t v;
    struct
    {
        uint32_t rx_cnt : 9;  // [8:0], read only
        uint32_t __31_9 : 23; // [31:9]
    } b;
} REG_AP_SPI_SPI_STS3_T;

// spi_ctl6
typedef union {
    uint32_t v;
    struct
    {
        uint32_t txf_full_thrhld : 5;  // [4:0]
        uint32_t __7_5 : 3;            // [7:5]
        uint32_t txf_empty_thrhld : 5; // [12:8]
        uint32_t __31_13 : 19;         // [31:13]
    } b;
} REG_AP_SPI_SPI_CTL6_T;

// spi_sts4
typedef union {
    uint32_t v;
    struct
    {
        uint32_t txf_raddr : 5; // [4:0], read only
        uint32_t __7_5 : 3;     // [7:5]
        uint32_t txf_waddr : 5; // [12:8], read only
        uint32_t __31_13 : 19;  // [31:13]
    } b;
} REG_AP_SPI_SPI_STS4_T;

// spi_fifo_rst
typedef union {
    uint32_t v;
    struct
    {
        uint32_t spi_fifo_rst : 1; // [0]
        uint32_t __31_1 : 31;      // [31:1]
    } b;
} REG_AP_SPI_SPI_FIFO_RST_T;

// spi_ctl7
typedef union {
    uint32_t v;
    struct
    {
        uint32_t csn_ie_ctl : 1;    // [0]
        uint32_t csn_i_sel : 2;     // [2:1]
        uint32_t spi_mode : 3;      // [5:3]
        uint32_t tx_cmd_set : 1;    // [6]
        uint32_t spi_tx_hld_en : 1; // [7]
        uint32_t spi_rx_hld_en : 1; // [8]
        uint32_t data_in_mode : 1;  // [9]
        uint32_t spi_slv_en : 1;    // [10]
        uint32_t spi_slv_sel : 1;   // [11]
        uint32_t rgb888_en : 1;     // [12]
        uint32_t rgb666_en : 1;     // [13]
        uint32_t rgb565_en : 1;     // [14]
        uint32_t data_line2_en : 1; // [15]
        uint32_t __31_16 : 16;      // [31:16]
    } b;
} REG_AP_SPI_SPI_CTL7_T;

// spi_sts5
typedef union {
    uint32_t v;
    struct
    {
        uint32_t csn_in_sync2 : 1;     // [0], read only
        uint32_t __3_1 : 3;            // [3:1]
        uint32_t csn_in_err_sync2 : 1; // [4], read only
        uint32_t __31_5 : 27;          // [31:5]
    } b;
} REG_AP_SPI_SPI_STS5_T;

// spi_ctl8
typedef union {
    uint32_t v;
    struct
    {
        uint32_t spi_tx_data_len_h : 4; // [3:0]
        uint32_t spi_tx_dumy_len : 6;   // [9:4]
        uint32_t __10_10 : 1;           // [10]
        uint32_t data_line2_sw : 1;     // [11]
        uint32_t rgb_pix_mode : 1;      // [12]
        uint32_t cd_data2_sel : 1;      // [13]
        uint32_t spi_cd_bit2 : 1;       // [14]
        uint32_t spi_cd_bit : 1;        // [15]
        uint32_t __31_16 : 16;          // [31:16]
    } b;
} REG_AP_SPI_SPI_CTL8_T;

// spi_ctl9
typedef union {
    uint32_t v;
    struct
    {
        uint32_t spi_tx_data_len_l : 16; // [15:0]
        uint32_t __31_16 : 16;           // [31:16]
    } b;
} REG_AP_SPI_SPI_CTL9_T;

// spi_ctl10
typedef union {
    uint32_t v;
    struct
    {
        uint32_t spi_rx_data_len_h : 4; // [3:0]
        uint32_t spi_rx_dumy_len : 6;   // [9:4]
        uint32_t __31_10 : 22;          // [31:10]
    } b;
} REG_AP_SPI_SPI_CTL10_T;

// spi_ctl11
typedef union {
    uint32_t v;
    struct
    {
        uint32_t spi_rx_data_len_l : 16; // [15:0]
        uint32_t __31_16 : 16;           // [31:16]
    } b;
} REG_AP_SPI_SPI_CTL11_T;

// spi_ctl12
typedef union {
    uint32_t v;
    struct
    {
        uint32_t sw_rx_req : 1; // [0]
        uint32_t sw_tx_req : 1; // [1]
        uint32_t __31_2 : 30;   // [31:2]
    } b;
} REG_AP_SPI_SPI_CTL12_T;

// spi_sts6
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tx_data_cnt : 16; // [15:0], read only
        uint32_t __31_16 : 16;     // [31:16]
    } b;
} REG_AP_SPI_SPI_STS6_T;

// spi_sts7
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tx_data_cnt : 4;  // [3:0], read only
        uint32_t __9_4 : 6;        // [9:4]
        uint32_t tx_dummy_cnt : 6; // [15:10], read only
        uint32_t __31_16 : 16;     // [31:16]
    } b;
} REG_AP_SPI_SPI_STS7_T;

// spi_sts8
typedef union {
    uint32_t v;
    struct
    {
        uint32_t rx_data_cnt : 16; // [15:0], read only
        uint32_t __31_16 : 16;     // [31:16]
    } b;
} REG_AP_SPI_SPI_STS8_T;

// spi_sts9
typedef union {
    uint32_t v;
    struct
    {
        uint32_t rx_data_cnt : 4;  // [3:0], read only
        uint32_t __9_4 : 6;        // [9:4]
        uint32_t rx_dummy_cnt : 6; // [15:10], read only
        uint32_t __31_16 : 16;     // [31:16]
    } b;
} REG_AP_SPI_SPI_STS9_T;

// spi_version
typedef union {
    uint32_t v;
    struct
    {
        uint32_t spi_version : 16; // [15:0]
        uint32_t __31_16 : 16;     // [31:16]
    } b;
} REG_AP_SPI_SPI_VERSION_T;

// spi_clkd
#define SPI_SPI_CLKD(n) (((n)&0xffff) << 0)

// spi_ctl0
#define SPI_NG_RX (1 << 0)
#define SPI_NG_TX (1 << 1)
#define SPI_CHNL_LEN(n) (((n)&0x1f) << 2)
#define SPI_LSB (1 << 7)
#define SPI_SPI_CSN_PRE(n) (((n)&0xf) << 8)
#define SPI_IS_SCK_REV (1 << 13)
#define SPI_SYNC_MD (1 << 14)
#define SPI_SYNC_3WRD_POL (1 << 15)

// spi_ctl1
#define SPI_S3W_POS(n) (((n)&0x1f) << 0)
#define SPI_S3W_MD (1 << 5)
#define SPI_CS_H_MD (1 << 6)
#define SPI_S8_MD (1 << 7)
#define SPI_SYNC_CSN_SEL(n) (((n)&0xf) << 8)
#define SPI_IS_RXMD (1 << 12)
#define SPI_IS_TXMD (1 << 13)
#define SPI_DO_HOLD_EN(n) (((n)&0x3) << 14)

// spi_ctl2
#define SPI_S3W_RD_STRT(n) (((n)&0x1f) << 0)
#define SPI_IS_SLVD (1 << 5)
#define SPI_DMA_EN (1 << 6)
#define SPI_RX_ONLY_NHD (1 << 7)
#define SPI_RX_DMA_SEL (1 << 8)
#define SPI_TX_DMA_SEL (1 << 9)
#define SPI_DMA_REQ_SEQ_SEL (1 << 10)

// spi_ctl3
#define SPI_RXF_FULL_THRHLD(n) (((n)&0x1f) << 0)
#define SPI_RXF_EMPTY_THRHLD(n) (((n)&0x1f) << 8)

// spi_ctl4
#define SPI_BLOCK_NUM(n) (((n)&0x1ff) << 0)
#define SPI_IS_RX_ONLY (1 << 9)
#define SPI_SYNC_HALF (1 << 10)
#define SPI_SYNC_CLKMASK_EN (1 << 11)
#define SPI_PHS_DLY(n) (((n)&0x3) << 12)
#define SPI_IS_FST (1 << 14)
#define SPI_RX_ONLY_DO (1 << 15)

// spi_ctl5
#define SPI_ITVL_NUM_SAM(n) (((n)&0xffff) << 0)

// spi_int_en
#define SPI_RXF_FULL_INT_EN (1 << 0)
#define SPI_RXF_EMPTY_INT_EN (1 << 1)
#define SPI_TXF_FULL_INT_EN (1 << 2)
#define SPI_TXF_EMPTY_EN (1 << 3)
#define SPI_RX_OVF_INT_EN (1 << 4)
#define SPI_TIME_OUT_INT_EN (1 << 5)
#define SPI_RXF_R_FULL_INT_EN (1 << 6)
#define SPI_TXF_W_EMPTY_INT_EN (1 << 7)
#define SPI_TX_END_INT_EN (1 << 8)
#define SPI_RX_END_INT_EN (1 << 9)

// spi_int_clr
#define SPI_RXF_FULL_INT_CLR (1 << 0)
#define SPI_RXF_EMPTY_INT_CLR (1 << 1)
#define SPI_TXF_FULL_INT_CLR (1 << 2)
#define SPI_TXF_EMPTY_INT_CLR (1 << 3)
#define SPI_RX_OVF_INT_CLR (1 << 4)
#define SPI_TIME_OUT_INT_CLR (1 << 5)
#define SPI_TX_END_INT_CLR (1 << 8)
#define SPI_RX_END_INT_CLR (1 << 9)

// spi_int_raw_sts
#define SPI_RX_FULL_RAW_STS (1 << 0)
#define SPI_RXF_EMPTY_RAW_STS (1 << 1)
#define SPI_TXF_FULL_RAW_STS (1 << 2)
#define SPI_TX_FIFO_EMPTY_W (1 << 3)
#define SPI_RX_OVF_RAW_STS (1 << 4)
#define SPI_TIME_OUT_RAW_STS (1 << 5)
#define SPI_RXF_FULL_R (1 << 6)
#define SPI_TXF_EMPTY_W (1 << 7)
#define SPI_TX_END_IRQ (1 << 8)
#define SPI_RX_END_IRQ (1 << 9)

// spi_int_mask_sts
#define SPI_RXF_EMPTY_MASK_STS (1 << 1)
#define SPI_TXF_FULL_MASK_STS (1 << 2)
#define SPI_RX_OVF_MASK_STS (1 << 4)
#define SPI_TIME_OUT_MASK_STS (1 << 5)
#define SPI_RXF_FULL_MASK_STS (1 << 6)
#define SPI_TXF_EMPTY_MASK_STS (1 << 7)
#define SPI_TX_END_IRQ_MASK_STS (1 << 8)
#define SPI_RX_END_IRQ_MASK_STS (1 << 9)

// spi_sts1
#define SPI_RXF_RADDR(n) (((n)&0x1f) << 0)
#define SPI_RXF_WADDR(n) (((n)&0x1f) << 8)

// spi_sts2
#define SPI_RXF_FULL (1 << 0)
#define SPI_RXF_EMPTY (1 << 1)
#define SPI_TXF_FULL (1 << 2)
#define SPI_TXF_EMPTY (1 << 3)
#define SPI_RXF_REAL_FULL (1 << 4)
#define SPI_RXF_REAL_EMPTY (1 << 5)
#define SPI_TXF_REAL_FULL (1 << 6)
#define SPI_TXF_REAL_EMPTY (1 << 7)
#define SPI_BUSY (1 << 8)
#define SPI_SPI_RXD (1 << 9)
#define SPI_SPI_TXD (1 << 10)
#define SPI_SPI_SCK (1 << 11)
#define SPI_SPI_CS (1 << 12)

// spi_dspwait
#define SPI_SPI_DSPWAIT(n) (((n)&0xf) << 0)
#define SPI_RX_DATA_SWT(n) (((n)&0x3) << 4)
#define SPI_TX_DATA_SWT(n) (((n)&0x3) << 6)

// spi_sts3
#define SPI_RX_CNT(n) (((n)&0x1ff) << 0)

// spi_ctl6
#define SPI_TXF_FULL_THRHLD(n) (((n)&0x1f) << 0)
#define SPI_TXF_EMPTY_THRHLD(n) (((n)&0x1f) << 8)

// spi_sts4
#define SPI_TXF_RADDR(n) (((n)&0x1f) << 0)
#define SPI_TXF_WADDR(n) (((n)&0x1f) << 8)

// spi_fifo_rst
#define SPI_SPI_FIFO_RST (1 << 0)

// spi_ctl7
#define SPI_CSN_IE_CTL (1 << 0)
#define SPI_CSN_I_SEL(n) (((n)&0x3) << 1)
#define SPI_SPI_MODE(n) (((n)&0x7) << 3)
#define SPI_TX_CMD_SET (1 << 6)
#define SPI_SPI_TX_HLD_EN (1 << 7)
#define SPI_SPI_RX_HLD_EN (1 << 8)
#define SPI_DATA_IN_MODE (1 << 9)
#define SPI_SPI_SLV_EN (1 << 10)
#define SPI_SPI_SLV_SEL (1 << 11)
#define SPI_RGB888_EN (1 << 12)
#define SPI_RGB666_EN (1 << 13)
#define SPI_RGB565_EN (1 << 14)
#define SPI_DATA_LINE2_EN (1 << 15)

// spi_sts5
#define SPI_CSN_IN_SYNC2 (1 << 0)
#define SPI_CSN_IN_ERR_SYNC2 (1 << 4)

// spi_ctl8
#define SPI_SPI_TX_DATA_LEN_H(n) (((n)&0xf) << 0)
#define SPI_SPI_TX_DUMY_LEN(n) (((n)&0x3f) << 4)
#define SPI_DATA_LINE2_SW (1 << 11)
#define SPI_RGB_PIX_MODE (1 << 12)
#define SPI_CD_DATA2_SEL (1 << 13)
#define SPI_SPI_CD_BIT2 (1 << 14)
#define SPI_SPI_CD_BIT (1 << 15)

// spi_ctl9
#define SPI_SPI_TX_DATA_LEN_L(n) (((n)&0xffff) << 0)

// spi_ctl10
#define SPI_SPI_RX_DATA_LEN_H(n) (((n)&0xf) << 0)
#define SPI_SPI_RX_DUMY_LEN(n) (((n)&0x3f) << 4)

// spi_ctl11
#define SPI_SPI_RX_DATA_LEN_L(n) (((n)&0xffff) << 0)

// spi_ctl12
#define SPI_SW_RX_REQ (1 << 0)
#define SPI_SW_TX_REQ (1 << 1)

// spi_sts6
#define SPI_SPI_STS6_TX_DATA_CNT(n) (((n)&0xffff) << 0)

// spi_sts7
#define SPI_SPI_STS7_TX_DATA_CNT(n) (((n)&0xf) << 0)
#define SPI_TX_DUMMY_CNT(n) (((n)&0x3f) << 10)

// spi_sts8
#define SPI_SPI_STS8_RX_DATA_CNT(n) (((n)&0xffff) << 0)

// spi_sts9
#define SPI_SPI_STS9_RX_DATA_CNT(n) (((n)&0xf) << 0)
#define SPI_RX_DUMMY_CNT(n) (((n)&0x3f) << 10)

// spi_version
#define SPI_SPI_VERSION(n) (((n)&0xffff) << 0)

#undef uint32_t

#endif
