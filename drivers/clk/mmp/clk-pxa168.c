/*
 * pxa168 clock framework source file
 *
 * Copyright (C) 2012 Marvell
 * Chao Xie <xiechao.mail@gmail.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/err.h>

#include <mach/addr-map.h>

#include "clk.h"

#define APBC_UART0	0x0000
#define APBC_UART1	0x0004
#define APBC_GPIO	0x0008
#define APBC_PWM0	0x000c
#define APBC_PWM1	0x0010
#define APBC_PWM2	0x0014
#define APBC_PWM3	0x0018
#define APBC_RTC	0x0028
#define APBC_TWSI0	0x002c
#define APBC_KPC	0x0030
#define APBC_TWSI1	0x006c
#define APBC_UART2	0x0070
#define APBC_SSP0	0x081c
#define APBC_SSP1	0x0820
#define APBC_SSP2	0x084c
#define APBC_SSP3	0x0858
#define APBC_SSP4	0x085c

#define	APBC_UART1_CLK_RST	0x0000	/*UART1 Clock/Reset Control*/
#define	APBC_UART2_CLK_RST	0x0004	/*UART2 Clock/Reset Control*/
#define	APBC_GPIO_CLK_RST	0x0008	/*GPIO Clock/Reset Control*/
#define	APBC_PWM1_CLK_RST	0x000C	/*PWM1 Clock/Reset Control*/
#define	APBC_PWM2_CLK_RST	0x0010	/*PWM2 Clock/Reset Control*/
#define	APBC_PWM3_CLK_RST	0x0014	/*PWM3 Clock/Reset Control*/
#define	APBC_PWM4_CLK_RST	0x0018	/*PWM4 Clock/Reset Control*/
#define	APBC_RTC_CLK_RST	0x0028	/*RTC Clock/Reset Control*/
#define	APBC_TWSI0_CLK_RST	0x002C	/*TWSI0 Clock/Reset Control*/
#define	APBC_KPC_CLK_RST	0x0030	/*Keypad Controller Clock/Reset
					  Control*/
#define	APBC_TIMERS_CLK_RST	0x0034	/*Timers Clock/Reset Control*/
#define	APBC_AIB_CLK_RST	0x003C	/*AIB Clock/Reset Control*/
#define	APBC_SW_JTAG_CLK_RST	0x0040	/*Software emulated JTAG
					  Clock/Reset Control*/
#define	APBC_ONEWIRE_CLK_RST	0x0048	/*OneWire Clock/Reset
					  Control*/
#define	APBC_PWR_TWSI_CLK_RST	0x006C	/*PWR_TWSI Clock/Reset
					  Control*/
#define	APBC_UART3_CLK_RST	0x0070	/*UART3 Clock/Reset Control*/
#define	APBC_AC97_CLK_RST	0x0084	/*AC97 Clock/Reset Control*/

#define	APBC_SSP0_CLK_RST	0x081C	/*SSP1 Clock/Reset Control*/
#define	APBC_SSP1_CLK_RST	0x0820	/*SSP2 Clock/Reset Control*/
#define	APBC_SSP2_CLK_RST	0x084C	/*SSP3 Clock/Reset Control*/
#define	APBC_SSP3_CLK_RST	0x0858	/*SSP4 Clock/Reset Control*/
#define	APBC_SSP4_CLK_RST	0x085C	/*SSP5 Clock/Reset Control*/

#define MPMU_UART_PLL	0x0014

#define APMU_CCIC_GATE	0x0028
#define APMU_IRE	0x0048
#define APMU_DISP0	0x004c
#define APMU_CCIC0	0x0050
#define APMU_SDH0	0x0054
#define APMU_SDH1	0x0058
#define APMU_USB	0x005c
#define APMU_DFC	0x0060
#define APMU_DMA	0x0064
#define APMU_GEU	0x0068
#define APMU_BUS	0x006c
#define APMU_WAKE_CLR	0x007c
#define APMU_CCIC_DBG	0x0088
#define APMU_GC		0x00cc
#define APMU_GC_PD	0x00d0
#define APMU_SMC	0x00d4
#define APMU_SDH2	0x00e0
#define APMU_SDH3	0x00e4
#define APMU_CF         0x00f0
#define APMU_ICR	0x00f8
#define	APMU_ETH	0x00fc

#define APMU_PCR		0x0000
#define APMU_CCR		0x0004
#define APMU_CCSR		0x000c
#define APMU_FC_TIMER		0x0010
#define APMU_CP_IDLE_CFG	0x0014	/*Not listed in PXA16X
					  Document*/
#define APMU_IDLE_CFG		0x0018
#define APMU_LCD_CLK_RES_CTRL	0x004c
#define APMU_CCIC_CLK_RES_CTRL	0x0050
#define APMU_SDH0_CLK_RES_CTRL	0x0054	/*SD1*/
#define APMU_SDH1_CLK_RES_CTRL	0x0058	/*SD2*/
#define APMU_SDH2_CLK_RES_CTRL	0x00e0	/*SD3*/
#define APMU_SDH3_CLK_RES_CTRL	0x00e4	/*SD4*/
#define APMU_USB_CLK_RES_CTRL	0x005c
#define APMU_NFC_CLK_RES_CTRL	0x0060	/*NAND Flash Controller 
					  Clock/Reset*/
#define APMU_DMA_CLK_RES_CTRL	0x0064	/*DMA Clock/Reset Control*/
#define APMU_BUS_CLK_RES_CTRL	0x006c	/*Bus Clock/Reste Control*/
#define APMU_WAKE_CLK		0x007c	/*Wake Clear*/
#define APMU_PWR_STBL_TIMER	0x0084	/*Not listed in PXA16X
					  Document*/
