# USB Switch Rev-A firmware

## About

Firmware for the USB Switch rev-A written using STM's toolchian
## How to use

Blue pill is configured to be a USB CDC device. Send the following command using a serial terminal to operate the switch.

| Command | DEV 1 | DEV 2 |
|---------|-------|-------|
| 1       | 1     | 0     |
| 2       | 0     | 1     |
| 0       | 0     | 0     |

## How to build

Make sure you have *gcc-arm-none-eabi* compiler installed on your system.

Run the following commands to build 
```
cd Debug
make -j all 
```

## How to flash

1. Use stm32cubeProgrammer
2. run *make flash* <- Currently not working
   - Make sure you have st-utils installed https://github.com/stlink-org/stlink