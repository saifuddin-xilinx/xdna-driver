// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2022-2026, Advanced Micro Devices, Inc.
 *
 * VE2/Auxiliary HAL stub - prototype for Auxiliary bus binding (Telluride).
 * Real implementation lives in amdxdna (amdxdna_aux_drv.c).
 */

#include <linux/errno.h>
#include <linux/module.h>

#include "amdxdna_hal.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("AMD XDNA HAL - VE2/Aux stub (prototype)");

static int ve2_dev_init(struct amdxdna_hal_dev *xdna)
{
	(void)xdna;
	return -ENODEV; /* Stub: use amdxdna driver for real VE2 */
}

static void ve2_dev_fini(struct amdxdna_hal_dev *xdna)
{
	(void)xdna;
}

static int ve2_hwctx_init(struct amdxdna_hal_hwctx *hwctx,
			 struct amdxdna_hal_hwctx_config *config)
{
	(void)hwctx;
	(void)config;
	return -ENODEV;
}

static void ve2_hwctx_fini(struct amdxdna_hal_hwctx *hwctx)
{
	(void)hwctx;
}

static int ve2_sched_init(struct amdxdna_hal_hwctx *hwctx)
{
	(void)hwctx;
	return -ENODEV;
}

static void ve2_sched_fini(struct amdxdna_hal_hwctx *hwctx)
{
	(void)hwctx;
}

static int ve2_fw_load(struct amdxdna_hal_dev *xdna)
{
	(void)xdna;
	return -ENODEV;
}

static void ve2_fw_unload(struct amdxdna_hal_dev *xdna)
{
	(void)xdna;
}

static const struct amdxdna_hal_dev_ops ve2_hal_ops = {
	.name		= "ve2_aux",
	.dev_init	= ve2_dev_init,
	.dev_fini	= ve2_dev_fini,
	.hwctx_init	= ve2_hwctx_init,
	.hwctx_fini	= ve2_hwctx_fini,
	.hwctx_config	= NULL,
	.hwctx_query	= NULL,
	.hwctx_suspend	= NULL,
	.hwctx_resume	= NULL,
	.sched_init	= ve2_sched_init,
	.sched_fini	= ve2_sched_fini,
	.sched_submit	= NULL,
	.sched_wait	= NULL,
	.fw_load	= ve2_fw_load,
	.fw_unload	= ve2_fw_unload,
};

const struct amdxdna_hal amdxdna_hal_ve2 = {
	.name	= "ve2_aux",
	.ops	= &ve2_hal_ops,
};

