// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2022-2026, Advanced Micro Devices, Inc.
 *
 * AIE2/PCI HAL stub - prototype for PCI device binding (Ryzen).
 * Real implementation lives in amdxdna (aie2_pci.c).
 */

#include <linux/errno.h>
#include <linux/module.h>

#include "amdxdna_hal.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("AMD XDNA HAL - AIE2/PCI stub (prototype)");

static int aie2_dev_init(struct amdxdna_hal_dev *xdna)
{
	(void)xdna;
	return -ENODEV; /* Stub: use amdxdna driver for real AIE2 */
}

static void aie2_dev_fini(struct amdxdna_hal_dev *xdna)
{
	(void)xdna;
}

static int aie2_hwctx_init(struct amdxdna_hal_hwctx *hwctx,
			   struct amdxdna_hal_hwctx_config *config)
{
	(void)hwctx;
	(void)config;
	return -ENODEV;
}

static void aie2_hwctx_fini(struct amdxdna_hal_hwctx *hwctx)
{
	(void)hwctx;
}

static int aie2_sched_init(struct amdxdna_hal_hwctx *hwctx)
{
	(void)hwctx;
	return -ENODEV;
}

static void aie2_sched_fini(struct amdxdna_hal_hwctx *hwctx)
{
	(void)hwctx;
}

static int aie2_fw_load(struct amdxdna_hal_dev *xdna)
{
	(void)xdna;
	return -ENODEV;
}

static void aie2_fw_unload(struct amdxdna_hal_dev *xdna)
{
	(void)xdna;
}

static const struct amdxdna_hal_dev_ops aie2_hal_ops = {
	.name		= "aie2_pci",
	.dev_init	= aie2_dev_init,
	.dev_fini	= aie2_dev_fini,
	.hwctx_init	= aie2_hwctx_init,
	.hwctx_fini	= aie2_hwctx_fini,
	.hwctx_config	= NULL,
	.hwctx_query	= NULL,
	.hwctx_suspend	= NULL,
	.hwctx_resume	= NULL,
	.sched_init	= aie2_sched_init,
	.sched_fini	= aie2_sched_fini,
	.sched_submit	= NULL,
	.sched_wait	= NULL,
	.fw_load	= aie2_fw_load,
	.fw_unload	= aie2_fw_unload,
};

const struct amdxdna_hal amdxdna_hal_aie2 = {
	.name	= "aie2_pci",
	.ops	= &aie2_hal_ops,
};

