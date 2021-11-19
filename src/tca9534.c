
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

#ifdef PKG_USING_TCA9534

#define DBG_TAG "tca9534"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

/* tca9534 register */
#define TCA9534_REG_INPORT      0x00
#define TCA9534_REG_OUTPORT     0x01
#define TCA9534_REG_PI          0x02
#define TCA9534_REG_CFG       	0x03

/* tca9534 register value */
#define TCA9534_MODE_IN         0x01    /* input mode */
#define TCA9534_MODE_OUT        0x00    /* output mode */
#define TCA9534_PI_NOR          0x00    /* normal polarity */
#define TCA9534_PI_NOT          0x01    /* inversion  polarity */
#define TCA9534_PIN_LOW         0x00    /* pin low */
#define TCA9534_PIN_HIGH        0x01    /* pin high */

/* tca9534 device */
typedef struct tca9534_dev
{
    struct rt_i2c_bus_device *i2c_bus;
    rt_uint8_t slave_addr;
}tca9534_dev_t;

static rt_err_t tca9534_read_reg(rt_device_t pdev, rt_uint8_t reg, rt_uint8_t *pdata)
{
    struct rt_i2c_msg msg[2] = {0};
    tca9534_dev_t *ptca_dev = RT_NULL;
    
	RT_ASSERT(pdev != RT_NULL);
	
	ptca_dev = (tca9534_dev_t*)pdev->user_data;
    msg[0].addr  = ptca_dev->slave_addr;
    msg[0].flags = RT_I2C_WR;
    msg[0].len   = 1;
    msg[0].buf   = &reg;
    msg[1].addr  = ptca_dev->slave_addr;
    msg[1].flags = RT_I2C_RD;
    msg[1].len   = 1;
    msg[1].buf   = pdata;

    if(rt_i2c_transfer(ptca_dev->i2c_bus, msg, 2) == 2)
	{
        return RT_EOK;
    }
    else
    {
	  	LOG_E("i2c bus read failed!\r\n");
        return -RT_ERROR;
    }
}
 
static rt_err_t tca9534_write_reg(rt_device_t pdev, rt_uint8_t reg, rt_uint8_t data)
{
    struct rt_i2c_msg msg[2] = {0};
	struct tca9534_dev *ptca_dev = RT_NULL;
    
	RT_ASSERT(pdev != RT_NULL);
	
    ptca_dev = (tca9534_dev_t*)pdev->user_data;
    msg[0].addr		= ptca_dev->slave_addr;
    msg[0].flags	= RT_I2C_WR;
    msg[0].len   	= 1;
    msg[0].buf   	= &reg;
    msg[1].addr  	= ptca_dev->slave_addr;
    msg[1].flags	= RT_I2C_WR | RT_I2C_NO_START;
    msg[1].len   	= 1;
    msg[1].buf   	= &data;
    if(rt_i2c_transfer(ptca_dev->i2c_bus, msg, 2) == 2)
	{
        return RT_EOK;
    }
    else
    {
	  	LOG_E("i2c bus write failed!\r\n");
        return -RT_ERROR;
    }
}

static rt_err_t rt_tca9534_open(rt_device_t dev, rt_uint16_t flag)
{
    if (dev->rx_indicate != RT_NULL)
    {
        /* open interrupt */
    }

    return RT_EOK;
}

rt_size_t rt_tca9534_write(rt_device_t dev, rt_off_t pos, void const *buffer, rt_size_t size)
{
    rt_uint8_t temp = 0;
    
    tca9534_read_reg(dev, TCA9534_REG_OUTPORT, &temp);

    if (PIN_LOW == *(rt_uint8_t*)buffer)
    {
        temp &= ~(TCA9534_PIN_HIGH << pos);
        tca9534_write_reg(dev, TCA9534_REG_OUTPORT, temp);
    }
    else
    {
        temp |= TCA9534_PIN_HIGH << pos;
        tca9534_write_reg(dev, TCA9534_REG_OUTPORT, temp);
    }
    
    return size;
}

rt_size_t rt_tca9534_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    rt_uint8_t temp = 0;
    
    tca9534_read_reg(dev, TCA9534_REG_INPORT, &temp);
    
    *(rt_uint8_t*)buffer = (temp>>pos)&0x01;
    
    return size;
}

