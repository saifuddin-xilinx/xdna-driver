// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2022-2026, Advanced Micro Devices, Inc.
 *
 * Auxiliary bus driver binding - prototype stub.
 * Real implementation lives in amdxdna (amdxdna_aux_drv.c).
 *
 * This layer binds auxiliary bus devices (VE2) to the HAL framework.
 * Reference: amdxdna/amdxdna_of_drv.c (OF driver, similar structure for aux)
 */

#include <linux/module.h>
#include <linux/version.h>
#include <linux/auxiliary_bus.h>
#include <linux/dma-mapping.h>
#include <linux/errno.h>
#include <drm/drm_managed.h>

#include "amdxdna_drm.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("AMD XDNA HAL - Auxiliary bus driver binding (prototype)");

static const struct auxiliary_device_id amdxdna_aux_id_table[] = {
	{ .name = "amdxdna.ve2", },
	{ }
};

MODULE_DEVICE_TABLE(auxiliary, amdxdna_aux_id_table);

/* Forward declarations for prototype */
struct amdxdna_dev;
struct amdxdna_dev_info;

static int amdxdna_aux_probe(struct auxiliary_device *adev,
			     const struct auxiliary_device_id *id)
{
	struct device *dev = &adev->dev;
	struct amdxdna_dev *xdna;

	/* Allocate DRM device (common framework) */
	xdna = devm_drm_dev_alloc(dev, &amdxdna_drm_drv, typeof(*xdna), ddev);
	if (IS_ERR(xdna))
		return PTR_ERR(xdna);

	/* Stub: would probe auxiliary device and register with HAL framework */
	/* Real implementation would continue with:
	 *   - Get device info from id->data: xdna->dev_info = (struct amdxdna_dev_info *)id->data
	 *   - Initialize device lock and client list
	 *   - Call dev_info->ops->init()
	 *   - Set vbnv to default
	 *   - Register DRM device: drm_dev_register(&xdna->ddev, 0)
	 *   - Configure DMA mask
	 *   - Initialize debugfs
	 *   - Set IOMMU mode
	 */
	(void)adev;
	(void)id;
	return -ENODEV;
}

static void amdxdna_aux_remove(struct auxiliary_device *adev)
{
	struct amdxdna_dev *xdna = auxiliary_get_drvdata(adev);

	/* Unregister DRM device (common framework) */
	/* Real: drm_dev_unplug(&xdna->ddev); */
	/* Stub: would unregister DRM device */

	/* Stub: would remove auxiliary device from HAL framework */
	/* Real implementation:
	 *   - Unregister DRM device: drm_dev_unplug(&xdna->ddev)
	 *   - Call dev_info->ops->fini()
	 *   - Clean up device resources
	 */
	(void)xdna;
	(void)adev;
}

static struct auxiliary_driver amdxdna_aux_driver = {
	.probe		= amdxdna_aux_probe,
	.remove		= amdxdna_aux_remove,
	.id_table	= amdxdna_aux_id_table,
};

/* Stub functions - would be called by framework to register/unregister auxiliary driver */
int amdxdna_aux_drv_init(void)
{
	/* Stub: would register auxiliary driver */
	/* Real: return auxiliary_driver_register(&amdxdna_aux_driver); */
	return -ENODEV;
}

void amdxdna_aux_drv_fini(void)
{
	/* Stub: would unregister auxiliary driver */
	/* Real: auxiliary_driver_unregister(&amdxdna_aux_driver); */
}

