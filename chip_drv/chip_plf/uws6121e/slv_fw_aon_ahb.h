/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
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

#ifndef _SLV_FW_AON_AHB_H_
#define _SLV_FW_AON_AHB_H_

#undef uint32_t
#define uint32_t uint32
// Auto generated by dtools(see dtools.txt for its version).
// Don't edit it manually!

#define REG_SLV_FW_AON_AHB_BASE (0x51302000)

typedef volatile struct
{
    uint32_t port0_default_address_0; // 0x00000000
    uint32_t port_int_en;             // 0x00000004
    uint32_t port_int_clr;            // 0x00000008
    uint32_t port_int_raw;            // 0x0000000c
    uint32_t port_int_fin;            // 0x00000010
    uint32_t rd_sec_0;                // 0x00000014
    uint32_t rd_sec_1;                // 0x00000018
    uint32_t wr_sec_0;                // 0x0000001c
    uint32_t wr_sec_1;                // 0x00000020
    uint32_t id0_first_addr_0;        // 0x00000024
    uint32_t id0_last_addr_0;         // 0x00000028
    uint32_t id0_mstid_0;             // 0x0000002c
    uint32_t id0_mstid_1;             // 0x00000030
    uint32_t id0_mstid_2;             // 0x00000034
    uint32_t id0_mstid_3;             // 0x00000038
    uint32_t id0_mstid_4;             // 0x0000003c
    uint32_t id0_mstid_5;             // 0x00000040
    uint32_t id0_mstid_6;             // 0x00000044
    uint32_t id0_mstid_7;             // 0x00000048
    uint32_t id1_first_addr_0;        // 0x0000004c
    uint32_t id1_last_addr_0;         // 0x00000050
    uint32_t id1_mstid_0;             // 0x00000054
    uint32_t id1_mstid_1;             // 0x00000058
    uint32_t id1_mstid_2;             // 0x0000005c
    uint32_t id1_mstid_3;             // 0x00000060
    uint32_t id1_mstid_4;             // 0x00000064
    uint32_t id1_mstid_5;             // 0x00000068
    uint32_t id1_mstid_6;             // 0x0000006c
    uint32_t id1_mstid_7;             // 0x00000070
    uint32_t id2_first_addr_0;        // 0x00000074
    uint32_t id2_last_addr_0;         // 0x00000078
    uint32_t id2_mstid_0;             // 0x0000007c
    uint32_t id2_mstid_1;             // 0x00000080
    uint32_t id2_mstid_2;             // 0x00000084
    uint32_t id2_mstid_3;             // 0x00000088
    uint32_t id2_mstid_4;             // 0x0000008c
    uint32_t id2_mstid_5;             // 0x00000090
    uint32_t id2_mstid_6;             // 0x00000094
    uint32_t id2_mstid_7;             // 0x00000098
    uint32_t id3_first_addr_0;        // 0x0000009c
    uint32_t id3_last_addr_0;         // 0x000000a0
    uint32_t id3_mstid_0;             // 0x000000a4
    uint32_t id3_mstid_1;             // 0x000000a8
    uint32_t id3_mstid_2;             // 0x000000ac
    uint32_t id3_mstid_3;             // 0x000000b0
    uint32_t id3_mstid_4;             // 0x000000b4
    uint32_t id3_mstid_5;             // 0x000000b8
    uint32_t id3_mstid_6;             // 0x000000bc
    uint32_t id3_mstid_7;             // 0x000000c0
    uint32_t id4_first_addr_0;        // 0x000000c4
    uint32_t id4_last_addr_0;         // 0x000000c8
    uint32_t id4_mstid_0;             // 0x000000cc
    uint32_t id4_mstid_1;             // 0x000000d0
    uint32_t id4_mstid_2;             // 0x000000d4
    uint32_t id4_mstid_3;             // 0x000000d8
    uint32_t id4_mstid_4;             // 0x000000dc
    uint32_t id4_mstid_5;             // 0x000000e0
    uint32_t id4_mstid_6;             // 0x000000e4
    uint32_t id4_mstid_7;             // 0x000000e8
    uint32_t id5_first_addr_0;        // 0x000000ec
    uint32_t id5_last_addr_0;         // 0x000000f0
    uint32_t id5_mstid_0;             // 0x000000f4
    uint32_t id5_mstid_1;             // 0x000000f8
    uint32_t id5_mstid_2;             // 0x000000fc
    uint32_t id5_mstid_3;             // 0x00000100
    uint32_t id5_mstid_4;             // 0x00000104
    uint32_t id5_mstid_5;             // 0x00000108
    uint32_t id5_mstid_6;             // 0x0000010c
    uint32_t id5_mstid_7;             // 0x00000110
    uint32_t clk_gate_bypass;         // 0x00000114
} HWP_SLV_FW_AON_AHB_T;

