/**
******************************************************************************
* @file    switch_ctrl.c
* @authors  Mudit Sharma
* @contact  mudit.sharma@codethink.co.uk
* @brief   Source file for switch control handler
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

#include "switch_ctrl.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include <stdint.h>

/* static port_enabled = PORTS_DISABLED; */

// High speed switch pin declaration
hs_switch_t switch_d = {
    .ctrl_port = GPIOB,
    .ctrl_pin = GPIO_PIN_6,
    .en_port = GPIOA,
    .en_pin = GPIO_PIN_8,
};

hs_switch_t switch_c = {
    .ctrl_port = GPIOB,
    .ctrl_pin = GPIO_PIN_7,
    .en_port = GPIOA,
    .en_pin = GPIO_PIN_7,
};

hs_switch_t switch_sbu = {
    .ctrl_port = GPIOB,
    .ctrl_pin = GPIO_PIN_11,
    .en_port = GPIOB,
    .en_pin = GPIO_PIN_10,

};

// Super speed switch pin declaration
ss_switch_t ss1 = {
    .ctrl_port = GPIOB,
    .ctrl_pin = GPIO_PIN_8,
    .en_port = GPIOA,
    .en_pin = GPIO_PIN_6,
};

ss_switch_t ss2 = {
    .ctrl_port = GPIOB,
    .ctrl_pin = GPIO_PIN_9,
    .en_port = GPIOA,
    .en_pin = GPIO_PIN_5,
};

// LED pin declaration
leds_t led_port_a = {
    .led_pin = GPIO_PIN_0,
    .led_port = GPIOA,
};

leds_t led_port_b = {
    .led_pin = GPIO_PIN_2,
    .led_port = GPIOA,
};

leds_t led_port_c = {
    .led_pin = GPIO_PIN_1,
    .led_port = GPIOA,
};

// Source sink pin declaration
source_sink_t port_a = {
    .port_sink_bank = GPIOB,
    .port_sink_pin = GPIO_PIN_0,
    .port_source_bank = GPIOB,
    .port_source_pin = GPIO_PIN_1,
};

source_sink_t port_b = {
    .port_sink_bank = GPIOB,
    .port_sink_pin = GPIO_PIN_4,
    .port_source_bank = GPIOB,
    .port_source_pin = GPIO_PIN_5,
};

source_sink_t port_c = {
    .port_sink_bank = GPIOB,
    .port_sink_pin = GPIO_PIN_2,
    .port_source_bank = GPIOB,
    .port_source_pin = GPIO_PIN_3,
};

// switch control logic


/**
 * @brief Set respective LEDs based on selected mode
 *
 * @param led_states_e led_state: Leds to turn on near
 * respective ports
 */

static void led_ctrl(led_states_e led_state) {

  switch (led_state) {
  case LED_PORT_A_C:
    HAL_GPIO_WritePin(led_port_a.led_port, led_port_a.led_pin,
                      GPIO_PIN_SET); // led for port A
    HAL_GPIO_WritePin(led_port_b.led_port, led_port_b.led_pin,
                      GPIO_PIN_SET); // led for port B
    HAL_GPIO_WritePin(led_port_c.led_port, led_port_c.led_pin,
                      GPIO_PIN_RESET); // led for port C

    break;

  case LED_PORT_A_B:
    HAL_GPIO_WritePin(led_port_a.led_port, led_port_a.led_pin,
                      GPIO_PIN_SET); // led for port A
    HAL_GPIO_WritePin(led_port_b.led_port, led_port_b.led_pin,
                      GPIO_PIN_RESET); // led for port B
    HAL_GPIO_WritePin(led_port_c.led_port, led_port_c.led_pin,
                      GPIO_PIN_SET); // led for port C

    break;

  case LED_PORT_A_B_C:
    HAL_GPIO_WritePin(led_port_a.led_port, led_port_a.led_pin,
                      GPIO_PIN_SET); // led for port A
    HAL_GPIO_WritePin(led_port_b.led_port, led_port_b.led_pin,
                      GPIO_PIN_SET); // led for port B
    HAL_GPIO_WritePin(led_port_c.led_port, led_port_c.led_pin,
                      GPIO_PIN_SET); // led for port C
    break;

  case LEDS_OFF:
    HAL_GPIO_WritePin(led_port_a.led_port, led_port_a.led_pin,
                      GPIO_PIN_RESET); // led for port A
    HAL_GPIO_WritePin(led_port_b.led_port, led_port_b.led_pin,
                      GPIO_PIN_RESET); // led for port B
    HAL_GPIO_WritePin(led_port_c.led_port, led_port_c.led_pin,
                      GPIO_PIN_RESET); // led for port C
    break;

  default:
    break;
  }
}

