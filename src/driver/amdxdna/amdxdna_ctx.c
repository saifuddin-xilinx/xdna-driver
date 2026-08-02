// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2022-2026, Advanced Micro Devices, Inc.
 */

#include <linux/version.h>
#include <drm/drm_cache.h>
#include <drm/drm_file.h>
#include <drm/drm_gem.h>
#include "amdxdna_ctx.h"
#include "amdxdna_gem.h"
#include "amdxdna_trace.h"

int amdxdna_drm_create_hwctx_ioctl(struct drm_device *dev, void *data, struct drm_file *filp)
{
	struct amdxdna_client *client = filp->driver_priv;
	struct amdxdna_drm_create_hwctx *args = data;
	struct amdxdna_dev *xdna = to_xdna_dev(dev);
	struct amdxdna_ctx *ctx;
	int ret, idx;

	if (args->ext_flags)
		return -EINVAL;

	if (!drm_dev_enter(dev, &idx))
		return -ENODEV;

	ctx = kzalloc(sizeof(*ctx), GFP_KERNEL);
	if (!ctx) {
		ret = -ENOMEM;
		goto exit;
	}

	ctx->client = client;
	ctx->xdna = xdna;
	ctx->qos.gops = args->qos.gops;
	ctx->qos.fps = args->qos.fps;
	ctx->qos.dma_bandwidth = args->qos.dma_bandwidth;
	ctx->qos.latency = args->qos.latency;
	ctx->qos.frame_exec_time = args->qos.frame_exec_time;
	ctx->qos.priority = args->qos.priority;
	ctx->max_opc = args->max_opc;
	ctx->num_tiles = args->num_tiles;
	ctx->mem_size = args->mem_size;
	ctx->umq_bo = args->umq_bo;
	ctx->log_buf_bo = args->log_buf_bo;
	ctx->doorbell_offset = AMDXDNA_INVALID_DOORBELL_OFFSET;
	ctx->syncobj = NULL;
	ctx->syncobj_hdl = AMDXDNA_INVALID_FENCE_HANDLE;
	mutex_init(&ctx->io_lock);
	fs_reclaim_acquire(GFP_KERNEL);
	might_lock(&ctx->io_lock);
	fs_reclaim_release(GFP_KERNEL);
	atomic64_set(&ctx->job_free_cnt, 0);

	/*
	 * Reserve a slot with XA_ZERO_ENTRY so the ctx ID is allocated but
	 * the entry is not yet visible to SRCU readers as a live context.
	 * Publish the real pointer only after ctx_init() has fully initialised
	 * ctx->priv, preventing a race where a concurrent ioctl dereferences
	 * ctx->priv before it is set.
	 */
	ret = xa_alloc_cyclic(&client->ctx_xa, &ctx->id, XA_ZERO_ENTRY,
			      XA_LIMIT(AMDXDNA_INVALID_CTX_HANDLE + 1, MAX_CTX_ID),
			      &client->next_ctxid, GFP_KERNEL);
	if (ret < 0) {
		XDNA_ERR(xdna, "Allocate ctx ID failed, ret %d", ret);
		goto destroy_io_lock;
	}

	ctx->name = kasprintf(GFP_KERNEL, "ctx.%d.%d", client->pid, ctx->id);
	if (!ctx->name) {
		ret = -ENOMEM;
		goto rm_id;
	}

	ret = xdna->dev_info->ops->ctx_init(ctx);
	if (ret) {
		XDNA_ERR(xdna, "Init ctx failed, ret %d", ret);
		goto free_name;
	}

	/* Publish the fully-initialised ctx; replaces XA_ZERO_ENTRY. */
	xa_store(&client->ctx_xa, ctx->id, ctx, GFP_KERNEL);

	args->handle = ctx->id;
	args->syncobj_handle = ctx->syncobj_hdl;
	args->umq_doorbell = ctx->doorbell_offset;

	XDNA_DBG(xdna, "PID %d create context %d, ret %d", client->pid, args->handle, ret);
	drm_dev_exit(idx);

	return 0;

free_name:
	kfree(ctx->name);
rm_id:
	xa_erase(&client->ctx_xa, ctx->id);
destroy_io_lock:
	mutex_destroy(&ctx->io_lock);
free_ctx:
	kfree(ctx);
exit:
	drm_dev_exit(idx);
	return ret;
}

int amdxdna_drm_destroy_hwctx_ioctl(struct drm_device *dev, void *data, struct drm_file *filp)
{
	struct amdxdna_client *client = filp->driver_priv;
	struct amdxdna_drm_destroy_hwctx *args = data;
	struct amdxdna_dev *xdna = to_xdna_dev(dev);
	struct amdxdna_ctx *ctx;
	int ret = 0, idx;

	trace_amdxdna_debug_point(current->comm, client->pid, "destroy hwctx");

	if (!drm_dev_enter(dev, &idx))
		return -ENODEV;

	ctx = xa_erase(&client->ctx_xa, args->handle);
	if (!ctx) {
		ret = -EINVAL;
		XDNA_DBG(xdna, "PID %d context %d not exist",
			 client->pid, args->handle);
		goto out;
	}

	xdna->dev_info->ops->ctx_fini(ctx);
	amdxdna_ctx_destroy(ctx);

out:
	drm_dev_exit(idx);
	return ret;
}