#define hwp_slvFwAonAhb ((HWP_SLV_FW_AON_AHB_T *)REG_ACCESS_ADDRESS(REG_SLV_FW_AON_AHB_BASE))

// port0_default_address_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t port0_default_address_0 : 27; // [26:0]
        uint32_t __31_27 : 5;                  // [31:27]
    } b;
} REG_SLV_FW_AON_AHB_PORT0_DEFAULT_ADDRESS_0_T;

// port_int_en
typedef union {
    uint32_t v;
    struct
    {
        uint32_t port_0_w_en : 1; // [0]
        uint32_t port_0_r_en : 1; // [1]
        uint32_t __31_2 : 30;     // [31:2]
    } b;
} REG_SLV_FW_AON_AHB_PORT_INT_EN_T;

// port_int_clr
typedef union {
    uint32_t v;
    struct
    {
        uint32_t port_0_w_clr : 1; // [0], write clear
        uint32_t port_0_r_clr : 1; // [1], write clear
        uint32_t __31_2 : 30;      // [31:2]
    } b;
} REG_SLV_FW_AON_AHB_PORT_INT_CLR_T;

// port_int_raw
typedef union {
    uint32_t v;
    struct
    {
        uint32_t port_0_w_raw : 1; // [0], read only
        uint32_t port_0_r_raw : 1; // [1], read only
        uint32_t __31_2 : 30;      // [31:2]
    } b;
} REG_SLV_FW_AON_AHB_PORT_INT_RAW_T;

// port_int_fin
typedef union {
    uint32_t v;
    struct
    {
        uint32_t port_0_w_fin : 1; // [0], read only
        uint32_t port_0_r_fin : 1; // [1], read only
        uint32_t __31_2 : 30;      // [31:2]
    } b;
} REG_SLV_FW_AON_AHB_PORT_INT_FIN_T;

// rd_sec_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t pub_nic_gpv_rd_sec : 2;  // [1:0]
        uint32_t dap_rd_sec : 2;          // [3:2]
        uint32_t pub_apb_reg_rd_sec : 2;  // [5:4]
        uint32_t pagespy_rd_sec : 2;      // [7:6]
        uint32_t psram_phy_rd_sec : 2;    // [9:8]
        uint32_t dmc400_rd_sec : 2;       // [11:10]
        uint32_t iomux_rd_sec : 2;        // [13:12]
        uint32_t ana_wrap2_rd_sec : 2;    // [15:14]
        uint32_t djtag_cfg_rd_sec : 2;    // [17:16]
        uint32_t gpt1_rd_sec : 2;         // [19:18]
        uint32_t spi2_rd_sec : 2;         // [21:20]
        uint32_t gpt2_rd_sec : 2;         // [23:22]
        uint32_t aud_2ad_rd_sec : 2;      // [25:24]
        uint32_t aon_clk_core_rd_sec : 2; // [27:26]
        uint32_t aon_clk_pre_rd_sec : 2;  // [29:28]
        uint32_t idle_timer_rd_sec : 2;   // [31:30]
    } b;
} REG_SLV_FW_AON_AHB_RD_SEC_0_T;