static rt_err_t rt_tca9534_ctrl(rt_device_t dev, int cmd, void *args)
{
    rt_uint8_t temp = 0;
    struct tc9534_pin_cfg *p = RT_NULL;
    
    p = (struct tc9534_pin_cfg *)args;
    switch(cmd)
    {
    case RT_TCA953_CTRL_MODE:
        if (RT_TCA953_MODE_OUTPUT == p->mode)
        {
            /* output setting */
            tca9534_read_reg(dev, TCA9534_REG_CFG, &temp);
            temp &= ~(TCA9534_MODE_IN << p->pin);
            tca9534_write_reg(dev, TCA9534_REG_CFG, temp);
        }				
        else if (RT_TCA953_MODE_INPUT == p->mode)
        {
            /* input setting */
            tca9534_read_reg(dev, TCA9534_REG_CFG, &temp);
            temp |= TCA9534_MODE_IN << p->pin;
            tca9534_write_reg(dev, TCA9534_REG_CFG, temp);
        }
        
        if (RT_TCA953_POLARITY_NOR == p->polarity)
        {
            /* normal polarity */
            tca9534_read_reg(dev, TCA9534_REG_PI, &temp);
            temp &= ~(TCA9534_PI_NOT << p->pin);
            tca9534_write_reg(dev, TCA9534_REG_PI, temp);
        }				
        else if (RT_TCA953_POLARITY_NOT == p->polarity)
        {
            /* inversion polarity */
            tca9534_read_reg(dev, TCA9534_REG_PI, &temp);
            temp |= TCA9534_PI_NOT << p->pin;
            tca9534_write_reg(dev, TCA9534_REG_PI, temp);
        }
    
      break;
    default:
      break;
    }
    return RT_EOK;
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops tca9534_dev_ops = 
{
    .init = RT_NULL,
    .open = rt_tca9534_open,
    .close = RT_NULL,
    .read = rt_tca9534_read,
    .write = rt_tca9534_write,
    .control = rt_tca9534_ctrl
};
#endif

rt_err_t rt_tca9534_init(const char *dev_name, const char *i2c_name, rt_uint8_t i2c_addr)
{
  	rt_err_t ret = RT_EOK;
	tca9534_dev_t *ptca_dev = RT_NULL;
	rt_device_t prt_dev = RT_NULL;
    
    ptca_dev = rt_calloc(1, sizeof(tca9534_dev_t));
 
    if (ptca_dev == RT_NULL)
    {
        LOG_E("can't allocate memory for tca9534 device on '%s' ", i2c_name);
        goto __exit;
    }
      
    ptca_dev->i2c_bus = rt_i2c_bus_device_find(i2c_name);
    if(ptca_dev->i2c_bus == RT_NULL)
    {
        LOG_E("i2c bus device %s not found!", i2c_name);
		ret = -RT_ERROR;
		goto __exit;
    }	
    ptca_dev->slave_addr = i2c_addr;
 
    prt_dev = rt_calloc(1, sizeof(struct rt_device));
    if (prt_dev == RT_NULL)
    {
        LOG_E("can't allocate memory for tca9534 device");
        goto __exit;
    }
    
    /* register device */
    prt_dev->type   = RT_Device_Class_Miscellaneous;
#ifdef RT_USING_DEVICE_OPS
    prt_dev->ops = &tca9534_dev_ops;
#else
    prt_dev->init   = RT_NULL;
    prt_dev->open   = rt_tca9534_open;
    prt_dev->close  = RT_NULL;
    prt_dev->read   = rt_tca9534_read;
    prt_dev->write  = rt_tca9534_write;
    prt_dev->control= rt_tca9534_ctrl;
#endif
    prt_dev->user_data 	= (void*)ptca_dev;	/* tca9534 device */;		
    rt_device_register(prt_dev, dev_name, RT_DEVICE_FLAG_RDWR);

    return ret;

__exit:
	if (ptca_dev != RT_NULL)
	{
		rt_free(ptca_dev);
	}
    
	if (prt_dev != RT_NULL)
	{
		rt_free(prt_dev);
	}
    
    return RT_ERROR;
}

void rt_tca9534_deinit(const char *dev_name)
{
    rt_device_t pdev = RT_NULL;
    
    pdev = rt_device_find(dev_name);
    RT_ASSERT(pdev);
    
    if (RT_NULL != pdev->user_data)
    {
        rt_free(pdev->user_data);
    }
    rt_free(pdev);
}


#endif /* PKG_USING_TCA9534 */
