/*
 * MUSB OTG driver register defines
 *
 * Copyright 2005 Mentor Graphics Corporation
 * Copyright (C) 2005-2006 by Texas Instruments
 * Copyright (C) 2006-2007 Nokia Corporation
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 * NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef __MUSB_REGS_H__
#define __MUSB_REGS_H__

#define MUSB_REG_BASE (0x02100000)

#define MUSB_EP0_FIFOSIZE 64 /* This is non-configurable */

/*
 * MUSB Register bits
 */

/* POWER */
#define MUSB_POWER_ISOUPDATE 0x80
#define MUSB_POWER_SOFTCONN 0x40
#define MUSB_POWER_HSENAB 0x20
#define MUSB_POWER_HSMODE 0x10
#define MUSB_POWER_RESET 0x08
#define MUSB_POWER_RESUME 0x04
#define MUSB_POWER_SUSPENDM 0x02
#define MUSB_POWER_ENSUSPEND 0x01

/* INTRUSB */
#define MUSB_INTR_SUSPEND 0x01
#define MUSB_INTR_RESUME 0x02
#define MUSB_INTR_RESET 0x04
#define MUSB_INTR_BABBLE 0x04
#define MUSB_INTR_SOF 0x08
#define MUSB_INTR_CONNECT 0x10
#define MUSB_INTR_DISCONNECT 0x20
#define MUSB_INTR_SESSREQ 0x40
#define MUSB_INTR_VBUSERROR 0x80 /* For SESSION end */

/* EPTXEN */
#define EPTXEN(n) (1 << n)
/* EPRXEN */
#define EPTXEN(n) (1 << n)
/* EP0EN */
#define EP0EN (1 << 0)

/* EP0 interrupt */
#define MUSB_EP0_INTERRUPT 0x0001

/* DEVCTL */
#define MUSB_DEVCTL_BDEVICE 0x80
#define MUSB_DEVCTL_FSDEV 0x40
#define MUSB_DEVCTL_LSDEV 0x20
#define MUSB_DEVCTL_VBUS 0x18
#define MUSB_DEVCTL_VBUS_SHIFT 3
#define MUSB_DEVCTL_HM 0x04
#define MUSB_DEVCTL_HR 0x02
#define MUSB_DEVCTL_SESSION 0x01

/* BABBLE_CTL */
#define MUSB_BABBLE_FORCE_TXIDLE 0x80
#define MUSB_BABBLE_SW_SESSION_CTRL 0x40
#define MUSB_BABBLE_STUCK_J 0x20
#define MUSB_BABBLE_RCV_DISABLE 0x04

/* MUSB ULPI VBUSCONTROL */
#define MUSB_ULPI_USE_EXTVBUS 0x01
#define MUSB_ULPI_USE_EXTVBUSIND 0x02
/* ULPI_REG_CONTROL */
#define MUSB_ULPI_REG_REQ (1 << 0)
#define MUSB_ULPI_REG_CMPLT (1 << 1)
#define MUSB_ULPI_RDN_WR (1 << 2)

/* TESTMODE */
#define MUSB_TEST_FORCE_HOST 0x80
#define MUSB_TEST_FIFO_ACCESS 0x40
#define MUSB_TEST_FORCE_FS 0x20
#define MUSB_TEST_FORCE_HS 0x10
#define MUSB_TEST_PACKET 0x08
#define MUSB_TEST_K 0x04
#define MUSB_TEST_J 0x02
#define MUSB_TEST_SE0_NAK 0x01

/* Allocate for double-packet buffering (effectively doubles assigned _SIZE) */
#define MUSB_FIFOSZ_DPB 0x10
/* Allocation size (8, 16, 32, ... 4096) */
#define MUSB_FIFOSZ_SIZE 0x0f
#define MUSB_FIFOSZ_SIZE_8BYTE 0x00
#define MUSB_FIFOSZ_SIZE_16BYTE 0x01
#define MUSB_FIFOSZ_SIZE_32BYTE 0x02
#define MUSB_FIFOSZ_SIZE_64BYTE 0x03
#define MUSB_FIFOSZ_SIZE_128BYTE 0x04
#define MUSB_FIFOSZ_SIZE_256BYTE 0x05
#define MUSB_FIFOSZ_SIZE_512BYTE 0x06
#define MUSB_FIFOSZ_SIZE_1024BYTE 0x07
#define MUSB_FIFOSZ_SIZE_2048BYTE 0x08
#define MUSB_FIFOSZ_SIZE_4096BYTE 0x09

