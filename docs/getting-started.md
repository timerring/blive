
# Introduction

自动监听并录制B站直播和弹幕、自动转换xml弹幕（含付费留言、礼物等）为ass并渲染进视频，自动投稿**弹幕版视频**和**无弹幕视频**至B站，无需GPU，兼容超低配置服务器与主机，**兼容Windows 和 linux操作系统**。

## Major features

- **速度快**：采用 `pipeline` 流水线处理视频，理想情况下录播与直播相差半小时以内，没下播就能上线录播！
- **多房间**：同时录制多个直播间内容视频以及弹幕文件（包含普通弹幕，付费弹幕以及礼物上舰等信息）。
- **占用小**：自动删除本地已上传的视频，极致节省空间。
- **模版化**：无需复杂配置，开箱即用，( :tada: NEW)通过 b 站搜索建议接口自动抓取相关热门标签。
- **检测片段并合并**：对于网络问题或者直播连线导致的视频流分段，能够自动检测合并成为完整视频。
- **渲染弹幕版视频**：自动转换xml为ass弹幕文件并且渲染到视频中形成**有弹幕版视频**并自动上传。
- **硬件要求极低**：无需GPU，只需最基础的单核CPU搭配最低的运存即可完成录制，弹幕渲染，上传等等全部过程，无最低配置要求，10年前的电脑或服务器依然可以使用！
- **( :tada: NEW)自动渲染字幕**(如需使用本功能，则需保证有 Nvidia 显卡)：采用 OpenAI 的开源模型 [`whisper`](https://github.com/openai/whisper)，自动识别视频内语音并转换为字幕渲染至视频中。


## Architecture diagram

项目架构流程如下：

```mermaid
graph TD
        User((用户))--record-->startRecord(启动录制)
        startRecord(启动录制)--保存视频和字幕文件-->videoFolder[(Video 文件夹)]

        User((用户))--scan-->startScan(启动扫描 Video 文件夹)
        videoFolder[(Video 文件夹)]<--间隔两分钟扫描一次-->startScan(启动扫描 Video 文件夹)
        startScan <--视频文件--> whisper[whisperASR模型]
        whisper[whisperASR模型] --生成字幕-->parameter[查询视频分辨率]
        subgraph 启动新进程
        parameter[查询分辨率] -->ifDanmaku{判断}
        ifDanmaku -->|有弹幕| DanmakuFactory[DanmakuFactory]
        ifDanmaku -->|无弹幕| ffmpeg1[ffmpeg]
        DanmakuFactory[DanmakuFactory] --根据分辨率转换弹幕--> ffmpeg1[ffmpeg]
        end
        ffmpeg1[ffmpeg] --渲染弹幕及字幕 --> uploadQueue[(上传队列)]

        User((用户))--upload-->startUpload(启动视频上传进程)
        startUpload(启动视频上传进程) <--扫描队列并上传视频--> uploadQueue[(上传队列)]
```
