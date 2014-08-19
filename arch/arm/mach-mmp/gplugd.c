/*
 *  linux/arch/arm/mach-mmp/gplugd.c
 *
 *  Support for the Marvell PXA168-based GuruPlug Display (gplugD) Platform.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  publishhed by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/gpio-pxa.h>
#include <linux/mmc/sdhci.h>
#include <linux/spi/spi.h>
#include <linux/spi/pxa2xx_spi.h>

#include <asm/mach/arch.h>
#include <asm/mach-types.h>
#include <asm/system.h>

#include <mach/irqs.h>
#include <mach/pxa168.h>
#include <mach/mfp-pxa168.h>
#include <mach/regs-mpmu.h>
#include <mach/regs-timers.h>

#include <uapi/linux/ethtool.h>
#include <linux/etherdevice.h>
#include "common.h"

static unsigned long gplugd_pin_config[] __initdata = {
	/* UART3 */
	GPIO8_UART3_TXD,
	GPIO9_UART3_RXD,
	GPIO1O_UART3_CTS,
	GPIO11_UART3_RTS,

	/* USB OTG PEN */
	GPIO18_GPIO,

	/* MMC2 */
	GPIO28_MMC2_CMD,
	GPIO29_MMC2_CLK,
	GPIO30_MMC2_DAT0,
	GPIO31_MMC2_DAT1,
	GPIO32_MMC2_DAT2,
	GPIO33_MMC2_DAT3,

	/* LCD & HDMI clock selection GPIO: 0: 74.176MHz, 1: 74.25 MHz */
	GPIO35_GPIO,
	GPIO36_GPIO, /* CEC Interrupt */

	/* MMC1 */
	GPIO43_MMC1_CLK,
	GPIO49_MMC1_CMD,
	GPIO41_MMC1_DAT0,
	GPIO40_MMC1_DAT1,
	GPIO52_MMC1_DAT2,
	GPIO51_MMC1_DAT3,
	GPIO53_MMC1_CD,

	/* LCD */
	GPIO56_LCD_FCLK_RD,
	GPIO57_LCD_LCLK_A0,
	GPIO58_LCD_PCLK_WR,
	GPIO59_LCD_DENA_BIAS,
	GPIO60_LCD_DD0,
	GPIO61_LCD_DD1,
	GPIO62_LCD_DD2,
	GPIO63_LCD_DD3,
	GPIO64_LCD_DD4,
	GPIO65_LCD_DD5,
	GPIO66_LCD_DD6,
	GPIO67_LCD_DD7,
	GPIO68_LCD_DD8,
	GPIO69_LCD_DD9,
	GPIO70_LCD_DD10,
	GPIO71_LCD_DD11,
	GPIO72_LCD_DD12,
	GPIO73_LCD_DD13,
	GPIO74_LCD_DD14,
	GPIO75_LCD_DD15,
	GPIO76_LCD_DD16,
	GPIO77_LCD_DD17,
	GPIO78_LCD_DD18,
	GPIO79_LCD_DD19,
	GPIO80_LCD_DD20,
	GPIO81_LCD_DD21,
	GPIO82_LCD_DD22,
	GPIO83_LCD_DD23,

	/* GPIO */
	GPIO84_GPIO,
	GPIO85_GPIO,

	/* Fast-Ethernet*/
	GPIO86_TX_CLK,
	GPIO87_TX_EN,
	GPIO88_TX_DQ3,
	GPIO89_TX_DQ2,
	GPIO90_TX_DQ1,
	GPIO91_TX_DQ0,
	GPIO92_MII_CRS,
	GPIO93_MII_COL,
	GPIO94_RX_CLK,
	GPIO95_RX_ER,
	GPIO96_RX_DQ3,
	GPIO97_RX_DQ2,
	GPIO98_RX_DQ1,
	GPIO99_RX_DQ0,
	GPIO100_MII_MDC,
	GPIO101_MII_MDIO,
	GPIO103_RX_DV,
	GPIO104_GPIO,     /* Reset PHY */

	/* RTC interrupt */
	GPIO102_GPIO,

	/* I2C */
	GPIO105_CI2C_SDA,
	GPIO106_CI2C_SCL,

	/* SPI NOR Flash on SSP2 */
	GPIO107_SSP2_RXD,
	GPIO108_SSP2_TXD,
	GPIO110_GPIO,     /* SPI_CSn */
	GPIO111_SSP2_CLK,

	/* Select JTAG */
	GPIO109_GPIO,

	/* I2S */
	GPIO114_I2S_FRM,
	GPIO115_I2S_BCLK,
	GPIO116_I2S_TXD
};