/* CSR0 */
#define MUSB_CSR0_FLUSHFIFO 0x0100
#define MUSB_CSR0_TXPKTRDY 0x0002
#define MUSB_CSR0_RXPKTRDY 0x0001

/* CSR0 in Peripheral mode */
#define MUSB_CSR0_P_SVDSETUPEND 0x0080
#define MUSB_CSR0_P_SVDRXPKTRDY 0x0040
#define MUSB_CSR0_P_SENDSTALL 0x0020
#define MUSB_CSR0_P_SETUPEND 0x0010
#define MUSB_CSR0_P_DATAEND 0x0008
#define MUSB_CSR0_P_SENTSTALL 0x0004

/* CSR0 in Host mode */
#define MUSB_CSR0_H_DIS_PING 0x0800
#define MUSB_CSR0_H_WR_DATATOGGLE 0x0400 /* Set to allow setting: */
#define MUSB_CSR0_H_DATATOGGLE 0x0200    /* Data toggle control */
#define MUSB_CSR0_H_NAKTIMEOUT 0x0080
#define MUSB_CSR0_H_STATUSPKT 0x0040
#define MUSB_CSR0_H_REQPKT 0x0020
#define MUSB_CSR0_H_ERROR 0x0010
#define MUSB_CSR0_H_SETUPPKT 0x0008
#define MUSB_CSR0_H_RXSTALL 0x0004

/* CSR0 bits to avoid zeroing (write zero clears, write 1 ignored) */
#define MUSB_CSR0_P_WZC_BITS \
    (MUSB_CSR0_P_SENTSTALL)
#define MUSB_CSR0_H_WZC_BITS \
    (MUSB_CSR0_H_NAKTIMEOUT | MUSB_CSR0_H_RXSTALL | MUSB_CSR0_RXPKTRDY)

/* TxType/RxType */
#define MUSB_TYPE_SPEED 0xc0
#define MUSB_TYPE_SPEED_SHIFT 6
#define MUSB_TYPE_PROTO 0x30 /* Implicitly zero for ep0 */
#define MUSB_TYPE_PROTO_SHIFT 4
#define MUSB_TYPE_REMOTE_END 0xf /* Implicitly zero for ep0 */

/* CONFIGDATA */
#define MUSB_CONFIGDATA_MPRXE 0x80 /* Auto bulk pkt combining */
#define MUSB_CONFIGDATA_MPTXE 0x40 /* Auto bulk pkt splitting */
#define MUSB_CONFIGDATA_BIGENDIAN 0x20
#define MUSB_CONFIGDATA_HBRXE 0x10    /* HB-ISO for RX */
#define MUSB_CONFIGDATA_HBTXE 0x08    /* HB-ISO for TX */
#define MUSB_CONFIGDATA_DYNFIFO 0x04  /* Dynamic FIFO sizing */
#define MUSB_CONFIGDATA_SOFTCONE 0x02 /* SoftConnect */
#define MUSB_CONFIGDATA_UTMIDW 0x01   /* Data width 0/1 => 8/16bits */

/* TXCSR in Peripheral and Host mode */
#define MUSB_TXCSR_AUTOSET 0x8000
#define MUSB_TXCSR_DMAENAB 0x1000
#define MUSB_TXCSR_FRCDATATOG 0x0800
#define MUSB_TXCSR_DMAMODE 0x0400
#define MUSB_TXCSR_CLRDATATOG 0x0040
#define MUSB_TXCSR_FLUSHFIFO 0x0008
#define MUSB_TXCSR_FIFONOTEMPTY 0x0002
#define MUSB_TXCSR_TXPKTRDY 0x0001

/* TXCSR in Peripheral mode */
#define MUSB_TXCSR_P_ISO 0x4000
#define MUSB_TXCSR_P_INCOMPTX 0x0080
#define MUSB_TXCSR_P_SENTSTALL 0x0020
#define MUSB_TXCSR_P_SENDSTALL 0x0010
#define MUSB_TXCSR_P_UNDERRUN 0x0004

/* TXCSR in Host mode */
#define MUSB_TXCSR_H_WR_DATATOGGLE 0x0200
#define MUSB_TXCSR_H_DATATOGGLE 0x0100
#define MUSB_TXCSR_H_NAKTIMEOUT 0x0080
#define MUSB_TXCSR_H_RXSTALL 0x0020
#define MUSB_TXCSR_H_ERROR 0x0004