/**
 * @brief Control all the HS switchs
 *
 * @param port_ctrl_emode: Set switch mode
 *  -PORT_C_EN,
 *  -PORT_B_EN,
 *  -PORTS_DISABLED
 */
static void hs_switch_control(port_ctrl_e mode) {
  switch (mode) {
  case PORT_C_EN:
    HAL_GPIO_WritePin(switch_c.en_port, switch_c.en_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(switch_c.ctrl_port, switch_c.ctrl_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(switch_d.en_port, switch_d.en_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(switch_d.ctrl_port, switch_d.ctrl_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(switch_sbu.en_port, switch_sbu.en_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(switch_sbu.ctrl_port, switch_sbu.ctrl_pin,
                      GPIO_PIN_RESET);

    break;

  case PORT_B_EN:
    HAL_GPIO_WritePin(switch_c.en_port, switch_c.en_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(switch_c.ctrl_port, switch_c.ctrl_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(switch_d.en_port, switch_d.en_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(switch_d.ctrl_port, switch_d.ctrl_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(switch_sbu.en_port, switch_sbu.en_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(switch_sbu.ctrl_port, switch_sbu.ctrl_pin, GPIO_PIN_SET);

    break;

  case PORTS_DISABLED:
    HAL_GPIO_WritePin(switch_c.en_port, switch_c.en_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(switch_c.ctrl_port, switch_c.ctrl_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(switch_d.en_port, switch_d.en_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(switch_d.ctrl_port, switch_d.ctrl_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(switch_sbu.en_port, switch_sbu.en_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(switch_sbu.ctrl_port, switch_sbu.ctrl_pin,
                      GPIO_PIN_RESET);
    break;

  default:
    break;
  }
}

/**
 * @brief Control the Super speed switchs
 *
 * @param port_ctrl_e mode: Set switch mode
 *  -PORT_C_EN,
 *  -PORT_B_EN,
 *  -PORTS_DISABLED
 * */
static void ss_switch_control(port_ctrl_e mode)
{

  switch (mode) {
  case PORT_C_EN:
    HAL_GPIO_WritePin(ss1.en_port, ss1.en_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ss1.ctrl_port, ss1.ctrl_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ss2.en_port, ss2.en_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ss2.ctrl_port, ss2.ctrl_pin, GPIO_PIN_SET);
    break;

  case PORT_B_EN:
    HAL_GPIO_WritePin(ss1.en_port, ss1.en_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ss1.ctrl_port, ss1.ctrl_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ss2.en_port, ss2.en_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ss2.ctrl_port, ss2.ctrl_pin, GPIO_PIN_RESET);
    break;

  case PORTS_DISABLED:
    HAL_GPIO_WritePin(ss1.en_port, ss1.en_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ss1.ctrl_port, ss1.ctrl_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ss2.en_port, ss2.en_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ss2.ctrl_port, ss2.ctrl_pin, GPIO_PIN_RESET);
    break;

  default:
    break;
  }
}


void port_switch_enable(port_ctrl_e mode, port_on_power_ctrl_e power_ctrl) {
  port_switch_disable(PORTS_OFF);
  HAL_Delay(DELAY_BW_SWITCH);
  switch (mode) {
  case PORT_C_EN:
     hs_switch_control(mode);
     ss_switch_control(PORT_C_EN);
     led_ctrl(LED_PORT_A_C);
    switch (power_ctrl) {
    case PORT_SINK:
      // disable b
      HAL_GPIO_WritePin(port_b.port_sink_bank, port_b.port_sink_pin,
                        GPIO_PIN_RESET);
      HAL_GPIO_WritePin(port_b.port_source_bank, port_b.port_source_pin,
                        GPIO_PIN_RESET);
      // enable c
      HAL_GPIO_WritePin(port_c.port_sink_bank, port_c.port_sink_pin,
                        GPIO_PIN_SET);
      HAL_GPIO_WritePin(port_c.port_source_bank, port_c.port_source_pin,
                        GPIO_PIN_RESET);
      break;

    case PORT_SOURCE:
      // disable b
      HAL_GPIO_WritePin(port_b.port_sink_bank, port_b.port_sink_pin,
                        GPIO_PIN_RESET);
      HAL_GPIO_WritePin(port_b.port_source_bank, port_b.port_source_pin,
                        GPIO_PIN_RESET);
      // enable c
      HAL_GPIO_WritePin(port_c.port_sink_bank, port_c.port_sink_pin,
                        GPIO_PIN_RESET);
      HAL_GPIO_WritePin(port_c.port_source_bank, port_c.port_source_pin,
                        GPIO_PIN_SET);
      break;

    case PORT_BI_DIR:
      // disable b
      HAL_GPIO_WritePin(port_b.port_sink_bank, port_b.port_sink_pin,
                        GPIO_PIN_RESET);
      HAL_GPIO_WritePin(port_b.port_source_bank, port_b.port_source_pin,
                        GPIO_PIN_RESET);
      // enable c
      HAL_GPIO_WritePin(port_c.port_sink_bank, port_c.port_sink_pin,
                        GPIO_PIN_SET);
      HAL_GPIO_WritePin(port_c.port_source_bank, port_c.port_source_pin,
                        GPIO_PIN_SET);
      break;

    default:
      break;
    }
    break;

  case PORT_B_EN:
    led_ctrl(LED_PORT_A_B);
    hs_switch_control(mode);
    ss_switch_control(PORT_B_EN);
    switch (power_ctrl) {
    case PORT_SINK:
      // disable b
      HAL_GPIO_WritePin(port_b.port_sink_bank, port_b.port_sink_pin,
                        GPIO_PIN_SET);
      HAL_GPIO_WritePin(port_b.port_source_bank, port_b.port_source_pin,
                        GPIO_PIN_RESET);
      // enable c
      HAL_GPIO_WritePin(port_c.port_sink_bank, port_c.port_sink_pin,
                        GPIO_PIN_RESET);
      HAL_GPIO_WritePin(port_c.port_source_bank, port_c.port_source_pin,
                        GPIO_PIN_RESET);
      break;

    case PORT_SOURCE:
      // disable b
      HAL_GPIO_WritePin(port_b.port_sink_bank, port_b.port_sink_pin,
                        GPIO_PIN_RESET);
      HAL_GPIO_WritePin(port_b.port_source_bank, port_b.port_source_pin,
                        GPIO_PIN_SET);
      // enable c
      HAL_GPIO_WritePin(port_c.port_sink_bank, port_c.port_sink_pin,
                        GPIO_PIN_RESET);
      HAL_GPIO_WritePin(port_c.port_source_bank, port_c.port_source_pin,
                        GPIO_PIN_RESET);
      break;

    case PORT_BI_DIR:
      // disable b
      HAL_GPIO_WritePin(port_b.port_sink_bank, port_b.port_sink_pin,
                        GPIO_PIN_SET);
      HAL_GPIO_WritePin(port_b.port_source_bank, port_b.port_source_pin,
                        GPIO_PIN_SET);
      // enable c
      HAL_GPIO_WritePin(port_c.port_sink_bank, port_c.port_sink_pin,
                        GPIO_PIN_RESET);
      HAL_GPIO_WritePin(port_c.port_source_bank, port_c.port_source_pin,
                        GPIO_PIN_RESET);
      break;

    default:
      break;
    }
    break;

    case PORTS_DISABLED:
    break;

    default:
      break;
  }
}

void port_switch_disable(port_off_power_ctrl_e power_ctrl) {
  hs_switch_control(PORTS_DISABLED);
  ss_switch_control(PORTS_DISABLED);

  switch (power_ctrl) {
  case BOTH_PORT_POWER_ON:
    led_ctrl(LED_PORT_A_B_C);
    HAL_GPIO_WritePin(port_b.port_sink_bank, port_b.port_sink_pin,
                      GPIO_PIN_SET);
    HAL_GPIO_WritePin(port_b.port_source_bank, port_b.port_source_pin,
                      GPIO_PIN_SET);
    HAL_GPIO_WritePin(port_c.port_sink_bank, port_c.port_sink_pin,
                      GPIO_PIN_SET);
    HAL_GPIO_WritePin(port_c.port_source_bank, port_c.port_source_pin,
                      GPIO_PIN_SET);
    break;

  case PORTS_OFF:
    // disable both
    led_ctrl(LEDS_OFF);
    HAL_GPIO_WritePin(port_b.port_sink_bank, port_b.port_sink_pin,
                      GPIO_PIN_RESET);
    HAL_GPIO_WritePin(port_b.port_source_bank, port_b.port_source_pin,
                      GPIO_PIN_RESET);
    HAL_GPIO_WritePin(port_c.port_sink_bank, port_c.port_sink_pin,
                      GPIO_PIN_RESET);
    HAL_GPIO_WritePin(port_c.port_source_bank, port_c.port_source_pin,
                      GPIO_PIN_RESET);
    break;

  default:
    break;
  }
}
