################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/CCS5.4/ccsv5/tools/compiler/arm_5.0.4

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./source/startup_ccs.obj" \
"./source/common.obj" \
"./source/TestMain.obj" \
"./source/TIMER0.obj" \
"./source/ST7735.obj" \
"./source/S25FL512S.obj" \
"./source/RS232.obj" \
"./source/PWM.obj" \
"./source/PLL.obj" \
"./source/Nokia5110.obj" \
"./source/ADC.obj" \
-l"libc.a" \
-l"C:\ti\TivaWare_C_Series-2.1.2.111\driverlib\ccs\Debug\driverlib.lib" \
"../tm4c1231c3pm.cmd" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include source/subdir_vars.mk
-include subdir_rules.mk
-include source/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Ezio.out

# Tool invocations
Ezio.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"D:/CCS5.4/ccsv5/tools/compiler/arm_5.0.4/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 --diag_warning=225 --display_error_number --diag_wrap=off -z --stack_size=512 -m"Ezio.map" --heap_size=0 -i"D:/CCS5.4/ccsv5/tools/compiler/arm_5.0.4/lib" -i"D:/CCS5.4/ccsv5/tools/compiler/arm_5.0.4/include" --reread_libs --warn_sections --display_error_number --diag_wrap=off --xml_link_info="Ezio_linkInfo.xml" --rom_model -o "Ezio.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(TMS470_EXECUTABLE_OUTPUTS__QUOTED) "Ezio.out"
	-$(RM) "source\ADC.pp" "source\Nokia5110.pp" "source\PLL.pp" "source\PWM.pp" "source\RS232.pp" "source\S25FL512S.pp" "source\ST7735.pp" "source\TIMER0.pp" "source\TestMain.pp" "source\common.pp" "source\startup_ccs.pp" 
	-$(RM) "source\ADC.obj" "source\Nokia5110.obj" "source\PLL.obj" "source\PWM.obj" "source\RS232.obj" "source\S25FL512S.obj" "source\ST7735.obj" "source\TIMER0.obj" "source\TestMain.obj" "source\common.obj" "source\startup_ccs.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