#define APMU_SRAM_PWR_DWN	0x008c	/*Not listed in PXA16X
					  Document*/
#define APMU_CORE_STATUS	0x0090	/*Core Status*/
#define APMU_RES_FRM_SLP_CLR	0x0094	/*Resume from Sleep clear*/
#define APMU_IMR		0x0098	/*PMU Interrupt Mask*/
#define APMU_IRWC		0x009c	/*Interrupt RD/WR Clear*/
#define APMU_ISR		0x00a0	/*Interrupt Status*/
#define APMU_DX8_CLK_RES_CTRL	0x00a4	/*Not listed in PXA16X
					  Document*/
#define APMU_DTC_CLK_RES_CTRL	0x00ac	/*Not listed in PXA16X
                                          Document*/
#define APMU_MC_HW_SLP_TYPE	0x00b0	/*Memory Controller Hardware
					  Sleep Type*/
#define APMU_MC_SLP_REQ		0x00b4	/*Memory Controller Sleep
					  Request*/
#define APMU_MC_SW_SLP_TYPE	0x00c0	/*Memory Controller Software
                                          Sleep Type*/
#define APMU_PLL_SEL_STATUS	0x00c4	/*PLL Clock select status*/
#define	APMU_SYNC_MODE_BYPASS	0x00c8	/*Sync Mode Bypass*/
#define APMU_GC_CLK_RES_CTRL	0x00cc	/*GC300 2D Graphics Controller
					  Clock/Reset control*/
#define APMU_SMC_CLK_RES_CTRL	0x00d4	/*Static Memory controller
					  Clock/Reset control*/
#define APMU_XD_CLK_RES_CTRL	0x00dc	/*XD Controller clock/reset
					  control*/
#define APMU_CF_CLK_RES_CTRL	0x00f0	/*Compact Flash controller
					  clock/reset control*/
#define APMU_MSP_CLK_RES_CTRL	0x00f4	/*Memory Stick Pro clock/reset
					  control*/
#define APMU_CMU_CLK_RES_CTRL	0x00f8	/*CMU clock/reset control*/
#define APMU_MFU_CLK_RES_CTRL	0x00fc	/*FE(Fast Ethernet) 
					  clock/reset control*/
#define APMU_PCIe_CLK_RES_CTRL	0x0100	/*PCIe clock/reset control*/
#define APMU_EPD_CLK_RES_CTRL	0x0104	/*EPD clock/reset control*/

#define APMU_GC_156M            0x0
#define APMU_GC_312M            0x40
#define APMU_GC_PLL2            0x80
#define APMU_GC_PLL2_DIV2       0xc0
#define APMU_GC_624M            0xc0 /* added according to Aspen SW spec v2.8*/

static DEFINE_SPINLOCK(clk_lock);

static struct clk_factor_masks uart_factor_masks = {
	.factor = 2,
	.num_mask = 0x1fff,
	.den_mask = 0x1fff,
	.num_shift = 16,
	.den_shift = 0,
};

static struct clk_factor_tbl uart_factor_tbl[] = {
	{.num = 8125, .den = 1536},	/*14.745MHZ */
};

#if 0
struct gc_rate_table {
	unsigned long   rate;
	unsigned int    flag;
};

static struct gc_rate_table gc300_rates [] = {
	/* put highest rate at the top of the table */
	{
		.rate	=	624000000,
		.flag	=	APMU_GC_624M,
	},
	{
		.rate	=	312000000,
		.flag	=	APMU_GC_312M,
	},
	{
		.rate	=	156000000,
		.flag	=	APMU_GC_156M,
	},
};

static int gc_lookaround_rate(int target_rate, u32 *flag)
{
	int i;

	for (i=0; i<ARRAY_SIZE(gc300_rates); i++) {
		if (target_rate >= gc300_rates[i].rate)
			break;
        }
	if (i==ARRAY_SIZE(gc300_rates)) i--;
	*flag = gc300_rates[i].flag;
	return gc300_rates[i].rate;
}

static void gc300_clk_enable(struct clk *clk)
{
	u32 tmp = __raw_readl(clk->clk_rst), flag;

	/* reset gc clock */
	__raw_writel(tmp & ~0x07, clk->clk_rst);
	tmp = __raw_readl(clk->clk_rst);
	udelay(1);

	/* select GC clock source */
	gc_lookaround_rate(clk->rate, &flag);
	tmp &= ~0xc0;
	tmp |= flag;
	__raw_writel(tmp, clk->clk_rst);

	/* enable GC CLK EN */
	__raw_writel(tmp | 0x10, clk->clk_rst);
	tmp = __raw_readl(clk->clk_rst);

	/* enable GC HCLK EN */
	__raw_writel(tmp | 0x08, clk->clk_rst);
	tmp = __raw_readl(clk->clk_rst);

	/* enable GC ACLK EN */
	__raw_writel(tmp | 0x20, clk->clk_rst);
	tmp = __raw_readl(clk->clk_rst);

	/* reset GC */
	__raw_writel(tmp & ~0x07, clk->clk_rst);
	tmp = __raw_readl(clk->clk_rst);

	/* pull GC out of reset */
	__raw_writel(tmp | 0x2, clk->clk_rst);
	tmp = __raw_readl(clk->clk_rst);

	/* delay 48 cycles */
	udelay(1);

	/* pull GC AXI/AHB out of reset */
	__raw_writel(tmp | 0x5, clk->clk_rst);
	tmp = __raw_readl(clk->clk_rst);
}

static void gc300_clk_disable(struct clk *clk)
{
	__raw_writel(0, clk->clk_rst);
}

