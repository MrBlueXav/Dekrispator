################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../System/my_stm32f4_discovery.c \
../System/my_stm32f4_discovery_audio_codec.c \
../System/startup_stm32f4xx.c \
../System/stm32fxxx_it.c \
../System/system_stm32f4xx.c 

OBJS += \
./System/my_stm32f4_discovery.o \
./System/my_stm32f4_discovery_audio_codec.o \
./System/startup_stm32f4xx.o \
./System/stm32fxxx_it.o \
./System/system_stm32f4xx.o 

C_DEPS += \
./System/my_stm32f4_discovery.d \
./System/my_stm32f4_discovery_audio_codec.d \
./System/startup_stm32f4xx.d \
./System/stm32fxxx_it.d \
./System/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
System/%.o: ../System/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O3 -ffunction-sections -fdata-sections -fno-builtin -fsingle-precision-constant -flto -Wall -DSTM32F40_41xxx -D__FPU_USED=1 -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -DSTM32F407VG -I../Synth -I../USB -I../USB/MIDIstream_class -I../Lib/STM32F4xx_StdPeriph_Driver/inc -I../Lib/STM32_USB_OTG_Driver/inc -I../Lib/STM32_USB_HOST_Library/Core/inc -I../System -std=gnu99 -flto -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


