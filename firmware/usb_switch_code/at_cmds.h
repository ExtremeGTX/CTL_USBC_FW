/**
******************************************************************************
* @file    at_cmds.h
* @authors  Mudit Sharma, John Graham
* @contact  mudit.sharma@codethink.co.uk, john.graham@codethink.co.uk
* @brief   Header file for AT command handler for USB switch.
******************************************************************************
* MIT License
*
* Copyright (c) 2023 Codethink Limited
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
******************************************************************************
*/

#ifndef _AT_CMDS_
#define _AT_CMDS_

#include "main.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "usb_device.h"
#include "gpio.h"
#include "stdio.h"
#include "usbd_cdc_if.h"
#include "switch_ctrl.h"
#include <stdlib.h>

// Enum defining detecting AT cmds.
typedef enum
{
  AT_TEST,
  AT_INFO,
  AT_PORTS_DISABLED,
  AT_PORT_A_SOURCE_B, // A SOURCE
  AT_PORT_A_SOURCE_C, // A SOURCE
  AT_PORT_B_SOURCE_A, // B SOURCE
  AT_PORT_C_SOURCE_A, // C SOURCE
  AT_PORT_A_B_ON,
  AT_PORT_A_C_ON,
  AT_PORT_A_B_C_ON,
  AT_PORT_NUMBERS_QUERY,
  AT_PORT_SEL,
  AT_PORT_CMD_ERROR,
  AT_UNKNOWN_CMD,
} at_cmds_e;

/**
 * @brief Prase input buffer data
 *
 * @param char *rx_data: receive buffer to pass
 * @return at_cmd_e: Recognized AT cmd
 */
at_cmds_e parse_input(const char *rx_data);

/**
 * @brief Process received data and respond to recognized
 * AT commands.
 */
void process_input(const char *input);

#endif
