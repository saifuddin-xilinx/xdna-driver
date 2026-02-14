// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2022-2026, Advanced Micro Devices, Inc.
 *
 * AIE2/PCI HAL - prototype implementation for PCI device binding (Ryzen).
 * Real implementation lives in amdxdna (aie2_pci.c, aie2_hwctx.c, etc.).
 *
 * This provides the HAL interface for AIE2 devices on PCI bus.
 * The HAL abstracts hardware-specific operations from the common framework.
 */

#include <linux/errno.h>
#include <linux/module.h>
#include <linux/pci.h>

#include "amdxdna_hal_drv.h"
#include "amdxdna_drm.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("AMD XDNA HAL - AIE2/PCI prototype implementation");

/**
 * aie2_dev_init - Initialize AIE2 device hardware
 * @xdna: HAL device handle (opaque, cast from amdxdna_dev)
 *
 * Execution flow: Device Init
 * Called from: amdxdna_hal_dev_probe() → ops->dev_init(xdna)
 *
 * Prototype: Initialize AIE2-specific hardware resources.
 * Real implementation would:
 *   - Map PCI BARs (registers, mailbox, SRAM, PSP, SMU)
 *   - Initialize mailbox communication
 *   - Initialize PSP/SMU for firmware loading
 *   - Set up interrupt handlers
 *   - Initialize device-specific registers
 *   - Store dev_handle for AIE2
 *
 * Returns: 0 on success, negative error code on failure
 */
static int aie2_dev_init(struct amdxdna_dev *xdna)
{
	struct amdxdna_dev *dev = xdna;

	/* Real implementation would:
	 *   - Access dev->dev_info->reg_bar, mbox_bar, etc.
	 *   - Call pci_ioremap_bar() for each BAR
	 *   - Initialize aie2_mailbox_init()
	 *   - Initialize aie2_psp_init() / aie2_smu_init()
	 *   - Set up MSI/MSI-X interrupts
	 *   - Initialize device registers via aie2_regs_init()
	 */
	(void)dev;
	return 0; /* Prototype: success for demonstration */
}

/**
 * aie2_dev_fini - Cleanup AIE2 device hardware
 * @xdna: HAL device handle
 *
 * Execution flow: Device Fini
 * Called from: amdxdna_hal_dev_remove() → ops->dev_fini(xdna)
 *
 * Prototype: Release AIE2-specific hardware resources.
 * Real implementation would:
 *   - Teardown mailbox, unload firmware
 *   - Free dev_handle
 *   - Unmap PCI BARs
 *   - Clean up mailbox
 *   - Clean up PSP/SMU
 *   - Free interrupt handlers
 */
static void aie2_dev_fini(struct amdxdna_dev *xdna)
{
	struct amdxdna_dev *dev = xdna;

	/* Real implementation would:
	 *   - aie2_mailbox_fini()
	 *   - aie2_psp_fini() / aie2_smu_fini()
	 *   - iounmap() for all mapped BARs
	 *   - free_irq() for interrupt handlers
	 */
	(void)dev;
}

/**
 * aie2_hwctx_init - Initialize AIE2 hardware context
 * @hwctx: HAL hardware context handle (opaque, cast from amdxdna_ctx)
 * @config: Hardware context configuration parameters
 *
 * Execution flow: Context Creation
 * Called from: amdxdna_hal_hwctx_init() → ops->hwctx_init(hwctx, config)
 *
 * Prototype: Set up hardware context for AIE2 device.
 * Real implementation would:
 *   - aie2_send_msg_wait(CREATE_CONTEXT)
 *   - Store partition_id
 *   - Allocate AIE2 context resources (tiles, columns, memory)
 *   - Configure context parameters (CU list, QoS, memory layout)
 *   - Initialize context runqueue
 *   - Set up context-specific registers
 *
 * Returns: 0 on success, negative error code on failure
 */
static int aie2_hwctx_init(struct amdxdna_ctx *hwctx,
			   struct amdxdna_hal_hwctx_config *config)
{
	struct amdxdna_ctx *ctx = hwctx;

	/* Real implementation would:
	 *   - Access ctx->client->xdna to get device
	 *   - Allocate AIE2 tiles/columns via aie2_ctx_alloc_tiles()
	 *   - Configure CU list: ctx->cus = aie2_ctx_config_cus()
	 *   - Initialize runqueue: aie2_ctx_runqueue_init()
	 *   - Set up context memory: aie2_ctx_mem_init()
	 *   - Configure QoS: aie2_ctx_qos_config()
	 */
	(void)ctx;
	(void)config;
	return 0; /* Prototype: success for demonstration */
}