/* TXCSR bits to avoid zeroing (write zero clears, write 1 ignored) */
#define MUSB_TXCSR_P_WZC_BITS \
    (MUSB_TXCSR_P_INCOMPTX | MUSB_TXCSR_P_SENTSTALL | MUSB_TXCSR_P_UNDERRUN | MUSB_TXCSR_FIFONOTEMPTY)
#define MUSB_TXCSR_H_WZC_BITS \
    (MUSB_TXCSR_H_NAKTIMEOUT | MUSB_TXCSR_H_RXSTALL | MUSB_TXCSR_H_ERROR | MUSB_TXCSR_FIFONOTEMPTY)

/* RXCSR in Peripheral and Host mode */
#define MUSB_RXCSR_AUTOCLEAR 0x8000
#define MUSB_RXCSR_DMAENAB 0x2000
#define MUSB_RXCSR_DISNYET 0x1000
#define MUSB_RXCSR_PID_ERR 0x1000
#define MUSB_RXCSR_DMAMODE 0x0800
#define MUSB_RXCSR_INCOMPRX 0x0100
#define MUSB_RXCSR_CLRDATATOG 0x0080
#define MUSB_RXCSR_FLUSHFIFO 0x0010
#define MUSB_RXCSR_DATAERROR 0x0008
#define MUSB_RXCSR_FIFOFULL 0x0002
#define MUSB_RXCSR_RXPKTRDY 0x0001
#define MUSB_FIFO_TIMEOUT 0x01

/* RXCSR in Peripheral mode */
#define MUSB_RXCSR_P_ISO 0x4000
#define MUSB_RXCSR_P_SENTSTALL 0x0040
#define MUSB_RXCSR_P_SENDSTALL 0x0020
#define MUSB_RXCSR_P_OVERRUN 0x0004

/* RXCSR in Host mode */
#define MUSB_RXCSR_H_AUTOREQ 0x4000
#define MUSB_RXCSR_H_WR_DATATOGGLE 0x0400
#define MUSB_RXCSR_H_DATATOGGLE 0x0200
#define MUSB_RXCSR_H_RXSTALL 0x0040
#define MUSB_RXCSR_H_REQPKT 0x0020
#define MUSB_RXCSR_H_ERROR 0x0004

/* RXCSR bits to avoid zeroing (write zero clears, write 1 ignored) */
#define MUSB_RXCSR_P_WZC_BITS \
    (MUSB_RXCSR_P_SENTSTALL | MUSB_RXCSR_P_OVERRUN | MUSB_RXCSR_RXPKTRDY)
#define MUSB_RXCSR_H_WZC_BITS \
    (MUSB_RXCSR_H_RXSTALL | MUSB_RXCSR_H_ERROR | MUSB_RXCSR_DATAERROR | MUSB_RXCSR_RXPKTRDY)

/* HUBADDR */
#define MUSB_HUBADDR_MULTI_TT 0x80

/*
 * Common USB registers
 */

#define MUSB_FADDR 0x00 /* 8-bit */
#define MUSB_POWER 0x01 /* 8-bit */

#define MUSB_INTRTX 0x02 /* 16-bit */
#define MUSB_INTRRX 0x04
#define MUSB_INTRTXE 0x06
#define MUSB_INTRRXE 0x08
#define MUSB_INTRUSB 0x0A  /* 8 bit */
#define MUSB_INTRUSBE 0x0B /* 8 bit */
#define MUSB_FRAME 0x0C
#define MUSB_INDEX 0x0E    /* 8 bit */
#define MUSB_TESTMODE 0x0F /* 8 bit */

/*
 * Additional Control Registers
 */

#define MUSB_DEVCTL 0x60     /* 8 bit */
#define MUSB_BABBLE_CTL 0x61 /* 8 bit */

////FIFOx (Addresses 20h-5Fh)
#define MUSB_EP0_FIFO (0x20)
#define Mentor_USB_EP0_FIFO (MUSB_REG_BASE + 0x20)

#define MUSB_TXFIFO_STARTADD(n) (8 + 256 * n + 128)
#define MUSB_RXFIFO_STARTADD(n) (8 + 256 * n)

/* These are always controlled through the INDEX register */
#define MUSB_TXFIFOSZ 0x62  /* 8-bit (see masks) */
#define MUSB_RXFIFOSZ 0x63  /* 8-bit (see masks) */
#define MUSB_TXFIFOADD 0x64 /* 16-bit offset shifted right 3 */
#define MUSB_RXFIFOADD 0x66 /* 16-bit offset shifted right 3 */

