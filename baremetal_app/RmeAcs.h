/** @file
 * Copyright (c) 2022-2023, Arm Limited or its affiliates. All rights reserved.
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

#ifndef __RME_ACS_LEVEL_H__
#define __RME_ACS_LEVEL_H__

  #define SIZE_4K 0x100

  #define RME_ACS_MAJOR_VER  0
  #define RME_ACS_MINOR_VER  7

  #define RME_MIN_LEVEL_SUPPORTED 3
  #define RME_MAX_LEVEL_SUPPORTED 6

  #ifdef _AARCH64_BUILD_
  unsigned long __stack_chk_guard = 0xBAAAAAAD;
  unsigned long __stack_chk_fail =  0xBAAFAAAD;
  #endif

#endif
