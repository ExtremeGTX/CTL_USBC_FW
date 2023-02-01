/**
******************************************************************************
* @file    at_cmds.h
* @authors  Mudit Sharma, John Graham
* @contact  mudit.sharma@codethink.co.uk, john.graham@codethink.co.uk
* @brief   Header file for AT command handler for USB switch.
******************************************************************************
* Ⓒ  Codethink Limited
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

// Enum defining detecting AT cmds.
typedef enum
{
  AT_ENTER,
  AT_TEST,
  AT_INFO,
  AT_PORTS_DISABLED,
  AT_PORT_A_SOURCE_B, // A SOURCE
  AT_PORT_A_SOURCE_C, // A SOURCE
  AT_PORT_B_SOURCE_A, // B SOURCE
  AT_PORT_C_SOURCE_A, // C SOURCE
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