static struct pxa_gpio_platform_data pxa168_gpio_pdata = {
	.irq_base	= MMP_GPIO_TO_IRQ(0),
};

static struct i2c_pxa_platform_data i2c_info __initdata = {
	.use_pio	= 1,
};

static struct i2c_board_info gplugd_i2c_board_info[] = {
	{
		.type = "tda998X",
		.addr = 0x70,
	},
	{
		.type = "tda99Xcec",
		.addr = 0x34,
	},
	{
		.type = "isl1208",
		.addr = 0x6F,
	}
};

struct sdhci_pxa_platdata gplugd_sdh_platdata = {
	.quirks = SDHCI_QUIRK_NO_HISPD_BIT | SDHCI_QUIRK_NO_BUSY_IRQ | SDHCI_QUIRK_32BIT_DMA_SIZE,
};

static void __init select_disp_freq(void)
{
	/* set GPIO 35 & clear GPIO 85 to set LCD External Clock to 74.25 MHz */
	if (unlikely(gpio_request(35, "DISP_FREQ_SEL"))) {
		printk(KERN_ERR "Can't get hold of GPIO 35 to select display "
				"frequency\n");
	} else {
		gpio_direction_output(35, 1);
		gpio_free(35);
	}

	if (unlikely(gpio_request(85, "DISP_FREQ_SEL_2"))) {
		printk(KERN_ERR "Can't get hold of GPIO 85 to select display "
				"frequency\n");
	} else {
		gpio_direction_output(85, 0);
		gpio_free(85);
	}
}

#ifdef CONFIG_USB_SUPPORT

#if defined(CONFIG_USB_EHCI_HCD) && defined(CONFIG_USB_EHCI_MV)

static char *pxa168_u2h_clock_name[] = {
	[0] = "U2HCLK",
};

static struct mv_usb_platform_data pxa168_u2h_pdata = {
	.mode           = MV_USB_MODE_HOST,
	.phy_init       = pxa_usb_phy_init,
	.phy_deinit     = pxa_usb_phy_deinit,
	.set_vbus       = NULL,
};
#endif

#if defined(CONFIG_USB_MV_UDC) || defined(CONFIG_USB_EHCI_MV_U2O)
static char *pxa168_u2o_clock_name[] = {
	[0] = "U2OCLK",
};

static struct mv_usb_platform_data pxa168_u2o_udc_pdata = {
	.vbus			= NULL,
	.mode			= MV_USB_MODE_OTG,
	.otg_force_a_bus_req	= 1,
	.phy_init		= pxa_usb_phy_init,
	.phy_deinit		= pxa_usb_phy_deinit,
	.set_vbus		= NULL,
};
static struct mv_usb_platform_data pxa168_u2o_pdata = {
	.vbus			= NULL,
	.mode			= MV_USB_MODE_OTG,
	.otg_force_a_bus_req	= 1,
	.phy_init		= pxa_usb_phy_init,
	.phy_deinit		= pxa_usb_phy_deinit,
	.set_vbus		= NULL,
};
static struct mv_usb_platform_data pxa168_u2o_otg_pdata = {
	.vbus			= NULL,
	.mode			= MV_USB_MODE_OTG,
	.otg_force_a_bus_req	= 1,
	.phy_init		= pxa_usb_phy_init,
	.phy_deinit		= pxa_usb_phy_deinit,

	.set_vbus		= NULL,
};

#endif
#endif

static struct pxa2xx_spi_master pxa_ssp_master_info = {
	.num_chipselect	= 1,
	.enable_dma = 1,
};

static struct pxa2xx_spi_chip AT45DB041D_spi_info = {
	.tx_threshold = 1,
	.rx_threshold = 1,
	.timeout = 1000,
	.gpio_cs = 110
};

static struct spi_board_info __initdata gplugD_spi_board_info[] = {
	{
		.modalias = "mtd_dataflash",
		.mode = SPI_MODE_0,
		.max_speed_hz = 260000,
		.bus_num = 2,
		.chip_select = 0,
		.platform_data = NULL,
		.controller_data = &AT45DB041D_spi_info,
		.irq = -1,
	},
};

