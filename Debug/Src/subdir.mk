################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/Ass-02-Lib.c \
../Src/Ass-02-Q01.c \
../Src/Ass-02-Q02.c \
../Src/Ass-02.c \
../Src/fsmc.c \
../Src/gpio.c \
../Src/main.c \
../Src/spi.c \
../Src/stm32f4xx_hal_msp.c \
../Src/stm32f4xx_it.c \
../Src/syscalls.c \
../Src/system_stm32f4xx.c \
../Src/usart.c 

OBJS += \
./Src/Ass-02-Lib.o \
./Src/Ass-02-Q01.o \
./Src/Ass-02-Q02.o \
./Src/Ass-02.o \
./Src/fsmc.o \
./Src/gpio.o \
./Src/main.o \
./Src/spi.o \
./Src/stm32f4xx_hal_msp.o \
./Src/stm32f4xx_it.o \
./Src/syscalls.o \
./Src/system_stm32f4xx.o \
./Src/usart.o 

C_DEPS += \
./Src/Ass-02-Lib.d \
./Src/Ass-02-Q01.d \
./Src/Ass-02-Q02.d \
./Src/Ass-02.d \
./Src/fsmc.d \
./Src/gpio.d \
./Src/main.d \
./Src/spi.d \
./Src/stm32f4xx_hal_msp.d \
./Src/stm32f4xx_it.d \
./Src/syscalls.d \
./Src/system_stm32f4xx.d \
./Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F407xx -I"U:/ELEC3730/jordanandevansharedworkspacefolder/ELEC3730-2018s1-Ass-02-STM32-r1234/Ass-02-STM32/Inc" -I"U:/ELEC3730/jordanandevansharedworkspacefolder/ELEC3730-2018s1-Ass-02-STM32-r1234/Ass-02-STM32/Drivers/STM32F4xx_HAL_Driver/Inc" -I"U:/ELEC3730/jordanandevansharedworkspacefolder/ELEC3730-2018s1-Ass-02-STM32-r1234/Ass-02-STM32/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"U:/ELEC3730/jordanandevansharedworkspacefolder/ELEC3730-2018s1-Ass-02-STM32-r1234/Ass-02-STM32/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"U:/ELEC3730/jordanandevansharedworkspacefolder/ELEC3730-2018s1-Ass-02-STM32-r1234/Ass-02-STM32/Drivers/CMSIS/Include" -I"U:/ELEC3730/jordanandevansharedworkspacefolder/ELEC3730-2018s1-Ass-02-STM32-r1234/Ass-02-STM32/Drivers/BSP" -I"U:/ELEC3730/jordanandevansharedworkspacefolder/ELEC3730-2018s1-Ass-02-STM32-r1234/Ass-02-STM32/Drivers/Fonts" -I"U:/ELEC3730/jordanandevansharedworkspacefolder/ELEC3730-2018s1-Ass-02-STM32-r1234/Ass-02-STM32/Inc"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


