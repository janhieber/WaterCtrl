################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../newlib/_exit.c \
../newlib/_sbrk.c \
../newlib/_startup.c \
../newlib/_syscalls.c 

OBJS += \
./newlib/_exit.o \
./newlib/_sbrk.o \
./newlib/_startup.o \
./newlib/_syscalls.o 

C_DEPS += \
./newlib/_exit.d \
./newlib/_sbrk.d \
./newlib/_startup.d \
./newlib/_syscalls.d 


# Each subdirectory must supply rules for building sources it contributes
newlib/%.o: ../newlib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DSTM32F10X_MD -DHSE_VALUE=8000000 -DUSE_HAL_DRIVER -DSTM32F103xB -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


