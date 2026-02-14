// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2022-2026, Advanced Micro Devices, Inc.
 *
 * AIE4/PCI HAL - prototype implementation for PCI device binding (AIE4).
 * Real implementation lives in amdxdna (aie4_pci.c, aie4_hwctx.c, etc.).
 *
 * This provides the HAL interface for AIE4 devices on PCI bus.
 * The HAL abstracts hardware-specific operations from the common framework.
 */

#include <linux/errno.h>
#include <linux/module.h>
#include <linux/pci.h>

#include "amdxdna_hal_drv.h"
#include "amdxdna_drm.h"
#include "amdxdna_drm.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("AMD XDNA HAL - AIE4/PCI prototype implementation");

/**
 * aie4_dev_init - Initialize AIE4 device hardware
 * @xdna: HAL device handle (opaque, cast from amdxdna_dev)
 *
 * Execution flow: Device Init
 * Called from: amdxdna_hal_dev_probe() → ops->dev_init(xdna)
 *
 * Prototype: Initialize AIE4-specific hardware resources.
 * Real implementation would:
 *   - Map PCI BARs (registers, mailbox, SRAM, PSP, SMU, doorbell)
 *   - Initialize mailbox communication
 *   - Initialize PSP/SMU for firmware loading
 *   - Set up interrupt handlers (MSI/MSI-X)
 *   - Initialize device-specific registers
 *   - Set up host queue for command submission
 *   - Store dev_handle for AIE4
 *
 * Returns: 0 on success, negative error code on failure
 */
static int aie4_dev_init(struct amdxdna_dev *xdna)
{
	struct amdxdna_dev *dev = xdna;

	/* Real implementation would:
	 *   - Access dev->dev_info->reg_bar, mbox_bar, doorbell_bar, etc.
	 *   - Call pci_ioremap_bar() for each BAR
	 *   - Initialize aie4_mailbox_init()
	 *   - Initialize aie4_psp_init() / aie4_smu_init()
	 *   - Set up MSI/MSI-X interrupts
	 *   - Initialize device registers via aie4_regs_init()
	 *   - Set up host queue: aie4_host_queue_init()
	 */
	(void)dev;
	return 0; /* Prototype: success for demonstration */
}

/**
 * aie4_dev_fini - Cleanup AIE4 device hardware
 * @xdna: HAL device handle
 *
 * Execution flow: Device Fini
 * Called from: amdxdna_hal_dev_remove() → ops->dev_fini(xdna)
 *
 * Prototype: Release AIE4-specific hardware resources.
 * Real implementation would:
 *   - Teardown mailbox, unload firmware
 *   - Free dev_handle
 */
static void aie4_dev_fini(struct amdxdna_dev *xdna)
{
	struct amdxdna_dev *dev = xdna;

	/* Real implementation would:
	 *   - aie4_host_queue_fini()
	 *   - aie4_mailbox_fini()
	 *   - aie4_psp_fini() / aie4_smu_fini()
	 *   - iounmap() for all mapped BARs
	 *   - free_irq() for interrupt handlers
	 */
	(void)dev;
}

/**
 * aie4_hwctx_init - Initialize AIE4 hardware context
 * @hwctx: HAL hardware context handle (opaque, cast from amdxdna_ctx)
 * @config: Hardware context configuration parameters
 *
 * Prototype: Set up hardware context for AIE4 device.
 * Real implementation would:
 *   - Allocate AIE4 context resources (tiles, columns, memory)
 *   - Configure context parameters (CU list, QoS, memory layout)
 *   - Initialize context-specific registers
 *   - Set up context doorbell
 *
 * Returns: 0 on success, negative error code on failure
 */
static int aie4_hwctx_init(struct amdxdna_ctx *hwctx,
			   struct amdxdna_hal_hwctx_config *config)
{
	struct amdxdna_ctx *ctx = hwctx;

	/* Real implementation would:
	 *   - Access ctx->client->xdna to get device
	 *   - Allocate AIE4 tiles/columns via aie4_ctx_alloc_tiles()
	 *   - Configure CU list: ctx->cus = aie4_ctx_config_cus()
	 *   - Initialize context memory: aie4_ctx_mem_init()
	 *   - Configure QoS: aie4_ctx_qos_config()
	 *   - Set up doorbell: aie4_ctx_doorbell_init()
	 */
	(void)ctx;
	(void)config;
	return 0; /* Prototype: success for demonstration */
}

/**
 * aie4_hwctx_fini - Cleanup AIE4 hardware context
 * @hwctx: HAL hardware context handle
 *
 * Prototype: Release AIE4 context resources.
 */
