// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2022-2026, Advanced Micro Devices, Inc.
 *
 * HAL Device and Context Management - Prototype Implementation
 *
 * This file implements the HAL layer functions that bridge:
 *   - Bus drivers (PCI/Aux) → HAL → Common Framework
 *   - DRM IOCTL → HAL → Platform-specific operations
 *
 * Following the execution flow from the design document.
 */

#include <linux/module.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/slab.h>

#include "amdxdna_hal_drv.h"
#include "amdxdna_drm.h"
#include "amdxdna_ctx.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("AMD XDNA HAL - Device and context management (prototype)");

/**
 * amdxdna_hal_dev_probe - Probe device and initialize via HAL
 *
 * Execution flow: Device Init
 *   - Allocate amdxdna_dev
 *   - Set xdna->dev_ops = hal->ops
 *   - Initialize framework (DRM, etc.)
 *   - Call ops->dev_init(xdna)
 *   - Register with framework
 */
struct amdxdna_dev *amdxdna_hal_dev_probe(struct device *dev,
					  const struct amdxdna_hal *hal)
{
	struct amdxdna_dev *xdna;
	int ret;

	/* Allocate amdxdna_dev (common framework structure) */
	/* Real: xdna = devm_drm_dev_alloc(dev, &amdxdna_drm_drv, ...) */
	xdna = kzalloc(sizeof(*xdna), GFP_KERNEL);
	if (!xdna)
		return ERR_PTR(-ENOMEM);

	/* Set xdna->dev_ops = hal->ops */
	/* Real: xdna->dev_info->ops = hal->ops (via dev_info structure) */
	/* Prototype: Store hal pointer for demonstration */
	(void)hal;

	/* Initialize framework (DRM, etc.) */
	/* Real: amdxdna_drm_init(xdna), amdxdna_iommu_init(xdna), etc. */

	/* Call ops->dev_init(xdna) */
	if (hal && hal->ops && hal->ops->dev_init) {
		ret = hal->ops->dev_init(xdna);
		if (ret) {
			/* Real: devm_drm_dev_put() or similar cleanup */
			kfree(xdna);
			return ERR_PTR(ret);
		}
	}

	/* Register with framework (accel, DRM, etc.) */
	/* Real: drm_dev_register(&xdna->ddev, 0) */
	/* Real: Initialize DPT, sysfs, debugfs */

	return xdna;
}

/**
 * amdxdna_hal_dev_remove - Remove device via HAL
 *
 * Execution flow: Device Fini
 *   - Unregister from framework
 *   - Call ops->dev_fini(xdna)
 *   - Free amdxdna_dev
 */
void amdxdna_hal_dev_remove(struct amdxdna_dev *xdna)
{
	struct amdxdna_dev *dev = xdna;
	const struct amdxdna_hal_dev_ops *ops;

	if (!dev)
		return;

	/* Get ops from dev_info (prototype: would be stored during probe) */
	/* Real: ops = dev->dev_info->ops */
	ops = NULL; /* Prototype: would be retrieved from dev structure */

	/* Unregister from framework */
	/* Real: drm_dev_unplug(&dev->ddev) */
	/* Real: Clean up clients and contexts */

	/* Call ops->dev_fini(xdna) */
	if (ops && ops->dev_fini)
		ops->dev_fini(xdna);

	/* Free amdxdna_dev */
	/* Real: devm_drm_dev_put() or similar */
	kfree(dev);
}

/**
 * HAL Wrapper Functions - Match base API for seamless integration
 * These functions replace direct ops calls in the base implementation.
 */

/**
 * amdxdna_hal_hwctx_init - Initialize hardware context (matches base API)
 */
