## @file
 # Copyright (c) 2025, Arm Limited or its affiliates. All rights reserved.
 # SPDX-License-Identifier : Apache-2.0
 #
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 #
 #  http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.
 ##

project(rme LANGUAGES C ASM)

set(EXE_NAME ${PROJECT_NAME})

set(VAL_LIB ${EXE_NAME}_val_lib)
set(PAL_LIB ${EXE_NAME}_pal_lib)


include(${ROOT_DIR}/platform/pal_baremetal/pal_rme.cmake)
include(${ROOT_DIR}/val/val_rme.cmake)
include(${ROOT_DIR}/test_pool/test.cmake)
