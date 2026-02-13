# SPDX-License-Identifier: GPL-2.0
#
# Copyright (C) 2022-2026, Advanced Micro Devices, Inc.
#
# AMD XDNA HAL prototype - parallel and independent of amdxdna.
# Build: make -C $(KERNEL_SRC) M=$(PWD) modules
#

KERNEL_VER ?= $(shell uname -r)
KERNEL_SRC ?= /lib/modules/$(KERNEL_VER)/build
XDNA_BUS_TYPE ?= pci

ifeq ($(XDNA_BUS_TYPE), pci)
PCI = y
AUX = n
else ifeq ($(XDNA_BUS_TYPE), aux)
PCI = n
AUX = y
else
$(error Unknown XDNA_BUS_TYPE $(XDNA_BUS_TYPE). Use 'pci' or 'aux'!!)
endif

mkfile_path = $(abspath $(lastword $(MAKEFILE_LIST)))
SRC_DIR = $(patsubst %/,%,$(dir $(mkfile_path)))
BUILD_ROOT_DIR ?= $(SRC_DIR)/build

.PHONY: all clean modules modules_install

all: $(BUILD_ROOT_DIR)
	$(MAKE) -C $(BUILD_ROOT_DIR)/driver/$(notdir $(SRC_DIR)) modules

modules:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC_DIR) OFT_CONFIG_AMDXDNA_PCI=$(PCI) OFT_CONFIG_AMDXDNA_AUX=$(AUX) modules

$(BUILD_ROOT_DIR): $(SRC_DIR)
	@echo "[INFO] BUILD directory not found or SRC newer - recreate $(BUILD_ROOT_DIR)"
	@rm -rf $@
	@mkdir -p $@/driver
	@cp -r $(SRC_DIR) $@/driver/
	@cp -r $(SRC_DIR)/../../include $@ 2>/dev/null || true

clean:
	rm -rf $(BUILD_ROOT_DIR)
	rm -rf $(SRC_DIR)/*.o $(SRC_DIR)/.*.cmd $(SRC_DIR)/*.ko $(SRC_DIR)/*.mod* $(SRC_DIR)/modules.order $(SRC_DIR)/Module.symvers
