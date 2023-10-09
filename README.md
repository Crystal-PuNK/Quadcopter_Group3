> # Quadcopter_Group3
>
> 第三组的四轴飞行器Github仓库~

# 进阶式挑战性综合课程设计
* 请前往 `develop` 分支查看工程、笔记、参考手册等文件*

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
- ### develop

    > 日常开发用的分支，平行于main分支。
    >
    > 平时有什么改动都push到这个分支，当develop分支当前的版本运行达到预期时，就向main分支合并一次  

    - #### feature_XXX

        > develop分支的下属分支，每有一个新的功能就新建一个feature分支，开发完毕后，将该feature分支合并回develop分支

- ### main

    > 稳定的，处于待发布状态的版本



