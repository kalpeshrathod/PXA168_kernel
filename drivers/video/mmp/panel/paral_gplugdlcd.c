/*
 * linux/drivers/video/mmp/panel/paral_gplugdlcd.c
 * active panel using spi interface to do init
 *
 * Copyright (C) 2013 Marvell Technology Group Ltd.
 * Authors:  Guoqing Li <ligq@marvell.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <video/mmp_disp.h>

struct gplugdlcd_plat_data {
	void (*plat_onoff)(int skip_on, int status);
};

static void gplugdlcd_onoff(struct mmp_panel *panel, int status)
{
	struct gplugdlcd_plat_data *plat = panel->plat_data;

	if (plat->plat_onoff)
		plat->plat_onoff(0, status);
}

static void gplugdlcd_reduced_onoff(struct mmp_panel *panel, int status)
{
	struct gplugdlcd_plat_data *plat = panel->plat_data;

	if (plat->plat_onoff)
		plat->plat_onoff(1, status);
}

static struct mmp_mode mmp_modes_gplugdlcd[] = {
	[0] = {
		.pixclock_freq = 13468,
/*		.pixclock_freq = 33264000,*/
		.refresh = 60,
		.xres = 1280,
		.yres = 720,
		.hsync_len = 40,
		.left_margin = 220,
		.right_margin = 110,
		.vsync_len = 5,
		.upper_margin = 20,
		.lower_margin = 6,
		.pix_fmt_out = PIXFMT_RGB565,

/*		.pixclock       = 13468,
                .refresh        = 60,
                .xres           = 1280,
                .yres           = 720,
                .hsync_len      = 40,
                .left_margin    = 220,
                .right_margin   = 110,
                .vsync_len      = 5,
                .upper_margin   = 20,
                .lower_margin   = 5,
                .sync           = 0,*/

	},
};

static int gplugdlcd_get_modelist(struct mmp_panel *panel,
		struct mmp_mode **modelist)
{
	*modelist = mmp_modes_gplugdlcd;
	return 1;
}

static struct mmp_panel panel_gplugdlcd = {
	.name = "gplugdlcd",
	.panel_type = PANELTYPE_ACTIVE,
	.get_modelist = gplugdlcd_get_modelist,
	.set_onoff = gplugdlcd_onoff,
/*	.reduced_onoff = gplugdlcd_reduced_onoff,*/
};

static int gplugdlcd_probe(struct platform_device *pdev)
{
	struct mmp_mach_panel_info *mi;
	struct gplugdlcd_plat_data *plat_data;

	pr_warn("%s %s %d: Entry\n", __FILE__, __func__, __LINE__);

	/* get configs from platform data */
	mi = pdev->dev.platform_data;
	if (mi == NULL) {
		dev_err(&pdev->dev, "no platform data defined\n");
	pr_warn("%s %s %d: Entry\n", __FILE__, __func__, __LINE__);
		return -EINVAL;
	}

	plat_data = kzalloc(sizeof(*plat_data), GFP_KERNEL);
	if (!plat_data)
		return -ENOMEM;

	pr_warn("%s %s %d: Entry\n", __FILE__, __func__, __LINE__);
	plat_data->plat_onoff = mi->plat_set_onoff;
	panel_gplugdlcd.plat_data = plat_data;
	panel_gplugdlcd.plat_path_name = mi->plat_path_name;
	panel_gplugdlcd.dev = &pdev->dev;

	pr_warn("%s %s %d: Entry\n", __FILE__, __func__, __LINE__);
	mmp_register_panel(&panel_gplugdlcd);

	pr_warn("%s %s %d: Entry\n", __FILE__, __func__, __LINE__);
	return 0;
}

static int gplugdlcd_remove(struct platform_device *dev)
{
	mmp_unregister_panel(&panel_gplugdlcd);
	kfree(panel_gplugdlcd.plat_data);

	return 0;
}

static struct platform_driver gplugdlcd_driver = {
	.driver		= {
		.name	= "mmp-gplugdlcd",
		.owner	= THIS_MODULE,
	},
	.probe		= gplugdlcd_probe,
	.remove		= gplugdlcd_remove,
};

static int gplugdlcd_init(void)
{
	return platform_driver_register(&gplugdlcd_driver);
}
static void gplugdlcd_exit(void)
{
	platform_driver_unregister(&gplugdlcd_driver);
}
module_init(gplugdlcd_init);
//module_exit(gplugdlcd_exit);

MODULE_AUTHOR("Guoqing Li<ligq@marvell.com>");
MODULE_DESCRIPTION("Panel driver for gPlugD Test LCD");
MODULE_LICENSE("GPL");
