/** @file
 * Copyright (c) 2022-2023, 2025, Arm Limited or its affiliates. All rights reserved.
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

#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>

#include <Protocol/AcpiTable.h>
#include "Include/IndustryStandard/Acpi61.h"

#include "include/pal_uefi.h"

static EFI_ACPI_6_1_GENERIC_TIMER_DESCRIPTION_TABLE *gGtdtHdr;

UINT64
pal_get_gtdt_ptr();

/* Information about only one timer can be mentioned as an Override */
static
VOID
pal_timer_platform_override(TIMER_INFO_TABLE *TimerTable)
{
  if (PLATFORM_OVERRIDE_PLATFORM_TIMER) {
      TimerTable->header.num_platform_timer = 1;
      TimerTable->gt_info[0].block_cntl_base = PLATFORM_OVERRIDE_CNTCTL_BASE;
      TimerTable->gt_info[0].timer_count = 1;
      TimerTable->gt_info[0].GtCntBase[0]  = PLATFORM_OVERRIDE_CNTBASE_N;
      TimerTable->gt_info[0].gsiv[0] = PLATFORM_OVERRIDE_PLATFORM_TIMER_GSIV;
  }

  //GTDT does not have this information yet.
  if (PLATFORM_OVERRIDE_EL2_VIR_TIMER_GSIV) {
      TimerTable->header.el2_virt_timer_gsiv = PLATFORM_OVERRIDE_EL2_VIR_TIMER_GSIV;
  }

}

/**
  @brief  This API fills in the TIMER_INFO_TABLE with information about local and system
          timers in the system. This is achieved by parsing the ACPI - GTDT table.

  @param  TimerTable  - Address where the Timer information needs to be filled.

  @return  None
**/
VOID
pal_timer_create_info_table(TIMER_INFO_TABLE *TimerTable)
{
  EFI_ACPI_6_1_GTDT_GT_BLOCK_STRUCTURE       *Entry = NULL;
  EFI_ACPI_6_1_GTDT_GT_BLOCK_TIMER_STRUCTURE *GtBlockTimer = NULL;
  TIMER_INFO_GTBLOCK         *GtEntry = NULL;
  UINT32                      Length= 0;
  UINT32                      i;
  UINT32                      num_of_entries;

  if (TimerTable == NULL) {
    rme_print(ACS_PRINT_ERR, L" Input Timer Table Pointer is NULL. Cannot create Timer INFO \n");
    return;
  }

  GtEntry = TimerTable->gt_info;
  TimerTable->header.num_platform_timer = 0;

  gGtdtHdr = (EFI_ACPI_6_1_GENERIC_TIMER_DESCRIPTION_TABLE *) pal_get_gtdt_ptr();

  if (gGtdtHdr == NULL) {
    rme_print(ACS_PRINT_ERR, L" GTDT not found \n");
    return;
  }
  rme_print(ACS_PRINT_INFO, L" GTDT is at %x and length is %x \n", gGtdtHdr, gGtdtHdr->Header.Length);

  //Fill in our internal table
  TimerTable->header.s_el1_timer_flag  = gGtdtHdr->SecurePL1TimerFlags;
  TimerTable->header.ns_el1_timer_flag = gGtdtHdr->NonSecurePL1TimerFlags;
  TimerTable->header.el2_timer_flag    = gGtdtHdr->NonSecurePL2TimerFlags;
  TimerTable->header.s_el1_timer_gsiv  = gGtdtHdr->SecurePL1TimerGSIV;
  TimerTable->header.ns_el1_timer_gsiv = gGtdtHdr->NonSecurePL1TimerGSIV;
  TimerTable->header.el2_timer_gsiv    = gGtdtHdr->NonSecurePL2TimerGSIV;
  TimerTable->header.virtual_timer_flag = gGtdtHdr->VirtualTimerFlags;
  TimerTable->header.virtual_timer_gsiv = gGtdtHdr->VirtualTimerGSIV;

  Length         = gGtdtHdr->PlatformTimerOffset;
  Entry          = (EFI_ACPI_6_1_GTDT_GT_BLOCK_STRUCTURE *) ((UINT8 *)gGtdtHdr + Length);
  Length         = sizeof (EFI_ACPI_6_1_GENERIC_TIMER_DESCRIPTION_TABLE);
  num_of_entries = gGtdtHdr->PlatformTimerCount;

  while(num_of_entries) {

    if (Entry->Type == EFI_ACPI_6_1_GTDT_GT_BLOCK) {
      rme_print(ACS_PRINT_INFO, L" Found block entry \n");
      GtEntry->type = TIMER_TYPE_SYS_TIMER;
      GtEntry->block_cntl_base = Entry->CntCtlBase;
      GtEntry->timer_count     = Entry->GTBlockTimerCount;
      rme_print(ACS_PRINT_DEBUG, L" CNTCTLBase = %llx \n", GtEntry->block_cntl_base);
      GtBlockTimer = (EFI_ACPI_6_1_GTDT_GT_BLOCK_TIMER_STRUCTURE *)(((UINT8 *)Entry) + Entry->GTBlockTimerOffset);
      for (i = 0; i < GtEntry->timer_count; i++) {
        rme_print(ACS_PRINT_INFO, L" Found timer entry \n");
        GtEntry->frame_num[i]    = GtBlockTimer->GTFrameNumber;
        GtEntry->GtCntBase[i]    = GtBlockTimer->CntBaseX;
        GtEntry->GtCntEl0Base[i] = GtBlockTimer->CntEL0BaseX;
        GtEntry->gsiv[i]         = GtBlockTimer->GTxPhysicalTimerGSIV;
        GtEntry->virt_gsiv[i]    = GtBlockTimer->GTxVirtualTimerGSIV;
        GtEntry->flags[i]        = GtBlockTimer->GTxPhysicalTimerFlags | (GtBlockTimer->GTxVirtualTimerFlags << 8) | (GtBlockTimer->GTxCommonFlags << 16);
        rme_print(ACS_PRINT_DEBUG,
                   L" CNTBaseN = %llx for sys counter = %d\n", GtEntry->GtCntBase[i], i);
        GtBlockTimer++;
        TimerTable->header.num_platform_timer++;
      }
      GtEntry++;
    }

    if (Entry->Type == EFI_ACPI_6_1_GTDT_RME_GENERIC_WATCHDOG) {
      //This is a Watchdog entry. Skip.. added in a separate function.
    }

    Entry = (EFI_ACPI_6_1_GTDT_GT_BLOCK_STRUCTURE *) ((UINT8 *)Entry + (Entry->Length));
    num_of_entries--;

  };

  pal_timer_platform_override(TimerTable);

}

/**
  @brief  This API gets the counter frequency value from user

  @param  None

  @return Counter frequency value
**/

UINT64
pal_timer_get_counter_frequency(VOID)
{
  return PLATFORM_OVERRIDE_TIMER_CNTFRQ;
}
