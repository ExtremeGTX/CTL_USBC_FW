/**
******************************************************************************
* @file    at_cmds.h
* @authors  Mudit Sharma, John Graham
* @contact  mudit.sharma@codethink.co.uk, john.graham@codethink.co.uk
* @brief   Source file for AT command handler for USB switch.
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

#include "at_cmds.h"
#include "board_config.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "switch_ctrl.h"
#include "usbd_desc.h"
#include <stdint.h>

const char *FIRMWARE_VERSION = "2.0";
const char *HARDWARE_VERSION = USBD_PRODUCT_STRING_FS;
at_cmds_e selected_port = AT_PORTS_DISABLED;

at_cmds_e parse_input(const char *rx_data) {

  const size_t rx_len = strlen(rx_data);

  if (!strcmp(rx_data, "AT")) {
    return AT_TEST;
  }

  // Does the received string starts with AT?
  if (strncmp(rx_data, "AT", 2) != 0)
  {
    // return unknown if not
    return AT_UNKNOWN_CMD;
  }

  // if first 2 bytes are "AT"
  else {
    // does it have a '+' after?
    if (rx_data[2] == '+') {
      // is the '+' followed by "GMR"?
      if ((strncmp(&rx_data[3], "GMR", 3) == 0)) {
        return (rx_len == 6U) ? AT_INFO : AT_UNKNOWN_CMD;
      }
      // is the '+' followed by "PORT"?
      else if (strncmp(&rx_data[3], "PORT", 4) == 0) {
        if (rx_len < 8U) {
          return AT_PORT_CMD_ERROR;
        }

        // is "PORT" followed by '='?
        if (rx_data[7] == '=' && strlen(rx_data) > 8) {
          char *mode_char = NULL;
          int8_t mode_num = -1; // init with -1 to prevent false
          mode_num = strtol(&rx_data[8], &mode_char, 10);
          if (*mode_char == '?') {
            return AT_PORT_NUMBERS_QUERY;
          } else if (*mode_char != '\0') {
            return AT_PORT_CMD_ERROR;
          } else {
            if (mode_num == 0) {
              return AT_PORTS_DISABLED;
            } else if (mode_num == 1) {
              return AT_PORT_A_C_ON;
            } else if (mode_num == 2) {
              return AT_PORT_A_B_ON;
            } else if (mode_num == 3) {
              return AT_PORT_C_SOURCE_A;
            } else if (mode_num == 4) {
              return AT_PORT_B_SOURCE_A;
            } else if (mode_num == 5) {
              return AT_PORT_A_SOURCE_C;
            } else if (mode_num == 6) {
              return AT_PORT_A_SOURCE_B;
            } else if (mode_num == 7) {
              return AT_PORT_A_B_C_ON;
            } else {
              return AT_PORT_CMD_ERROR;
            }
          }
        } else if (rx_data[7] == '?') {
          return (rx_len == 8U) ? AT_PORT_SEL : AT_PORT_CMD_ERROR;
        }

        return AT_PORT_CMD_ERROR;
      }
      else if (strncmp(&rx_data[3], "ID", 2) == 0) {
        if (rx_len < 6U) {
          return AT_BOARD_ID_CMD_ERROR;
        }

        if ((rx_data[5] == '?') && (rx_len == 6U)) {
          return AT_BOARD_ID_QUERY;
        } else if (rx_data[5] == '=') {
          return BoardConfig_IsValidBoardId(&rx_data[6]) ? AT_BOARD_ID_SET
                                                        : AT_BOARD_ID_CMD_ERROR;
        } else {
          return AT_BOARD_ID_CMD_ERROR;
        }
      }
      // if not followed by "PORT" its unknown
      else {
        return AT_UNKNOWN_CMD;
      }
    }
    // if input string starts with "AT" but it is not followed by anything
    else {
      return AT_UNKNOWN_CMD;
    }
  }
  // For any other unknown case
  // Note: Code should never reach here as all cases should be handled above
  return AT_UNKNOWN_CMD;
}

void process_input(const char *input) {
  at_cmds_e parsed_input = parse_input(input);
  switch (parsed_input) {
  case AT_TEST:
    printf("\rOK\r\n");
    break;

  case AT_INFO:
    printf("Codethink USB C switch\n\rHW:%s\r\nFW:V%s\r\nID:%s\r\n",
           HARDWARE_VERSION, FIRMWARE_VERSION, BoardConfig_GetBoardId());
    break;

  case AT_PORTS_DISABLED:
    port_switch_disable(PORTS_OFF);
    selected_port = parsed_input;
    printf("\rOK\r\n");
    break;

  case AT_PORT_A_SOURCE_B:
    port_switch_enable(PORT_B_EN, PORT_SOURCE);
    selected_port = parsed_input;
    printf("\rOK\r\n");
    break;

  case AT_PORT_A_SOURCE_C:
    port_switch_enable(PORT_C_EN, PORT_SOURCE);
    selected_port = parsed_input;
    printf("\rOK\r\n");
    break;

  case AT_PORT_B_SOURCE_A:
    port_switch_enable(PORT_B_EN, PORT_SINK);
    selected_port = parsed_input;
    printf("\rOK\r\n");
    break;

  case AT_PORT_C_SOURCE_A:
    port_switch_enable(PORT_C_EN, PORT_SINK);
    selected_port = parsed_input;
    printf("\rOK\r\n");
    break;

  case AT_PORT_A_B_ON:
    port_switch_enable(PORT_B_EN, PORT_BI_DIR);
    selected_port = parsed_input;
    printf("\rOK\r\n");
    break;

  case AT_PORT_A_C_ON:
    port_switch_enable(PORT_C_EN, PORT_BI_DIR);
    selected_port = parsed_input;
    printf("\rOK\r\n");
    break;

  case AT_PORT_A_B_C_ON:
    port_switch_disable(BOTH_PORT_POWER_ON);
    selected_port = parsed_input;
    printf("\rOK\r\n");
    break;

  case AT_PORT_SEL:
    if (selected_port == AT_PORT_A_SOURCE_B) {
      printf("\rA source B sink\r\n");
    } else if (selected_port == AT_PORT_A_SOURCE_C) {
      printf("\rA source C sink\r\n");
    } else if (selected_port == AT_PORT_B_SOURCE_A) {
      printf("\rB source A sink\r\n");
    } else if (selected_port == AT_PORT_C_SOURCE_A) {
      printf("\rC source A sink\r\n");
    } else if (selected_port == AT_PORT_A_C_ON) {
      printf("\rBi-Dir A - C\r\n");
    } else if (selected_port == AT_PORT_A_B_ON) {
      printf("\rBi-Dir A - B\r\n");
    } else if (selected_port == AT_PORT_A_B_C_ON) {
      printf("\rAll ports power on\r\n");
    } else {
      printf("\rPORTS DISABLED\r\n");
    }
    break;

  case AT_PORT_NUMBERS_QUERY:
    printf("\r2\r\nOK\r\n");
    break;

  case AT_BOARD_ID_SET:
    if (BoardConfig_SetBoardId(&input[6])) {
      printf("\rOK\r\n");
    } else {
      printf("\rERROR: ID WRITE FAILED\r\n");
    }
    break;

  case AT_BOARD_ID_QUERY:
    printf("\r%s\r\nOK\r\n", BoardConfig_GetBoardId());
    break;

  case AT_BOARD_ID_CMD_ERROR:
    printf("\rERROR: ID MUST BE 1-32 CHARS [A-Z,a-z,0-9,_,-]\r\n");
    break;

  case AT_PORT_CMD_ERROR:
    printf("\rERROR: ENTER CORRECT PORT NUMBER\r\n");
    break;

  case AT_UNKNOWN_CMD:
    port_switch_disable(PORTS_OFF);
    selected_port = AT_UNKNOWN_CMD;
    printf("\rERROR: UNKNOWN COMMAND\r\n");
    break;

  default:
    break;
  }
}
