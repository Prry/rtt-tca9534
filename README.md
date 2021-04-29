# TCA9534 8位I/O扩展器驱动软件包



## 1 简介

tca9534软件包是基于RT-Thread 设备框架实现的一个驱动包。基于该软件包，RT-Thread应用程序可以使用RT-Thread标准IO设备接口(rt_device_open/rt_device_read/rt_device_write/rt_device_control)操作tca9534。



### 1.1 目录结构

| 名称       | 说明                  |
| ---------- | --------------------- |
| docs       | 文档目录              |
| examples   | 例子目录              |
| inc        | 头文件目录            |
| src        | 源代码目录            |
| LICENSE    | 许可证文件            |
| SConscript | RT-Thread默认构建脚本 |



### 1.2 许可证

tca9534软件包遵循 Apache license v2.0 许可，详见 `LICENSE` 文件。

<br>

## 2 芯片介绍

tca9534是德州仪器（TI）的一款i2c/SMBus转8位I/O的扩展芯片，具有功耗低、供电范围宽、控制灵活、应用范围广等优点。tca9534支持单独输入/输出设置、极性翻转设置、中断信号、多地址选择，通过地址引脚，最多支持8个设备挂在同一根i2c总线上。典型应用场合如下。

* 服务器

* 路由器(电信交换设备)

* 个人电脑

* 个人电子产品(如游戏机)

* 工业自动化

* gpio有限的处理器产品



<br>

## 3 支持情况



| 包含设备     | tca9534 |
| ------------ | ------- |
| **通信接口** |         |
| IIC          | √       |
| SPI          |         |
| **工作模式** |         |
| 轮询         | √       |
| 中断         |         |
| FIFO         |         |
| **电源模式** |         |
| 掉电         |         |
| 低功耗       |         |
| 普通         | √       |



<br>

## 4 使用说明

### 4.1 依赖

- RT-Thread 3.0.0+
- I2C 驱动，tca9534设备使用 I2C 进行数据通讯，需要系统 I2C 驱动框架支持



### 4.2 获取软件包

使用 tca9534 package 需要在 RT-Thread 的包管理器中选择它，具体路径如下。然后让 RT-Thread 的包管理器自动更新，或者使用 `pkgs --update` 命令更新包到 BSP 中。如需使用示例程序，则使能<code>Enable tca9534 sample</code>。

```
RT-Thread online packages --->
    peripheral libraries and drivers --->
        sensors drivers --->
            [*] TCA9534: a 8-bit I/O expander for i2c-bus.
            		[*] Enable tca9534 sample
                    Version (latest)  --->
```

>  **Version**：软件包版本选择，默认选择最新版本。 



### 4.3 初始化

tca9534软件包初始化函数如下所示：

```C
rt_err_t rt_tca9534_init(const char *dev_name, const char *i2c_name, rt_uint8_t i2c_addr);
```

该函数需要由用户调用，函数主要完成的功能有:

- dev_name，tca9534设备名称

- i2c_name，i2c总线设备名称

- i2c_addr，tca9534从地址

  

**参考示例：**

```c
#include "tca9534.h"

static int rt_hw_tca9534_port(void)
{
    return rt_tca9534_init("tca9534", "i2c1", 0x20);
}
INIT_DEVICE_EXPORT(rt_hw_tca9534_port);
```



### 4.4 访问设备

tca9534 软件包基于RT-Thread标准设备框架，可以使用RT-Thread标准设备口(rt_device_open/rt_device_read/rt_device_write/rt_device_control)来访问tca9534。



**参考伪代码:**

```c
dev = rt_device_find("tca9534");/* 查找设备 */

/* 设置P0输出模式，不翻转极性 */
cfg.pin = 0;
cfg.mode = RT_TCA953_MODE_OUTPUT;
cfg.polarity = RT_TCA953_POLARITY_NOR;
rt_device_control(dev, RT_TCA953_CTRL_MODE, (void*)&cfg);/

rt_device_write(dev, 0, 1, 1);	/* P0输出高电平（1） */
rt_device_read(dev, 1, &read, 1); /* 读取P1输入状态 */
```

<br>

### 4.5 msh/finsh测试

#### 查看设备注册

```b
msh >list_device
device           type         ref count
-------- -------------------- ----------
tca9534  Sensor Device        1            
i2c1     I2C Bus              0            
uart1    Character Device     2 
```



#### 执行sample

```b
tca9534 P0 status,[0x00]
tca9534 P0 status,[0x01]
tca9534 P0 status,[0x00]
tca9534 P0 status,[0x01]
```

<br>

## 5 注意事项

暂无

<br>

## 6 联系方式

- 维护：[Acuity](https://github.com/Prry)
- 主页：<https://github.com/Prry/rtt-tca9534>    