/* REVISIT: vctrl/vstatus: optional vendor utmi+phy register at 0x68 */
#define MUSB_HWVERS 0x6C           /* 8 bit */
#define MUSB_ULPI_BUSCONTROL 0x70  /* 8 bit */
#define MUSB_ULPI_INT_MASK 0x72    /* 8 bit */
#define MUSB_ULPI_INT_SRC 0x73     /* 8 bit */
#define MUSB_ULPI_REG_DATA 0x74    /* 8 bit */
#define MUSB_ULPI_REG_ADDR 0x75    /* 8 bit */
#define MUSB_ULPI_REG_CONTROL 0x76 /* 8 bit */
#define MUSB_ULPI_RAW_DATA 0x77    /* 8 bit */

#define MUSB_EPINFO 0x78   /* 8 bit */
#define MUSB_RAMINFO 0x79  /* 8 bit */
#define MUSB_LINKINFO 0x7a /* 8 bit */
#define MUSB_VPLEN 0x7b    /* 8 bit */
#define MUSB_HS_EOF1 0x7c  /* 8 bit */
#define MUSB_FS_EOF1 0x7d  /* 8 bit */
#define MUSB_LS_EOF1 0x7e  /* 8 bit */

#define MUSB_RX_DPKTBUFDIS 0x340 /* 16 bit */
#define MUSB_TX_DPKTBUFDIS 0x342 /* 16 bit */

/* Offsets to endpoint registers */
#define MUSB_TXMAXP 0x00
#define MUSB_TXCSR 0x02
#define MUSB_CSR0 MUSB_TXCSR /* Re-used for EP0 */
#define MUSB_RXMAXP 0x04
#define MUSB_RXCSR 0x06
#define MUSB_RXCOUNT 0x08
#define MUSB_COUNT0 MUSB_RXCOUNT /* Re-used for EP0 */
#define MUSB_TXTYPE 0x0A
#define MUSB_TYPE0 MUSB_TXTYPE /* Re-used for EP0 */
#define MUSB_TXINTERVAL 0x0B
#define MUSB_NAKLIMIT0 MUSB_TXINTERVAL /* Re-used for EP0 */
#define MUSB_RXTYPE 0x0C
#define MUSB_RXINTERVAL 0x0D
#define MUSB_FIFOSIZE 0x0F
#define MUSB_CONFIGDATA MUSB_FIFOSIZE /* Re-used for EP0 */
#define MUSB_OTG_EXT_CSR 0x34b
#define MUSB_OTG_FIFO_CHECK 0x349
#define MUSB_OTG_RXPKTCNT(n) (0x300 + 0x4 * n)

#define MUSB_CSRL_EP0 (0x102)  //8
#define MUSB_CSR_EP0 (0x102)   //16
#define MUSB_COUNT_EP0 (0x108) //16

#define MUSB_TXMAXP_EPN(n) (0x100 + n * 0x10)  //16
#define MUSB_TXCSR_EPN(n) (0x102 + n * 0x10)   //16
#define MUSB_TXCSRL_EPN(n) (0x102 + n * 0x10)  //8
#define MUSB_TXCSRH_EPN(n) (0x103 + n * 0x10)  //8
#define MUSB_RXMAXP_EPN(n) (0x104 + n * 0x10)  //16
#define MUSB_RXCSR_EPN(n) (0x106 + n * 0x10)   //16
#define MUSB_RXCSRL_EPN(n) (0x106 + n * 0x10)  //8
#define MUSB_RXCSRH_EPN(n) (0x107 + n * 0x10)  //8
#define MUSB_RXCOUNT_EPN(n) (0x108 + n * 0x10) //8

/* RESET */
#define MUSB_CSR0RESET (0x0000)   //16
#define MUSB_COUNT0RESET (0x0000) //16
#define MUSB_TXMAXPRESET (0x0000) //16
#define MUSB_TXCSRRESET (0x0000)  //16
#define MUSB_TXCSRLRESET (0x00)   //8
#define MUSB_TXCSRHRESET (0x00)   //8
#define MUSB_RXMAXPRESET (0x0000) //16
#define MUSB_RXCSRRESET (0x0000)  //16
#define MUSB_RXCSRLRESET (0x00)   //8
#define MUSB_RXCSRHRESET (0x00)   //8
#define MUSB_RXCOUNTRESET (0x00)  //8s