/**
 * aie2_hwctx_fini - Cleanup AIE2 hardware context
 * @hwctx: HAL hardware context handle
 *
 * Execution flow: Context Destruction
 * Called from: amdxdna_hal_hwctx_fini() → ops->hwctx_fini(hwctx)
 *
 * Prototype: Release AIE2 context resources.
 * Real implementation would:
 *   - aie2_send_msg_wait(DESTROY_CONTEXT)
 *   - Stop all running commands
 *   - Free tiles/columns
 *   - Clean up runqueue
 *   - Free context memory
 */
static void aie2_hwctx_fini(struct amdxdna_ctx *hwctx)
{
	struct amdxdna_ctx *ctx = hwctx;

	/* Real implementation would:
	 *   - aie2_ctx_stop_all_commands()
	 *   - aie2_ctx_free_tiles()
	 *   - aie2_ctx_runqueue_fini()
	 *   - aie2_ctx_mem_fini()
	 */
	(void)ctx;
}

/**
 * aie2_sched_init - Initialize AIE2 scheduler
 * @hwctx: HAL hardware context handle
 *
 * Prototype: Set up command scheduler for AIE2 context.
 * Real implementation would:
 *   - Initialize command queue
 *   - Set up scheduler workqueue
 *   - Initialize command submission path
 *
 * Returns: 0 on success, negative error code on failure
 */
static int aie2_sched_init(struct amdxdna_ctx *hwctx)
{
	struct amdxdna_ctx *ctx = hwctx;

	/* Real implementation would:
	 *   - Initialize command queue: aie2_cmd_queue_init()
	 *   - Set up scheduler: drm_sched_init() with AIE2-specific ops
	 *   - Initialize message handling: aie2_message_init()
	 */
	(void)ctx;
	return 0; /* Prototype: success for demonstration */
}

/**
 * aie2_sched_fini - Cleanup AIE2 scheduler
 * @hwctx: HAL hardware context handle
 *
 * Prototype: Release scheduler resources.
 * Real implementation would:
 *   - Stop scheduler
 *   - Clean up command queue
 *   - Free workqueue
 */
static void aie2_sched_fini(struct amdxdna_ctx *hwctx)
{
	struct amdxdna_ctx *ctx = hwctx;

	/* Real implementation would:
	 *   - drm_sched_fini()
	 *   - aie2_cmd_queue_fini()
	 *   - aie2_message_fini()
	 */
	(void)ctx;
}

/**
 * aie2_fw_load - Load firmware for AIE2 device
 * @xdna: HAL device handle
 *
 * Prototype: Load and initialize AIE2 firmware.
 * Real implementation would:
 *   - Load firmware image via PSP/SMU
 *   - Verify firmware version
 *   - Initialize firmware communication
 *
 * Returns: 0 on success, negative error code on failure
 */
static int aie2_fw_load(struct amdxdna_dev *xdna)
{
	struct amdxdna_dev *dev = xdna;

	/* Real implementation would:
	 *   - aie2_psp_load_fw() or aie2_smu_load_fw()
	 *   - Verify firmware: aie2_fw_verify()
	 *   - Initialize firmware mailbox: aie2_fw_mbox_init()
	 */
	(void)dev;
	return 0; /* Prototype: success for demonstration */
}

/**
 * aie2_fw_unload - Unload AIE2 firmware
 * @xdna: HAL device handle
 *
 * Prototype: Cleanup firmware resources.
 * Real implementation would:
 *   - Stop firmware communication
 *   - Release firmware resources
 */
static void aie2_fw_unload(struct amdxdna_dev *xdna)
{
	struct amdxdna_dev *dev = xdna;

	/* Real implementation would:
	 *   - aie2_fw_mbox_fini()
	 *   - aie2_psp_unload_fw() or aie2_smu_unload_fw()
	 */
	(void)dev;
}

/* HAL device operations structure for AIE2/PCI */
static const struct amdxdna_hal_dev_ops aie2_hal_ops = {
	.name		= "aie2_pci",
	.dev_init	= aie2_dev_init,
	.dev_fini	= aie2_dev_fini,
	.hwctx_init	= aie2_hwctx_init,
	.hwctx_fini	= aie2_hwctx_fini,
	.hwctx_config	= NULL, /* Optional: aie2_hwctx_config() */
	.hwctx_query	= NULL, /* Optional: aie2_hwctx_query() */
	.hwctx_suspend	= NULL, /* Optional: aie2_hwctx_suspend() */
	.hwctx_resume	= NULL, /* Optional: aie2_hwctx_resume() */
	.sched_init	= aie2_sched_init,
	.sched_fini	= aie2_sched_fini,
	.sched_submit	= NULL, /* Execution flow: Would be aie2_sched_submit() */
	.sched_wait	= NULL, /* Execution flow: Would be aie2_sched_wait() */
	.fw_load	= aie2_fw_load,
	.fw_unload	= aie2_fw_unload,
};

/* HAL descriptor exported for PCI driver binding */
const struct amdxdna_hal amdxdna_hal_aie2 = {
	.name	= "aie2_pci",
	.ops	= &aie2_hal_ops,
};

