#include "at_cmds.h"
#include "main.h"

const char *FIRMWARE_VERSION = "1.0";
const char *HARDWARE_VERSION = "Rev-B";
uint8_t selected_port = AT_PORTS_DISABLED;

at_cmds_e parse_input(char *rx_data)
{

    if (strncmp(rx_data, "AT", 2)) // If first 2 char != 'AT'
    {
        return AT_UNKNOWN_CMD;
    }
    else
    {
        if ((rx_data[2] == '\n' || rx_data[2] == '\r'))
        {
            return AT_TEST;
        }
        else if (rx_data[2] == '+')
        {
            if ((strncmp(&rx_data[3], "GMR\n", 4) == 0) || (strncmp(&rx_data[3], "GMR\r", 4) == 0))
            {
                return AT_INFO;
            }
            else if (strncmp(&rx_data[3], "PORT", 4) == 0)
            {
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
            else
            {
                return AT_UNKNOWN_CMD;
            }
        }
        else
        {
            return AT_UNKNOWN_CMD;
        }
    }
    return AT_UNKNOWN_CMD;
}

void process_input(uint8_t *input)
{
    switch (parse_input((char *)input))
    {
    case AT_TEST:
        printf("\rOK\r\n");
        local_buffer_flush(input, LOCAL_BUFFER_SIZE);
        break;

    case AT_INFO:
        printf("Codethink USB C switch\n\rHW:%s\r\nFW:V%s\r\n", HARDWARE_VERSION, FIRMWARE_VERSION);
        local_buffer_flush(input, LOCAL_BUFFER_SIZE);
        break;

    case AT_PORTS_DISABLED:
        printf("\rOK\r\n");
        port_switch_control(PORTS_DISABLED, PORTS_OFF);
        selected_port = AT_PORTS_DISABLED;
        local_buffer_flush(input, LOCAL_BUFFER_SIZE);
        break;

    case AT_PORT_A_SOURCE_B:
        printf("\rOK\r\n");
        port_switch_control(PORTS_DISABLED, PORTS_OFF);
        HAL_Delay(500);
        port_switch_control(PORT_D_EN, PORT_SINK);
        selected_port = AT_PORT_A_SOURCE_B;
        local_buffer_flush(input, LOCAL_BUFFER_SIZE);
        break;

    case AT_PORT_A_SOURCE_C:
        printf("\rOK\r\n");
        port_switch_control(PORTS_DISABLED, PORTS_OFF);
        HAL_Delay(500);
        port_switch_control(PORT_C_EN, PORT_SINK);
        selected_port = AT_PORT_A_SOURCE_C;
        local_buffer_flush(input, LOCAL_BUFFER_SIZE);
        break;

    case AT_PORT_B_SOURCE_A:
        printf("\rOK\r\n");
        port_switch_control(PORTS_DISABLED, PORTS_OFF);
        HAL_Delay(500);
        port_switch_control(PORT_D_EN, PORT_SOURCE);
        selected_port = AT_PORT_B_SOURCE_A;
        local_buffer_flush(input, LOCAL_BUFFER_SIZE);
        break;

    case AT_PORT_C_SOURCE_A:
        printf("\rOK\r\n");
        port_switch_control(PORTS_DISABLED, PORTS_OFF);
        HAL_Delay(500);
        port_switch_control(PORT_C_EN, PORT_SOURCE);
        selected_port = AT_PORT_C_SOURCE_A;
        local_buffer_flush(input, LOCAL_BUFFER_SIZE);
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
        local_buffer_flush(input, LOCAL_BUFFER_SIZE);
        break;

    case AT_PORT_NUMBERS_QUERY:
        printf("\r2\r\nOK\r\n");
        local_buffer_flush(input, LOCAL_BUFFER_SIZE);
        break;

    case AT_PORT_CMD_ERROR:
        printf("\rERROR: ENTER CORRECT PORT NUMBER\r\n");
        local_buffer_flush(input, LOCAL_BUFFER_SIZE);
        break;

    case AT_UNKNOWN_CMD:
        printf("\rERROR\r\n");
        port_switch_control(PORTS_DISABLED, PORTS_OFF);
        local_buffer_flush(input, LOCAL_BUFFER_SIZE);
        break;

    default:
        local_buffer_flush(input, LOCAL_BUFFER_SIZE);
        break;
    }
}