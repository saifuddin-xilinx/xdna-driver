// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2022-2026, Advanced Micro Devices, Inc.
 *
 * AIE4/PCI HAL stub - prototype for PCI device binding (AIE4).
 * Real implementation lives in amdxdna (aie4_pci.c).
 */

#include <linux/errno.h>
#include <linux/module.h>

#include "amdxdna_hal.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("AMD XDNA HAL - AIE4/PCI stub (prototype)");

static int aie4_dev_init(struct amdxdna_hal_dev *xdna)
{
	(void)xdna;
	return -ENODEV; /* Stub: use amdxdna driver for real AIE4 */
}

static void aie4_dev_fini(struct amdxdna_hal_dev *xdna)
{
	(void)xdna;
}

static int aie4_hwctx_init(struct amdxdna_hal_hwctx *hwctx,
			   struct amdxdna_hal_hwctx_config *config)
{
	(void)hwctx;
	(void)config;
	return -ENODEV;
}

static void aie4_hwctx_fini(struct amdxdna_hal_hwctx *hwctx)
{
	(void)hwctx;
}

static int aie4_sched_init(struct amdxdna_hal_hwctx *hwctx)
{
	(void)hwctx;
	return -ENODEV;
}

static void aie4_sched_fini(struct amdxdna_hal_hwctx *hwctx)
{
	(void)hwctx;
}

static int aie4_fw_load(struct amdxdna_hal_dev *xdna)
{
	(void)xdna;
	return -ENODEV;
}

static void aie4_fw_unload(struct amdxdna_hal_dev *xdna)
{
	(void)xdna;
}

static const struct amdxdna_hal_dev_ops aie4_hal_ops = {
	.name		= "aie4_pci",
	.dev_init	= aie4_dev_init,
	.dev_fini	= aie4_dev_fini,
	.hwctx_init	= aie4_hwctx_init,
	.hwctx_fini	= aie4_hwctx_fini,
	.hwctx_config	= NULL,
	.hwctx_query	= NULL,
	.hwctx_suspend	= NULL,
	.hwctx_resume	= NULL,
	.sched_init	= aie4_sched_init,
	.sched_fini	= aie4_sched_fini,
	.sched_submit	= NULL,
	.sched_wait	= NULL,
	.fw_load	= aie4_fw_load,
	.fw_unload	= aie4_fw_unload,
};

const struct amdxdna_hal amdxdna_hal_aie4 = {
	.name	= "aie4_pci",
	.ops	= &aie4_hal_ops,
};