// rd_sec_1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t sysmail_rd_sec : 2;     // [1:0]
        uint32_t scc_top_rd_sec : 2;     // [3:2]
        uint32_t i2c3_rd_sec : 2;        // [5:4]
        uint32_t gpio2_rd_sec : 2;       // [7:6]
        uint32_t mon_ctrl_rd_sec : 2;    // [9:8]
        uint32_t ana_wrap1_rd_sec : 2;   // [11:10]
        uint32_t sys_ctrl_rd_sec : 2;    // [13:12]
        uint32_t tzpc_rd_sec : 2;        // [15:14]
        uint32_t efuse_rd_sec : 2;       // [17:16]
        uint32_t adi_mst_sp1_rd_sec : 2; // [19:18]
        uint32_t adi_mst_sp0_rd_sec : 2; // [21:20]
        uint32_t adi_mst_rd_sec : 2;     // [23:22]
        uint32_t spinlock_rd_sec : 2;    // [25:24]
        uint32_t __31_26 : 6;            // [31:26]
    } b;
} REG_SLV_FW_AON_AHB_RD_SEC_1_T;

// wr_sec_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t pub_nic_gpv_wr_sec : 2;  // [1:0]
        uint32_t dap_wr_sec : 2;          // [3:2]
        uint32_t pub_apb_reg_wr_sec : 2;  // [5:4]
        uint32_t pagespy_wr_sec : 2;      // [7:6]
        uint32_t psram_phy_wr_sec : 2;    // [9:8]
        uint32_t dmc400_wr_sec : 2;       // [11:10]
        uint32_t iomux_wr_sec : 2;        // [13:12]
        uint32_t ana_wrap2_wr_sec : 2;    // [15:14]
        uint32_t djtag_cfg_wr_sec : 2;    // [17:16]
        uint32_t gpt1_wr_sec : 2;         // [19:18]
        uint32_t spi2_wr_sec : 2;         // [21:20]
        uint32_t gpt2_wr_sec : 2;         // [23:22]
        uint32_t aud_2ad_wr_sec : 2;      // [25:24]
        uint32_t aon_clk_core_wr_sec : 2; // [27:26]
        uint32_t aon_clk_pre_wr_sec : 2;  // [29:28]
        uint32_t idle_timer_wr_sec : 2;   // [31:30]
    } b;
} REG_SLV_FW_AON_AHB_WR_SEC_0_T;

// wr_sec_1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t sysmail_wr_sec : 2;     // [1:0]
        uint32_t scc_top_wr_sec : 2;     // [3:2]
        uint32_t i2c3_wr_sec : 2;        // [5:4]
        uint32_t gpio2_wr_sec : 2;       // [7:6]
        uint32_t mon_ctrl_wr_sec : 2;    // [9:8]
        uint32_t ana_wrap1_wr_sec : 2;   // [11:10]
        uint32_t sys_ctrl_wr_sec : 2;    // [13:12]
        uint32_t tzpc_wr_sec : 2;        // [15:14]
        uint32_t efuse_wr_sec : 2;       // [17:16]
        uint32_t adi_mst_sp1_wr_sec : 2; // [19:18]
        uint32_t adi_mst_sp0_wr_sec : 2; // [21:20]
        uint32_t adi_mst_wr_sec : 2;     // [23:22]
        uint32_t spinlock_wr_sec : 2;    // [25:24]
        uint32_t __31_26 : 6;            // [31:26]
    } b;
} REG_SLV_FW_AON_AHB_WR_SEC_1_T;

// id0_first_addr_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t first_addr_0 : 27; // [26:0]
        uint32_t __31_27 : 5;       // [31:27]
    } b;
} REG_SLV_FW_AON_AHB_ID0_FIRST_ADDR_0_T;

// id0_last_addr_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t last_addr_0 : 27; // [26:0]
        uint32_t __31_27 : 5;      // [31:27]
    } b;
} REG_SLV_FW_AON_AHB_ID0_LAST_ADDR_0_T;

// id1_first_addr_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t first_addr_0 : 27; // [26:0]
        uint32_t __31_27 : 5;       // [31:27]
    } b;
} REG_SLV_FW_AON_AHB_ID1_FIRST_ADDR_0_T;

// id1_last_addr_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t last_addr_0 : 27; // [26:0]
        uint32_t __31_27 : 5;      // [31:27]
    } b;
} REG_SLV_FW_AON_AHB_ID1_LAST_ADDR_0_T;

