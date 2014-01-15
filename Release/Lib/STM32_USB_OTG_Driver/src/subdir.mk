################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Lib/STM32_USB_OTG_Driver/src/usb_core.c \
../Lib/STM32_USB_OTG_Driver/src/usb_hcd.c \
../Lib/STM32_USB_OTG_Driver/src/usb_hcd_int.c 

OBJS += \
./Lib/STM32_USB_OTG_Driver/src/usb_core.o \
./Lib/STM32_USB_OTG_Driver/src/usb_hcd.o \
./Lib/STM32_USB_OTG_Driver/src/usb_hcd_int.o 

C_DEPS += \
./Lib/STM32_USB_OTG_Driver/src/usb_core.d \
./Lib/STM32_USB_OTG_Driver/src/usb_hcd.d \
./Lib/STM32_USB_OTG_Driver/src/usb_hcd_int.d 


# Each subdirectory must supply rules for building sources it contributes
Lib/STM32_USB_OTG_Driver/src/%.o: ../Lib/STM32_USB_OTG_Driver/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O3 -ffunction-sections -fdata-sections -fno-builtin -fsingle-precision-constant -flto -Wall -DSTM32F40_41xxx -D__FPU_USED=1 -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -DSTM32F407VG -I../Synth -I../USB -I../USB/MIDIstream_class -I../Lib/STM32F4xx_StdPeriph_Driver/inc -I../Lib/STM32_USB_OTG_Driver/inc -I../Lib/STM32_USB_HOST_Library/Core/inc -I../System -std=gnu99 -flto -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


