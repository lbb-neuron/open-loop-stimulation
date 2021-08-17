################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/Program Files (x86)/Texas Instruments/C6000 Code Generation Tools 7.3.4

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./DSP.obj" \
"./Device_lib.obj" \
"./global.obj" \
"./intvecs.obj" \
"./irq.obj" \
"./main.obj" \
"./Libraries/led.obj" \
"./Libraries/pattern_circle.obj" \
"./Libraries/pattern_full.obj" \
"./Libraries/stim_pattern.obj" \
"./Libraries/stimulation.obj" \
"./Libraries/time.obj" \
"../c6454.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include Libraries/subdir_vars.mk
-include subdir_rules.mk
-include Libraries/subdir_rules.mk
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
Open_Loop_mini.out \

EXE_OUTPUTS__QUOTED += \
"Open_Loop_mini.out" \


# All Target
all: Open_Loop_mini.out

# Tool invocations
Open_Loop_mini.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C6000 Linker'
	"C:/Program Files (x86)/Texas Instruments/C6000 Code Generation Tools 7.3.4/bin/cl6x" -mv64+ -O2 --define=c6454 --display_error_number --diag_warning=225 --abi=coffabi -z -m"Open_Loop_mini.map" --stack_size=0x800 --heap_size=0x800 --warn_sections --display_error_number -i"C:/Program Files (x86)/Texas Instruments/C6000 Code Generation Tools 7.3.4/lib" -i"C:/Program Files (x86)/Texas Instruments/C6000 Code Generation Tools 7.3.4/include" --reread_libs --xml_link_info="Open_Loop_mini_linkInfo.xml" --rom_model -o "Open_Loop_mini.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "DSP.obj" "Device_lib.obj" "global.obj" "intvecs.obj" "irq.obj" "main.obj" "Libraries\led.obj" "Libraries\pattern_circle.obj" "Libraries\pattern_full.obj" "Libraries\stim_pattern.obj" "Libraries\stimulation.obj" "Libraries\time.obj" 
	-$(RM) "DSP.d" "Device_lib.d" "global.d" "irq.d" "main.d" "Libraries\led.d" "Libraries\pattern_circle.d" "Libraries\pattern_full.d" "Libraries\stim_pattern.d" "Libraries\stimulation.d" "Libraries\time.d" 
	-$(RM) "intvecs.d" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-"C:/Users/MEA2100/Documents/Stephan/DSP/Open_Loop_mini/postbuild.bat" "C:/Program Files (x86)/Texas Instruments/C6000 Code Generation Tools 7.3.4/bin/hex6x.exe" "Open_Loop_mini.out" "Open_Loop_mini"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

