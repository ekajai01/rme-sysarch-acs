## @file
#  Copyright (c) 2025, Arm Limited or its affiliates. All rights reserved.
#  SPDX-License-Identifier : Apache-2.0
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
##

if [ -v $GCC49_AARCH64_PREFIX ]
then
    echo "GCC49_AARCH64_PREFIX is not set"
    echo "set using export GCC49_AARCH64_PREFIX=<lib_path>/bin/aarch64-linux-gnu-"
    return 0
fi

if [ "$1" == "ENABLE_OOB" ]; then
    build -a AARCH64 -t GCC49 -p ShellPkg/ShellPkg.dsc \
            -m ShellPkg/Application/rme-acs/baremetal_app/RmeAcs.inf -D ENABLE_OOB
    return 0;
fi

    build -a AARCH64 -t GCC49 -p ShellPkg/ShellPkg.dsc \
        -m ShellPkg/Application/rme-acs/uefi_app/RmeAcs.inf


