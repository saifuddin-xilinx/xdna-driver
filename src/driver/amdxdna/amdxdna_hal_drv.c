// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2022-2026, Advanced Micro Devices, Inc.
 *
 * Common Framework - Prototype only (no implementation).
 *
 * In the real driver this layer provides:
 *   • IOCTL interface    → amdxdna_ioctl.c / DRM ioctl handlers
 *   • GEM               → amdxdna_gem.c
 *   • DRM core          → dma-buf, file ops, device registration
 *
 * This file is a placeholder so the layer exists in the prototype.
 */

#include <linux/module.h>

#include "amdxdna_hal_drv.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("AMD XDNA HAL - Common framework placeholder");

int amdxdna_hal_drv_init(void)
{
	/* Placeholder: would register DRM driver, GEM, IOCTL */
	return 0;
}

void amdxdna_hal_drv_fini(void)
{
	/* Placeholder: would unregister DRM driver */
}