// id2_first_addr_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t first_addr_0 : 27; // [26:0]
        uint32_t __31_27 : 5;       // [31:27]
    } b;
} REG_SLV_FW_AON_AHB_ID2_FIRST_ADDR_0_T;

// id2_last_addr_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t last_addr_0 : 27; // [26:0]
        uint32_t __31_27 : 5;      // [31:27]
    } b;
} REG_SLV_FW_AON_AHB_ID2_LAST_ADDR_0_T;

// id3_first_addr_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t first_addr_0 : 27; // [26:0]
        uint32_t __31_27 : 5;       // [31:27]
    } b;
} REG_SLV_FW_AON_AHB_ID3_FIRST_ADDR_0_T;

// id3_last_addr_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t last_addr_0 : 27; // [26:0]
        uint32_t __31_27 : 5;      // [31:27]
    } b;
} REG_SLV_FW_AON_AHB_ID3_LAST_ADDR_0_T;

// id4_first_addr_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t first_addr_0 : 27; // [26:0]
        uint32_t __31_27 : 5;       // [31:27]
    } b;
} REG_SLV_FW_AON_AHB_ID4_FIRST_ADDR_0_T;

// id4_last_addr_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t last_addr_0 : 27; // [26:0]
        uint32_t __31_27 : 5;      // [31:27]
    } b;
} REG_SLV_FW_AON_AHB_ID4_LAST_ADDR_0_T;

// id5_first_addr_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t first_addr_0 : 27; // [26:0]
        uint32_t __31_27 : 5;       // [31:27]
    } b;
} REG_SLV_FW_AON_AHB_ID5_FIRST_ADDR_0_T;

// id5_last_addr_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t last_addr_0 : 27; // [26:0]
        uint32_t __31_27 : 5;      // [31:27]
    } b;
} REG_SLV_FW_AON_AHB_ID5_LAST_ADDR_0_T;

// clk_gate_bypass
typedef union {
    uint32_t v;
    struct
    {
        uint32_t clk_gate_bypass : 1; // [0]
        uint32_t fw_resp_en : 1;      // [1]
        uint32_t __31_2 : 30;         // [31:2]
    } b;
} REG_SLV_FW_AON_AHB_CLK_GATE_BYPASS_T;

// port0_default_address_0
#define SLV_FW_AON_AHB_PORT0_DEFAULT_ADDRESS_0(n) (((n)&0x7ffffff) << 0)

// port_int_en
#define SLV_FW_AON_AHB_PORT_0_W_EN (1 << 0)
#define SLV_FW_AON_AHB_PORT_0_R_EN (1 << 1)

// port_int_clr
#define SLV_FW_AON_AHB_PORT_0_W_CLR (1 << 0)
#define SLV_FW_AON_AHB_PORT_0_R_CLR (1 << 1)

// port_int_raw
#define SLV_FW_AON_AHB_PORT_0_W_RAW (1 << 0)
#define SLV_FW_AON_AHB_PORT_0_R_RAW (1 << 1)

// port_int_fin
#define SLV_FW_AON_AHB_PORT_0_W_FIN (1 << 0)
#define SLV_FW_AON_AHB_PORT_0_R_FIN (1 << 1)

