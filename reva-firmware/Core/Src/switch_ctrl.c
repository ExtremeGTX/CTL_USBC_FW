#include "switch_ctrl.h"

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
    .led_pin = GPIO_PIN_1,
    .led_port = GPIOA,
};

leds_t led_port_c = {
    .led_pin = GPIO_PIN_2,
    .led_port = GPIOA,
};

// Source sink pin declaration
source_sink_t port_a =
    {
        .port_sink_bank = GPIOB,
        .port_sink_pin = GPIO_PIN_0,
        .port_source_bank = GPIOB,
        .port_source_pin = GPIO_PIN_1,
};

source_sink_t port_b =
    {
        .port_sink_bank = GPIOB,
        .port_sink_pin = GPIO_PIN_2,
        .port_source_bank = GPIOB,
        .port_source_pin = GPIO_PIN_3,
};

source_sink_t port_c =
    {
        .port_sink_bank = GPIOB,
        .port_sink_pin = GPIO_PIN_4,
        .port_source_bank = GPIOB,
        .port_source_pin = GPIO_PIN_5,
};


// switch control logic

/**
 * @brief Control the CC switch
 *
 * @param port_ctrl_emode: Set switch mode
 *  -PORT_C_EN,
 *  -PORT_D_EN,
 *  -CC_SW_OFF
 */
static void cc_switch_control(port_ctrl_e mode)
{
    if (mode == PORT_C_EN)
    {
#if RS2228_LOGIC_INVERTED
        HAL_GPIO_WritePin(switch_c.en_port, switch_c.en_pin, GPIO_PIN_SET);
#else
        HAL_GPIO_WritePin(switch_c.en_port, switch_c.en_pin, GPIO_PIN_RESET);
#endif
        HAL_GPIO_WritePin(switch_c.ctrl_port, switch_c.ctrl_pin, GPIO_PIN_RESET);
    }
    else if (mode == PORT_D_EN)
    {
#if RS2228_LOGIC_INVERTED
        HAL_GPIO_WritePin(switch_c.en_port, switch_c.en_pin, GPIO_PIN_SET);
#else
        HAL_GPIO_WritePin(switch_c.en_port, switch_c.en_pin, GPIO_PIN_RESET);
#endif
        HAL_GPIO_WritePin(switch_c.ctrl_port, switch_c.ctrl_pin, GPIO_PIN_SET);
    }
    else if (mode == CC_SW_OFF)
    {

#if RS2228_LOGIC_INVERTED
        HAL_GPIO_WritePin(switch_c.en_port, switch_c.en_pin, GPIO_PIN_RESET);
#else
        HAL_GPIO_WritePin(switch_c.en_port, switch_c.en_pin, GPIO_PIN_SET);
#endif
        HAL_GPIO_WritePin(switch_c.ctrl_port, switch_c.ctrl_pin, GPIO_PIN_SET);
    }
}


/**
 * @brief Control the USB 2 Data line switch
 *
 * @param port_ctrl_e mode: Set switch mode
 *  @note Possible inputs param value:
 *  -PORT_C_EN,
 *  -PORT_D_EN,
 *  -DL_SW_OFF
 */
static void dl_switch_control(port_ctrl_e mode)
{
    if (mode == PORT_C_EN)
    {
#if RS2228_LOGIC_INVERTED
        HAL_GPIO_WritePin(switch_d.en_port, switch_d.en_pin, GPIO_PIN_SET);
#else
        HAL_GPIO_WritePin(switch_d.en_port, switch_d.en_pin, GPIO_PIN_RESET);
#endif
        HAL_GPIO_WritePin(switch_d.ctrl_port, switch_d.ctrl_pin, GPIO_PIN_RESET);
    }
    else if (mode == PORT_D_EN)
    {
#if RS2228_LOGIC_INVERTED
        HAL_GPIO_WritePin(switch_d.en_port, switch_d.en_pin, GPIO_PIN_SET);
#else
        HAL_GPIO_WritePin(switch_d.en_port, switch_d.en_pin, GPIO_PIN_RESET);
#endif
        HAL_GPIO_WritePin(switch_d.ctrl_port, switch_d.ctrl_pin, GPIO_PIN_SET);
    }
    else if (mode == DL_SW_OFF)
    {
#if RS2228_LOGIC_INVERTED
        HAL_GPIO_WritePin(switch_d.en_port, switch_d.en_pin, GPIO_PIN_RESET);
#else
        HAL_GPIO_WritePin(switch_d.en_port, switch_d.en_pin, GPIO_PIN_SET);
#endif
        HAL_GPIO_WritePin(switch_d.ctrl_port, switch_d.ctrl_pin, GPIO_PIN_SET);
    }
}

/**
 * @brief Control the Super speed switchs
 *
 * @param port_ctrl_e mode: Set switch mode
 *  -PORT_C_EN,
 *  -PORT_D_EN,
 *  -SS_SW_OFF
 */
