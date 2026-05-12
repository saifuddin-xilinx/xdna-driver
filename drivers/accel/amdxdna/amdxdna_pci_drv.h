/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2022-2026, Advanced Micro Devices, Inc.
 */

#ifndef _AMDXDNA_PCI_DRV_H_
#define _AMDXDNA_PCI_DRV_H_

#include "amdxdna_drv.h"

/* Add device info below */
extern const struct amdxdna_dev_info dev_npu1_info;
extern const struct amdxdna_dev_info dev_npu3_classic_info;
extern const struct amdxdna_dev_info dev_npu3_pf_info;
extern const struct amdxdna_dev_info dev_npu3_vf_info;
extern const struct amdxdna_dev_info dev_npu4_info;
extern const struct amdxdna_dev_info dev_npu5_info;
extern const struct amdxdna_dev_info dev_npu6_info;

/* True if the current task may examine @client's contexts. */
static inline bool amdxdna_client_visible(struct amdxdna_client *client)
{
	return capable(CAP_SYS_ADMIN) || (current->pid == client->pid);
}

#endif /* _AMDXDNA_PCI_DRV_H_ */
