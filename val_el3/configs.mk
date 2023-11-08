## @file
 # Copyright (c) 2022-2023, Arm Limited or its affiliates. All rights reserved.
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

# Compiler and linker paths
C_COMPILER = ${RDINFRA}/tools/gcc/arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc

ASM_COMPILER = ${RDINFRA}/tools/gcc/arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc

CPP_COMPILER = ${RDINFRA}/tools/gcc/arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-cpp

LD = ${RDINFRA}/tools/gcc/arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-ld

OBJ_DUMP = ${RDINFRA}/tools/gcc/arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-objdump

OBJ_COPY = ${RDINFRA}/tools/gcc/arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-objcopy

# Source and include variables
EXTRA_SOURCES = ${ACS_HOME}/val_el3/aarch64/ack_helper_functions.S  \
                ${ACS_HOME}/val_el3/pgt_common.c  \
                ${ACS_HOME}/pal_el3/acs_el3.c  \
                ${ACS_HOME}/val_el3/SmcHandlerAck.c  \
                ${ACS_HOME}/val_el3/ack_common.c

INCDIRS = -I ${ACS_HOME}/ -I ${RDINFRA} -I ${RDINFRA}/tf-a/include -I ${RDINFRA}/tf-a/include/arch/aarch64 -I ${RDINFRA}/tf-a/include/lib/cpus/aarch64 -I ${RDINFRA}/tf-a/include/lib/el3_runtime/aarch64 -I ${RDINFRA}/tf-a/plat/arm/css/sgi/include -I ${RDINFRA}/tf-a/include/plat/arm/common/aarch64 -I ${RDINFRA}/tf-a/include/plat/arm/css/common/aarch64 -I ${RDINFRA}/tf-a/plat/arm/board/rdfremont/include/  -I ${RDINFRA}/tf-a/include/lib/libc -I ${RDINFRA}/tf-a/include/lib/libc/aarch64  -I ${RDINFRA}/tf-a/include/lib/libfdt

LD_PATHS = -L${RDINFRA}/tf-a/build/rdfremont/debug/lib

LD_LIBS =  -lfdt -lc -lmbedtls

#Flags
C_FLAGS = -g -gdwarf-4 -nostdinc -Werror -Wall -Wmissing-include-dirs -Wunused -Wdisabled-optimization -Wvla -Wshadow -Wno-unused-parameter -Wredundant-decls -Wunused-but-set-variable -Wmaybe-uninitialized -Wpacked-bitfield-compat -Wshift-overflow=2 -Wlogical-op -Wno-error=deprecated-declarations -Wno-error=cpp -march=armv8-a -mgeneral-regs-only -mstrict-align -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fno-common -Os -std=gnu99 -fno-omit-frame-pointer -fno-stack-protector

ASM_FLAGS = -g -Wa,-gdwarf-4 -nostdinc -Werror -Wall -Wmissing-include-dirs -Wunused -Wdisabled-optimization -Wvla -Wshadow -Wno-unused-parameter -Wredundant-decls -Wunused-but-set-variable -Wmaybe-uninitialized -Wpacked-bitfield-compat -Wshift-overflow=2 -Wlogical-op -Wno-error=deprecated-declarations -Wno-error=cpp -march=armv8-a -ffreestanding -Wa,--fatal-warnings

