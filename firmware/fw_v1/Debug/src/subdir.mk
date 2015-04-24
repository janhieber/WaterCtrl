################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/flash.c \
../src/helpers.c \
../src/main.c \
../src/moistureMeasure.c \
../src/motors.c \
../src/printf.c 

OBJS += \
./src/flash.o \
./src/helpers.o \
./src/main.o \
./src/moistureMeasure.o \
./src/motors.o \
./src/printf.o 

C_DEPS += \
./src/flash.d \
./src/helpers.d \
./src/main.d \
./src/moistureMeasure.d \
./src/motors.d \
./src/printf.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DSTM32F051 -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f0-stdperiph" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


