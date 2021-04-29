
/*
 * Copyright (c) 2020 panrui <https://github.com/Prry/rtt-tca9543>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-28     panrui      the first version
 */

#ifndef _TCA9543_H_
#define _TCA9543_H_

#include <rtthread.h>

/* control cmd */
#define RT_TCA953_CTRL_MODE     0x00

/* control value */
#define RT_TCA953_MODE_OUTPUT  0x00
#define RT_TCA953_MODE_INPUT   0x01
#define RT_TCA953_POLARITY_NOR 0x00
#define RT_TCA953_POLARITY_NOT 0x01

struct tc9534_pin_cfg
{
    rt_uint8_t pin;
    rt_uint8_t mode;
    rt_uint8_t polarity;
};

extern rt_err_t rt_tca9534_init(const char *dev_name, const char *i2c_name, rt_uint8_t i2c_addr);
extern void rt_tca9534_deinit(const char *dev_name);

#endif /* _TCA9543_H_ */
