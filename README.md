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
### 硬件

OS: Ubuntu 22.04.4 LTS

CPU：2核 Intel(R) Xeon(R) Platinum

GPU：无

内存：2G

硬盘：40G

### 环境
```
pip install -r requirements.txt
```

### 运行
- Windows

```bash
run.bat
```


- Linux

```bash
bash ./run.sh
```
### 配置

#### biliup-rs

首先使用[biliup-rs](https://github.com/biliup/biliup-rs)登录b站，将登录产生的`cookies.json`文件原留在 `biliup` 文件夹即可。

修改 `biliup` 文件夹内的 `config.yaml` 模板，具体修改方式见[biliup-rs上传文档](https://biliup.github.io/biliup-rs/Guide.html#useage)。

#### blrec
- 在 `run.sh` 或 `run.bat` 启动脚本中设置端口 `port`
- 打开 `http://localhost:port` 进入blrec前端界面进行设置。
- 在 `settings.toml` 中设置视频存放目录、日志目录，可使用绝对路径，详见 [blrec](https://github.com/acgnhiki/blrec)，也可以在上一步可视化前端中进行调整。

#### 自动投稿
- 投稿的配置文件为 `upload_config.json`
- 请在将一级键值名称取为**字符串格式**的对应直播间的房间号（4位数以上）。

#### 弹幕版视频压制与上传

```bash
# xml弹幕转换为ass弹幕
../../DanmakuFactory/DanmakuFactory -o "11111111_20240528-22-58-00.ass" -i "11111111_20240528-22-58-00.xml"

# ass弹幕压制进对应的视频中
ffmpeg -i 11111111_20240714-12-15-57.mp4 -vf "ass=11111111_20240714-12-15-57.ass" out.mp4

# 使用biliup根据config.yaml上传对应的视频
./biliup upload /root/blive/Videos/11111111/11111111_20240716-19-35-33.mp4 --config ./config.yaml
```

## 特别感谢

- [biliup/biliup-rs](https://github.com/biliup/biliup-rs)
- [FortuneDayssss/BilibiliUploader](https://github.com/FortuneDayssss/BilibiliUploader)
- [acgnhiki/blrec](https://github.com/acgnhiki/blrec)
- [qqyuanxinqq/AutoUpload_Blrec](https://github.com/qqyuanxinqq/AutoUpload_Blrec)