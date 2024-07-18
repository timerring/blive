### DanmakuFactory CLI 参数说明

#### 用法

```DanmakuFactory -o [输出文件格式] <输出文件名> -i [输入文件1模板] 输出文件1文件名  [输入文件2模板] 输出文件2文件名  ... [-t 输入文件1时间偏移 ...] [设定名称 设定值] ...```

#### 设定

| 选项（设定名称） | 解释                     | 有效值                                         | 举例                           |
| ---------------- | ------------------------ | ---------------------------------------------- | ------------------------------ |
| -r, --resolution | 指定分辨率               | 两个大于0的整数                                | -r 1920x1080<br />-r 1920 1080 |
| -s, --scrolltime | 指定滚动弹幕的通过时间   | 大于0.0的浮点数                                | -s 12.0                        |
| -f, --fixtime    | 指定固定弹幕的显示时间   | 大于0.0的浮点数                                | -f 12.0                        |
| -d, --density    | 指定弹幕密度（条）       | -1 不重叠<br />0 无限制<br />其他 表示限定条数 | -d -1                          |
| -S, --fontsize   | 指定文字大小             | 大于0的整数                                    | -S 38                          |
| -N, --fontname   | 指定字体名               | 字符串                                         | -N “Microsoft YaHei”           |
| -O, --opacity    | 指定不透明度，越小越透明 | 1-255的整数                                    | -O 255                         |
| -L, --outline    | 指定描边成都             | 0-4整数                                        | -L 4                           |
| -D, --shadow | 指定阴影深度 | 0-4整数 | -D 4 |
| -B, --bold | 指定是否使用粗体字 | true / false | -B true |
| --displayarea | 全部弹幕的显示范围 | 0.0-1.0浮点数 | --displayarea 0.5 |
| --scrollarea | 滚动弹幕的显示范围 | 0.0-1.0浮点数 | --scrollarea 0.5 |
| -b, --blockmode | 设置按类型屏蔽 | 要屏蔽的弹幕类型名称，用-连接<br />L2R, R2L, TOP, BOTTOM, SPECIAL, COLOR, REPEAT | -b L2R-TOP-BOTTOM |
| --statmode | 设置统计模式 | 要显示的统计图型名称，用-连接<br /> TABLE, HISTOGRAM | --statmode TABLE, HISTOGRAM |
| --showusernames | 是否显示用户名（如有） | true / false | --showusernames true |
| --showmsgbox | 是否显示礼物框 | true / false | --showmsgbox true |
| --msgboxsize | 礼物框尺寸 | 两个大于0的整数 | --msgboxsize 400x1000 |
| --msgboxpos | 礼物框位置 | 两个大于0的整数 | --msgboxpos 50x50 |
| --msgboxfontsize | 礼物框文字大小 | 大于0的整数 | --msgboxfontsize 25 |
| --msgboxduration | 消息框的持续时长 | 大于0.0的浮点数 | --msgboxduration 5.0 |
| --giftminprice | 显示礼物的最小价值 | 大于0的整数 | --giftminprice 10 |
| --giftmergetolerance | 同一用户相同礼物自动合并的时间窗 | 大于0.0的浮点数 | --giftmergetolerance 10.0 |



