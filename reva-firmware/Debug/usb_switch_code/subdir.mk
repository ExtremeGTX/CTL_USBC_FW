# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../usb_switch_code/at_cmds.c \
../usb_switch_code/switch_ctrl.c \
../usb_switch_code/usbd_cdc_if_ctrl.c 

OBJS += \
./usb_switch_code/at_cmds.o \
./usb_switch_code/switch_ctrl.o \
./usb_switch_code/usbd_cdc_if_ctrl.o 

C_DEPS += \
./usb_switch_code/at_cmds.d \
./usb_switch_code/switch_ctrl.d \
./usb_switch_code/usbd_cdc_if_ctrl.d 


# Each subdirectory must supply rules for building sources it contributes
usb_switch_code/%.o usb_switch_code/%.su: ../usb_switch_code/%.c usb_switch_code/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../usb_switch_code -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-usb-switch-code-App

clean-usb-switch-code-App:
	-$(RM) ./usb_switch_code/at_cmds.d ./usb_switch_code/at_cmds.o ./usb_switch_code/at_cmds.su ./usb_switch_code/switch_ctrl.d ./usb_switch_code/switch_ctrl.o ./usb_switch_code/switch_ctrl.su ./usb_switch_code/usbd_cdc_if_ctrl.d ./usb_switch_code/usbd_cdc_if_ctrl.o ./usb_switch_code/usbd_cdc_if_ctrl.su

.PHONY: clean-usb-switch-code-App