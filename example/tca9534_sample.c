
/*
 * Copyright (c) 2020 panrui <https://github.com/Prry/rtt-tca9534>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-28     panrui      the first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "tca9534.h"

static void tca9534_sample_entry(void *parameter)
{
    rt_device_t dev = RT_NULL;
    rt_uint8_t value = 0;
    rt_uint8_t read = 0;
    struct tc9534_pin_cfg cfg = {0};
    
	dev = rt_device_find("tca9534");
    if (dev == RT_NULL)
    {
	  	rt_kprintf("not found tca9534 device\r\n");
        return;
    }

    if (rt_device_open(dev, RT_DEVICE_FLAG_RDONLY) != RT_EOK)
    {
        rt_kprintf("open tca9534 failed\r\n");
        return;
    }
	
    /* set P0 output mode for led blink */
    cfg.pin = 0;
    cfg.mode = RT_TCA953_MODE_OUTPUT;
    cfg.polarity = RT_TCA953_POLARITY_NOR;
    rt_device_control(dev, RT_TCA953_CTRL_MODE, (void*)&cfg);
    
    /* set P1 input mode*/
    cfg.pin = 1;
    cfg.mode = RT_TCA953_MODE_INPUT;
    cfg.polarity = RT_TCA953_POLARITY_NOR;
    rt_device_control(dev, RT_TCA953_CTRL_MODE, (void*)&cfg);
    
    for (;;)
    { 
		rt_device_write(dev, 0, &value, 1);
        rt_device_read(dev, 0, &read, 1);
        rt_kprintf("tca9534 P0 status,[0x%02x]\r\n", read);
        value = !value;
        rt_thread_delay(RT_TICK_PER_SECOND/2);
        rt_device_write(dev, 0, &value, 1); 
        rt_device_read(dev, 0, &read, 1);
        rt_kprintf("tca9534 P0 status,[0x%02x]\r\n", read);
        value = !value;
        rt_thread_delay(RT_TICK_PER_SECOND/2);
       
    }
}

static int tca9534_sample(void)
{
    rt_thread_t thread;

    thread = rt_thread_create("tca",
                                     tca9534_sample_entry,
                                     RT_NULL,
                                     512,
                                     RT_THREAD_PRIORITY_MAX / 2,
                                     20);
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }

    return RT_EOK;
}
INIT_APP_EXPORT(tca9534_sample);

static int rt_hw_tca9534_port(void)
{
    return rt_hw_tca9534_init("tca9534", "i2c1", 0x20);
}
INIT_DEVICE_EXPORT(rt_hw_tca9534_port);
