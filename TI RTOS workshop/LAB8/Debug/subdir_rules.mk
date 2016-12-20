################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
EK_TM4C123GXL.obj: ../EK_TM4C123GXL.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/CCS6/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/Users/csxwin/workspace_v6.1.2/LAB8" --include_path="C:/ti/TivaWare_C_Series-2.1.2.111" --include_path="D:/CCS6/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b" --include_path="D:/CCS6/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages/ti/sysbios/posix" --include_path="D:/CCS6/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/include" -g --gcc --define=ccs="ccs" --define=TARGET_IS_TM4C123_RB1 --define=PART_TM4C123GH6PM --define=ccs --define=TIVAWARE --diag_wrap=off --display_error_number --diag_warning=225 --diag_warning=255 --gen_func_subsections=on --preproc_with_compile --preproc_dependency="EK_TM4C123GXL.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

empty_min.obj: ../empty_min.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/CCS6/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/Users/csxwin/workspace_v6.1.2/LAB8" --include_path="C:/ti/TivaWare_C_Series-2.1.2.111" --include_path="D:/CCS6/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b" --include_path="D:/CCS6/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages/ti/sysbios/posix" --include_path="D:/CCS6/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/include" -g --gcc --define=ccs="ccs" --define=TARGET_IS_TM4C123_RB1 --define=PART_TM4C123GH6PM --define=ccs --define=TIVAWARE --diag_wrap=off --display_error_number --diag_warning=225 --diag_warning=255 --gen_func_subsections=on --preproc_with_compile --preproc_dependency="empty_min.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

configPkg/linker.cmd: ../empty_min.cfg
	@echo 'Building file: $<'
	@echo 'Invoking: XDCtools'
	"D:/CCS6/xdctools_3_32_00_06_core/xs" --xdcpath="D:/CCS6/tirtos_tivac_2_16_01_14/packages;D:/CCS6/tirtos_tivac_2_16_01_14/products/tidrivers_tivac_2_16_01_13/packages;D:/CCS6/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages;D:/CCS6/tirtos_tivac_2_16_01_14/products/ndk_2_25_00_09/packages;D:/CCS6/tirtos_tivac_2_16_01_14/products/uia_2_00_05_50/packages;D:/CCS6/tirtos_tivac_2_16_01_14/products/ns_1_11_00_10/packages;D:/CCS6/ccsv6/ccs_base;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M4F -p ti.platforms.tiva:TM4C123GH6PM -r release -c "D:/CCS6/ccsv6/tools/compiler/ti-cgt-arm_5.2.7" --compileOptions "-mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path=\"C:/Users/csxwin/workspace_v6.1.2/LAB8\" --include_path=\"C:/ti/TivaWare_C_Series-2.1.2.111\" --include_path=\"D:/CCS6/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b\" --include_path=\"D:/CCS6/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages/ti/sysbios/posix\" --include_path=\"D:/CCS6/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/include\" -g --gcc --define=ccs=\"ccs\" --define=TARGET_IS_TM4C123_RB1 --define=PART_TM4C123GH6PM --define=ccs --define=TIVAWARE --diag_wrap=off --display_error_number --diag_warning=225 --diag_warning=255 --gen_func_subsections=on  " "$<"
	@echo 'Finished building: $<'
	@echo ' '

configPkg/compiler.opt: | configPkg/linker.cmd
configPkg/: | configPkg/linker.cmd


