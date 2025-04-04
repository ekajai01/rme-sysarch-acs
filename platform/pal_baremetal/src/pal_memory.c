/** @file
 * Copyright (c) 2025, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0

 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#include "platform_image_def.h"
#include "platform_override_struct.h"
#include "platform_override_fvp.h"
#include "pal_common_support.h"

// Note that while creating a list of mem map, the size of mappings will vary across platforms.
// The below mapping size is specific to FVP RDN2.

extern PLATFORM_OVERRIDE_GIC_INFO_TABLE     platform_gic_cfg;
extern PLATFORM_OVERRIDE_TIMER_INFO_TABLE   platform_timer_cfg;
extern PLATFORM_OVERRIDE_IOVIRT_INFO_TABLE  platform_iovirt_cfg;
extern PLATFORM_OVERRIDE_NODE_DATA          platform_node_type;
extern PLATFORM_OVERRIDE_UART_INFO_TABLE    platform_uart_cfg;
extern PLATFORM_OVERRIDE_MEMORY_INFO_TABLE  platform_mem_cfg;
extern PCIE_INFO_TABLE                      platform_pcie_cfg;

extern addr_t __TEXT_START__, __TEXT_END__;
#define TEXT_START    ((addr_t)&__TEXT_START__)
#define TEXT_END      ((addr_t)&__TEXT_END__)

extern addr_t __RODATA_START__, __RODATA_END__;
#define RODATA_START  ((addr_t)&__RODATA_START__)
#define RODATA_END    ((addr_t)&__RODATA_END__)

extern addr_t __DATA_START__, __DATA_END__;
#define DATA_START    ((addr_t)&__DATA_START__)
#define DATA_END      ((addr_t)&__DATA_END__)

extern addr_t __BSS_START__, __BSS_END__;
#define BSS_START  ((addr_t)&__BSS_START__)
#define BSS_END    ((addr_t)&__BSS_END__)


#define MAX_MMAP_REGION_COUNT 75
memory_region_descriptor_t mmap_region_list[MAX_MMAP_REGION_COUNT];

uint64_t device_mem_region_attr       = ATTR_DEVICE_RW;
uint64_t normal_mem_region_attributes = ATTR_RW_DATA;
uint64_t map_length                   = MEM_SIZE_64K;

uint64_t image_size                = PLATFORM_NORMAL_WORLD_IMAGE_SIZE;
uint64_t image_base                = PLATFORM_NORMAL_WORLD_IMAGE_BASE;
uint64_t mem_pool_size             = PLATFORM_MEMORY_POOL_SIZE;

static uint32_t mmap_list_curr_index;
uint32_t is_mem_pool_mapped        = 0;
uint32_t is_uart_region_mapped     = 0;
uint32_t is_watchdog_region_mapped = 0;
uint32_t is_timer_region_mapped    = 0;
uint32_t is_gic_region_mapped      = 0;
uint32_t is_smmu_region_mapped     = 0;
uint32_t is_pcie_region_mapped     = 0;
uint32_t is_platform_mem_mapped    = 0;

void map_text_region()
{
    mmap_region_list[mmap_list_curr_index].virtual_address  = TEXT_START;
    mmap_region_list[mmap_list_curr_index].physical_address = TEXT_START;
    mmap_region_list[mmap_list_curr_index].length           = TEXT_END - TEXT_START;
    mmap_region_list[mmap_list_curr_index].attributes       = ATTR_CODE;
    mmap_list_curr_index++;
}

void map_rodata_region()
{
    mmap_region_list[mmap_list_curr_index].virtual_address  = RODATA_START;
    mmap_region_list[mmap_list_curr_index].physical_address = RODATA_START;
    mmap_region_list[mmap_list_curr_index].length           = RODATA_END - RODATA_START;
    mmap_region_list[mmap_list_curr_index].attributes       = ATTR_RO_DATA;
    mmap_list_curr_index++;
}

void map_data_region()
{
    mmap_region_list[mmap_list_curr_index].virtual_address  = DATA_START;
    mmap_region_list[mmap_list_curr_index].physical_address = DATA_START;
    mmap_region_list[mmap_list_curr_index].length           = DATA_END - DATA_START;
    mmap_region_list[mmap_list_curr_index].attributes       = ATTR_RW_DATA;
    mmap_list_curr_index++;
}

void map_bss_region()
{
    mmap_region_list[mmap_list_curr_index].virtual_address  = BSS_START;
    mmap_region_list[mmap_list_curr_index].physical_address = BSS_START;
    mmap_region_list[mmap_list_curr_index].length           = BSS_END - BSS_START;
    mmap_region_list[mmap_list_curr_index].attributes       = ATTR_RW_DATA;
    mmap_list_curr_index++;
}

void map_mem_pool_region()
{
    mmap_region_list[mmap_list_curr_index].virtual_address  = image_base + image_size;
    mmap_region_list[mmap_list_curr_index].physical_address = image_base + image_size;
    mmap_region_list[mmap_list_curr_index].length           = mem_pool_size;
    mmap_region_list[mmap_list_curr_index].attributes       = ATTR_RW_DATA;
    mmap_list_curr_index++;
}

void map_uart_device_region(uint64_t length, uint64_t attr)
{
    uint64_t uart_base;
    uart_base = platform_uart_cfg.BaseAddress.Address;
    mmap_region_list[mmap_list_curr_index].virtual_address  = uart_base;
    mmap_region_list[mmap_list_curr_index].physical_address = uart_base;
    mmap_region_list[mmap_list_curr_index].length           = length;
    mmap_region_list[mmap_list_curr_index].attributes       = attr;
    mmap_list_curr_index++;
}

void map_timer_device_region(uint64_t num_timer, uint64_t length, uint64_t attr)
{
    uint8_t i = 0;
    uint64_t gen_timer_base;
    uint64_t block_cntl_base;

    block_cntl_base = platform_timer_cfg.gt_info.block_cntl_base;
    mmap_region_list[mmap_list_curr_index].virtual_address  = block_cntl_base;
    mmap_region_list[mmap_list_curr_index].physical_address = block_cntl_base;
    mmap_region_list[mmap_list_curr_index].length           = length;
    mmap_region_list[mmap_list_curr_index].attributes       = attr;
    mmap_list_curr_index++;

    for(i = 0; i < num_timer; i++)
    {
        gen_timer_base    = platform_timer_cfg.gt_info.GtCntBase[i];
        mmap_region_list[mmap_list_curr_index].virtual_address  = gen_timer_base;
        mmap_region_list[mmap_list_curr_index].physical_address = gen_timer_base;
        mmap_region_list[mmap_list_curr_index].length           = length;
        mmap_region_list[mmap_list_curr_index].attributes       = attr;
        mmap_list_curr_index++;
    }
}

void map_gic_device_region(uint32_t gicc_count, uint32_t gicd_count,
                           uint32_t gic_rd_count, uint32_t gich_count, uint32_t gic_its_count,
                           uint64_t length, uint64_t attr)
{
    uint8_t i;
    uint64_t base_address;
    for (i = 0; i < gicc_count; i++)
    {
        base_address = platform_gic_cfg.gicc_base[i];
        mmap_region_list[mmap_list_curr_index].virtual_address  = base_address;
        mmap_region_list[mmap_list_curr_index].physical_address = base_address;
        mmap_region_list[mmap_list_curr_index].length           = 0x4 * length;//0x40000 map size
        mmap_region_list[mmap_list_curr_index].attributes       = attr;
        mmap_list_curr_index++;
    }

    for (i = 0; i < gicd_count; i++)
    {
        base_address = platform_gic_cfg.gicd_base[i];
        mmap_region_list[mmap_list_curr_index].virtual_address  = base_address;
        mmap_region_list[mmap_list_curr_index].physical_address = base_address;
        mmap_region_list[mmap_list_curr_index].length           = 20 * length;
        mmap_region_list[mmap_list_curr_index].attributes       = attr;
        mmap_list_curr_index++;
    }

    for (i = 0; i < gic_rd_count; i++)
    {
        base_address = platform_gic_cfg.gicrd_base[i];
        mmap_region_list[mmap_list_curr_index].virtual_address  = base_address;
        mmap_region_list[mmap_list_curr_index].physical_address = base_address;
        mmap_region_list[mmap_list_curr_index].length           = 0x4 * length;
        mmap_region_list[mmap_list_curr_index].attributes       = attr;
        mmap_list_curr_index++;
    }

    for (i = 0; i < gich_count; i++)
    {
        base_address = platform_gic_cfg.gich_base[i];
        mmap_region_list[mmap_list_curr_index].virtual_address  = base_address;
        mmap_region_list[mmap_list_curr_index].physical_address = base_address;
        mmap_region_list[mmap_list_curr_index].length           = 0x4 * length;//0x40000 map length
        mmap_region_list[mmap_list_curr_index].attributes       = attr;
        mmap_list_curr_index++;
    }

    for (i = 0; i < gic_its_count; i++)
    {
        base_address = platform_gic_cfg.gicits_base[i];
        mmap_region_list[mmap_list_curr_index].virtual_address  = base_address;
        mmap_region_list[mmap_list_curr_index].physical_address = base_address;
        mmap_region_list[mmap_list_curr_index].length           = 0x2 * length;//0x20000 map length
        mmap_region_list[mmap_list_curr_index].attributes       = attr;
        mmap_list_curr_index++;
    }
}

void map_smmu_device_region(uint64_t num_smmu, uint64_t length, uint64_t attr)
{
    uint8_t i = 0;
    uint64_t smmu_base;

    for(i = 0; i < num_smmu; i++)
    {
        smmu_base    = platform_node_type.smmu[i].base;
        mmap_region_list[mmap_list_curr_index].virtual_address  = smmu_base;
        mmap_region_list[mmap_list_curr_index].physical_address = smmu_base;
        mmap_region_list[mmap_list_curr_index].length           = 0x5 * length;//0x50000 map length
        mmap_region_list[mmap_list_curr_index].attributes       = attr;
        mmap_list_curr_index++;
    }
}

void map_pcie_ecam_bar_region(uint32_t num_ecam, uint64_t length, uint64_t attr)
{
    uint8_t i = 0;
    uint64_t ecam_base;

    // Map ECAM region to memory
    for(i = 0; i < num_ecam; i++)
    {
        ecam_base    = platform_pcie_cfg.block[i].ecam_base;
        mmap_region_list[mmap_list_curr_index].virtual_address  = ecam_base;
        mmap_region_list[mmap_list_curr_index].physical_address = ecam_base;
        mmap_region_list[mmap_list_curr_index].length           = 0x1000 * length;
        mmap_region_list[mmap_list_curr_index].attributes       = attr;
        mmap_list_curr_index++;
    }

    // Map BAR region to memory only if ECAM is present
    if (num_ecam)
    {
        mmap_region_list[mmap_list_curr_index].virtual_address  = PLATFORM_OVERRIDE_PCIE_ECAM0_EP_BAR64;
        mmap_region_list[mmap_list_curr_index].physical_address = PLATFORM_OVERRIDE_PCIE_ECAM0_EP_BAR64;
        mmap_region_list[mmap_list_curr_index].length           = 0x10 * length;
        mmap_region_list[mmap_list_curr_index].attributes       = attr;
        mmap_list_curr_index++;

        mmap_region_list[mmap_list_curr_index].virtual_address  = PLATFORM_OVERRIDE_PCIE_ECAM0_RP_BAR64;
        mmap_region_list[mmap_list_curr_index].physical_address = PLATFORM_OVERRIDE_PCIE_ECAM0_RP_BAR64;
        mmap_region_list[mmap_list_curr_index].length           = 0x10 * length;
        mmap_region_list[mmap_list_curr_index].attributes       = attr;
        mmap_list_curr_index++;

        mmap_region_list[mmap_list_curr_index].virtual_address  = PLATFORM_OVERRIDE_PCIE_ECAM0_EP_NPBAR32;
        mmap_region_list[mmap_list_curr_index].physical_address = PLATFORM_OVERRIDE_PCIE_ECAM0_EP_NPBAR32;
        mmap_region_list[mmap_list_curr_index].length           = 0x60 * length;
        mmap_region_list[mmap_list_curr_index].attributes       = attr;
        mmap_list_curr_index++;

        mmap_region_list[mmap_list_curr_index].virtual_address  = PLATFORM_OVERRIDE_PCIE_ECAM0_EP_PBAR32;
        mmap_region_list[mmap_list_curr_index].physical_address = PLATFORM_OVERRIDE_PCIE_ECAM0_EP_PBAR32;
        mmap_region_list[mmap_list_curr_index].length           = 0x100 * length;
        mmap_region_list[mmap_list_curr_index].attributes       = attr;
        mmap_list_curr_index++;

        mmap_region_list[mmap_list_curr_index].virtual_address  = PLATFORM_OVERRIDE_PCIE_ECAM0_RP_BAR32;
        mmap_region_list[mmap_list_curr_index].physical_address = PLATFORM_OVERRIDE_PCIE_ECAM0_RP_BAR32;
        mmap_region_list[mmap_list_curr_index].length           = 0x20 * length;
        mmap_region_list[mmap_list_curr_index].attributes       = attr;
        mmap_list_curr_index++;
    }
}

void map_test_mem_region(uint32_t length, uint32_t attr)
{
    // Free Mem region. 2MB map.
    mmap_region_list[mmap_list_curr_index].virtual_address  = FREE_MEM_START;
    mmap_region_list[mmap_list_curr_index].physical_address = FREE_MEM_START;
    mmap_region_list[mmap_list_curr_index].length           = 0x20 * length;
    mmap_region_list[mmap_list_curr_index].attributes       = attr;
    mmap_list_curr_index++;

    // VA and PA TEST Mem region.
    mmap_region_list[mmap_list_curr_index].virtual_address  = MEM_FREE_VA_TEST;
    mmap_region_list[mmap_list_curr_index].physical_address = MEM_FREE_PA_TEST;
    mmap_region_list[mmap_list_curr_index].length           = 0x20 * length;
    mmap_region_list[mmap_list_curr_index].attributes       = attr;
    mmap_list_curr_index++;

    // NVM memory map.
    mmap_region_list[mmap_list_curr_index].virtual_address  = RME_NVM_MEM;
    mmap_region_list[mmap_list_curr_index].physical_address = RME_NVM_MEM;
    mmap_region_list[mmap_list_curr_index].length           = 0x20 * length;
    mmap_region_list[mmap_list_curr_index].attributes       = attr;
    mmap_list_curr_index++;

    // Shared data region
    mmap_region_list[mmap_list_curr_index].virtual_address  = SHARED_ADDRESS;
    mmap_region_list[mmap_list_curr_index].physical_address = SHARED_ADDRESS;
    mmap_region_list[mmap_list_curr_index].length           = 0x20 * length;
    mmap_region_list[mmap_list_curr_index].attributes       = attr;
    mmap_list_curr_index++;
}

void pal_mmu_add_mmap(void)
{
    uint32_t timer_count, smmu_count, ecam_count;
    uint32_t gicc_count, gicd_count, gic_rd_count, gich_count, gic_its_count;
    mmap_list_curr_index = 0;

    /* Map Image regions */
    // Text region - Get the start and end addressed from linker script and map with ATTR_CODE
    map_text_region();

    // RODATA region - Get the start and end addressed from linker script and map with ATTR_DATA_RO
    map_rodata_region();

    // Data region - Get the start and end addressed from linker script and map with ATTR_DATA_RW
    map_data_region();

    // BSS region - Get the start and end addressed from linker script and map with ATTR_DATA_RW
    map_bss_region();

    // Mem Pool region - Get the start and end addressed from platform_override.h
    // and map with ATTR_DATA_RW
    map_mem_pool_region();

    // Map UART device region
    map_uart_device_region(map_length, device_mem_region_attr);

    // Iterate through number of System timers and map the device region
    timer_count = platform_timer_cfg.gt_info.timer_count;
    map_timer_device_region(timer_count, map_length, device_mem_region_attr);

    // Map GIC device region to memory. This includes GICC, GICD, GICRD, GICH and GIC_ITS
    gicc_count    = platform_gic_cfg.num_gicc;
    gicd_count    = platform_gic_cfg.num_gicd;
    gic_rd_count = platform_gic_cfg.num_gicrd;
    gich_count    = platform_gic_cfg.num_gich;
    gic_its_count = platform_gic_cfg.num_gicits;
    map_gic_device_region(gicc_count, gicd_count, gic_rd_count, gich_count,
                                gic_its_count, map_length, device_mem_region_attr);

    // Map SMMU device region to memory. Iterate through number of SMMUs
    smmu_count = IOVIRT_SMMUV3_COUNT;
    map_smmu_device_region(smmu_count, map_length, device_mem_region_attr);

    // Map PCIe ECAM region and BAR regions to memory.
    ecam_count = platform_pcie_cfg.num_entries;
    map_pcie_ecam_bar_region(ecam_count, map_length, device_mem_region_attr);

    // Map FREE mem and TEST mem to NS EL2 MMU
    map_test_mem_region(map_length, normal_mem_region_attributes);
    return;
}

void *pal_mmu_get_mmap_list(void)
{
    return (memory_region_descriptor_t *)mmap_region_list;
}

uint32_t pal_mmu_get_mapping_count(void)
{
    return mmap_list_curr_index;
}