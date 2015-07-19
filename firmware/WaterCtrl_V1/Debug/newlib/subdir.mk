################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../newlib/_exit.c \
../newlib/_sbrk.c \
../newlib/_startup.c \
../newlib/_syscalls.c \
../newlib/assert.c 

CPP_SRCS += \
../newlib/_cxx.cpp 

OBJS += \
./newlib/_cxx.o \
./newlib/_exit.o \
./newlib/_sbrk.o \
./newlib/_startup.o \
./newlib/_syscalls.o \
./newlib/assert.o 

C_DEPS += \
./newlib/_exit.d \
./newlib/_sbrk.d \
./newlib/_startup.d \
./newlib/_syscalls.d \
./newlib/assert.d 

CPP_DEPS += \
./newlib/_cxx.d 


# Each subdirectory must supply rules for building sources it contributes
newlib/%.o: ../newlib/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DSTM32F10X_MD -DHSE_VALUE=8000000 -DUSE_HAL_DRIVER -DSTM32F103xB -I"../Inc" -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/STM32F1xx_HAL_Driver/Inc -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

newlib/%.o: ../newlib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DSTM32F10X_MD -DHSE_VALUE=8000000 -DUSE_HAL_DRIVER -DSTM32F103xB -I"../Inc" -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/STM32F1xx_HAL_Driver/Inc -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


