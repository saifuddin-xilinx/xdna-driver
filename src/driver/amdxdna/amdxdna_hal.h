/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2022-2026, Advanced Micro Devices, Inc.
 *
 * AMD XDNA Hardware Abstraction Layer (HAL) - Prototype
 *
 * Standalone header: no dependency on amdxdna driver.
 * Design follows VE2_HAL_REFACTORING_PROPOSAL_ONLY.md:
 *   - Common Framework (amdxdna_drv.c) → HAL Layer → PCI (AIE2, AIE4) / Aux (VE2) binding
 *
 * This defines the HAL interface so amdxdna_hal can build independently
 * as a prototype for the unified driver design.
 */

#ifndef _AMDXDNA_HAL_H_
#define _AMDXDNA_HAL_H_

#include <linux/types.h>

/* Opaque types - HAL does not depend on amdxdna structure definitions */
struct amdxdna_hal_dev;
struct amdxdna_hal_hwctx;
struct amdxdna_hal_hwctx_config;
struct amdxdna_hal_cmd;

/**
 * struct amdxdna_hal_dev_ops - Platform operations (device, hwctx, sched, fw)
 *
 * One struct per platform (aie2_pci, aie4_pci, ve2_aux). Optional callbacks can be NULL.
 * Mirrors proposal section 3.3.
 */
struct amdxdna_hal_dev_ops {
	const char *name;  /* "aie2_pci" or "ve2_aux" - for debug */

	/* Device-level (probe/remove) */
	int (*dev_init)(struct amdxdna_hal_dev *xdna);
	void (*dev_fini)(struct amdxdna_hal_dev *xdna);

	/* Hardware context */
	int (*hwctx_init)(struct amdxdna_hal_hwctx *hwctx,
			  struct amdxdna_hal_hwctx_config *config);
	void (*hwctx_fini)(struct amdxdna_hal_hwctx *hwctx);
	int (*hwctx_config)(struct amdxdna_hal_hwctx *hwctx, void *config);
	int (*hwctx_query)(struct amdxdna_hal_hwctx *hwctx, void *info);
	int (*hwctx_suspend)(struct amdxdna_hal_hwctx *hwctx);
	int (*hwctx_resume)(struct amdxdna_hal_hwctx *hwctx);

	/* Scheduler */
	int (*sched_init)(struct amdxdna_hal_hwctx *hwctx);
	void (*sched_fini)(struct amdxdna_hal_hwctx *hwctx);
	int (*sched_submit)(struct amdxdna_hal_hwctx *hwctx, struct amdxdna_hal_cmd *cmd);
	int (*sched_wait)(struct amdxdna_hal_hwctx *hwctx, u64 seq, u32 timeout_ms);

	/* Firmware */
	int (*fw_load)(struct amdxdna_hal_dev *xdna);
	void (*fw_unload)(struct amdxdna_hal_dev *xdna);
};

/**
 * struct amdxdna_hal - HAL descriptor (one per IP: AIE2/PCI, VE2/Aux)
 */
struct amdxdna_hal {
	const char *name;
	const struct amdxdna_hal_dev_ops *ops;
};

/* AIE2 PCI binding (Ryzen) - stub in prototype */
extern const struct amdxdna_hal amdxdna_hal_aie2;

/* AIE4 PCI binding - stub in prototype */
extern const struct amdxdna_hal amdxdna_hal_aie4;

/* VE2 Auxiliary binding (Telluride) - stub in prototype */
extern const struct amdxdna_hal amdxdna_hal_ve2;

#endif /* _AMDXDNA_HAL_H_ */