static void aie4_hwctx_fini(struct amdxdna_ctx *hwctx)
{
	struct amdxdna_ctx *ctx = hwctx;

	/* Real implementation would:
	 *   - aie4_ctx_stop_all_commands()
	 *   - aie4_ctx_free_tiles()
	 *   - aie4_ctx_mem_fini()
	 *   - aie4_ctx_doorbell_fini()
	 */
	(void)ctx;
}

/**
 * aie4_sched_init - Initialize AIE4 scheduler
 * @hwctx: HAL hardware context handle
 *
 * Prototype: Set up command scheduler for AIE4 context.
 * Real implementation would:
 *   - Initialize command queue
 *   - Set up scheduler workqueue
 *   - Initialize command submission path via host queue
 *
 * Returns: 0 on success, negative error code on failure
 */
static int aie4_sched_init(struct amdxdna_ctx *hwctx)
{
	struct amdxdna_ctx *ctx = hwctx;

	/* Real implementation would:
	 *   - Initialize command queue: aie4_cmd_queue_init()
	 *   - Set up scheduler: drm_sched_init() with AIE4-specific ops
	 *   - Initialize message handling: aie4_message_init()
	 *   - Set up host queue submission: aie4_host_queue_submit_init()
	 */
	(void)ctx;
	return 0; /* Prototype: success for demonstration */
}

/**
 * aie4_sched_fini - Cleanup AIE4 scheduler
 * @hwctx: HAL hardware context handle
 *
 * Prototype: Release scheduler resources.
 */
static void aie4_sched_fini(struct amdxdna_ctx *hwctx)
{
	struct amdxdna_ctx *ctx = hwctx;

	/* Real implementation would:
	 *   - drm_sched_fini()
	 *   - aie4_cmd_queue_fini()
	 *   - aie4_message_fini()
	 *   - aie4_host_queue_submit_fini()
	 */
	(void)ctx;
}

/**
 * aie4_fw_load - Load firmware for AIE4 device
 * @xdna: HAL device handle
 *
 * Prototype: Load and initialize AIE4 firmware.
 * Real implementation would:
 *   - Load firmware image via PSP/SMU
 *   - Verify firmware version
 *   - Initialize firmware communication
 *
 * Returns: 0 on success, negative error code on failure
 */
static int aie4_fw_load(struct amdxdna_dev *xdna)
{
	struct amdxdna_dev *dev = xdna;

	/* Real implementation would:
	 *   - aie4_psp_load_fw() or aie4_smu_load_fw()
	 *   - Verify firmware: aie4_fw_verify()
	 *   - Initialize firmware mailbox: aie4_fw_mbox_init()
	 */
	(void)dev;
	return 0; /* Prototype: success for demonstration */
}

/**
 * aie4_fw_unload - Unload AIE4 firmware
 * @xdna: HAL device handle
 *
 * Prototype: Cleanup firmware resources.
 */
static void aie4_fw_unload(struct amdxdna_dev *xdna)
{
	struct amdxdna_dev *dev = xdna;

	/* Real implementation would:
	 *   - aie4_fw_mbox_fini()
	 *   - aie4_psp_unload_fw() or aie4_smu_unload_fw()
	 */
	(void)dev;
}

/* HAL device operations structure for AIE4/PCI */
static const struct amdxdna_hal_dev_ops aie4_hal_ops = {
	.name		= "aie4_pci",
	.dev_init	= aie4_dev_init,
	.dev_fini	= aie4_dev_fini,
	.hwctx_init	= aie4_hwctx_init,
	.hwctx_fini	= aie4_hwctx_fini,
	.hwctx_config	= NULL, /* Optional: aie4_hwctx_config() */
	.hwctx_query	= NULL, /* Optional: aie4_hwctx_query() */
	.hwctx_suspend	= NULL, /* Optional: aie4_hwctx_suspend() */
	.hwctx_resume	= NULL, /* Optional: aie4_hwctx_resume() */
	.sched_init	= aie4_sched_init,
	.sched_fini	= aie4_sched_fini,
	.sched_submit	= NULL, /* Execution flow: Would be aie4_sched_submit() */
	.sched_wait	= NULL, /* Execution flow: Would be aie4_sched_wait() */
	.fw_load	= aie4_fw_load,
	.fw_unload	= aie4_fw_unload,
};

/* HAL descriptor exported for PCI driver binding */
const struct amdxdna_hal amdxdna_hal_aie4 = {
	.name	= "aie4_pci",
	.ops	= &aie4_hal_ops,
};