// #include "tusb6010.h"		/* Needed "only" for TUSB_EP0_CONF */

#define MUSB_TXCSR_MODE 0x2000

/* "bus control"/target registers, for host side multipoint (external hubs) */
#define MUSB_TXFUNCADDR 0x00
#define MUSB_TXHUBADDR 0x02
#define MUSB_TXHUBPORT 0x03

#define MUSB_RXFUNCADDR 0x04
#define MUSB_RXHUBADDR 0x06
#define MUSB_RXHUBPORT 0x07
#define MUSB_HOST_FORCE_EN 0x01
#define MUSB_CLEAR_TXBUFF 0x10
#define MUSB_CLEAR_RXBUFF 0x20
#define MUSB_TX_CMPL_MODE 0x40

/* DMA */
/*channel configure*/
#define CHN_EN (0x00000001)
#define CHN_LLIST_END (0x00000002)
#define CHN_LLIST_NODE_VLD (0x00000004)
#define CHN_SP_FLAG (0x00000100)

/*channel interrupt*/
#define CHN_FRAGMENT_INT_EN (0x00000001)
#define CHN_BLK_INT_EN (0x00000002)
#define CHN_LLIST_INT_EN (0x00000004)
#define CHN_START_INT_EN (0x00000008)
#define CHN_USBRX_INT_EN (0x00000010)
#define CHN_FRAGMENT_INT_RAW_STATUS (0x00000100)
#define CHN_BLK_INT_RAW_STATUS (0x00000200)
#define CHN_LLIST_INT_RAW_STATUS (0x00000400)
#define CHN_START_INT_RAW_STATUS (0x00000800)
#define CHN_USBRX_INT_RAW_STATUS (0x00001000)
#define CHN_FRAGMENT_INT_MASK_STATUS (0x00010000)
#define CHN_BLK_INT_MASK_STATUS (0x00020000)
#define CHN_LLISTINT_MASK_STATUS (0x00040000)
#define CHN_START_INT_MASK_STATUS (0x00080000)
#define CHN_USBRX_INT_MASK_STATUS (0x00100000)
#define CHN_FRAGMENT_INT_CLR (0x01000000)
#define CHN_BLK_INT_CLR (0x02000000)
#define CHN_LLIST_INT_CLR (0x04000000)
#define CHN_START_INT_CLR (0x08000000)
#define CHN_USBRX_LAST_INT_CLR (0x10000000)

#define MUSB_DMA_CHN_PAUSE(n) (0x1C00 + ((n)-1) * 0x20)
#define MUSB_DMA_CHN_CFG(n) (0x1C04 + ((n)-1) * 0x20)
#define MUSB_DMA_CHN_INTR(n) (0x1C08 + ((n)-1) * 0x20)
#define MUSB_DMA_CHN_ADDR(n) (0x1C0C + ((n)-1) * 0x20)
#define MUSB_DMA_CHN_LEN(n) (0x1C10 + ((n)-1) * 0x20)
#define MUSB_DMA_CHN_LLIST_PTR(n) (0x1C14 + ((n)-1) * 0x20)
#define MUSB_DMA_CHN_BYTE_CNT(n) (0x1C18 + ((n)-1) * 0x20)
#define MUSB_DMA_CHN_REQ(n) (0x1C1C + ((n)-1) * 0x20)

// static inline void musb_write_txfifosz(void __iomem *mbase, u8 c_size)
// {
// 	musb_writeb(mbase, MUSB_TXFIFOSZ, c_size);
// }

// static inline void musb_write_txfifoadd(void __iomem *mbase, u16 c_off)
// {
// 	musb_writew(mbase, MUSB_TXFIFOADD, c_off);
// }

// static inline void musb_write_rxfifosz(void __iomem *mbase, u8 c_size)
// {
// 	musb_writeb(mbase, MUSB_RXFIFOSZ, c_size);
// }

// static inline void  musb_write_rxfifoadd(void __iomem *mbase, u16 c_off)
// {
// 	musb_writew(mbase, MUSB_RXFIFOADD, c_off);
// }

// static inline void musb_write_ulpi_buscontrol(void __iomem *mbase, u8 val)
// {
// 	musb_writeb(mbase, MUSB_ULPI_BUSCONTROL, val);
// }

// static inline u8 musb_read_txfifosz(void __iomem *mbase)
// {
// 	return musb_readb(mbase, MUSB_TXFIFOSZ);
// }