CPP_FLAGS = -DDEBUG=1 -DENABLE_BACKTRACE=1 -DGICV3_SUPPORT_GIC600=1 -DGICV3_SUPPORT_GIC600AE_FMU=0 -DGIC_ENABLE_V4_EXTN=1 -DGIC_EXT_INTID=0 -DGIC600_ERRATA_WA_2384374=1 -DSGI_PLAT -DCSS_SGI_CHIP_COUNT=1 -DCSS_SGI_PLATFORM_VARIANT=0 -DARM_BL31_IN_DRAM=1 -DARM_TSP_RAM_LOCATION_ID=ARM_DRAM_ID -DARM_RECOM_STATE_ID_ENC=1 -DARM_DISABLE_TRUSTED_WDOG=0 -DARM_CONFIG_CNTACR=1 -DARM_BL31_IN_DRAM=1 -DARM_PLAT_MT=1 -DARM_XLAT_TABLES_LIB_V1=0 -DARM_LINUX_KERNEL_AS_BL33=1 -DARM_PRELOADED_DTB_BASE=0xFEF00000 -DARM_ETHOSN_NPU_DRIVER=0 -DMBEDTLS_SHA256_SMALLER -DARM_CRYPTOCELL_INTEG=0 -DARM_GPT_SUPPORT=0 -DXLAT_TABLES_LIB_V2=1 -DCSS_LOAD_SCP_IMAGES=0 -DCSS_USE_SCMI_SDS_DRIVER=1 -DCSS_NON_SECURE_UART=0 -DA57_ENABLE_NONCACHEABLE_LOAD_FWD=0 -DSKIP_A57_L1_FLUSH_PWR_DWN=0 -DA53_DISABLE_NON_TEMPORAL_HINT=1 -DA57_DISABLE_NON_TEMPORAL_HINT=1 -DWORKAROUND_CVE_2017_5715=1 -DWORKAROUND_CVE_2018_3639=1 -DDYNAMIC_WORKAROUND_CVE_2018_3639=0 -DWORKAROUND_CVE_2022_23960=1 -DNEOVERSE_Nx_EXTERNAL_LLC=0 -DERRATA_A9_794073=0 -DERRATA_A15_816470=0 -DERRATA_A15_827671=0 -DERRATA_A17_852421=0 -DERRATA_A17_852423=0 -DERRATA_A35_855472=0 -DERRATA_A53_819472=0 -DERRATA_A53_824069=0 -DERRATA_A53_826319=0 -DERRATA_A53_827319=0 -DERRATA_A53_835769=0 -DERRATA_A53_836870=0 -DERRATA_A53_843419=0 -DERRATA_A53_855873=0 -DERRATA_A53_1530924=0 -DERRATA_A55_768277=0 -DERRATA_A55_778703=0 -DERRATA_A55_798797=0 -DERRATA_A55_846532=0 -DERRATA_A55_903758=0 -DERRATA_A55_1221012=0 -DERRATA_A55_1530923=0 -DERRATA_A57_806969=0 -DERRATA_A57_813419=0 -DERRATA_A57_813420=0 -DERRATA_A57_814670=0 -DERRATA_A57_817169=0 -DERRATA_A57_826974=0 -DERRATA_A57_826977=0 -DERRATA_A57_828024=0 -DERRATA_A57_829520=0 -DERRATA_A57_833471=0 -DERRATA_A57_859972=0 -DERRATA_A57_1319537=0 -DERRATA_A72_859971=0 -DERRATA_A72_1319367=0 -DERRATA_A73_852427=0 -DERRATA_A73_855423=0 -DERRATA_A75_764081=0 -DERRATA_A75_790748=0 -DERRATA_A76_1073348=0 -DERRATA_A76_1130799=0 -DERRATA_A76_1220197=0 -DERRATA_A76_1257314=0 -DERRATA_A76_1262606=0 -DERRATA_A76_1262888=0 -DERRATA_A76_1275112=0 -DERRATA_A76_1286807=0 -DERRATA_A76_1791580=0 -DERRATA_A76_1165522=0 -DERRATA_A76_1868343=0 -DERRATA_A76_1946160=0 -DERRATA_A77_1508412=0 -DERRATA_A77_1925769=0 -DERRATA_A77_1946167=0 -DERRATA_A77_1791578=0 -DERRATA_A77_2356587=0 -DERRATA_A78_1688305=0 -DERRATA_A78_1941498=0 -DERRATA_A78_1951500=0 -DERRATA_A78_1821534=0 -DERRATA_A78_1952683=0 -DERRATA_A78_2132060=0 -DERRATA_A78_2242635=0 -DERRATA_A78_2376745=0 -DERRATA_A78_2395406=0 -DERRATA_A78_AE_1941500=0 -DERRATA_A78_AE_1951502=0 -DERRATA_A78_AE_2376748=0 -DERRATA_A78_AE_2395408=0 -DERRATA_A78C_2132064=0 -DERRATA_A78C_2242638=0 -DERRATA_X1_1821534=0 -DERRATA_X1_1688305=0 -DERRATA_X1_1827429=0 -DERRATA_N1_1043202=0 -DERRATA_N1_1073348=0 -DERRATA_N1_1130799=0 -DERRATA_N1_1165347=0 -DERRATA_N1_1207823=0 -DERRATA_N1_1220197=0 -DERRATA_N1_1257314=0 -DERRATA_N1_1262606=0 -DERRATA_N1_1262888=0 -DERRATA_N1_1275112=0 -DERRATA_N1_1315703=0 -DERRATA_N1_1542419=0 -DERRATA_N1_1868343=0 -DERRATA_N1_1946160=0 -DERRATA_N2_2002655=0 -DERRATA_V1_1774420=0 -DERRATA_V1_1791573=0 -DERRATA_V1_1852267=0 -DERRATA_V1_1925756=0 -DERRATA_V1_1940577=0 -DERRATA_V1_1966096=0 -DERRATA_V1_2139242=0 -DERRATA_V1_2108267=0 -DERRATA_V1_2216392=0 -DERRATA_V1_2294912=0 -DERRATA_V1_2372203=0 -DERRATA_A710_1987031=0 -DERRATA_A710_2081180=0 -DERRATA_A710_2083908=0 -DERRATA_A710_2058056=0 -DERRATA_A710_2055002=0 -DERRATA_A710_2017096=0 -DERRATA_A710_2267065=0 -DERRATA_A710_2136059=0 -DERRATA_A710_2282622=0 -DERRATA_A710_2008768=0 -DERRATA_A710_2371105=0 -DERRATA_N2_2067956=0 -DERRATA_N2_2025414=0 -DERRATA_N2_2189731=0 -DERRATA_N2_2138956=0 -DERRATA_N2_2138953=0 -DERRATA_N2_2242415=0 -DERRATA_N2_2138958=0 -DERRATA_N2_2242400=0 -DERRATA_N2_2280757=0 -DERRATA_N2_2388450=0 -DERRATA_X2_2002765=0 -DERRATA_X2_2058056=0 -DERRATA_X2_2083908=0 -DERRATA_X2_2017096=0 -DERRATA_X2_2081180=0 -DERRATA_X2_2216384=0 -DERRATA_X2_2147715=0 -DERRATA_X2_2371105=0 -DERRATA_A510_1922240=0 -DERRATA_A510_2288014=0 -DERRATA_A510_2042739=0 -DERRATA_A510_2041909=0 -DERRATA_A510_2250311=0 -DERRATA_A510_2218950=0 -DERRATA_A510_2172148=0 -DERRATA_DSU_798953=0 -DERRATA_DSU_936184=0 -DERRATA_DSU_2313941=0 -DSTACK_PROTECTOR_ENABLED=0 -DCRASH_REPORTING=1 -DEL3_EXCEPTION_HANDLING=0 -DSDEI_SUPPORT=0 -DALLOW_RO_XLAT_TABLES=0 -DAMU_RESTRICT_COUNTERS=0 -DARM_ARCH_MAJOR=8 -DARM_ARCH_MINOR=0 -DARM_IO_IN_DTB=0 -DBL2_AT_EL3=0 -DBL2_ENABLE_SP_LOAD=0 -DBL2_INV_DCACHE=1 -DBL2_IN_XIP_MEM=0 -DCOLD_BOOT_SINGLE_CPU=0 -DCOT_DESC_IN_DTB=0 -DCRYPTO_SUPPORT=0 -DCTX_INCLUDE_AARCH32_REGS=0 -DCTX_INCLUDE_EL2_REGS=0 -DCTX_INCLUDE_FPREGS=1 -DCTX_INCLUDE_MTE_REGS=0 -DCTX_INCLUDE_NEVE_REGS=0 -DCTX_INCLUDE_PAUTH_REGS=0 -DDECRYPTION_SUPPORT_none -DDISABLE_MTPMU=0 -DDRTM_SUPPORT=0 -DEL3_EXCEPTION_HANDLING=0 -DENABLE_AMU=1 -DENABLE_AMU_AUXILIARY_COUNTERS=0 -DENABLE_AMU_FCONF=0 -DENABLE_ASSERTIONS=1 -DENABLE_BRBE_FOR_NS=0 -DENABLE_BTI=0 -DENABLE_FEAT_AMUv1=0 -DENABLE_FEAT_AMUv1p1=0 -DENABLE_FEAT_CSV2_2=0 -DENABLE_FEAT_DIT=0 -DENABLE_FEAT_ECV=0 -DENABLE_FEAT_FGT=0 -DENABLE_FEAT_HCX=0 -DENABLE_FEAT_PAN=0 -DENABLE_FEAT_RNG=0 -DENABLE_FEAT_SB=0 -DENABLE_FEAT_SEL2=0 -DENABLE_FEAT_TWED=0 -DENABLE_FEAT_VHE=0 -DENABLE_MPAM_FOR_LOWER_ELS=0 -DENABLE_MPMM=0 -DENABLE_MPMM_FCONF=0 -DENABLE_PAUTH=0 -DENABLE_PIE=1 -DENABLE_PMF=1 -DENABLE_PSCI_STAT=1 -DENABLE_RME=0 -DENABLE_RUNTIME_INSTRUMENTATION=0 -DENABLE_SME_FOR_NS=0 -DENABLE_SME_FOR_SWD=0 -DENABLE_SPE_FOR_LOWER_ELS=1 -DENABLE_SVE_FOR_NS=0 -DENABLE_SVE_FOR_SWD=0 -DENABLE_SYS_REG_TRACE_FOR_NS=0 -DENABLE_TRBE_FOR_NS=0 -DENABLE_TRF_FOR_NS=0 -DENCRYPT_BL31=0 -DENCRYPT_BL32=0 -DERRATA_SPECULATIVE_AT=0 -DERROR_DEPRECATED=0 -DFAULT_INJECTION_SUPPORT=0 -DFEATURE_DETECTION=0 -DGICV2_G0_FOR_EL3=0 -DHANDLE_EA_EL3_FIRST=0 -DHW_ASSISTED_COHERENCY=1 -DLOG_LEVEL=40 -DMEASURED_BOOT=0 -DNR_OF_FW_BANKS=2 -DNR_OF_IMAGES_IN_FW_BANK=1 -DNS_TIMER_SWITCH=0 -DPL011_GENERIC_UART=0 -DPLAT_RSS_NOT_SUPPORTED=0 -DPLAT_rdfremont -DPROGRAMMABLE_RESET_ADDRESS=0 -DPSA_FWU_SUPPORT=0 -DPSCI_EXTENDED_STATE_ID=1 -DRAS_EXTENSION=0 -DRAS_TRAP_LOWER_EL_ERR_ACCESS=0 -DRECLAIM_INIT_CODE=0 -DRESET_TO_BL31=1 -DRESET_TO_BL31_WITH_PARAMS=0 -DSDEI_IN_FCONF=0 -DSEC_INT_DESC_IN_FCONF=0 -DSEPARATE_BL2_NOLOAD_REGION=0 -DSEPARATE_CODE_AND_RODATA=1 -DSEPARATE_NOBITS_REGION=0 -DSIMICS_BUILD=0 -DSPD_none -DSPIN_ON_BL1_EXIT=0 -DSPMC_AT_EL3=0 -DSPMD_SPM_AT_SEL2=1 -DSPM_MM=0 -DTRNG_SUPPORT=0 -DTRUSTED_BOARD_BOOT=0 -DTWED_DELAY=0 -DUSE_COHERENT_MEM=0 -DUSE_DEBUGFS=0 -DUSE_ROMLIB=0 -DUSE_SP804_TIMER=0 -DUSE_SPINLOCK_CAS=0 -DUSE_TBBR_DEFS=1 -DWARMBOOT_ENABLE_DCACHE_EARLY=0 -DPRELOADED_BL33_BASE=0xE0000000

LD_FLAGS = --fatal-warnings -O1 --gc-sections --no-warn-rwx-segments

#BL31 variables
BL31_OBJ_DIR = ${RDINFRA}/tf-a/build/rdfremont/debug/bl31/

LINKERFILE = ${RDINFRA}/tf-a/build/rdfremont/debug/bl31/bl31.ld

BL31_LINKERFILE = ${RDINFRA}/tf-a/build/rdfremont/debug/bl31/bl31/bl31.ld

BL_CPPFLAGS = -DIMAGE_AT_EL3 -DIMAGE_BL31

BL_LDFLAGS = -pie --no-dynamic-linker

LINKER_FLAGS = -march=armv8-a -mgeneral-regs-only -mstrict-align -P -x assembler-with-cpp -D__LINKER__

BL31_MAP = ${RDINFRA}/tf-a/build/rdfremont/debug/bl31/bl31.map

BL31_ELF = ${RDINFRA}/tf-a/build/rdfremont/debug/bl31/bl31_new.elf

BL31_BIN = ${RDINFRA}/tf-a/build/rdfremont/debug/bl31/bl31_new.bin

BL31_DUMP = ${RDINFRA}/tf-a/build/rdfremont/debug/bl31/bl31_new.dump

