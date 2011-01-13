/*
 * LCD panel driver for PSP 1000 TFT
 *
 * Copyright (C) 2010 Robert Spanton
 * Copyright (C) 2008 Nokia Corporation
 * Author: Tomi Valkeinen <tomi.valkeinen@nokia.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/module.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/regulator/consumer.h>
#include <linux/err.h>

#include <plat/display.h>

static struct omap_video_timings psp_1000_timings = {
	.x_res = 480,
	.y_res = 272,

	.pixel_clock	= 9000,

	.hsw		= 41,
	.hfp		= 2,
	.hbp		= 2,

	.vsw		= 10,
	.vfp		= 2,
	.vbp		= 2,
};

static int psp_1000_panel_probe(struct omap_dss_device *dssdev)
{
	dssdev->panel.config = OMAP_DSS_LCD_TFT | OMAP_DSS_LCD_IVS |
		OMAP_DSS_LCD_IHS;
	dssdev->panel.acb = 0x28;
	dssdev->panel.timings = psp_1000_timings;

	return 0;
}

static void psp_1000_panel_remove(struct omap_dss_device *dssdev)
{

}

static int psp_1000_panel_enable(struct omap_dss_device *dssdev)
{
	int r = 0;

	/* wait couple of vsyncs until enabling the LCD */
	msleep(50);

	if (dssdev->platform_enable)
		r = dssdev->platform_enable(dssdev);

	return r;
}

static void psp_1000_panel_disable(struct omap_dss_device *dssdev)
{
	if (dssdev->platform_disable)
		dssdev->platform_disable(dssdev);

	/* wait at least 5 vsyncs after disabling the LCD */

	msleep(100);
}

static int psp_1000_panel_suspend(struct omap_dss_device *dssdev)
{
	psp_1000_panel_disable(dssdev);
	return 0;
}

static int psp_1000_panel_resume(struct omap_dss_device *dssdev)
{
	return psp_1000_panel_enable(dssdev);
}

static struct omap_dss_driver psp_1000_driver = {
	.probe		= psp_1000_panel_probe,
	.remove		= psp_1000_panel_remove,

	.enable		= psp_1000_panel_enable,
	.disable	= psp_1000_panel_disable,
	.suspend	= psp_1000_panel_suspend,
	.resume		= psp_1000_panel_resume,

	.driver         = {
		.name   = "psp_1000_panel",
		.owner  = THIS_MODULE,
	},
};

static int __init psp_1000_panel_drv_init(void)
{
	return omap_dss_register_driver(&psp_1000_driver);
}

static void __exit psp_1000_panel_drv_exit(void)
{
	omap_dss_unregister_driver(&psp_1000_driver);
}

module_init(psp_1000_panel_drv_init);
module_exit(psp_1000_panel_drv_exit);
MODULE_LICENSE("GPL");