static inline int pxa168_add_spi(int id, struct pxa2xx_spi_master *pdata)
{
	struct platform_device *pd;

	pd = platform_device_alloc("pxa2xx-spi", id);
	if (pd == NULL) {
		pr_err("pxa2xx-spi: failed to allocate device (id=%d)\n", id);
		return -ENOMEM;
	}

	platform_device_add_data(pd, pdata, sizeof(*pdata));

	return platform_device_add(pd);
}

#ifdef CONFIG_MTD
static void gplugd_parse_enetaddr(const u8 *addr, u8 *enetaddr)
{
	char *end;
	int i;

	for (i = 0; i < 6; ++i) {
		enetaddr[i] = addr ? simple_strtoul(addr, &end, 16) : 0;
		if (addr)
			addr = (*end) ? end + 1 : end;
	}
}

static struct pxa168_eth_platform_data gplugd_eth_platform_data;

static void __init read_device_serial_mac(void)
{
#define GPLUGD_SERIAL_NUM_LENGTH        14
#define GPLUGD_MAC_ADDR_LENGTH          18
	char otp_serial_num[GPLUGD_SERIAL_NUM_LENGTH];
	char otp_mac_addr[GPLUGD_MAC_ADDR_LENGTH];
	u8 *mac_addr = gplugd_eth_platform_data.mac_addr;
	size_t retlen;
	struct mtd_info *mtd = NULL;

	mtd = get_mtd_device(NULL, 0x00);

	if (!mtd) {
		pr_err("%s %d: No MTD devices found", __func__, __LINE__);
		return;
	}

	if (mtd->_read_user_prot_reg) {
		if (!mtd->_read_user_prot_reg(mtd, 0, GPLUGD_SERIAL_NUM_LENGTH,
					&retlen, otp_serial_num)) {
			if (!mtd->_read_user_prot_reg(mtd,
						GPLUGD_SERIAL_NUM_LENGTH,
						GPLUGD_MAC_ADDR_LENGTH,
						&retlen, otp_mac_addr)) {

				gplugd_parse_enetaddr(otp_mac_addr, mac_addr);
			} else {
				pr_err("%s %d: OTP MAC Address read failed\n",
						__func__, __LINE__);
			}
		} else {
			pr_err("%s %d: OTP Serial Number read failed\n",
						__func__, __LINE__);
		}
	}

	put_mtd_device(mtd);
}
#else
static void __init read_device_serial_mac(void) {}
#endif

static int device_mac_address(u8 *mac_addr)
{
	pr_warn("%s %d: Entry\n", __func__, __LINE__);
	read_device_serial_mac();
	if (!is_valid_ether_addr(gplugd_eth_platform_data.mac_addr))
		return -EINVAL;
	memcpy(mac_addr, gplugd_eth_platform_data.mac_addr, 6);
	return 0;
}

/* Bring PHY out of reset by setting GPIO 104 */
static int gplugd_eth_init(void)
{
	int gpio_enet_reset = 104;
	int gpio_enet_coma = 102;

	if (unlikely(gpio_request(gpio_enet_reset, "ETH_RESET_N"))) {
		printk(KERN_ERR "Can't get hold of GPIO %d to bring Ethernet "
				"PHY out of reset\n", gpio_enet_reset);
		return -EIO;
	}
	if (unlikely(gpio_request(gpio_enet_coma, "ETH_COMA_N"))) {
		printk(KERN_ERR "Can't get hold of GPIO %d to bring Ethernet "
				"PHY out of coma\n", gpio_enet_coma);
		return -EIO;
	}


	gpio_direction_output(gpio_enet_reset, 1);
	gpio_direction_output(gpio_enet_coma, 1);

	gpio_free(gpio_enet_reset);
	gpio_free(gpio_enet_coma);
	return 0;
}

static struct pxa168_eth_platform_data gplugd_eth_platform_data = {
	.port_number = 0,
	.phy_addr    = 0,
	.speed       = 0, /* Autonagotiation */
	.mac_addr    = {0x00},
	.init        = gplugd_eth_init,
	.device_mac  = device_mac_address,
};


static struct fb_videomode video_modes[] = {
	/* TDA9989 Video Mode */
	[0] = {
		.pixclock	= 13468,
		.refresh	= 60,
		.xres		= 1280,
		.yres		= 720,
		.hsync_len	= 40,
		.left_margin	= 220,
		.right_margin	= 110,
		.vsync_len	= 5,
		.upper_margin	= 20,
		.lower_margin	= 5,
		.sync		= 0,
	},
};