int amdxdna_hal_hwctx_init(struct amdxdna_ctx *ctx)
{
	struct amdxdna_dev *xdna = ctx->client->xdna;
	const struct amdxdna_hal_dev_ops *ops;
	struct amdxdna_hal_hwctx_config config;

	if (!ctx || !xdna)
		return -EINVAL;

	/* Get ops from dev_info */
	/* Real: ops = xdna->dev_info->ops */
	ops = NULL; /* Prototype: would be retrieved from dev structure */

	/* Prepare config from context */
	config.client = ctx->client;
	config.num_tiles = ctx->num_tiles;
	config.mem_size = ctx->mem_size;
	config.max_opc = ctx->max_opc;
	config.umq_bo = ctx->umq_bo;
	config.log_buf_bo = ctx->log_buf_bo;
	config.qos_p = &ctx->qos; /* Already copied */

	/* Call platform-specific hwctx_init */
	if (ops && ops->hwctx_init) {
		int ret = ops->hwctx_init(ctx, &config);
		if (ret)
			return ret;
	}

	/* Initialize scheduler if needed */
	if (ops && ops->sched_init) {
		int ret = ops->sched_init(ctx);
		if (ret) {
			if (ops->hwctx_fini)
				ops->hwctx_fini(ctx);
			return ret;
		}
	}

	/* Create syncobj */
	return amdxdna_ctx_syncobj_create(ctx);
}

/**
 * amdxdna_hal_hwctx_fini - Finalize hardware context (matches base API)
 */
void amdxdna_hal_hwctx_fini(struct amdxdna_ctx *ctx)
{
	struct amdxdna_dev *xdna = ctx->client->xdna;
	const struct amdxdna_hal_dev_ops *ops;

	if (!ctx || !xdna)
		return;

	/* Get ops from dev_info */
	/* Real: ops = xdna->dev_info->ops */
	ops = NULL; /* Prototype: would be retrieved from dev structure */

	/* Cleanup scheduler */
	if (ops && ops->sched_fini)
		ops->sched_fini(ctx);

	/* Call platform-specific hwctx_fini */
	if (ops && ops->hwctx_fini)
		ops->hwctx_fini(ctx);

	/* Destroy syncobj */
	amdxdna_ctx_syncobj_destroy(ctx);
}

/**
 * amdxdna_hal_cmd_submit_job - Submit command job (matches base API)
 */
int amdxdna_hal_cmd_submit_job(struct amdxdna_sched_job *job,
			       u32 *syncobj_hdls, u64 *syncobj_points,
			       u32 syncobj_cnt, u64 *seq)
{
	struct amdxdna_ctx *ctx = job->ctx;
	struct amdxdna_dev *xdna = ctx->client->xdna;
	const struct amdxdna_hal_dev_ops *ops;

	if (!job || !ctx || !xdna)
		return -EINVAL;

	/* Get ops from dev_info */
	/* Real: ops = xdna->dev_info->ops */
	ops = NULL; /* Prototype: would be retrieved from dev structure */

	/* Call platform-specific sched_submit */
	if (ops && ops->sched_submit) {
		struct amdxdna_hal_cmd cmd = {
			.client = ctx->client,
			.type = job->opcode,
		};
		/* Real: would populate cmd with job details */
		return ops->sched_submit(ctx, &cmd);
	}

	/* Fallback to direct ops call if HAL not available */
	if (xdna->dev_info && xdna->dev_info->ops && xdna->dev_info->ops->cmd_submit)
		return xdna->dev_info->ops->cmd_submit(job, syncobj_hdls,
						       syncobj_points, syncobj_cnt, seq);

	return -EOPNOTSUPP;
}

/**
 * amdxdna_hal_cmd_wait_ctx - Wait for command completion (matches base API)
 */
int amdxdna_hal_cmd_wait_ctx(struct amdxdna_ctx *ctx, u64 seq, u32 timeout)
{
	struct amdxdna_dev *xdna = ctx->client->xdna;
	const struct amdxdna_hal_dev_ops *ops;

	if (!ctx || !xdna)
		return -EINVAL;

	/* Get ops from dev_info */
	/* Real: ops = xdna->dev_info->ops */
	ops = NULL; /* Prototype: would be retrieved from dev structure */

	/* Call platform-specific sched_wait */
	if (ops && ops->sched_wait)
		return ops->sched_wait(ctx, seq, timeout);

	/* Fallback to direct ops call if HAL not available */
	if (xdna->dev_info && xdna->dev_info->ops && xdna->dev_info->ops->cmd_wait)
		return xdna->dev_info->ops->cmd_wait(ctx, seq, timeout);

	return -EOPNOTSUPP;
}

/* Legacy framework init/fini functions (kept for compatibility) */
int amdxdna_hal_drv_init(void)
{
	/* Placeholder: would register DRM driver, GEM, IOCTL */
	return 0;
}

void amdxdna_hal_drv_fini(void)
{
	/* Placeholder: would unregister DRM driver */
}
