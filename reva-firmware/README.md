# USB Switch Rev-A firmware

## About

Firmware for the USB Switch written using STM's toolchian
## How to use

USB switch is configured to be a USB CDC device. Send the following command using a serial terminal to operate the switch.

| Command       | A      | B      | C      | Message             |
|---------------|--------|--------|--------|---------------------|
| "AT"          | -      | -      | -      | "OK\n"              |
| "AT+GMR\n"    | -      | -      | -      | Returns device info |
| "AT+PORT=1\n" | Source | Sink   | 0      | "OK\n"              |
| "AT+PORT=2\n" | Source | 0      | Sink   | "OK\n"              |
| "AT+PORT=3\n" | Sink   | Source | 0      | "OK\n"              |
| "AT+PORT=4\n" | Sink   | 0      | Source | "OK\n"              |
| "AT+PORT=?\n" | Sink   | Source | 0      | "\r2\r\nOK\r\n"     |
| "AT+PORT?\n"  | -      | -      | -      | Returns which ports are active<br/> *Output can be:*<br/><ul><li>"\rA source B sink\r\n"</li><li>"\rA source C sink\r\n"</li><li>"\rB source A sink\r\n"</li><li>"\rC source A sink\r\n"</li></ul>|
                                                  
### Using USB-C to USB-C cable

If you are using a USB-C to USB-C cable to connecting a peripheral to host and the peripheral does not seem to connect, try flipping the USB-C connector's orientation of the USB C cable.  

## Files to look into for USB switch code

### Code for handling AT commands
[usb_switch_code](/usb_switch_code/)

### Code for handling USB line switching
[usb_switch_code](/usb_switch_code/)

### Code for handling USB CDC communication
[usb_switch_code](usb_switch_code/)
[USB_DEVICE](/USB_DEVICE/App/)

## How to build

Make sure you have *gcc-arm-none-eabi* compiler installed on your system.

Run the following commands to build 
```
cd Debug
make -j all 
```

## How to flash

1. Use stm32cubeProgrammer to flash 
2. run *make flash* 
   - Make sure you have st-utils installed https://github.com/stlink-org/stlink

## What to install

On Debian:

```
apt install \
    gdb-multiarch \
    gcc-arm-none-eabi \
    openocd \
    stlink-tools
```