// static inline u16 musb_read_txfifoadd(void __iomem *mbase)
// {
// 	return musb_readw(mbase, MUSB_TXFIFOADD);
// }

// static inline u8 musb_read_rxfifosz(void __iomem *mbase)
// {
// 	return musb_readb(mbase, MUSB_RXFIFOSZ);
// }

// static inline u16  musb_read_rxfifoadd(void __iomem *mbase)
// {
// 	return musb_readw(mbase, MUSB_RXFIFOADD);
// }

// static inline u8 musb_read_ulpi_buscontrol(void __iomem *mbase)
// {
// 	return musb_readb(mbase, MUSB_ULPI_BUSCONTROL);
// }

// static inline u8 musb_read_configdata(void __iomem *mbase)
// {
// 	musb_writeb(mbase, MUSB_INDEX, 0);
// 	return musb_readb(mbase, 0x10 + MUSB_CONFIGDATA);
// }

// static inline u16 musb_read_hwvers(void __iomem *mbase)
// {
// 	return musb_readw(mbase, MUSB_HWVERS);
// }

// static inline void musb_write_rxfunaddr(struct musb *musb, u8 epnum,
// 		u8 qh_addr_reg)
// {
// 	musb_writeb(musb->mregs,
// 		    musb->io.busctl_offset(epnum, MUSB_RXFUNCADDR),
// 		    qh_addr_reg);
// }

// static inline void musb_write_rxhubaddr(struct musb *musb, u8 epnum,
// 		u8 qh_h_addr_reg)
// {
// 	musb_writeb(musb->mregs, musb->io.busctl_offset(epnum, MUSB_RXHUBADDR),
// 			qh_h_addr_reg);
// }

// static inline void musb_write_rxhubport(struct musb *musb, u8 epnum,
// 		u8 qh_h_port_reg)
// {
// 	musb_writeb(musb->mregs, musb->io.busctl_offset(epnum, MUSB_RXHUBPORT),
// 			qh_h_port_reg);
// }

// static inline void musb_write_txfunaddr(struct musb *musb, u8 epnum,
// 		u8 qh_addr_reg)
// {
// 	musb_writeb(musb->mregs,
// 		    musb->io.busctl_offset(epnum, MUSB_TXFUNCADDR),
// 		    qh_addr_reg);
// }

// static inline void musb_write_txhubaddr(struct musb *musb, u8 epnum,
// 		u8 qh_addr_reg)
// {
// 	musb_writeb(musb->mregs, musb->io.busctl_offset(epnum, MUSB_TXHUBADDR),
// 			qh_addr_reg);
// }

// static inline void musb_write_txhubport(struct musb *musb, u8 epnum,
// 		u8 qh_h_port_reg)
// {
// 	musb_writeb(musb->mregs, musb->io.busctl_offset(epnum, MUSB_TXHUBPORT),
// 			qh_h_port_reg);
// }

// static inline u8 musb_read_rxfunaddr(struct musb *musb, u8 epnum)
// {
// 	return musb_readb(musb->mregs,
// 			  musb->io.busctl_offset(epnum, MUSB_RXFUNCADDR));
// }

// static inline u8 musb_read_rxhubaddr(struct musb *musb, u8 epnum)
// {
// 	return musb_readb(musb->mregs,
// 			  musb->io.busctl_offset(epnum, MUSB_RXHUBADDR));
// }

// static inline u8 musb_read_rxhubport(struct musb *musb, u8 epnum)
// {
// 	return musb_readb(musb->mregs,
// 			  musb->io.busctl_offset(epnum, MUSB_RXHUBPORT));
// }

// static inline u8 musb_read_txfunaddr(struct musb *musb, u8 epnum)
// {
// 	return musb_readb(musb->mregs,
// 			  musb->io.busctl_offset(epnum, MUSB_TXFUNCADDR));
// }

// static inline u8 musb_read_txhubaddr(struct musb *musb, u8 epnum)
// {
// 	return musb_readb(musb->mregs,
// 			  musb->io.busctl_offset(epnum, MUSB_TXHUBADDR));
// }

// static inline u8 musb_read_txhubport(struct musb *musb, u8 epnum)
// {
// 	return musb_readb(musb->mregs,
// 			  musb->io.busctl_offset(epnum, MUSB_TXHUBPORT));
// }

#endif /* __MUSB_REGS_H__ */
