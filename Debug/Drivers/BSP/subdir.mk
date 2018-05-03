################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/ili9325.c \
../Drivers/BSP/openx07v_c_lcd.c \
../Drivers/BSP/touch_panel.c 

OBJS += \
./Drivers/BSP/ili9325.o \
./Drivers/BSP/openx07v_c_lcd.o \
./Drivers/BSP/touch_panel.o 

C_DEPS += \
./Drivers/BSP/ili9325.d \
./Drivers/BSP/openx07v_c_lcd.d \
./Drivers/BSP/touch_panel.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/%.o: ../Drivers/BSP/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F407xx -I"U:/ELEC3730/jordanandevansharedworkspacefolder/ELEC3730-2018s1-Ass-02-STM32-r1234/Ass-02-STM32/Inc" -I"U:/ELEC3730/jordanandevansharedworkspacefolder/ELEC3730-2018s1-Ass-02-STM32-r1234/Ass-02-STM32/Drivers/STM32F4xx_HAL_Driver/Inc" -I"U:/ELEC3730/jordanandevansharedworkspacefolder/ELEC3730-2018s1-Ass-02-STM32-r1234/Ass-02-STM32/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"U:/ELEC3730/jordanandevansharedworkspacefolder/ELEC3730-2018s1-Ass-02-STM32-r1234/Ass-02-STM32/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"U:/ELEC3730/jordanandevansharedworkspacefolder/ELEC3730-2018s1-Ass-02-STM32-r1234/Ass-02-STM32/Drivers/CMSIS/Include" -I"U:/ELEC3730/jordanandevansharedworkspacefolder/ELEC3730-2018s1-Ass-02-STM32-r1234/Ass-02-STM32/Drivers/BSP" -I"U:/ELEC3730/jordanandevansharedworkspacefolder/ELEC3730-2018s1-Ass-02-STM32-r1234/Ass-02-STM32/Drivers/Fonts" -I"U:/ELEC3730/jordanandevansharedworkspacefolder/ELEC3730-2018s1-Ass-02-STM32-r1234/Ass-02-STM32/Inc"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


