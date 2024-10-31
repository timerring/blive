# blive
自动监听、B站直播录制和弹幕、自动转换xml弹幕（含付费留言、礼物等）为ass并压制进视频，自动投稿**弹幕版视频**和**无弹幕视频**至B站，无需GPU，兼容超低配置服务器与主机，**兼容Windows 和 linux操作系统**。
Feature：

- **速度快**：录制的同时启动**无弹幕版**视频的上传进程，下播延迟检测（防止误关播）五分钟后即可直接上线平台。
- **范围广**：多线程同时监听数个直播间，同时录制内容并投稿。
- **空间小**：自动删除已上传的往期直播回放，节省空间，硬盘空间可以重复利用。
- **灵活高**：模版化自定义投稿，支持自定义投稿分区，动态内容，视频描述，视频标题，视频标签等，同时支持多P上传。
- **弹幕版视频**：录制视频同时录制弹幕文件（包含普通弹幕，付费弹幕以及礼物等信息），支持自动转换xml为ass弹幕文件并且压制到视频中形成**有弹幕版视频**，每天固定时间自动上传。
- **硬件要求低**：无需GPU，只需最基础的单核CPU搭配最低的运存即可完成录制，压制，上传等等全部过程，15年前的电脑依然可以使用！
## 基本结构
### 测试硬件
OS: Ubuntu 22.04.4 LTS
CPU：2核 Intel(R) Xeon(R) Platinum
GPU：无
内存：2G
硬盘：40G
带宽: 3Mbps
### 环境
```
pip install -r requirements.txt
```
### 自动录制运行

- 在 `startRecord.sh` 或 `startRecord.bat` 启动脚本中设置端口 `port`
- 在 `settings.toml` 中设置视频存放目录、日志目录，可使用绝对路径，详见 [blrec](https://github.com/acgnhiki/blrec)，也可在 blrec 前端界面进行设置与添加（直观）。
  - 打开 `http://localhost:port` 进入blrec前端界面进行设置。

然后执行：
- Windows
```bash
./startRecord.bat
```
- Linux
```bash
./startRecord.sh
```

### 自动投稿配置
#### biliup-rs 登录
首先使用[biliup-rs](https://github.com/biliup/biliup-rs)登录b站，将登录产生的`cookies.json`文件复制一份到项目根目录中。

#### 无弹幕实时自动投稿

- 投稿的配置文件为 `upload_config.json`，可以参考给出的示例添加。
- 请在将一级键值名称取为**字符串格式**的对应直播间的房间号（4位数以上）。
- 自动投稿将在录制的同时启动上传进程，结束后几分钟内即可上传完成（本过程依据配置全自动进行，无需人为操作）。
#### 弹幕版视频压制与上传

> 请先执行一次 `./setRoutineTask.sh`，将项目根目录文件存储。

参照 `upload/config` 文件夹内的 `22230707.yaml` 模板，添加你需要录制的房间信息，如有多个房间，请添加多个`roomid.yaml`文件，具体见[biliup-rs上传文档](https://biliup.github.io/biliup-rs/Guide.html#useage)。

弹幕版视频现需手动操作，在`upload`目录下输入一条指令即可自动让已录制的视频匹配模版，自动进行弹幕转换与压制，自动上传的全过程：

```bash
./operateStart.sh [/absolute/path/to/your/video]
```
### 注意事项

>  1. `[/absolute/path/to/your/video]` 代表可选参数，您可以在执行时指定只操作该文件夹的视频。如果没有指定路径，则使用默认的视频文件夹。该默认路径是硬编码完成的，因此如果只执行 `./operateStart.sh` 而不添加参数，请在启动时请确保视频保存路径与固定的路径一致，即`/root/blive/Videos`，如果不一致也可以自行替换 `operateStart.sh`脚本里的 `target_folder` 变量为你保存视频的根目录。
> 
> 2. 由于不同机器的配置不同，因此压制弹幕的速率也不同，如果机器无GPU且CPU性能极差，**不建议两种上传方式**都开启，二选一即可。弹幕压制具体时间可通过 `压制速率x视频时长` 估算。


## 特别感谢

- [biliup/biliup-rs](https://github.com/biliup/biliup-rs)
- [FortuneDayssss/BilibiliUploader](https://github.com/FortuneDayssss/BilibiliUploader)
- [acgnhiki/blrec](https://github.com/acgnhiki/blrec)
- [qqyuanxinqq/AutoUpload_Blrec](https://github.com/qqyuanxinqq/AutoUpload_Blrec)