static struct pxa168fb_mach_info tda9981_hdmi_info __initdata = {
	.id			= "tda9981",
	.modes			= video_modes,
	.num_modes		= ARRAY_SIZE(video_modes),
	.pix_fmt		= PIX_FMT_RGB565,
	.io_pin_allocation_mode	= PIN_MODE_DUMB_24,
	.dumb_mode		= DUMB_MODE_RGB888,
	.active			= 1,
	.panel_rbswap		= 1,
	.invert_pixclock	= 0,
	.max_fb_size	    = (1280 * 720 * 4),
};

static void gplugd_restart(char mode, const char *cmd)
{
#define MPMU_APRR_WDTR	(1<<4)
	volatile u32 watchdog_virt_base;

	watchdog_virt_base = (u32)TIMERS1_VIRT_BASE;
	/* negate hardware reset to the WDT after system reset */
	writel((readl(MPMU_APRR) | MPMU_APRR_WDTR), MPMU_APRR);

	/*disable functional WDT clock */
	writel(0x1, MPMU_WDTPCR);

	/* clear previous WDT status */
	writel(0xbaba, (volatile void*)(watchdog_virt_base + TMR_WFAR));
	writel(0xeb10, (volatile void*)(watchdog_virt_base + TMR_WSAR));
	writel(0, (volatile void*)(watchdog_virt_base + TMR_WSR));

	/* set match counter */
	writel(0xbaba, (volatile void*)(watchdog_virt_base + TMR_WFAR));
	writel(0xeb10, (volatile void*)(watchdog_virt_base + TMR_WSAR));
	writel(0xf, (volatile void*)(watchdog_virt_base + TMR_WMR));

	/* enable WDT reset */
	writel(0xbaba, (volatile void*)(watchdog_virt_base + TMR_WFAR));
	writel(0xeb10, (volatile void*)(watchdog_virt_base + TMR_WSAR));
	writel(0x3, (volatile void*)(watchdog_virt_base + TMR_WMER));

	/*enable functional WDT clock */
	writel(0x3, MPMU_WDTPCR);
}

static void __init gplugd_init(void)
{
	mfp_config(ARRAY_AND_SIZE(gplugd_pin_config));
	platform_device_add_data(&pxa168_device_gpio, &pxa168_gpio_pdata,
				 sizeof(struct pxa_gpio_platform_data));
	platform_device_register(&pxa168_device_gpio);

	select_disp_freq();

	/* on-chip devices */
	pxa168_add_uart(3);

	pxa168_add_ssp(1);

	pxa168_add_twsi(0, &i2c_info, ARRAY_AND_SIZE(gplugd_i2c_board_info));

	pxa168_add_spi(2, &pxa_ssp_master_info);
	spi_register_board_info(gplugD_spi_board_info,
			ARRAY_SIZE(gplugD_spi_board_info));

	pxa168_add_ssp(2);

	pxa168_add_sdh(1, &gplugd_sdh_platdata);
	pxa168_add_sdh(2, &gplugd_sdh_platdata);

#if defined(CONFIG_USB_EHCI_HCD) && defined(CONFIG_USB_EHCI_MV)
	pxa168_device_u2h.dev.platform_data = &pxa168_u2h_pdata;
	platform_device_register(&pxa168_device_u2h);
#endif
#ifdef CONFIG_USB_MV_UDC
	pxa168_device_u2o.dev.platform_data = &pxa168_u2o_udc_pdata;
	platform_device_register(&pxa168_device_u2o);
#endif

#ifdef CONFIG_USB_EHCI_MV_U2O
	pxa168_device_u2oehci.dev.platform_data = &pxa168_u2o_pdata;
	platform_device_register(&pxa168_device_u2oehci);
#endif
	pxa168_add_fb(&tda9981_hdmi_info);
	pxa168_add_fb_ovly(&tda9981_hdmi_info);

	pxa168_add_eth(&gplugd_eth_platform_data);
}

MACHINE_START(GPLUGD, "PXA168-based GuruPlug Display (gplugD) Platform")
	.map_io		= mmp_map_io,
	.nr_irqs	= MMP_NR_IRQS,
	.init_irq       = pxa168_init_irq,
	.init_time	= pxa168_timer_init,
	.init_machine   = gplugd_init,
	.restart	= gplugd_restart,
MACHINE_END
