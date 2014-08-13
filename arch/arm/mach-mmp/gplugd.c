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
		.type = "isl1208",
		.addr = 0x6F,
	}
};

/* Bring PHY out of reset by setting GPIO 104 */
static int gplugd_eth_init(void)
{
	if (unlikely(gpio_request(104, "ETH_RESET_N"))) {
		printk(KERN_ERR "Can't get hold of GPIO 104 to bring Ethernet "
				"PHY out of reset\n");
		return -EIO;
	}

	gpio_direction_output(104, 1);
	gpio_free(104);
	return 0;
}

struct pxa168_eth_platform_data gplugd_eth_platform_data = {
	.port_number = 0,
	.phy_addr    = 0,
	.speed       = 0, /* Autonagotiation */
	.init        = gplugd_eth_init,
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
/* gplugD uses serial number tag to pass MAC address */
static void __init read_store_mac_addr(void)
{
	int i;
	u8 *mac_addr = gplugd_eth_platform_data.mac_addr;

	*mac_addr++ = (system_serial_high >> 8) & 0xff;
	*mac_addr++ = system_serial_high & 0xff;

	for (i = 3; i >= 0; i--)
		*mac_addr++ = (system_serial_low >> i*8) & 0xff;
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

	read_store_mac_addr();
	pxa168_add_eth(&gplugd_eth_platform_data);

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

	pxa168_add_ssp(2);

	pxa168_add_spi(2, &pxa_ssp_master_info);
	spi_register_board_info(gplugD_spi_board_info, ARRAY_SIZE(gplugD_spi_board_info));
}

MACHINE_START(GPLUGD, "PXA168-based GuruPlug Display (gplugD) Platform")
	.map_io		= mmp_map_io,
	.nr_irqs	= MMP_NR_IRQS,
	.init_irq       = pxa168_init_irq,
	.init_time	= pxa168_timer_init,
	.init_machine   = gplugd_init,
	.restart	= pxa168_restart,
MACHINE_END
