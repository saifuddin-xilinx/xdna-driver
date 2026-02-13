// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2022-2026, Advanced Micro Devices, Inc.
 *
 * PCI bus driver binding - prototype stub.
 * Real implementation lives in amdxdna (amdxdna_pci_drv.c).
 *
 * This layer binds PCI devices (AIE2, AIE4) to the HAL framework.
 * Reference: amdxdna/amdxdna_pci_drv.c
 */

#include <linux/module.h>
#include <linux/version.h>
#include <linux/pci.h>
#include <linux/errno.h>
#include <drm/drm_managed.h>

#include "amdxdna_pci_drv.h"
#include "amdxdna_drm.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("AMD XDNA HAL - PCI bus driver binding (prototype)");

/* common util inline functions */
static inline int is_pf_dev(const struct pci_dev *pdev)
{
	/* Stub: would check if device is PF (Physical Function) */
	return 0;
}

/*
 *  There are platforms which share the same PCI device ID
 *  but have different PCI revision IDs. So, let the PCI class
 *  determine the probe and later use the (device_id, rev_id)
 *  pair as a key to select the devices.
 */
static const struct pci_device_id pci_ids[] = {
	{ PCI_DEVICE(PCI_VENDOR_ID_AMD, PCI_ANY_ID),
		.class = PCI_CLASS_SP_OTHER << 8,  /* Signal Processing */
		.class_mask = 0xFFFF00,
	},
	{0}
};

MODULE_DEVICE_TABLE(pci, pci_ids);

/* Forward declarations for prototype */
struct amdxdna_dev;
struct amdxdna_dev_info;

static const struct amdxdna_device_id amdxdna_ids[] = {
	/* Stub: would contain device ID table mapping to dev_info */
	{0}
};

static const struct amdxdna_dev_info *
amdxdna_get_dev_info(struct pci_dev *pdev)
{
	/* Stub: would lookup device info based on PCI device/revision ID */
	(void)pdev;
	return NULL;
}

static void amdxdna_vbnv_init(struct amdxdna_dev *xdna)
{
	/* Stub: would initialize VBNV (board name) from firmware or default */
	(void)xdna;
}

static int amdxdna_probe(struct pci_dev *pdev, const struct pci_device_id *id)
{
	struct device *dev = &pdev->dev;
	struct amdxdna_dev *xdna;

	/* Allocate DRM device (common framework) */
	xdna = devm_drm_dev_alloc(dev, &amdxdna_drm_drv, typeof(*xdna), ddev);
	if (IS_ERR(xdna))
		return PTR_ERR(xdna);

	/* Stub: would probe PCI device and register with HAL framework */
	/* Real implementation would continue with:
	 *   - Get device info: xdna->dev_info = amdxdna_get_dev_info(pdev)
	 *   - Initialize IOMMU: amdxdna_iommu_init(xdna)
	 *   - Initialize hardware via dev_info->ops->init()
	 *   - Register DRM device: drm_dev_register(&xdna->ddev, 0)
	 *   - Initialize DPT, sysfs, debugfs
	 */
	(void)pdev;
	(void)id;
	return -ENODEV;
}

static void amdxdna_remove(struct pci_dev *pdev)
{
	struct amdxdna_dev *xdna = pci_get_drvdata(pdev);

	/* Unregister DRM device (common framework) */
	/* Real: drm_dev_unplug(&xdna->ddev); */
	/* Stub: would unregister DRM device */

	/* Stub: would remove PCI device from HAL framework */
	/* Real implementation:
	 *   - Unregister DRM device: drm_dev_unplug(&xdna->ddev)
	 *   - Clean up clients and contexts
	 *   - Call dev_info->ops->fini()
	 *   - Clean up IOMMU, workqueues, etc.
	 */
	(void)xdna;
	(void)pdev;
}

static pci_ers_result_t amdxdna_error_detected(struct pci_dev *pdev,
					       pci_channel_state_t state)
{
	/* Stub: would handle PCI error detection */
	(void)pdev;
	(void)state;
	return PCI_ERS_RESULT_NEED_RESET;
}

static void amdxdna_reset_prepare(struct pci_dev *pdev)
{
	/* Stub: would prepare device for PCI reset */
	(void)pdev;
}

static void amdxdna_reset_done(struct pci_dev *pdev)
{
	/* Stub: would handle PCI reset completion */
	(void)pdev;
}

static const struct pci_error_handlers amdxdna_err_handler = {
	.error_detected = amdxdna_error_detected,
	.reset_prepare = amdxdna_reset_prepare,
	.reset_done = amdxdna_reset_done,
};

static int amdxdna_sriov_configure(struct pci_dev *pdev, int num_vfs)
{
	/* Stub: would configure SR-IOV virtual functions */
	(void)pdev;
	(void)num_vfs;
	return -EOPNOTSUPP;
}

static struct pci_driver amdxdna_pci_driver = {
	.name = KBUILD_MODNAME,
	.id_table = pci_ids,
	.probe = amdxdna_probe,
	.remove = amdxdna_remove,
	.err_handler = &amdxdna_err_handler,
	.sriov_configure = amdxdna_sriov_configure,
};

/* Stub functions - would be called by framework to register/unregister PCI driver */
int amdxdna_pci_drv_init(void)
{
	/* Stub: would register PCI driver */
	/* Real: return pci_register_driver(&amdxdna_pci_driver); */
	return -ENODEV;
}

void amdxdna_pci_drv_fini(void)
{
	/* Stub: would unregister PCI driver */
	/* Real: pci_unregister_driver(&amdxdna_pci_driver); */
}