// rd_sec_0
#define SLV_FW_AON_AHB_PUB_NIC_GPV_RD_SEC(n) (((n)&0x3) << 0)
#define SLV_FW_AON_AHB_DAP_RD_SEC(n) (((n)&0x3) << 2)
#define SLV_FW_AON_AHB_PUB_APB_REG_RD_SEC(n) (((n)&0x3) << 4)
#define SLV_FW_AON_AHB_PAGESPY_RD_SEC(n) (((n)&0x3) << 6)
#define SLV_FW_AON_AHB_PSRAM_PHY_RD_SEC(n) (((n)&0x3) << 8)
#define SLV_FW_AON_AHB_DMC400_RD_SEC(n) (((n)&0x3) << 10)
#define SLV_FW_AON_AHB_IOMUX_RD_SEC(n) (((n)&0x3) << 12)
#define SLV_FW_AON_AHB_ANA_WRAP2_RD_SEC(n) (((n)&0x3) << 14)
#define SLV_FW_AON_AHB_DJTAG_CFG_RD_SEC(n) (((n)&0x3) << 16)
#define SLV_FW_AON_AHB_GPT1_RD_SEC(n) (((n)&0x3) << 18)
#define SLV_FW_AON_AHB_SPI2_RD_SEC(n) (((n)&0x3) << 20)
#define SLV_FW_AON_AHB_GPT2_RD_SEC(n) (((n)&0x3) << 22)
#define SLV_FW_AON_AHB_AUD_2AD_RD_SEC(n) (((n)&0x3) << 24)
#define SLV_FW_AON_AHB_AON_CLK_CORE_RD_SEC(n) (((n)&0x3) << 26)
#define SLV_FW_AON_AHB_AON_CLK_PRE_RD_SEC(n) (((n)&0x3) << 28)
#define SLV_FW_AON_AHB_IDLE_TIMER_RD_SEC(n) (((n)&0x3) << 30)

// rd_sec_1
#define SLV_FW_AON_AHB_SYSMAIL_RD_SEC(n) (((n)&0x3) << 0)
#define SLV_FW_AON_AHB_SCC_TOP_RD_SEC(n) (((n)&0x3) << 2)
#define SLV_FW_AON_AHB_I2C3_RD_SEC(n) (((n)&0x3) << 4)
#define SLV_FW_AON_AHB_GPIO2_RD_SEC(n) (((n)&0x3) << 6)
#define SLV_FW_AON_AHB_MON_CTRL_RD_SEC(n) (((n)&0x3) << 8)
#define SLV_FW_AON_AHB_ANA_WRAP1_RD_SEC(n) (((n)&0x3) << 10)
#define SLV_FW_AON_AHB_SYS_CTRL_RD_SEC(n) (((n)&0x3) << 12)
#define SLV_FW_AON_AHB_TZPC_RD_SEC(n) (((n)&0x3) << 14)
#define SLV_FW_AON_AHB_EFUSE_RD_SEC(n) (((n)&0x3) << 16)
#define SLV_FW_AON_AHB_ADI_MST_SP1_RD_SEC(n) (((n)&0x3) << 18)
#define SLV_FW_AON_AHB_ADI_MST_SP0_RD_SEC(n) (((n)&0x3) << 20)
#define SLV_FW_AON_AHB_ADI_MST_RD_SEC(n) (((n)&0x3) << 22)
#define SLV_FW_AON_AHB_SPINLOCK_RD_SEC(n) (((n)&0x3) << 24)

// wr_sec_0
#define SLV_FW_AON_AHB_PUB_NIC_GPV_WR_SEC(n) (((n)&0x3) << 0)
#define SLV_FW_AON_AHB_DAP_WR_SEC(n) (((n)&0x3) << 2)
#define SLV_FW_AON_AHB_PUB_APB_REG_WR_SEC(n) (((n)&0x3) << 4)
#define SLV_FW_AON_AHB_PAGESPY_WR_SEC(n) (((n)&0x3) << 6)
#define SLV_FW_AON_AHB_PSRAM_PHY_WR_SEC(n) (((n)&0x3) << 8)
#define SLV_FW_AON_AHB_DMC400_WR_SEC(n) (((n)&0x3) << 10)
#define SLV_FW_AON_AHB_IOMUX_WR_SEC(n) (((n)&0x3) << 12)
#define SLV_FW_AON_AHB_ANA_WRAP2_WR_SEC(n) (((n)&0x3) << 14)
#define SLV_FW_AON_AHB_DJTAG_CFG_WR_SEC(n) (((n)&0x3) << 16)
#define SLV_FW_AON_AHB_GPT1_WR_SEC(n) (((n)&0x3) << 18)
#define SLV_FW_AON_AHB_SPI2_WR_SEC(n) (((n)&0x3) << 20)
#define SLV_FW_AON_AHB_GPT2_WR_SEC(n) (((n)&0x3) << 22)
#define SLV_FW_AON_AHB_AUD_2AD_WR_SEC(n) (((n)&0x3) << 24)
#define SLV_FW_AON_AHB_AON_CLK_CORE_WR_SEC(n) (((n)&0x3) << 26)
#define SLV_FW_AON_AHB_AON_CLK_PRE_WR_SEC(n) (((n)&0x3) << 28)
#define SLV_FW_AON_AHB_IDLE_TIMER_WR_SEC(n) (((n)&0x3) << 30)

