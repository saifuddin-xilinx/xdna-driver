/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2022-2026, Advanced Micro Devices, Inc.
 *
 * HAL Layer Interface - Prototype
 *
 * This defines the HAL interface that bridges:
 *   - Bus drivers (PCI/Aux) → HAL → Common Framework
 *   - DRM IOCTL → HAL → Platform-specific operations
 *
 * Uses amdxdna_dev and amdxdna_ctx directly (no separate HAL types).
 */

#ifndef _AMDXDNA_HAL_DRV_H_
#define _AMDXDNA_HAL_DRV_H_

#include <linux/types.h>

/* Forward declarations */
struct device;
struct amdxdna_dev;
struct amdxdna_ctx;
struct amdxdna_client;

/* HAL context configuration - wraps DRM IOCTL arguments */
struct amdxdna_hal_hwctx_config {
	struct amdxdna_client *client;
	u32 num_tiles;
	u32 mem_size;
	u32 max_opc;
	u32 umq_bo;
	u32 log_buf_bo;
	void *qos_p; /* User pointer to QoS info */
};

/* HAL command - wraps DRM IOCTL command arguments */
struct amdxdna_hal_cmd {
	struct amdxdna_client *client;
	u32 type;
	u32 cmd_bo_hdl;
	u32 *arg_bo_hdls;
	u32 arg_bo_cnt;
	u32 *sync_obj_hdls;
	u64 *sync_obj_pts;
	u32 sync_obj_cnt;
};

/**
 * struct amdxdna_hal_dev_ops - Platform operations (device, hwctx, sched, fw)
 *
 * One struct per platform (aie2_pci, aie4_pci, ve2_aux). Optional callbacks can be NULL.
 */
struct amdxdna_hal_dev_ops {
	const char *name;  /* "aie2_pci" or "ve2_aux" - for debug */

	/* Device-level (probe/remove) */
	int (*dev_init)(struct amdxdna_dev *xdna);
	void (*dev_fini)(struct amdxdna_dev *xdna);

	/* Hardware context */
	int (*hwctx_init)(struct amdxdna_ctx *hwctx,
			  struct amdxdna_hal_hwctx_config *config);
	void (*hwctx_fini)(struct amdxdna_ctx *hwctx);
	int (*hwctx_config)(struct amdxdna_ctx *hwctx, void *config);
	int (*hwctx_query)(struct amdxdna_ctx *hwctx, void *info);
	int (*hwctx_suspend)(struct amdxdna_ctx *hwctx);
	int (*hwctx_resume)(struct amdxdna_ctx *hwctx);

	/* Scheduler */
	int (*sched_init)(struct amdxdna_ctx *hwctx);
	void (*sched_fini)(struct amdxdna_ctx *hwctx);
	int (*sched_submit)(struct amdxdna_ctx *hwctx, struct amdxdna_hal_cmd *cmd);
	int (*sched_wait)(struct amdxdna_ctx *hwctx, u64 seq, u32 timeout_ms);

	/* Firmware */
	int (*fw_load)(struct amdxdna_dev *xdna);
	void (*fw_unload)(struct amdxdna_dev *xdna);
};

/**
 * struct amdxdna_hal - HAL descriptor (one per IP: AIE2/PCI, VE2/Aux)
 */
struct amdxdna_hal {
	const char *name;
	const struct amdxdna_hal_dev_ops *ops;
};

/* AIE2 PCI binding (Ryzen) */
extern const struct amdxdna_hal amdxdna_hal_aie2;

/* AIE4 PCI binding */
extern const struct amdxdna_hal amdxdna_hal_aie4;

/* VE2 Auxiliary binding (Telluride) */
extern const struct amdxdna_hal amdxdna_hal_ve2;

/**
 * HAL Device Management Functions
 * These functions bridge the bus drivers (PCI/Aux) with the HAL layer.
 */

/**
 * amdxdna_hal_dev_probe - Probe device and initialize via HAL
 * @dev: Kernel device (PCI or auxiliary)
 * @hal: HAL descriptor for this device (amdxdna_hal_aie2, amdxdna_hal_ve2, etc.)
 *
 * Execution flow: Device Init
 * This is called from PCI/Aux probe functions to:
 *   - Allocate amdxdna_dev
 *   - Set xdna->dev_ops = hal->ops
 *   - Initialize framework (DRM, etc.)
 *   - Call hal->ops->dev_init(xdna)
 *   - Register with framework
 *
 * Returns: Pointer to amdxdna_dev on success, ERR_PTR on failure
 */
struct amdxdna_dev *amdxdna_hal_dev_probe(struct device *dev,
					  const struct amdxdna_hal *hal);

/**
 * amdxdna_hal_dev_remove - Remove device via HAL
 * @xdna: Device handle
 *
 * Execution flow: Device Fini
 * This is called from PCI/Aux remove functions to:
 *   - Unregister from framework
 *   - Call ops->dev_fini(xdna)
 *   - Free amdxdna_dev
 */
void amdxdna_hal_dev_remove(struct amdxdna_dev *xdna);

/**
 * HAL Wrapper Functions - Match base API for seamless integration
 * These functions provide the same interface as the base implementation
 * but route through the HAL layer.
 *
 * Execution flow integration:
 * 4.3.3: Context Creation - amdxdna_hal_hwctx_init() replaces ops->ctx_init()
 * 4.3.4: Context Destruction - amdxdna_hal_hwctx_fini() replaces ops->ctx_fini()
 * 4.3.5: Command Submit - amdxdna_hal_cmd_submit_job() replaces ops->cmd_submit()
 * 4.3.6: Command Wait - amdxdna_hal_cmd_wait_ctx() replaces ops->cmd_wait()
 */

/* Forward declarations */
struct amdxdna_sched_job;

/**
 * amdxdna_hal_hwctx_init - Initialize hardware context (matches base API)
 * @ctx: Context to initialize
 *
 * Wrapper that routes ctx_init through HAL layer.
 * Execution flow: Context Creation
 */
int amdxdna_hal_hwctx_init(struct amdxdna_ctx *ctx);

/**
 * amdxdna_hal_hwctx_fini - Finalize hardware context (matches base API)
 * @ctx: Context to finalize
 *
 * Wrapper that routes ctx_fini through HAL layer.
 * Execution flow: Context Destruction
 */
void amdxdna_hal_hwctx_fini(struct amdxdna_ctx *ctx);

/**
 * amdxdna_hal_cmd_submit_job - Submit command job (matches base API)
 * @job: Scheduler job to submit
 * @syncobj_hdls: Sync object handles
 * @syncobj_points: Sync object points
 * @syncobj_cnt: Number of sync objects
 * @seq: Output sequence number
 *
 * Wrapper that routes cmd_submit through HAL layer.
 * Execution flow: Command Submit
 */
int amdxdna_hal_cmd_submit_job(struct amdxdna_sched_job *job,
			       u32 *syncobj_hdls, u64 *syncobj_points,
			       u32 syncobj_cnt, u64 *seq);

/**
 * amdxdna_hal_cmd_wait_ctx - Wait for command completion (matches base API)
 * @ctx: Context to wait on
 * @seq: Sequence number to wait for
 * @timeout: Timeout in milliseconds
 *
 * Wrapper that routes cmd_wait through HAL layer.
 * Execution flow: Command Wait
 */
int amdxdna_hal_cmd_wait_ctx(struct amdxdna_ctx *ctx, u64 seq, u32 timeout);

/* Legacy framework init/fini functions (kept for compatibility) */
int amdxdna_hal_drv_init(void);
void amdxdna_hal_drv_fini(void);

#endif /* _AMDXDNA_HAL_DRV_H_ */
