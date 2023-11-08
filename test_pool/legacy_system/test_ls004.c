/** @file
 * Copyright (c) 2023, Arm Limited or its affiliates. All rights reserved.
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

#include "val/include/rme_acs_val.h"
#include "val/include/val_interface.h"

#include "val/include/rme_acs_pe.h"
#include "val/include/rme_acs_smmu.h"
#include "val/include/rme_acs_pcie.h"
#include "val/include/rme_acs_el32.h"

#define TEST_NUM  (ACS_LEGACY_TEST_NUM_BASE + 04)
#define TEST_DESC  "Check that LEGACY_TZ_EN does not take effect when system reset is de-asserted"
#define TEST_RULE  "SYS_RST_05"

#define BIT_30 30
#define BIT_55 55
#define BIT_52 52

/*
 * @brief  The test validates that enabling LEGACY_TZ_EN doesn't take any effect when system
 *         reset has been de-asserted.
 * 1. The system reset is de-asserted and then LEGACY_TZ_EN is enabled.
 * 2. The bit[52:55] of ID_AA64PFR0_EL1 register is checked for PE's RME implementation.
 * 3. The bit[30] of SMMU_IDR0 register is checked for SMMU's RME implementation.
 * 4. These bits are expected to be set still, after the de-assertion of system reset indicating
 *    that LEGACY_TZ_EN doesn't have any effect.
 */
static
void
payload()
{

  uint64_t rme_impl_smmu, rme_impl_pe;
  uint32_t num_smmu, smmu_index;
  uint32_t index = val_pe_get_index_mpid(val_pe_get_mpid());

  //Enable the Legacy_TZ_EN tie-off in the system
  val_prog_legacy_tz(SET);
  num_smmu = val_smmu_get_info(SMMU_NUM_CTRL, 0);

  if (num_smmu == 0) {
      val_print(ACS_PRINT_ERR, "\nNo SMMU Controllers are discovered ", 0);
      val_set_status(index, RESULT_SKIP(TEST_NUM, 01));
      return;
  }

  smmu_index = 0;
  if (val_smmu_get_info(SMMU_CTRL_ARCH_MAJOR_REV, smmu_index) == 2) {
      val_print(ACS_PRINT_WARN, "\nNot valid for SMMU v2           ", 0);
      val_set_status(index, RESULT_SKIP(TEST_NUM, 02));
      return;
  }

  /* VAL_EXTRACT_BITS(data, start_bit, end_bit) */
  rme_impl_smmu = VAL_EXTRACT_BITS(val_smmu_read_cfg(SMMUv3_IDR0, smmu_index), BIT_30, BIT_30);
  val_print(ACS_PRINT_DEBUG, "\n The RME implementation bit of SMMUv3_IDR0 = %lx", rme_impl_smmu);

  rme_impl_pe = VAL_EXTRACT_BITS(val_pe_reg_read(ID_AA64PFR0_EL1), BIT_52, BIT_55);
  val_print(ACS_PRINT_DEBUG, "\n The RME implementation bit of ID_AA64PFR0_EL1 = %lx", rme_impl_pe);

  /*Check If SMMU_IDR0.RME_IMPL[30] == 0b0 and ID_AA64PFR0_EL1.RME_IMPL[52:55] == 0b00010*/
  if (rme_impl_smmu && rme_impl_pe) {

    if (rme_impl_pe)
      val_print(ACS_PRINT_DEBUG, "\n  The RME implementation bit of PE is set\
                      even after enabling LEGACY_TZ_EN", 0);
    if (rme_impl_smmu)
      val_print(ACS_PRINT_DEBUG, "\n  The RME implementation bit of SMMU is set\
                      even after enabling LEGACY_TZ_EN", 0);
    val_set_status(index, RESULT_PASS(TEST_NUM, 01));
    return;
  } else {

    if (!rme_impl_pe)
      val_print(ACS_PRINT_ERR, "\n  The RME implementation bit of PE is reset to RES0 ", 0);

    if (!rme_impl_smmu)
      val_print(ACS_PRINT_ERR, "\n  The RME implementation bit of SMMU is reset to RES0", 0);

    val_set_status(index, RESULT_FAIL(TEST_NUM, 01));
    return;
  }

}

uint32_t
ls004_entry(uint32_t num_pe)
{
  num_pe = 1;
  uint32_t  status = ACS_STATUS_FAIL;

  status = val_initialize_test(TEST_NUM, TEST_DESC, num_pe, TEST_RULE);

  if (status != ACS_STATUS_SKIP)
      val_run_test_payload(TEST_NUM, num_pe, payload, 0);

  /*get the result from all PE and check for failure */
  status = val_check_for_error(TEST_NUM, num_pe, TEST_RULE);
  val_report_status(0, RME_ACS_END(TEST_NUM), TEST_RULE);

  return  status;
}

