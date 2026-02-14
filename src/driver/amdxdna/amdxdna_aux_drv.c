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
#include "amdxdna_hal_drv.h"

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
	const struct amdxdna_hal *hal;

	/* Execution flow: Device Init */
	/* Kernel binds accel-vdev → amdxdna_aux_probe() */

	/* Determine which HAL to use (VE2 for auxiliary bus) */
	hal = &amdxdna_hal_ve2;

	/* Call HAL device probe */
	/* amdxdna_hal_dev_probe(dev, hal) */
	xdna = amdxdna_hal_dev_probe(dev, hal);
	if (IS_ERR(xdna))
		return PTR_ERR(xdna);

	/* Store device in auxiliary device data */
	/* Real: auxiliary_set_drvdata(adev, xdna) */
	auxiliary_set_drvdata(adev, xdna);

	/* Device ready */
	return 0;
}

static void amdxdna_aux_remove(struct auxiliary_device *adev)
{
	struct amdxdna_dev *xdna = auxiliary_get_drvdata(adev);

	/* Execution flow: Device Fini */
	/* amdxdna_aux_remove() */

	/* Call HAL device remove */
	/* amdxdna_hal_dev_remove(xdna) */
	amdxdna_hal_dev_remove(xdna);

	/* Device gone */
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

