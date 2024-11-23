################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/codecomposer1260/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./main.obj" \
"./tm4c123gh6pm_startup_ccs.obj" \
"./Critical_Files/Det.obj" \
"./Dio_Driver/Dio.obj" \
"./Dio_Driver/Dio_PBcfg.obj" \
"./NVIC_Driver/NVIC.obj" \
"./Port_Driver/Port.obj" \
"./Port_Driver/Port_PBcfg.obj" \
"./SYSTICK_Driver/SYSTICK.obj" \
"./SYSTICK_Driver/SYSTICK_PBcfg.obj" \
"../tm4c123gh6pm.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include Critical_Files/subdir_vars.mk
-include Dio_Driver/subdir_vars.mk
-include NVIC_Driver/subdir_vars.mk
-include Port_Driver/subdir_vars.mk
-include SYSTICK_Driver/subdir_vars.mk
-include subdir_rules.mk
-include Critical_Files/subdir_rules.mk
-include Dio_Driver/subdir_rules.mk
-include NVIC_Driver/subdir_rules.mk
-include Port_Driver/subdir_rules.mk
-include SYSTICK_Driver/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
GP_NVIC_SYSTICK.out 

EXE_OUTPUTS__QUOTED += \
"GP_NVIC_SYSTICK.out" 


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "GP_NVIC_SYSTICK.out"

# Tool invocations
GP_NVIC_SYSTICK.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: Arm Linker'
	"C:/ti/codecomposer1260/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi -z -m"GP_NVIC_SYSTICK.map" --heap_size=0 --stack_size=512 -i"C:/ti/codecomposer1260/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/lib" -i"C:/ti/codecomposer1260/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --reread_libs --diag_wrap=off --display_error_number --warn_sections --xml_link_info="GP_NVIC_SYSTICK_linkInfo.xml" --rom_model -o "GP_NVIC_SYSTICK.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "main.obj" "tm4c123gh6pm_startup_ccs.obj" "Critical_Files\Det.obj" "Dio_Driver\Dio.obj" "Dio_Driver\Dio_PBcfg.obj" "NVIC_Driver\NVIC.obj" "Port_Driver\Port.obj" "Port_Driver\Port_PBcfg.obj" "SYSTICK_Driver\SYSTICK.obj" "SYSTICK_Driver\SYSTICK_PBcfg.obj" 
	-$(RM) "main.d" "tm4c123gh6pm_startup_ccs.d" "Critical_Files\Det.d" "Dio_Driver\Dio.d" "Dio_Driver\Dio_PBcfg.d" "NVIC_Driver\NVIC.d" "Port_Driver\Port.d" "Port_Driver\Port_PBcfg.d" "SYSTICK_Driver\SYSTICK.d" "SYSTICK_Driver\SYSTICK_PBcfg.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

