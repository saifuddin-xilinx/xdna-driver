/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2022-2026, Advanced Micro Devices, Inc.
 *
 * Common Framework - Prototype (no implementation).
 *
 * This layer sits above the HAL in the unified driver design:
 *   • IOCTL / uAPI
 *   • GEM memory management
 *   • DRM core integration (dma-buf, file ops, etc.)
 *
 * In the real amdxdna driver this is implemented in amdxdna_drm.c and
 * related files. Here we only declare the layer for structure.
 */

#ifndef _AMDXDNA_HAL_DRV_H_
#define _AMDXDNA_HAL_DRV_H_

/*
 * Common framework placeholder types.
 * Real driver: struct amdxdna_dev, amdxdna_client, drm_device, etc.
 */
struct amdxdna_hal_drv_device;

/* Placeholder: framework init (would register DRM, use HAL ops) */
int amdxdna_hal_drv_init(void);
void amdxdna_hal_drv_fini(void);

#endif /* _AMDXDNA_HAL_DRV_H_ */
