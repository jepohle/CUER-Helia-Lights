################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/DSP/Examples/ARM/arm_fir_example/arm_fir_data.c \
../Drivers/CMSIS/DSP/Examples/ARM/arm_fir_example/arm_fir_example_f32.c \
../Drivers/CMSIS/DSP/Examples/ARM/arm_fir_example/math_helper.c 

OBJS += \
./Drivers/CMSIS/DSP/Examples/ARM/arm_fir_example/arm_fir_data.o \
./Drivers/CMSIS/DSP/Examples/ARM/arm_fir_example/arm_fir_example_f32.o \
./Drivers/CMSIS/DSP/Examples/ARM/arm_fir_example/math_helper.o 

C_DEPS += \
./Drivers/CMSIS/DSP/Examples/ARM/arm_fir_example/arm_fir_data.d \
./Drivers/CMSIS/DSP/Examples/ARM/arm_fir_example/arm_fir_example_f32.d \
./Drivers/CMSIS/DSP/Examples/ARM/arm_fir_example/math_helper.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/DSP/Examples/ARM/arm_fir_example/%.o Drivers/CMSIS/DSP/Examples/ARM/arm_fir_example/%.su: ../Drivers/CMSIS/DSP/Examples/ARM/arm_fir_example/%.c Drivers/CMSIS/DSP/Examples/ARM/arm_fir_example/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303x8 -c -I../../Core/Inc -I../../Drivers/STM32F3xx_HAL_Driver/Inc -I../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-DSP-2f-Examples-2f-ARM-2f-arm_fir_example

clean-Drivers-2f-CMSIS-2f-DSP-2f-Examples-2f-ARM-2f-arm_fir_example:
	-$(RM) ./Drivers/CMSIS/DSP/Examples/ARM/arm_fir_example/arm_fir_data.d ./Drivers/CMSIS/DSP/Examples/ARM/arm_fir_example/arm_fir_data.o ./Drivers/CMSIS/DSP/Examples/ARM/arm_fir_example/arm_fir_data.su ./Drivers/CMSIS/DSP/Examples/ARM/arm_fir_example/arm_fir_example_f32.d ./Drivers/CMSIS/DSP/Examples/ARM/arm_fir_example/arm_fir_example_f32.o ./Drivers/CMSIS/DSP/Examples/ARM/arm_fir_example/arm_fir_example_f32.su ./Drivers/CMSIS/DSP/Examples/ARM/arm_fir_example/math_helper.d ./Drivers/CMSIS/DSP/Examples/ARM/arm_fir_example/math_helper.o ./Drivers/CMSIS/DSP/Examples/ARM/arm_fir_example/math_helper.su

.PHONY: clean-Drivers-2f-CMSIS-2f-DSP-2f-Examples-2f-ARM-2f-arm_fir_example

