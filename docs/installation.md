
# Quick start
## Mode
首先介绍本项目三种不同的处理模式：
1. `pipeline` 模式(默认): 目前最快的模式，需要 GPU 支持，最好在 `blrec` 设置片段为半小时以内，asr 识别和渲染并行执行，分 p 上传视频片段。
![](https://cdn.jsdelivr.net/gh/timerring/scratchpad2023/2024/2024-12-11-17-33-15.png)
2. `append` 模式: 基本同上，但 asr 识别与渲染过程串行执行，比 pipeline 慢预计 25% 左右，对 GPU 显存要求较低，兼顾硬件性能与处理上传效率。
![](https://cdn.jsdelivr.net/gh/timerring/scratchpad2023/2024/2024-12-11-19-07-12.png)
3. `merge` 模式: 等待所有录制完成，再进行识别渲染合并过程，上传均为完整版录播（非分 P 投稿），等待时间较长，效率较慢，适合需要上传完整录播的场景。
![](https://cdn.jsdelivr.net/gh/timerring/scratchpad2023/2024/2024-12-11-19-08-58.png)

> [!IMPORTANT]
> 凡是用到 GPU 均需保证 GPU 显存大于运行程序所需 VRAM，具体计算 VRAM 方法可以参考[该部分](./models.md#计算-vram-需求)。

## Installation(有 GPU 版本)

> 是否有 GPU 以 `nvidia-smi` 显示 nvidia GPU 驱动以及 `nvcc -V` 显示 `CUDA` 版本号为准。如果未配置显卡驱动或未安装 `CUDA`，即使有 GPU 也无法使用，而会使用 CPU 推理（不推荐，可根据自身硬件条件判断是否尝试 CPU 推理）。

> [!TIP]
> 如果你是 windows 用户，请不要使用命令提示符（Command Prompt）或 Windows PowerShell，请使用 [PowerShell](https://learn.microsoft.com/en-us/powershell/scripting/install/installing-powershell-on-windows?view=powershell-7.4) 或 WSL 或 **Git Bash**(推荐)。
> 
> **注意：PowerShell 和 Windows PowerShell 是[不同的应用程序](https://learn.microsoft.com/en-us/powershell/scripting/whats-new/differences-from-windows-powershell?view=powershell-7.4&viewFallbackFrom=powershell-7.3)。**

### 1. 安装依赖(推荐先 `conda` 创建虚拟环境)

```
cd bilive
pip install -r requirements.txt
```

此外请根据各自的系统类型安装对应的 [`ffmpeg`](https://www.ffmpeg.org/download.html)，例如 [ubuntu 安装 ffmpeg](https://gcore.com/learning/how-to-install-ffmpeg-on-ubuntu/)。

[常见问题收集](./install-questions)

### 2. 设置环境变量用于保存项目根目录

```
./setPath.sh && source ~/.bashrc
```

### 3. 配置 whisper 模型

项目默认采用 [`small`](https://openaipublic.azureedge.net/main/whisper/models/9ecf779972d90ba49c06d968637d720dd632c55bbf19d441fb42bf17a411e794/small.pt) 模型，请点击下载所需文件，并放置在 `src/subtitle/models` 文件夹中。

> [!TIP]
> 使用该参数模型至少需要保证有显存大于 2.7GB 的 GPU，否则请使用其他参数量的模型。
> + 更多模型请参考 [whisper 参数模型](./models) 部分。
> + 更换模型方法请参考 [更换模型方法](./models.md#更换模型方法) 部分。


### 4. biliup-rs 登录

首先按照 [biliup-rs](https://github.com/biliup/biliup-rs) 登录b站，登录脚本在 `src/upload/biliup` ，登录产生的`cookies.json`保留在该文件夹下即可。

[常见问题收集](./biliup)

### 5. 启动自动录制

```bash
./record.sh
```

[常见问题收集](./record)

### 6. 启动自动上传

请先确保你已经完成`步骤 3`，正确下载并放置了模型文件。

#### 6.1 启动扫描渲染进程

输入以下指令即可检测已录制的视频并且自动合并分段，自动进行弹幕转换，字幕识别与渲染的过程：

```bash
./scan.sh
```

[常见问题收集](./scan)

#### 6.2 启动自动上传进程

```bash
./upload.sh
```

[常见问题收集](./upload)


### 7. 查看执行日志

相应的执行日志请在 `logs` 文件夹中查看，如果有问题欢迎在 [`issue`](https://github.com/timerring/bilive/issues/new/choose) 中提出。
```
logs # 日志文件夹
├── blrecLog # blrec 录制日志
│   └── ...
├── burningLog # 弹幕渲染日志
│   └── ...
├── mergeLog # 片段合并日志
│   └── ...
├── scanLog # scan运行日志
│   └── ...
├── uploadLog # 视频上传日志
│   └── ...
└── blrec.log # record.sh 运行日志
```


## Installation(无 GPU 版本)
无 GPU 版本过程基本同上，可以跳过步骤 3，需要注意在执行步骤 5 **之前**完成以下设置将确保完全用 CPU 渲染视频弹幕。

1. 请将 `src/config.py` 文件中的 `GPU_EXIST` 参数设置为 `False`。（若不置为 `False` 且则会使用 CPU 推理，不推荐，可自行根据硬件条件进行尝试。）
2. 将 `MODEL_TYPE` 调整为 `merge` 或者 `append`。
