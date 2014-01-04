################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include USB/MIDIstream_class/subdir.mk
-include USB/subdir.mk
-include System/subdir.mk
-include Synth/subdir.mk
-include Lib/STM32_USB_OTG_Driver/src/subdir.mk
-include Lib/STM32_USB_HOST_Library/Core/src/subdir.mk
-include Lib/STM32F4xx_StdPeriph_Driver/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
Dekrispator.hex \

SECONDARY_LIST += \
Dekrispator.lst \

SECONDARY_SIZE += \
Dekrispator.siz \


# All Target
all: Dekrispator.elf secondary-outputs

# Tool invocations
Dekrispator.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fsingle-precision-constant -Wall -T "C:\xav_dev\eclipse_kepler\Dekrispator\System\stm32f407xav.ld" -nostartfiles -Xlinker --gc-sections -Wl,-Map,"Dekrispator.map" -flto -o "Dekrispator.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Dekrispator.hex: Dekrispator.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy "Dekrispator.elf" -O ihex  "Dekrispator.hex"
	@echo 'Finished building: $@'
	@echo ' '

Dekrispator.lst: Dekrispator.elf
	@echo 'Invoking: Cross ARM GNU Create Listing'
	arm-none-eabi-objdump "Dekrispator.elf" --source --all-headers --demangle --line-numbers --wide > "Dekrispator.lst"
	@echo 'Finished building: $@'
	@echo ' '

Dekrispator.siz: Dekrispator.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size  --format=berkeley --totals "Dekrispator.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(S_UPPER_DEPS) Dekrispator.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
