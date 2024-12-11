# Test hardware

## 测试硬件环境 1 (极低配置服务器)
+ OS: Ubuntu 22.04.4 LTS
+ CPU：2核 Intel(R) Xeon(R) Platinum 85
+ GPU：无
+ 内存：2G
+ 硬盘：40G
+ 带宽: 3Mbps

> [!TIP]
> 个人经验：若想尽可能快地更新视频，主要取决于上传速度而非弹幕渲染速度，因此建议网络带宽越大越好。


## 测试硬件环境 2 (低配置显卡电脑)

+ OS: Ubuntu 20.04.4 LTS

> [!WARNING]
> 尽量使用 22.04+ 的版本，更早版本的 ubuntu 自带 gcc 版本无法更新至 DanmakuFactory 以及 biliup-rs 所需版本，若使用较早版本，请参考 [version `GLIBC_2.34‘ not found简单有效解决方法](https://blog.csdn.net/huazhang_001/article/details/128828999)。

+ CPU：Intel(R) Core(TM) i5-9300H CPU 8 核
+ GPU：NVIDIA GeForce GTX 1650 显存 4GB
+ 内存：24G
+ 硬盘：100G
+ 带宽: 50Mbps