// wr_sec_1
#define SLV_FW_AON_AHB_SYSMAIL_WR_SEC(n) (((n)&0x3) << 0)
#define SLV_FW_AON_AHB_SCC_TOP_WR_SEC(n) (((n)&0x3) << 2)
#define SLV_FW_AON_AHB_I2C3_WR_SEC(n) (((n)&0x3) << 4)
#define SLV_FW_AON_AHB_GPIO2_WR_SEC(n) (((n)&0x3) << 6)
#define SLV_FW_AON_AHB_MON_CTRL_WR_SEC(n) (((n)&0x3) << 8)
#define SLV_FW_AON_AHB_ANA_WRAP1_WR_SEC(n) (((n)&0x3) << 10)
#define SLV_FW_AON_AHB_SYS_CTRL_WR_SEC(n) (((n)&0x3) << 12)
#define SLV_FW_AON_AHB_TZPC_WR_SEC(n) (((n)&0x3) << 14)
#define SLV_FW_AON_AHB_EFUSE_WR_SEC(n) (((n)&0x3) << 16)
#define SLV_FW_AON_AHB_ADI_MST_SP1_WR_SEC(n) (((n)&0x3) << 18)
#define SLV_FW_AON_AHB_ADI_MST_SP0_WR_SEC(n) (((n)&0x3) << 20)
#define SLV_FW_AON_AHB_ADI_MST_WR_SEC(n) (((n)&0x3) << 22)
#define SLV_FW_AON_AHB_SPINLOCK_WR_SEC(n) (((n)&0x3) << 24)

// id0_first_addr_0
#define SLV_FW_AON_AHB_FIRST_ADDR_0(n) (((n)&0x7ffffff) << 0)

// id0_last_addr_0
#define SLV_FW_AON_AHB_LAST_ADDR_0(n) (((n)&0x7ffffff) << 0)

// id1_first_addr_0
#define SLV_FW_AON_AHB_FIRST_ADDR_0(n) (((n)&0x7ffffff) << 0)

// id1_last_addr_0
#define SLV_FW_AON_AHB_LAST_ADDR_0(n) (((n)&0x7ffffff) << 0)

// id2_first_addr_0
#define SLV_FW_AON_AHB_FIRST_ADDR_0(n) (((n)&0x7ffffff) << 0)

// id2_last_addr_0
#define SLV_FW_AON_AHB_LAST_ADDR_0(n) (((n)&0x7ffffff) << 0)

// id3_first_addr_0
#define SLV_FW_AON_AHB_FIRST_ADDR_0(n) (((n)&0x7ffffff) << 0)

// id3_last_addr_0
#define SLV_FW_AON_AHB_LAST_ADDR_0(n) (((n)&0x7ffffff) << 0)

// id4_first_addr_0
#define SLV_FW_AON_AHB_FIRST_ADDR_0(n) (((n)&0x7ffffff) << 0)

// id4_last_addr_0
#define SLV_FW_AON_AHB_LAST_ADDR_0(n) (((n)&0x7ffffff) << 0)

// id5_first_addr_0
#define SLV_FW_AON_AHB_FIRST_ADDR_0(n) (((n)&0x7ffffff) << 0)

// id5_last_addr_0
#define SLV_FW_AON_AHB_LAST_ADDR_0(n) (((n)&0x7ffffff) << 0)

// clk_gate_bypass
#define SLV_FW_AON_AHB_CLK_GATE_BYPASS (1 << 0)
#define SLV_FW_AON_AHB_FW_RESP_EN (1 << 1)

#undef uint32_t

#endif // _SLV_FW_AON_AHB_H_