static int gc300_clk_setrate(struct clk *clk, unsigned long target_rate)
{
	u32 flag;

	clk->rate = gc_lookaround_rate(target_rate, &flag);

	return 0;
}

static unsigned long gc300_clk_getrate(struct clk *clk)
{
	return clk->rate;
}

struct clkops gc300_clk_ops = {
	.enable		= gc300_clk_enable,
	.disable	= gc300_clk_disable,
	.setrate	= gc300_clk_setrate,
	.getrate	= gc300_clk_getrate,
};
#endif

static const char *uart_parent[] = {"pll1_3_16", "uart_pll"};
static const char *ssp_parent[] = {"pll1_96", "pll1_48", "pll1_24", "pll1_12"};
static const char *sdh_parent[] = {"pll1_12", "pll1_13"};
static const char *disp_parent[] = {"pll1_2", "pll1_12"};
static const char *ccic_parent[] = {"pll1_2", "pll1_12"};
static const char *ccic_phy_parent[] = {"pll1_6", "pll1_12"};
static const char *usb_sph_otg_parent[] = {"pll1_2_1_5"};

void __init pxa168_clk_init(void)
{
#if !defined(CONFIG_MACH_GPLUGD)
	struct clk *clk;
	struct clk *uart_pll;
	void __iomem *mpmu_base;
	void __iomem *apmu_base;
	void __iomem *apbc_base;

	mpmu_base = ioremap(APB_PHYS_BASE + 0x50000, SZ_4K);
	if (mpmu_base == NULL) {
		pr_err("error to ioremap MPMU base\n");
		return;
	}

	apmu_base = ioremap(AXI_PHYS_BASE + 0x82800, SZ_4K);
	if (apmu_base == NULL) {
		pr_err("error to ioremap APMU base\n");
		return;
	}

	apbc_base = ioremap(APB_PHYS_BASE + 0x15000, SZ_4K);
	if (apbc_base == NULL) {
		pr_err("error to ioremap APBC base\n");
		return;
	}

	clk = clk_register_fixed_rate(NULL, "clk32", NULL, CLK_IS_ROOT, 3200);
	clk_register_clkdev(clk, "clk32", NULL);

	clk = clk_register_fixed_rate(NULL, "vctcxo", NULL, CLK_IS_ROOT,
				26000000);
	clk_register_clkdev(clk, "vctcxo", NULL);

	clk = clk_register_fixed_rate(NULL, "pll1", NULL, CLK_IS_ROOT,
				624000000);
	clk_register_clkdev(clk, "pll1", NULL);

	clk = clk_register_fixed_factor(NULL, "pll1_2", "pll1",
				CLK_SET_RATE_PARENT, 1, 2);
	clk_register_clkdev(clk, "pll1_2", NULL);

	clk = clk_register_fixed_factor(NULL, "pll1_4", "pll1_2",
				CLK_SET_RATE_PARENT, 1, 2);
	clk_register_clkdev(clk, "pll1_4", NULL);

	clk = clk_register_fixed_factor(NULL, "pll1_8", "pll1_4",
				CLK_SET_RATE_PARENT, 1, 2);
	clk_register_clkdev(clk, "pll1_8", NULL);

	clk = clk_register_fixed_factor(NULL, "pll1_16", "pll1_8",
				CLK_SET_RATE_PARENT, 1, 2);
	clk_register_clkdev(clk, "pll1_16", NULL);

	clk = clk_register_fixed_factor(NULL, "pll1_6", "pll1_2",
				CLK_SET_RATE_PARENT, 1, 3);
	clk_register_clkdev(clk, "pll1_6", NULL);

	clk = clk_register_fixed_factor(NULL, "pll1_12", "pll1_6",
				CLK_SET_RATE_PARENT, 1, 2);
	clk_register_clkdev(clk, "pll1_12", NULL);

	clk = clk_register_fixed_factor(NULL, "pll1_24", "pll1_12",
				CLK_SET_RATE_PARENT, 1, 2);
	clk_register_clkdev(clk, "pll1_24", NULL);

	clk = clk_register_fixed_factor(NULL, "pll1_48", "pll1_24",
				CLK_SET_RATE_PARENT, 1, 2);
	clk_register_clkdev(clk, "pll1_48", NULL);

	clk = clk_register_fixed_factor(NULL, "pll1_96", "pll1_48",
				CLK_SET_RATE_PARENT, 1, 2);
	clk_register_clkdev(clk, "pll1_96", NULL);

	clk = clk_register_fixed_factor(NULL, "pll1_13", "pll1",
				CLK_SET_RATE_PARENT, 1, 13);
	clk_register_clkdev(clk, "pll1_13", NULL);

	clk = clk_register_fixed_factor(NULL, "pll1_13_1_5", "pll1",
				CLK_SET_RATE_PARENT, 2, 3);
	clk_register_clkdev(clk, "pll1_13_1_5", NULL);

	clk = clk_register_fixed_factor(NULL, "pll1_2_1_5", "pll1",
				CLK_SET_RATE_PARENT, 2, 3);
	clk_register_clkdev(clk, "pll1_2_1_5", NULL);

	clk = clk_register_fixed_factor(NULL, "pll1_3_16", "pll1",
				CLK_SET_RATE_PARENT, 3, 16);
	clk_register_clkdev(clk, "pll1_3_16", NULL);

	uart_pll = mmp_clk_register_factor("uart_pll", "pll1_4", 0,
				mpmu_base + MPMU_UART_PLL,
				&uart_factor_masks, uart_factor_tbl,
				ARRAY_SIZE(uart_factor_tbl));
	clk_set_rate(uart_pll, 14745600);
	clk_register_clkdev(uart_pll, "uart_pll", NULL);

	clk = mmp_clk_register_apbc("twsi0", "pll1_13_1_5",
				apbc_base + APBC_TWSI0, 10, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa2xx-i2c.0");

	clk = mmp_clk_register_apbc("twsi1", "pll1_13_1_5",
				apbc_base + APBC_TWSI1, 10, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa2xx-i2c.1");

	clk = mmp_clk_register_apbc("gpio", "vctcxo",
				apbc_base + APBC_GPIO, 10, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "mmp-gpio");

	clk = mmp_clk_register_apbc("kpc", "clk32",
				apbc_base + APBC_KPC, 10, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa27x-keypad");

	clk = mmp_clk_register_apbc("rtc", "clk32",
				apbc_base + APBC_RTC, 10, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "sa1100-rtc");

	clk = mmp_clk_register_apbc("pwm0", "pll1_48",
				apbc_base + APBC_PWM0, 10, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa168-pwm.0");

	clk = mmp_clk_register_apbc("pwm1", "pll1_48",
				apbc_base + APBC_PWM1, 10, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa168-pwm.1");

	clk = mmp_clk_register_apbc("pwm2", "pll1_48",
				apbc_base + APBC_PWM2, 10, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa168-pwm.2");

	clk = mmp_clk_register_apbc("pwm3", "pll1_48",
				apbc_base + APBC_PWM3, 10, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa168-pwm.3");

	clk = clk_register_mux(NULL, "uart0_mux", uart_parent,
				ARRAY_SIZE(uart_parent), CLK_SET_RATE_PARENT,
				apbc_base + APBC_UART0, 4, 3, 0, &clk_lock);
	clk_set_parent(clk, uart_pll);
	clk_register_clkdev(clk, "uart_mux.0", NULL);

	clk = mmp_clk_register_apbc("uart0", "uart0_mux",
				apbc_base + APBC_UART0, 10, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa2xx-uart.0");

	clk = clk_register_mux(NULL, "uart1_mux", uart_parent,
				ARRAY_SIZE(uart_parent), CLK_SET_RATE_PARENT,
				apbc_base + APBC_UART1, 4, 3, 0, &clk_lock);
	clk_set_parent(clk, uart_pll);
	clk_register_clkdev(clk, "uart_mux.1", NULL);

	clk = mmp_clk_register_apbc("uart1", "uart1_mux",
				apbc_base + APBC_UART1,	10, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa2xx-uart.1");

	clk = clk_register_mux(NULL, "uart2_mux", uart_parent,
				ARRAY_SIZE(uart_parent), CLK_SET_RATE_PARENT,
				apbc_base + APBC_UART2, 4, 3, 0, &clk_lock);
	clk_set_parent(clk, uart_pll);
	clk_register_clkdev(clk, "uart_mux.2", NULL);

	clk = mmp_clk_register_apbc("uart2", "uart2_mux",
				apbc_base + APBC_UART2,	10, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa2xx-uart.2");

	clk = clk_register_mux(NULL, "ssp0_mux", ssp_parent,
				ARRAY_SIZE(ssp_parent), CLK_SET_RATE_PARENT,
				apbc_base + APBC_SSP0, 4, 3, 0, &clk_lock);
	clk_register_clkdev(clk, "uart_mux.0", NULL);

	clk = mmp_clk_register_apbc("ssp0", "ssp0_mux", apbc_base + APBC_SSP0,
				10, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "mmp-ssp.0");

	clk = clk_register_mux(NULL, "ssp1_mux", ssp_parent,
				ARRAY_SIZE(ssp_parent), CLK_SET_RATE_PARENT,
				apbc_base + APBC_SSP1, 4, 3, 0, &clk_lock);
	clk_register_clkdev(clk, "ssp_mux.1", NULL);

	clk = mmp_clk_register_apbc("ssp1", "ssp1_mux", apbc_base + APBC_SSP1,
				10, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "mmp-ssp.1");

	clk = clk_register_mux(NULL, "ssp2_mux", ssp_parent,
				ARRAY_SIZE(ssp_parent), CLK_SET_RATE_PARENT,
				apbc_base + APBC_SSP2, 4, 3, 0, &clk_lock);
	clk_register_clkdev(clk, "ssp_mux.2", NULL);

	clk = mmp_clk_register_apbc("ssp2", "ssp1_mux", apbc_base + APBC_SSP2,
				10, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "mmp-ssp.2");

	clk = clk_register_mux(NULL, "ssp3_mux", ssp_parent,
				ARRAY_SIZE(ssp_parent), CLK_SET_RATE_PARENT,
				apbc_base + APBC_SSP3, 4, 3, 0, &clk_lock);
	clk_register_clkdev(clk, "ssp_mux.3", NULL);

	clk = mmp_clk_register_apbc("ssp3", "ssp1_mux", apbc_base + APBC_SSP3,
				10, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "mmp-ssp.3");

	clk = clk_register_mux(NULL, "ssp4_mux", ssp_parent,
				ARRAY_SIZE(ssp_parent), CLK_SET_RATE_PARENT,
				apbc_base + APBC_SSP4, 4, 3, 0, &clk_lock);
	clk_register_clkdev(clk, "ssp_mux.4", NULL);

	clk = mmp_clk_register_apbc("ssp4", "ssp1_mux", apbc_base + APBC_SSP4,
				10, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "mmp-ssp.4");

	clk = mmp_clk_register_apmu("dfc", "pll1_4", apmu_base + APMU_DFC,
				0x19b, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa3xx-nand.0");

	clk = clk_register_mux(NULL, "sdh0_mux", sdh_parent,
				ARRAY_SIZE(sdh_parent), CLK_SET_RATE_PARENT,
				apmu_base + APMU_SDH0, 6, 1, 0, &clk_lock);
	clk_register_clkdev(clk, "sdh0_mux", NULL);

	clk = mmp_clk_register_apmu("sdh0", "sdh_mux", apmu_base + APMU_SDH0,
				0x1b, &clk_lock);
	clk_register_clkdev(clk, NULL, "sdhci-pxa.0");

	clk = clk_register_mux(NULL, "sdh1_mux", sdh_parent,
				ARRAY_SIZE(sdh_parent), CLK_SET_RATE_PARENT,
				apmu_base + APMU_SDH1, 6, 1, 0, &clk_lock);
	clk_register_clkdev(clk, "sdh1_mux", NULL);

	clk = mmp_clk_register_apmu("sdh1", "sdh1_mux", apmu_base + APMU_SDH1,
				0x1b, &clk_lock);
	clk_register_clkdev(clk, NULL, "sdhci-pxa.1");

	clk = mmp_clk_register_apmu("usb", "usb_pll", apmu_base + APMU_USB,
				0x9, &clk_lock);
	clk_register_clkdev(clk, "usb_clk", NULL);

	clk = mmp_clk_register_apmu("sph", "usb_pll", apmu_base + APMU_USB,
				0x12, &clk_lock);
	clk_register_clkdev(clk, "sph_clk", NULL);

	clk = clk_register_mux(NULL, "disp0_mux", disp_parent,
				ARRAY_SIZE(disp_parent), CLK_SET_RATE_PARENT,
				apmu_base + APMU_DISP0, 6, 1, 0, &clk_lock);
	clk_register_clkdev(clk, "disp_mux.0", NULL);

	clk = mmp_clk_register_apmu("disp0", "disp0_mux",
				apmu_base + APMU_DISP0, 0x1b, &clk_lock);
	clk_register_clkdev(clk, "fnclk", "mmp-disp.0");

	clk = mmp_clk_register_apmu("disp0_hclk", "disp0_mux",
				apmu_base + APMU_DISP0, 0x24, &clk_lock);
	clk_register_clkdev(clk, "hclk", "mmp-disp.0");

	clk = clk_register_mux(NULL, "ccic0_mux", ccic_parent,
				ARRAY_SIZE(ccic_parent), CLK_SET_RATE_PARENT,
				apmu_base + APMU_CCIC0, 6, 1, 0, &clk_lock);
	clk_register_clkdev(clk, "ccic_mux.0", NULL);

	clk = mmp_clk_register_apmu("ccic0", "ccic0_mux",
				apmu_base + APMU_CCIC0, 0x1b, &clk_lock);
	clk_register_clkdev(clk, "fnclk", "mmp-ccic.0");

	clk = clk_register_mux(NULL, "ccic0_phy_mux", ccic_phy_parent,
				ARRAY_SIZE(ccic_phy_parent),
				CLK_SET_RATE_PARENT, apmu_base + APMU_CCIC0,
				7, 1, 0, &clk_lock);
	clk_register_clkdev(clk, "ccic_phy_mux.0", NULL);

	clk = mmp_clk_register_apmu("ccic0_phy", "ccic0_phy_mux",
				apmu_base + APMU_CCIC0, 0x24, &clk_lock);
	clk_register_clkdev(clk, "phyclk", "mmp-ccic.0");

	clk = clk_register_divider(NULL, "ccic0_sphy_div", "ccic0_mux",
				CLK_SET_RATE_PARENT, apmu_base + APMU_CCIC0,
				10, 5, 0, &clk_lock);
	clk_register_clkdev(clk, "sphyclk_div", NULL);

	clk = mmp_clk_register_apmu("ccic0_sphy", "ccic0_sphy_div",
				apmu_base + APMU_CCIC0, 0x300, &clk_lock);
	clk_register_clkdev(clk, "sphyclk", "mmp-ccic.0");
#else
	struct clk *clk;
	struct clk *uart_pll;
	void __iomem *mpmu_base;
	void __iomem *apmu_base;
	void __iomem *apbc_base;

	mpmu_base = ioremap(APB_PHYS_BASE + 0x50000, SZ_4K);
	if (mpmu_base == NULL) {
		pr_err("error to ioremap MPMU base\n");
		return;
	}

	apmu_base = ioremap(AXI_PHYS_BASE + 0x82800, SZ_4K);
	if (apmu_base == NULL) {
		pr_err("error to ioremap APMU base\n");
		return;
	}

	apbc_base = ioremap(APB_PHYS_BASE + 0x15000, SZ_4K);
	if (apbc_base == NULL) {
		pr_err("error to ioremap APBC base\n");
		return;
	}

	clk = clk_register_fixed_rate(NULL, "clk32", NULL, CLK_IS_ROOT, 3200);
	clk_register_clkdev(clk, "clk32", NULL);

	clk = clk_register_fixed_rate(NULL, "vctcxo", NULL, CLK_IS_ROOT,
				26000000);
	clk_register_clkdev(clk, "vctcxo", NULL);

	clk = clk_register_fixed_rate(NULL, "pll1", NULL, CLK_IS_ROOT,
				624000000);
	clk_register_clkdev(clk, "pll1", NULL);

	clk = clk_register_fixed_factor(NULL, "pll1_2", "pll1",
				CLK_SET_RATE_PARENT, 1, 2);
	clk_register_clkdev(clk, "pll1_2", NULL);

	clk = clk_register_fixed_factor(NULL, "pll1_4", "pll1_2",
				CLK_SET_RATE_PARENT, 1, 2);
	clk_register_clkdev(clk, "pll1_4", NULL);

	clk = clk_register_fixed_factor(NULL, "pll1_8", "pll1_4",
				CLK_SET_RATE_PARENT, 1, 2);
	clk_register_clkdev(clk, "pll1_8", NULL);

	clk = clk_register_fixed_factor(NULL, "pll1_16", "pll1_8",
				CLK_SET_RATE_PARENT, 1, 2);
	clk_register_clkdev(clk, "pll1_16", NULL);

	clk = clk_register_fixed_factor(NULL, "pll1_6", "pll1_2",
				CLK_SET_RATE_PARENT, 1, 3);
	clk_register_clkdev(clk, "pll1_6", NULL);

	clk = clk_register_fixed_factor(NULL, "pll1_12", "pll1_6",
				CLK_SET_RATE_PARENT, 1, 2);
	clk_register_clkdev(clk, "pll1_12", NULL);

	clk = clk_register_fixed_factor(NULL, "pll1_24", "pll1_12",
				CLK_SET_RATE_PARENT, 1, 2);
	clk_register_clkdev(clk, "pll1_24", NULL);

	clk = clk_register_fixed_factor(NULL, "pll1_48", "pll1_24",
				CLK_SET_RATE_PARENT, 1, 2);
	clk_register_clkdev(clk, "pll1_48", NULL);

	clk = clk_register_fixed_factor(NULL, "pll1_96", "pll1_48",
				CLK_SET_RATE_PARENT, 1, 2);
	clk_register_clkdev(clk, "pll1_96", NULL);

	clk = clk_register_fixed_factor(NULL, "pll1_13", "pll1",
				CLK_SET_RATE_PARENT, 1, 13);
	clk_register_clkdev(clk, "pll1_13", NULL);

	clk = clk_register_fixed_factor(NULL, "pll1_13_1_5", "pll1",
				CLK_SET_RATE_PARENT, 2, 3);
	clk_register_clkdev(clk, "pll1_13_1_5", NULL);

	clk = clk_register_fixed_factor(NULL, "pll1_2_1_5", "pll1",
				CLK_SET_RATE_PARENT, 2, 3);
	clk_register_clkdev(clk, "pll1_2_1_5", NULL);

	clk = clk_register_fixed_factor(NULL, "pll1_3_16", "pll1",
				CLK_SET_RATE_PARENT, 3, 16);
	clk_register_clkdev(clk, "pll1_3_16", NULL);

	uart_pll = mmp_clk_register_factor("uart_pll", "pll1_4", 0,
				mpmu_base + MPMU_UART_PLL,
				&uart_factor_masks, uart_factor_tbl,
				ARRAY_SIZE(uart_factor_tbl));
	clk_set_rate(uart_pll, 14745600);
	clk_register_clkdev(uart_pll, "uart_pll", NULL);

	clk = mmp_clk_register_apbc("twsi0", "pll1_13_1_5",
				apbc_base + APBC_TWSI0, 0, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa2xx-i2c.0");

	clk = mmp_clk_register_apbc("twsi1", "pll1_13_1_5",
				apbc_base + APBC_TWSI1, 0, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa2xx-i2c.1");

	clk = mmp_clk_register_apbc("gpio", "vctcxo",
				apbc_base + APBC_GPIO, 10, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa-gpio");

	clk = mmp_clk_register_apbc("kpc", "clk32",
				apbc_base + APBC_KPC, 10, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa27x-keypad");

	clk = mmp_clk_register_apbc("rtc", "clk32",
				apbc_base + APBC_RTC, 10, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "sa1100-rtc");

	clk = mmp_clk_register_apbc("pwm0", "pll1_48",
				apbc_base + APBC_PWM0, 10, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa168-pwm.0");

	clk = mmp_clk_register_apbc("pwm1", "pll1_48",
				apbc_base + APBC_PWM1, 10, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa168-pwm.1");

	clk = mmp_clk_register_apbc("pwm2", "pll1_48",
				apbc_base + APBC_PWM2, 10, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa168-pwm.2");

	clk = mmp_clk_register_apbc("pwm3", "pll1_48",
				apbc_base + APBC_PWM3, 10, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa168-pwm.3");

	clk = clk_register_mux(NULL, "uart0_mux", uart_parent,
				ARRAY_SIZE(uart_parent), CLK_SET_RATE_PARENT,
				apbc_base + APBC_UART0, 4, 3, 0, &clk_lock);
	clk_set_parent(clk, uart_pll);
	clk_register_clkdev(clk, "uart_mux.0", NULL);

	clk = mmp_clk_register_apbc("uart0", "uart0_mux",
				apbc_base + APBC_UART0, 10, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa2xx-uart.0");

	clk = clk_register_mux(NULL, "uart1_mux", uart_parent,
				ARRAY_SIZE(uart_parent), CLK_SET_RATE_PARENT,
				apbc_base + APBC_UART1, 4, 3, 0, &clk_lock);
	clk_set_parent(clk, uart_pll);
	clk_register_clkdev(clk, "uart_mux.1", NULL);

	clk = mmp_clk_register_apbc("uart1", "uart1_mux",
				apbc_base + APBC_UART1,	10, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa2xx-uart.1");

	clk = clk_register_mux(NULL, "uart2_mux", uart_parent,
				ARRAY_SIZE(uart_parent), CLK_SET_RATE_PARENT,
				apbc_base + APBC_UART2, 4, 3, 0, &clk_lock);
	clk_set_parent(clk, uart_pll);
	clk_register_clkdev(clk, "uart_mux.2", NULL);

	clk = mmp_clk_register_apbc("uart2", "uart2_mux",
				apbc_base + APBC_UART2,	10, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa2xx-uart.2");

	clk = clk_register_mux(NULL, "ssp0_mux", ssp_parent,
				ARRAY_SIZE(ssp_parent), CLK_SET_RATE_PARENT,
				apbc_base + APBC_SSP0, 4, 3, 0, &clk_lock);
	clk_register_clkdev(clk, "ssp_mux.0", NULL);

	clk = mmp_clk_register_apbc("ssp0", "ssp0_mux", apbc_base + APBC_SSP0,
				0, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa168-ssp.0");

	clk = clk_register_mux(NULL, "ssp1_mux", ssp_parent,
				ARRAY_SIZE(ssp_parent), CLK_SET_RATE_PARENT,
				apbc_base + APBC_SSP1, 4, 3, 0, &clk_lock);
	clk_register_clkdev(clk, "ssp_mux.1", NULL);

	clk = mmp_clk_register_apbc("ssp1", "ssp1_mux", apbc_base + APBC_SSP1,
				0, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa168-ssp.1");

	clk = clk_register_mux(NULL, "ssp2_mux", ssp_parent,
				ARRAY_SIZE(ssp_parent), CLK_SET_RATE_PARENT,
				apbc_base + APBC_SSP2, 4, 3, 0, &clk_lock);
	clk_register_clkdev(clk, "ssp_mux.2", NULL);

	clk = mmp_clk_register_apbc("ssp2", "ssp2_mux", apbc_base + APBC_SSP2,
				0, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa168-ssp.2");

	clk = clk_register_mux(NULL, "ssp3_mux", ssp_parent,
				ARRAY_SIZE(ssp_parent), CLK_SET_RATE_PARENT,
				apbc_base + APBC_SSP3, 4, 3, 0, &clk_lock);
	clk_register_clkdev(clk, "ssp_mux.3", NULL);

	clk = mmp_clk_register_apbc("ssp3", "ssp3_mux", apbc_base + APBC_SSP3,
				0, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa168-ssp.3");

	clk = clk_register_mux(NULL, "ssp4_mux", ssp_parent,
				ARRAY_SIZE(ssp_parent), CLK_SET_RATE_PARENT,
				apbc_base + APBC_SSP4, 4, 3, 0, &clk_lock);
	clk_register_clkdev(clk, "ssp_mux.4", NULL);

	clk = mmp_clk_register_apbc("ssp4", "ssp4_mux", apbc_base + APBC_SSP4,
				0, 0, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa168-ssp.4");

	clk = mmp_clk_register_apmu("dfc", "pll1_4", apmu_base + APMU_DFC,
				0x19b, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa3xx-nand.0");

	clk = clk_register_mux(NULL, "PXA-SDHCLK", sdh_parent,
				ARRAY_SIZE(sdh_parent), CLK_SET_RATE_PARENT,
				apmu_base + APMU_SDH0, 6, 1, 0, &clk_lock);
	clk_register_clkdev(clk, "PXA-SDHCLK", NULL);

	clk = mmp_clk_register_apmu("sdh0", "PXA-SDHCLK", apmu_base + APMU_SDH0,
				0x1b, &clk_lock);
	clk_register_clkdev(clk, NULL, "sdhci-pxav2.0");

	clk = mmp_clk_register_apmu("sdh1", "PXA-SDHCLK", apmu_base + APMU_SDH1,
				0x1b, &clk_lock);
	clk_register_clkdev(clk, NULL, "sdhci-pxav2.1");

	clk = mmp_clk_register_apmu("sdh2", "PXA-SDHCLK", apmu_base + APMU_SDH2,
				0x1b, &clk_lock);
	clk_register_clkdev(clk, NULL, "sdhci-pxav2.2");

	clk = mmp_clk_register_apmu("sdh3", "PXA-SDHCLK", apmu_base + APMU_SDH3,
				0x1b, &clk_lock);
	clk_register_clkdev(clk, NULL, "sdhci-pxav2.3");

#if 0
	clk = mmp_clk_register_apmu("usb", "usb_pll", apmu_base + APMU_USB,
				0x9, &clk_lock);
	clk_register_clkdev(clk, "usb_clk", NULL);

	clk = mmp_clk_register_apmu("sph", "usb_pll", apmu_base + APMU_USB,
				0x12, &clk_lock);
	clk_register_clkdev(clk, "sph_clk", NULL);
#endif

	clk = mmp_clk_register_apmu("sph", "U2HCLK", apmu_base + APMU_USB,
				0x12, &clk_lock);
	clk_register_clkdev(clk, "U2HCLK", NULL);

	clk = mmp_clk_register_apmu("usb", "U2OCLK", apmu_base + APMU_USB,
				0x09, &clk_lock);
	clk_register_clkdev(clk, "U2OCLK", NULL);

#if 0
	/*Check USB clock start*/
	/*clk = mmp_clk_register_apmu("u2h", "PXA168-U2HCLK", apmu_base + APMU_USB,*/
	clk = clk_register_mux(NULL, "PXA168-U2HCLK", usb_sph_otg_parent/*"pll1_2_1_5"*/,
				ARRAY_SIZE(usb_sph_otg_parent)/*1*/, CLK_SET_RATE_PARENT,
				apmu_base + APMU_USB, 0, 5, 0, &clk_lock);
	clk_register_clkdev(clk, "PXA168-U2HCLK", NULL);

	clk = mmp_clk_register_apmu("pxa-sph", "PXA168-U2HCLK", apmu_base + APMU_USB,
				0x12, &clk_lock);
	clk_register_clkdev(clk, "pxa-sph", NULL);

	clk = clk_register_mux(NULL, "PXA168-U2OCLK", usb_sph_otg_parent/*"pll1_2_1_5"*/,
				ARRAY_SIZE(usb_sph_otg_parent)/*1*/, CLK_SET_RATE_PARENT,
				apmu_base + APMU_USB, 0, 5, 0, &clk_lock);
	clk_register_clkdev(clk, "PXA168-U2OCLK", NULL);

	clk = mmp_clk_register_apmu("mv-otg", "PXA168-U2OCLK", apmu_base + APMU_USB,
				0x09, &clk_lock);
	clk_register_clkdev(clk, NULL, "mv-otg");
#if 1
	clk = clk_register_mux(NULL, "PXA168-U2OEHCICLK", usb_sph_otg_parent/*"pll1_2_1_5"*/,
				ARRAY_SIZE(usb_sph_otg_parent)/*1*/, CLK_SET_RATE_PARENT,
				apmu_base + APMU_USB, 0, 5, 0, &clk_lock);
	clk_register_clkdev(clk, "PXA168-U2OCLK", NULL);

	clk = mmp_clk_register_apmu("pxa-sph.0", "PXA168-U2OEHCICLK", apmu_base + APMU_USB,
				0x09, &clk_lock);
	clk_register_clkdev(clk, NULL, "pxa-sph.0");

	/*clk = mmp_clk_register_apmu("PXA168-U2OEHCICLK", "PXA168-U2HCLK", apmu_base + APMU_USB,
				0x12, &clk_lock);
	clk_register_clkdev(clk, "PXA168-U2OEHCICLK", NULL);*/

	clk = clk_register_mux(NULL, "PXA168-U2OGADGETCLK", usb_sph_otg_parent/*"pll1_2_1_5"*/,
				ARRAY_SIZE(usb_sph_otg_parent)/*1*/, CLK_SET_RATE_PARENT,
				apmu_base + APMU_USB, 0, 5, 0, &clk_lock);
	clk_register_clkdev(clk, "PXA168-U2OGADGETCLK", NULL);
	clk = mmp_clk_register_apmu("mv-udc", "PXA168-U2OGADGETCLK", apmu_base + APMU_USB,
				0x09, &clk_lock);
	clk_register_clkdev(clk, NULL, "mv-udc");
	/*clk_register_clkdev(clk, "PXA168-U2OGADGETCLK", NULL);*/
#endif

		/*Check USB clock End*/
#endif

	/*clk = clk_register_mux(NULL, "disp0_mux", disp_parent,*/
	clk = clk_register_mux(NULL, "LCDCLK", disp_parent,
				ARRAY_SIZE(disp_parent), CLK_SET_RATE_PARENT,
				apmu_base + APMU_DISP0, 0, 7, 0, &clk_lock);
				/*apmu_base + APMU_DISP0, 6, 1, 0, &clk_lock);*/
	/*clk_register_clkdev(clk, "disp_mux.0", NULL);*/
	clk_register_clkdev(clk, "LCDCLK", NULL);

	clk = mmp_clk_register_apmu("disp0", "LCDCLK",
				apmu_base + APMU_DISP0, 0x7f, &clk_lock);
				/*apmu_base + APMU_DISP0, 0x1b, &clk_lock);*/
	clk_register_clkdev(clk, NULL, "pxa168-fb");

	clk = clk_register_mux(NULL, "ccic0_mux", ccic_parent,
				ARRAY_SIZE(ccic_parent), CLK_SET_RATE_PARENT,
				apmu_base + APMU_CCIC0, 6, 1, 0, &clk_lock);
	clk_register_clkdev(clk, "ccic_mux.0", NULL);

	clk = mmp_clk_register_apmu("ccic0", "ccic0_mux",
				apmu_base + APMU_CCIC0, 0x1b, &clk_lock);
	clk_register_clkdev(clk, "fnclk", "mmp-ccic.0");

	clk = clk_register_mux(NULL, "ccic0_phy_mux", ccic_phy_parent,
				ARRAY_SIZE(ccic_phy_parent),
				CLK_SET_RATE_PARENT, apmu_base + APMU_CCIC0,
				7, 1, 0, &clk_lock);
	clk_register_clkdev(clk, "ccic_phy_mux.0", NULL);

	clk = mmp_clk_register_apmu("ccic0_phy", "ccic0_phy_mux",
				apmu_base + APMU_CCIC0, 0x24, &clk_lock);
	clk_register_clkdev(clk, "phyclk", "mmp-ccic.0");

	clk = clk_register_divider(NULL, "ccic0_sphy_div", "ccic0_mux",
				CLK_SET_RATE_PARENT, apmu_base + APMU_CCIC0,
				10, 5, 0, &clk_lock);
	clk_register_clkdev(clk, "sphyclk_div", NULL);

	clk = mmp_clk_register_apmu("ccic0_sphy", "ccic0_sphy_div",
				apmu_base + APMU_CCIC0, 0x300, &clk_lock);
	clk_register_clkdev(clk, "sphyclk", "mmp-ccic.0");

#if 0
	clk = clk_register_mux(NULL, "GCCLK", "pll1_16",
				1, CLK_SET_RATE_PARENT, apmu_base + APMU_GC,
				0, 8, 0, &clk_lock);
	/*clk->ops = gc300_clk_ops;*/
	clk_register_clkdev(clk, "GCCLK", NULL);
#endif
	clk = mmp_clk_register_apmu("pxa168-eth", "MFUCLK", apmu_base + APMU_ETH,
				0x9, &clk_lock);
	clk_register_clkdev(clk, "MFUCLK", NULL);
#endif
}
