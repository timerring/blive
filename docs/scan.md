# scan 常见问题

## 关于渲染速率

渲染速率主要与硬件以及弹幕数量有关，测试硬件的基本区间 2核 Xeon(R) Platinum 85 的 CPU 的渲染速率在 3 ~ 6 倍之间，也可使用 Nvidia GPU 加速，项目的测试显卡为 GTX1650，其渲染速率在 16 ～ 20 倍之间。 

弹幕渲染具体时间可通过 `渲染速率x视频时长` 估算。

使用 Nvidia GPU 加速的相关参考：
+ [Using FFmpeg with NVIDIA GPU Hardware Acceleration](https://docs.nvidia.com/video-technologies/video-codec-sdk/12.0/ffmpeg-with-nvidia-gpu/index.html)
+ [使用GPU为FFmpeg 加速](https://yukihane.work/li-gong/ffmpeg-with-gpu)


## requests 请求错误
```
requests.exceptions.ConnectionError: ('Connection aborted.', ConnectionResetError(104, 'Connection reset by peer'))
```

Reference: https://stackoverflow.com/questions/70379603/python-script-is-failing-with-connection-aborted-connectionreseterror104

解决方案：网络问题，你应该知道怎么做。

## 字体信息错误
```
Glyph 0x... not found, selecting one more font for (Microsoft YaHei, 400, 0)
```
Reference：https://github.com/timerring/bilive/issues/35

解决方案：通常 ffmpeg 无法渲染表情，因此很多情况下关于表情的渲染都会报错，我已经通过一个通用正则滤除了 99% 的表情，当然可能会有其他奇怪的表情和字符，如果报错忽略即可，ffmpeg 会渲染为一个方框，不影响效果。