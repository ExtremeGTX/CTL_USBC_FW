/**
******************************************************************************
* @file    at_cmds.h
* @authors  Mudit Sharma, John Graham
* @contact  mudit.sharma@codethink.co.uk, john.graham@codethink.co.uk
* @brief   Source file for AT command handler for USB switch.
******************************************************************************
* Ⓒ  Codethink Limited
*
******************************************************************************
*/

#include "at_cmds.h"
#include "main.h"

// Delay between USB port switching
#define DELAY_BW_SWITCH 500 // ms

const char *FIRMWARE_VERSION = "1.0";
const char *HARDWARE_VERSION = "Rev-B";
uint8_t selected_port = AT_PORTS_DISABLED;

at_cmds_e parse_input(const char *rx_data)
{

    if (!strcmp(rx_data, "AT"))
    {
        return AT_TEST;
    }

    // Does the received string starts with AT?
    if (strncmp(rx_data, "AT", 2) != 0) // If first 2 char != 'AT'
    {
        // return unknown if not
        return AT_UNKNOWN_CMD;
    }

    // if first 2 bytes are "AT"
    else
    {
        // does it have a '+' after?
        if (rx_data[2] == '+')
        {
            // is the '+' followed by "GMR"?
            if ((strncmp(&rx_data[3], "GMR", 3) == 0) || (strncmp(&rx_data[3], "GMR", 3) == 0))
            {
                return AT_INFO;
            }
            // is the '+' followed by "PORT"?
            else if (strncmp(&rx_data[3], "PORT", 4) == 0)
            {
                // is "PORT" followed by '='?
                if (rx_data[7] == '=')
                {
                    if (rx_data[8] == '0')
                    {
                        return AT_PORTS_DISABLED;
                    }
                    else if (rx_data[8] == '1')
                    {
                        return AT_PORT_A_SOURCE_C;
                    }
                    else if (rx_data[8] == '2')
                    {
                        return AT_PORT_A_SOURCE_B;
                    }
                    else if (rx_data[8] == '3')
                    {
                        return AT_PORT_C_SOURCE_A;
                    }
                    else if (rx_data[8] == '4')
                    {
                        return AT_PORT_B_SOURCE_A;
                    }
                    else if (rx_data[8] == '?')
                    {
                        return AT_PORT_NUMBERS_QUERY;
                    }
                    else
                    {
                        return AT_PORT_CMD_ERROR;
                    }
                }
                else if (rx_data[7] == '?')
                {
                    return AT_PORT_SEL;
                }
            }
            // if not followed by "PORT" its unknown
            else
            {
                return AT_UNKNOWN_CMD;
            }
        }
        // if input string starts with "AT" but it is not followed by anything
        else
        {
            return AT_UNKNOWN_CMD;
        }
    }
    // For any other unknown case
    // Note: Code not never reach here as all cases should be handled above
    return AT_UNKNOWN_CMD;
}

void process_input(const char *input)
{
    switch (parse_input(input))
    {
    case AT_TEST:
        printf("\rOK\r\n");
        break;

    case AT_INFO:
        printf("Codethink USB C switch\n\rHW:%s\r\nFW:V%s\r\n", HARDWARE_VERSION, FIRMWARE_VERSION);
        break;

    case AT_PORTS_DISABLED:
        port_switch_control(PORTS_DISABLED, PORTS_OFF);
        selected_port = AT_PORTS_DISABLED;
        printf("\rOK\r\n");
        break;

    case AT_PORT_A_SOURCE_B:
        port_switch_control(PORTS_DISABLED, PORTS_OFF);
        HAL_Delay(DELAY_BW_SWITCH);
        port_switch_control(PORT_D_EN, PORT_SINK);
        selected_port = AT_PORT_A_SOURCE_B;
        printf("\rOK\r\n");
        break;

    case AT_PORT_A_SOURCE_C:
        port_switch_control(PORTS_DISABLED, PORTS_OFF);
        HAL_Delay(DELAY_BW_SWITCH);
        port_switch_control(PORT_C_EN, PORT_SINK);
        selected_port = AT_PORT_A_SOURCE_C;
        printf("\rOK\r\n");
        break;

    case AT_PORT_B_SOURCE_A:
        port_switch_control(PORTS_DISABLED, PORTS_OFF);
        HAL_Delay(DELAY_BW_SWITCH);
        port_switch_control(PORT_D_EN, PORT_SOURCE);
        selected_port = AT_PORT_B_SOURCE_A;
        printf("\rOK\r\n");
        break;

    case AT_PORT_C_SOURCE_A:
        port_switch_control(PORTS_DISABLED, PORTS_OFF);
        HAL_Delay(DELAY_BW_SWITCH);
        port_switch_control(PORT_C_EN, PORT_SOURCE);
        selected_port = AT_PORT_C_SOURCE_A;
        printf("\rOK\r\n");
        break;

    case AT_PORT_SEL:
        if (selected_port == AT_PORT_A_SOURCE_B)
        {
            printf("\rA source B sink\r\n");
        }
        else if (selected_port == AT_PORT_A_SOURCE_C)
        {
            printf("\rA source C sink\r\n");
        }
        else if (selected_port == AT_PORT_B_SOURCE_A)
        {
            printf("\rB source A sink\r\n");
        }
        else if (selected_port == AT_PORT_C_SOURCE_A)
        {
            printf("\rC source A sink\r\n");
        }
        else
        {
            printf("\rPORTS DISABLED\r\n");
        }
        break;

    case AT_PORT_NUMBERS_QUERY:
        printf("\r2\r\nOK\r\n");
        break;

    case AT_PORT_CMD_ERROR:
        printf("\rERROR: ENTER CORRECT PORT NUMBER\r\n");
        break;

    case AT_UNKNOWN_CMD:
        port_switch_control(PORTS_DISABLED, PORTS_OFF);
        printf("\rERROR: UNKNOWN COMMAND\r\n");
        break;

    default:
        break;
    }
}