static void ss_switch_control(port_ctrl_e mode)
{
    if (mode == PORT_D_EN)
    {
        HAL_GPIO_WritePin(ss1.en_port, ss1.en_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(ss1.ctrl_port, ss1.ctrl_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(ss2.en_port, ss2.ctrl_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(ss2.ctrl_port, ss2.ctrl_pin, GPIO_PIN_RESET);
    }
    else if (mode == PORT_C_EN)
    {
        HAL_GPIO_WritePin(ss1.en_port, ss1.en_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(ss1.ctrl_port, ss1.ctrl_pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(ss2.en_port, ss2.ctrl_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(ss2.ctrl_port, ss2.ctrl_pin, GPIO_PIN_SET);
    }
    else if (mode == SS_SW_OFF)
    {
        HAL_GPIO_WritePin(ss1.en_port, ss1.en_pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(ss1.ctrl_port, ss1.ctrl_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(ss2.en_port, ss2.ctrl_pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(ss2.ctrl_port, ss2.ctrl_pin, GPIO_PIN_RESET);
    }
}

// Logic for power switching
void switch_power(port_ctrl_e mode, sink_source_ctrl_e power_ctrl)
{
    if (mode == PORT_C_EN)
    {
        if (power_ctrl == PORT_SINK)
        // enable a
        {
            HAL_GPIO_WritePin(port_a.port_sink_bank, port_a.port_sink_pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(port_a.port_source_bank, port_a.port_source_pin, GPIO_PIN_SET);
            // enable b
            HAL_GPIO_WritePin(port_b.port_sink_bank, port_b.port_sink_pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(port_a.port_source_bank, port_a.port_source_pin, GPIO_PIN_RESET);
            // disable c
            HAL_GPIO_WritePin(port_c.port_sink_bank, port_c.port_sink_pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(port_c.port_source_bank, port_c.port_source_pin, GPIO_PIN_RESET);
        }
        else if (power_ctrl == PORT_SOURCE)
        {
            // enable a
            HAL_GPIO_WritePin(port_a.port_sink_bank, port_a.port_sink_pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(port_a.port_source_bank, port_a.port_source_pin, GPIO_PIN_SET);
            // enable b
            HAL_GPIO_WritePin(port_b.port_sink_bank, port_b.port_sink_pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(port_a.port_source_bank, port_a.port_source_pin, GPIO_PIN_SET);
            // disable c
            HAL_GPIO_WritePin(port_c.port_sink_bank, port_c.port_sink_pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(port_c.port_source_bank, port_c.port_source_pin, GPIO_PIN_RESET);
        }
    }
    else if (mode == PORT_D_EN)
    {
        if (power_ctrl == PORT_SINK)
        {
            // enable a
            HAL_GPIO_WritePin(port_a.port_sink_bank, port_a.port_sink_pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(port_a.port_source_bank, port_a.port_source_pin, GPIO_PIN_SET);
            // disable b
            HAL_GPIO_WritePin(port_b.port_sink_bank, port_b.port_sink_pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(port_a.port_source_bank, port_a.port_source_pin, GPIO_PIN_RESET);
            // enable c
            HAL_GPIO_WritePin(port_c.port_sink_bank, port_c.port_sink_pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(port_c.port_source_bank, port_c.port_source_pin, GPIO_PIN_RESET);
        }
        else if (power_ctrl == PORT_SOURCE)
        {
            // enable a
            HAL_GPIO_WritePin(port_a.port_sink_bank, port_a.port_sink_pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(port_a.port_source_bank, port_a.port_source_pin, GPIO_PIN_SET);
            // disable b
            HAL_GPIO_WritePin(port_b.port_sink_bank, port_b.port_sink_pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(port_a.port_source_bank, port_a.port_source_pin, GPIO_PIN_RESET);
            // enable c
            HAL_GPIO_WritePin(port_c.port_sink_bank, port_c.port_sink_pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(port_c.port_source_bank, port_c.port_source_pin, GPIO_PIN_SET);
        }
    }
    else if (mode == PORTS_DISABLED)
    {
        // disable both
        HAL_GPIO_WritePin(port_b.port_sink_bank, port_b.port_sink_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(port_a.port_source_bank, port_a.port_source_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(port_c.port_sink_bank, port_c.port_sink_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(port_c.port_source_bank, port_c.port_source_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(port_a.port_sink_bank, port_a.port_sink_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(port_a.port_source_bank, port_a.port_source_pin, GPIO_PIN_RESET);
    }
}


void port_switch_control(port_ctrl_e mode, sink_source_ctrl_e power_ctrl)
{
    if (mode == PORT_C_EN)
    {
        cc_switch_control(PORT_C_EN);
        ss_switch_control(PORT_C_EN);
        dl_switch_control(PORT_C_EN);
        switch_power(PORT_C_EN, power_ctrl);
        HAL_GPIO_WritePin(led_port_a.led_port, led_port_a.led_pin, GPIO_PIN_SET);   // led for port A
        HAL_GPIO_WritePin(led_port_b.led_port, led_port_b.led_pin, GPIO_PIN_SET);   // led for port B
        HAL_GPIO_WritePin(led_port_c.led_port, led_port_c.led_pin, GPIO_PIN_RESET); // led for port C
    }
    else if (mode == PORT_D_EN)
    {
        cc_switch_control(PORT_D_EN);
        ss_switch_control(PORT_D_EN);
        dl_switch_control(PORT_D_EN);
        switch_power(PORT_D_EN, power_ctrl);
        HAL_GPIO_WritePin(led_port_a.led_port, led_port_a.led_pin, GPIO_PIN_SET);   // led for port A
        HAL_GPIO_WritePin(led_port_b.led_port, led_port_b.led_pin, GPIO_PIN_RESET); // led for port B
        HAL_GPIO_WritePin(led_port_c.led_port, led_port_c.led_pin, GPIO_PIN_SET);   // led for port C
    }
    else if (mode == PORTS_DISABLED)
    {
        cc_switch_control(CC_SW_OFF);
        ss_switch_control(SS_SW_OFF);
        dl_switch_control(DL_SW_OFF);
        switch_power(PORTS_DISABLED, power_ctrl);
        HAL_GPIO_WritePin(led_port_a.led_port, led_port_a.led_pin, GPIO_PIN_RESET); // led for port A
        HAL_GPIO_WritePin(led_port_b.led_port, led_port_b.led_pin, GPIO_PIN_RESET); // led for port B
        HAL_GPIO_WritePin(led_port_c.led_port, led_port_c.led_pin, GPIO_PIN_RESET); // led for port C
    }
}