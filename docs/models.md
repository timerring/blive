# whisper 参数模型

本项目采用 [OpenAI 开源的 whisper 模型](https://github.com/openai/whisper)进行 Automatic Speech Recognition (ASR) 任务。

## 模型信息
模型基本参数参数及链接如下，注意 GPU 显存必须大于所需 VRAM：

> [!TIP]
> 如果追求识别准确率，推荐使用参数量 `small` 及以上的模型。

|  Size  | Parameters | Multilingual model | Required VRAM |
|:------:|:----------:|:------------------:|:-------------:|
|  tiny  |    39 M    |       [`tiny`](https://openaipublic.azureedge.net/main/whisper/models/65147644a518d12f04e32d6f3b26facc3f8dd46e5390956a9424a650c0ce22b9/tiny.pt)       |     ~1 GB     |
|  base  |    74 M    |       [`base`](https://openaipublic.azureedge.net/main/whisper/models/ed3a0b6b1c0edf879ad9b11b1af5a0e6ab5db9205f891f668f8b0e6c6326e34e/base.pt)       |     ~1 GB     |
| small  |   244 M    |      [`small`](https://openaipublic.azureedge.net/main/whisper/models/9ecf779972d90ba49c06d968637d720dd632c55bbf19d441fb42bf17a411e794/small.pt)       |     ~2 GB     |
| medium |   769 M    |      [`medium`](https://openaipublic.azureedge.net/main/whisper/models/345ae4da62f9b3d59415adc60127b97c714f32e89e936602e85993674d08dcb1/medium.pt)      |     ~5 GB     |
| large  |   1550 M   |      [`large`](https://openaipublic.azureedge.net/main/whisper/models/81f7c96c852ee8fc832187b0132e569d6c3065a3252ed18e56effd0b6a73e524/large-v2.pt)       |    ~10 GB     |


## 计算 VRAM 需求

用 Nvidia 显卡加速 ffmpeg 渲染过程，每个任务所需的 VRAM 约为 180 MB。`whisper` 模型运行所需显存如上表所示。
因此可以大约计算所需显存。

以 `small` 模型为例:
+ 如果采用 `pipeline` 模式，由于并行运行，则运行至少需要 180 + 2620 = 2800 MB 显存。
+ 如果采用 `append` 或者 `merge` 模式，则运行至少需要 2620 MB 显存。

> [!WARNING]
> 请一定保证 GPU 显存大于计算结果，否则会爆显存，`RuntimeError: CUDA out of memory.`。

## 更换模型方法

1. 请将 `src/settings.ini` 文件中的 `Mode` 参数设置为模型对应Size名称，如 `tiny`，`base`，`small`，`medium`，`large`。
2. 将对应的模型文件下载，并放置在 src/subtitle/models 文件夹中。
3. 重新运行 `./scan.sh` 脚本。
