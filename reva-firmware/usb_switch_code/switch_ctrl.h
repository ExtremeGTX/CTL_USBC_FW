/**
******************************************************************************
* @file    switch_ctrl.h
* @authors  Mudit Sharma
* @contact  mudit.sharma@codethink.co.uk
* @brief   Header file for switch control handler
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

#ifndef _SWITCH_CTRL_H_
#define _SWITCH_CTRL_H_

#include "gpio.h"
#include <stdint.h>
#include <stdbool.h>

// Delay between USB port switching
#define DELAY_BW_SWITCH 500 // ms

// Struct definition for switches
typedef struct hs_switch
{
    GPIO_TypeDef *ctrl_port;
    uint16_t ctrl_pin;
    GPIO_TypeDef *en_port;
    uint16_t en_pin;
} hs_switch_t;

typedef struct ss_switch
{
    GPIO_TypeDef *ctrl_port;
    uint16_t ctrl_pin;
    GPIO_TypeDef *en_port;
    uint16_t en_pin;
} ss_switch_t;

// Struct definition for LEDs
typedef struct leds
{
    GPIO_TypeDef *led_port;
    uint16_t led_pin;
} leds_t;

// Struct definition for Power switching
typedef struct source_sink
{
    GPIO_TypeDef *port_source_bank;
    uint16_t port_source_pin;
    GPIO_TypeDef *port_sink_bank;
    uint16_t port_sink_pin;
} source_sink_t;

typedef enum
{
    PORT_C_EN,
    PORT_B_EN,
    PORTS_DISABLED,
} port_ctrl_e;

typedef enum
{
    PORT_SINK,
    PORT_SOURCE,
    PORT_BI_DIR,
} port_on_power_ctrl_e;

typedef enum 
{
  BOTH_PORT_POWER_ON,
  PORTS_OFF,
} port_off_power_ctrl_e;

typedef enum 
{
    LED_PORT_A_C,
    LED_PORT_A_B,
    LED_PORT_A_B_C,
    LEDS_OFF,
}led_states_e;

/**
 * @brief Disable data lines set power mode.
 *
 * @param port_off_power_ctrl_e power_ctrl: Set power
 *  - BOTH_PORT_POWER_ON
 *  - PORTS_OFF
 */
void port_switch_disable(port_off_power_ctrl_e power_ctrl);

/**
 * @brief Enable switch data lines and set power mode. 
 *
 * @param port_ctrl_e mode: Set data line switch mode
 *  - PORT_C_EN,
 *  - PORT_B_EN,
 *  - PORTS_DISABLED,
 * @param port_on_power_ctrl_e: Set power control mode when data lines
 * are enabled
 *  - PORT_SINK,
 *  - PORT_SOURCE,
 *  - PORT_BI_DIR,
 */
void port_switch_enable(port_ctrl_e mode, port_on_power_ctrl_e power_ctrl);

#endif
