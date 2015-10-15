################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/_write.c \
../Src/isr_moisture.c \
../Src/logging.c \
../Src/main.c \
../Src/moistureMeasure.c \
../Src/motors.c \
../Src/printf.c \
../Src/spicomm.c \
../Src/stm32f1xx_hal_msp.c \
../Src/stm32f1xx_it.c 

S_UPPER_SRCS += \
../Src/startup_stm32f103xb.S 

OBJS += \
./Src/_write.o \
./Src/isr_moisture.o \
./Src/logging.o \
./Src/main.o \
./Src/moistureMeasure.o \
./Src/motors.o \
./Src/printf.o \
./Src/spicomm.o \
./Src/startup_stm32f103xb.o \
./Src/stm32f1xx_hal_msp.o \
./Src/stm32f1xx_it.o 

S_UPPER_DEPS += \
./Src/startup_stm32f103xb.d 

C_DEPS += \
./Src/_write.d \
./Src/isr_moisture.d \
./Src/logging.d \
./Src/main.d \
./Src/moistureMeasure.d \
./Src/motors.d \
./Src/printf.d \
./Src/spicomm.d \
./Src/stm32f1xx_hal_msp.d \
./Src/stm32f1xx_it.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra  -g -DSTM32F10X_MD -DSTR_MAX_SIZE=127 -DHSE_VALUE=8000000 -DUSE_HAL_DRIVER -DSTM32F103xB -I"../Inc" -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/STM32F1xx_HAL_Driver/Inc -std=gnu11 -Wno-unused -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/%.o: ../Src/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra  -g -x assembler-with-cpp -DSTM32F10X_MD -DHSE_VALUE=8000000 -DUSE_HAL_DRIVER -DSTM32F103xB -I"../Inc" -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/STM32F1xx_HAL_Driver/Inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


