> # Quadcopter_Group3
>
> 第三组的四轴飞行器Github仓库~

# 进阶式挑战性综合课程设计

![image-20230927214301617](./README.assets/image-20230927214301617.png)

## 挑战点

### 阶段一

- 用 KEIL5 点亮STM32的灯并使之闪烁（汇编实现）
- 使用 Altium Designer 设计 STM32 硬件原理图
- 使用 Altium Designer 设计 STM32 PCB板图
- 裸机下驱动姿态传感器 GY-86
- 裸机下驱动蓝牙模块
- 裸机下驱动电机
- 使用 Altium Designer 完成四轴飞行器转接板原理图及PCB图设计与制作
- 硬件子系统接受并解析遥控器命令
- 启动硬件子系统
- 在自己设计的硬件系统上实现裸机软件集成

### 阶段二

> “前面的区域，以后再来探索吧!"

### 阶段三

> “前面的区域，以后再来探索吧!"

## 器件模块

| 器件            | 型号                          |
| --------------- | ----------------------------- |
| 机架            | *待定*                        |
| 主控板          | STM32F401RE @84MHz            |
| 加速度计&陀螺仪 | MPU6050（集成于GY-86）        |
| 磁力计          | MHC5883L（集成于GY-86）       |
| 蓝牙            | *待定*                        |
| 遥控器          | 乐迪T4EU 2.4G 四通道遥控器    |
| 接收器          | 乐迪R7EH 2.4G 七通道接收器    |
| 电机            | *待定*                        |
| 电调            | *待定*                        |
| 螺旋桨          | *待定*                        |
| 电池            | *待定*                        |
| 转接板          | 使用 Altium Designer 自行制作 |

## TODOs

- 2023.9.1
    - ~~购买 STM32F401RE~~
    - ~~购买 GY-86~~
    - ~~汇编点灯~~

- 2023.9.27
    - ~~建好GitHub仓库~~
    - 转接板原理图&PCB绘制
    - 选购蓝牙模块
    - 使用库函数让GY-86正常运行
    - usart串口调试GY-86

## Trouble

- 找不到F4的库函数  :（

# Repository - Quadcopter_Group3

> Group 3

## Structure

- #### README.md

    - 本文档，对整个项目做简要介绍。

- #### Hardware

    - Board
        - 一个Markdown说明文档
        - 用于存放使用 Altium Designer 的 Project，包括原理图、PCB板图
    - Board_TEST_XXX
        - 用于存放用于测试的PCB工程
    - Reference
        - 用于存放
            - F401RE白色板子的原理图

- #### Firmware

    - Quadcoper
        - 用于存放 Keil5工程，这个工程将是未来四轴飞行器的最终版本
    - Quadcoper_TEST_XXX
        - 用于存放用于测试的Keil5工程
    - Reference
        - 一个Markdown说明文档
        - 用于存放F4系列参考手册、库函数等等

- #### Notes

    - 各个挑战点的相关文档
    - 在网络平台上搜到的教程

## Branches

> 参考文章：[实际项目中如何使用Git做分支管理](https://zhuanlan.zhihu.com/p/38772378)

<img src="./README.assets/image-20230927213823221.png" alt="image-20230927213823221" style="zoom:25%;" /> <img src="./README.assets/image-20230927213906992.png" alt="image-20230927213906992" style="zoom:25%;" />

- ### develop

    > 日常开发用的分支，平行于main分支。
    >
    > 平时有什么改动都push到这个分支，当develop分支当前的版本运行达到预期时，就向main分支合并一次  

    - #### feature_XXX

        > develop分支的下属分支，每有一个新的功能就新建一个feature分支，开发完毕后，将该feature分支合并回develop分支

- ### main

    > 稳定的，处于待发布状态的版本



