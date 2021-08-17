################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Libraries/%.obj: ../Libraries/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"C:/Program Files (x86)/Texas Instruments/C6000 Code Generation Tools 7.3.4/bin/cl6x" -mv64+ -O2 --define=c6454 --include_path="C:/Program Files (x86)/Texas Instruments/C6000 Code Generation Tools 7.3.4/include" --include_path="../../TI-Header/csl_c6455/inc" --include_path="../../TI-Header/csl_c64xplus_intc/inc" --display_error_number --diag_warning=225 --abi=coffabi --preproc_with_compile --preproc_dependency="Libraries/$(basename $(<F)).d_raw" --obj_directory="Libraries" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


