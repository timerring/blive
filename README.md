<div align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="assets/headerDark.svg" />
    <img src="assets/headerLight.svg" alt="BILIVE" />
  </picture>

*7 x 24 小时无人监守录制、渲染弹幕、自动上传，启动项目，人人都是录播员。*

[:page_facing_up: Documentation](#major-features) |
[:gear: Installation](#quick-start) |
[:thinking: Reporting Issues](https://github.com/timerring/bilive/issues/new/choose)

</div>

## Introduction

> 如果您觉得项目不错，欢迎 :star: 也欢迎 PR 合作，如果有任何疑问，欢迎提 issue 交流。

自动监听并录制B站直播和弹幕、自动转换xml弹幕（含付费留言、礼物等）为ass并渲染进视频，自动投稿**弹幕版视频**和**无弹幕视频**至B站，无需GPU，兼容超低配置服务器与主机，**兼容Windows 和 linux操作系统**。


### Major features

- **速度快**：录制的同时可以选择启动无弹幕版视频的上传进程，下播延迟检测后即可直接上线平台。
- **范围广**：多线程同时监听数个直播间，同时录制内容并投稿。
- **空间小**：自动删除已上传的往期直播回放，节省空间，硬盘空间可以重复利用。
- **灵活高**：模版化自定义投稿，支持自定义投稿分区，动态内容，视频描述，视频标题，视频标签等，同时支持多P上传。
- **自动检测合并**：对于网络问题或者连线导致的视频流分段，支持自动检测并按小时合并视频片段。
- **弹幕版视频**：录制视频同时录制弹幕文件（包含普通弹幕，付费弹幕以及礼物上舰等信息），支持自动转换xml为ass弹幕文件并且渲染到视频中形成**有弹幕版视频**，转换完成后即在上传队列中自动上传。
- **硬件要求低**：无需GPU，只需最基础的单核CPU搭配最低的运存即可完成录制，渲染，上传等等全部过程，10年前的电脑或服务器依然可以使用！

### 测试硬件
+ OS: Ubuntu 22.04.4 LTS

  >尽量使用 22.04+ 的版本，更早版本的 ubuntu 自带 gcc 版本无法更新至 DanmakuFactory 以及 biliup-rs 所需版本，若使用较早版本，请参考 [version `GLIBC_2.34‘ not found简单有效解决方法](https://blog.csdn.net/huazhang_001/article/details/128828999)。
+ CPU：2核 Intel(R) Xeon(R) Platinum 85
+ GPU：无
+ 内存：2G
+ 硬盘：40G
+ 带宽: 3Mbps
  > 个人经验：若想尽可能快地更新视频，主要取决于上传速度而非弹幕渲染速度，因此建议带宽越大越好。

> [!TIP]
> 关于渲染速率：与弹幕数量有关，测试硬件的基本区间 2核 Xeon(R) Platinum 85 的 CPU 的渲染速率在 3 ~ 6 倍之间，也可使用 Nvidia GPU 加速，项目的测试显卡为 GTX1650，其渲染速率在 16 ～ 20 倍之间。 
> 
> 弹幕渲染具体时间可通过 `渲染速率x视频时长` 估算，如无需 GPU 加速渲染过程，请忽略本条提示。
> 
> 如需使用 Nvidia GPU 加速，
> 请参考：
> + [Using FFmpeg with NVIDIA GPU Hardware Acceleration](https://docs.nvidia.com/video-technologies/video-codec-sdk/12.0/ffmpeg-with-nvidia-gpu/index.html)
> + [使用GPU为FFmpeg 加速](https://yukihane.work/li-gong/ffmpeg-with-gpu)

## Quick start
### 环境
```
pip install -r requirements.txt
```
### biliup-rs 登录
首先使用[biliup-rs](https://github.com/biliup/biliup-rs)登录b站，将登录产生的`cookies.json`文件复制一份到项目根目录中。

### 自动录制运行

- 在 `startRecord.sh` 或 `startRecord.bat` 启动脚本中设置端口 `port`
- 在 `settings.toml` 中设置视频存放目录、日志目录，可使用绝对路径，详见 [blrec](https://github.com/acgnhiki/blrec)，也可在 blrec 前端界面进行设置与添加（直观）。
  - 打开 `http://localhost:port` 进入blrec前端界面进行设置。

然后执行：

```bash
./startRecord.sh
```

### 无弹幕版自动上传

- 投稿的配置文件为 `upload_config.json`，可以参考给出的示例添加。
- 请在将一级键值名称取为**字符串格式**的对应直播间的房间号（4位数以上）。
- 自动投稿将在录制的同时启动上传进程，结束后几分钟内即可上传完成（本过程依据配置全自动进行，无需人为操作）。


### 弹幕版视频渲染与自动上传

#### 启动弹幕渲染进程

输入以下指令即可开始检测已录制的视频并且自动合并分段，自动进行弹幕转换与渲染的过程：


```bash
./startScan.sh
```

#### 上传配置

> 请先执行一次 `./setRoutineTask.sh`，将项目根目录文件存储。

参照 `upload/config` 文件夹内的 `22230707.yaml` 模板，添加你需要录制的房间信息，如有多个房间，请添加多个`roomid.yaml`文件，具体见[biliup-rs上传文档](https://biliup.github.io/biliup-rs/Guide.html#useage)。

#### 启动自动上传进程

输入以下指令即可自动使上传队列中的视频匹配对应模版并自动上传：

```bash
./startUpload.sh
```

> [!NOTE]
> 相应的执行结果请在 `logs` 文件夹中查看。
> ```
> logs # 日志文件夹
> ├── blrecLog # blrec 录制日志
> │   └── ...
> ├── burningLog # 弹幕渲染日志
> │   └── ...
> ├── mergeLog # 片段合并日志
> │   └── ...
> ├── uploadDanmakuLog # 有弹幕版上传日志
> │   └── ...
> ├── uploadNoDanmakuLog # 无弹幕版上传日志
> │   └── ...
> ├── removeEmojis.log # 移除弹幕表情日志
> ├── scanSegments.log # startScan.sh 运行日志
> └── uploadQueue.log # startUpload.sh 运行日志
> ```

## 特别感谢

- [biliup/biliup-rs](https://github.com/biliup/biliup-rs)
- [FortuneDayssss/BilibiliUploader](https://github.com/FortuneDayssss/BilibiliUploader)
- [hihkm/DanmakuFactory](https://github.com/hihkm/DanmakuFactory)
- [acgnhiki/blrec](https://github.com/acgnhiki/blrec)
- [qqyuanxinqq/AutoUpload_Blrec](https://github.com/qqyuanxinqq/AutoUpload_Blrec)