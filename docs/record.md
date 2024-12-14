# record 常见问题

> 如果没有找到遇到的问题，请及时在 [issues](https://github.com/timerring/bilive/issues/new/choose) 中提出。

## 录制 cookies 用处是什么

主要用于录制时验证账号，B 站默认未登录状态下最高画质参数为 250 (超清)，如果想要录制更高参数的画质视频，需要登录账号，请在配置文件中指定账号的 cookies。 

获取 cookies 的方法可以参考：https://zmtblog.xdkd.ltd/2021/10/06/Get_bilibili_cookie/

可以在项目目录下的 `settings.toml` 文件中指定 cookies，也可以在录制面板的 `http://localhost:2233/settings` （默认启动端口 2233，可以自行修改）中的 settings 填写 cookies。
```
[header]
user_agent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.114 Safari/537.36"
cookie = "xxxx"
```

重启 record 后，选择更高画质录制即可。

> [!TIP]
> 不建议使用 cookies 登录 blrec 录制，因为 B 站官方有时会风控，导致录制全部失败。此外，cookies 登录后，录制效果不太稳定，可能会出现抓取不到对应流的情况。
> 
> 因此个人更推荐 **不使用 cookies 的默认录制 250 超清画质**，目前没有获取流的问题。

## 如果端口已经占用
如果端口冲突，请在 `record.sh`启动脚本中重新设置端口 `port`并重启（默认端口号 2233）。

## 如何调整目录
录制部分采用的是 `blrec`，在 `settings.toml` 中设置视频存放目录、日志目录，也可启动后在 blrec 前端界面即`http://localhost:port` 中进行设置。详见 [blrec](https://github.com/acgnhiki/blrec)。

> [!TIP]
> 如果要调整目录，请相应调整 `src/config.py` 中的值。

## 完全无法获取直播流信息

```
[2023-06-15 13:07:09,375] [DEBUG] [parse] [xxxxxxxx] Error occurred while parsing stream: ValueError('12 is not a valid CodecID')
Traceback (most recent call last):
```

通常这种情况发生在录制主播在海外的情况，由于海外直播流编码是 HEVC，但是只支持处理 AVC 编码，因此无法获取直播流信息。
参考 issue：https://github.com/BililiveRecorder/BililiveRecorder/issues/470

解决方法：切换录制编码，选择 hls 编码，然后重新录制。

## 添加房间失败

```
[2024-11-22 14:29:04,304] [ERROR] [task_manager] Failed to add task xxxxxxxxx due to: KeyError('sex')
[2024-11-22 14:29:04,305] [CRITICAL] [exception_handler] KeyError
```

通常是短时间内添加该房间次数过多，api 请求限制了，可以等几分钟再添加该房间，也可以打开 port 对应面板后在面板里手动添加。

## 已经在录制状态,重启项目又无法正常工作

重启需要等约半分钟，因为它添加房间并且验证启动弹幕服务器需要一点时间，可以尝试关闭该房间录制，再打开。

