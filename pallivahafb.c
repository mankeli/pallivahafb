/*
 * linux/drivers/video/pallivahafb.c -- bram kulli frame buffer device
 *
 * Copyright (C) 2001
 *
 *      Richard Zidlicky <rz@linux-m68k.org> (q40fb.c)
 *
 * 
 * 
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License. See the file COPYING in the main directory of this archive for
 *  more details.
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>

#include <linux/uaccess.h>
#include <asm/setup.h>
#include <linux/fb.h>
#include <linux/module.h>
#include <asm/pgtable.h>

#define PALLIVAHA_PHYS_SCREEN_ADDR  0x40000000
#define PALLIVAHA_PHYS_PAL_ADDR 0x42000000

static struct fb_fix_screeninfo pallivahafb_fix = {
	.id= "Pallivaha",
	.smem_len = 320*200,
	.type = FB_TYPE_PACKED_PIXELS,
	.visual = FB_VISUAL_PSEUDOCOLOR,
	.line_length = 320,
	.accel = FB_ACCEL_NONE,
};

static const struct fb_var_screeninfo pallivahafb_var = {
	.xres= 320,
	.yres= 200,
	.xres_virtual= 320,
	.yres_virtual= 200,
	.bits_per_pixel= 8,
	.activate= FB_ACTIVATE_NOW,
	.height= 200,
	.width= 200,
	.vmode= FB_VMODE_NONINTERLACED,
	.red = { 0, 8, 0 },
	.green = { 0, 8, 0 },
	.blue = { 0, 8, 0 },
	.transp = { 0, 8, 0 },
};

#include "colormapper.h"


#if 0
struct pallivahafb_drvdata {

	struct fb_info	info;

	void __iomem	*palette;

	u32		pseudo_palette[PALETTE_ENTRIES_NO];
};
#endif


static int pallivahafb_setcolreg(unsigned regno, unsigned red, unsigned green,
 unsigned blue, unsigned transp,
 struct fb_info *info)
{
		/*
		 *  Set a single color register. The values supplied have a 16 bit
		 *  magnitude.
		 *  Return != 0 for invalid regno.
		 */

	char __iomem *pal_base;

	if (regno > 255)
		return 1;

	pal_base = ioremap_wc(PALLIVAHA_PHYS_PAL_ADDR, 256*4);

	printk("pal set: %u = %u,%u,%u\n", regno, red, green, blue);


	((uint32_t *)pal_base)[regno] = getcolor(red, green, blue);

	iounmap(pal_base);

//       red = green = blue = (red * 77 + green * 151 + blue * 28) >> 8;


	if (regno < 16) {
		((u32 *)info->pseudo_palette)[regno] = ((red & 31) <<6) |
		((green & 31) << 11) |
		(blue & 63);
	}
	return 0;
}

static struct fb_ops pallivahafb_ops = {
	.owner= THIS_MODULE,
	.fb_setcolreg= pallivahafb_setcolreg,
	.fb_fillrect= cfb_fillrect,
	.fb_copyarea= cfb_copyarea,
	.fb_imageblit= cfb_imageblit,
};

static int pallivahafb_probe(struct platform_device *dev)
{
	struct fb_info *info;

	pallivahafb_fix.smem_start = PALLIVAHA_PHYS_SCREEN_ADDR;

	// + 16 because pseudo palette is there
	info = framebuffer_alloc(sizeof(u32) * 16, &dev->dev);
	if (!info)
		return -ENOMEM;

	info->var = pallivahafb_var;
	info->fix = pallivahafb_fix;
	info->fbops = &pallivahafb_ops;
  info->flags = FBINFO_DEFAULT;  /* not as module for now */
	info->pseudo_palette = info->par;
	info->par = NULL;

	info->screen_base = ioremap_wc(info->fix.smem_start,
	 info->fix.smem_len);

	if (fb_alloc_cmap(&info->cmap, 256, 0) < 0) {
		framebuffer_release(info);
		return -ENOMEM;
	}

	if (register_framebuffer(info) < 0) {
		printk(KERN_ERR "Unable to register Pallivaha frame buffer\n");
		fb_dealloc_cmap(&info->cmap);
		framebuffer_release(info);
		return -EINVAL;
	}

	fb_info(info, "Pallivaha frame buffer alive and kicking !\n");
	return 0;
}

static struct platform_driver pallivahafb_driver = {
	.probe= pallivahafb_probe,
	.driver= {
		.name= "pallivahafb",
	},
};

static struct platform_device pallivahafb_device = {
	.name= "pallivahafb",
};

int __init pallivahafb_init(void)
{
	int ret = 0;

	if (fb_get_options("pallivahafb", NULL))
		return -ENODEV;

	ret = platform_driver_register(&pallivahafb_driver);

	if (!ret) {
		ret = platform_device_register(&pallivahafb_device);
		if (ret)
			platform_driver_unregister(&pallivahafb_driver);
	}
	return ret;
}

module_init(pallivahafb_init);

static void __exit pallivahafb_exit(void)
{
	platform_device_unregister(&pallivahafb_device);
	platform_driver_unregister(&pallivahafb_driver);
}

module_exit(pallivahafb_exit);

MODULE_LICENSE("GPL");
