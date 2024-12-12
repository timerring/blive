# upload 常见问题

## 上传默认参数

上传默认参数如下，[]中内容全部自动替换：
+ 默认标题是"【弹幕+字幕】[XXX]直播回放-[日期]-[直播间标题]"。
+ 默认描述是"【弹幕+字幕】[XXX]直播，直播间地址：[https://live.bilibili.com/XXX] 内容仅供娱乐，直播中主播的言论、观点和行为均由主播本人负责，不代表录播员的观点或立场。"
+ 默认标签是根据主播名字自动在 b 站搜索推荐中抓取的[热搜词]，详见[bilibili-API-collect](https://github.com/SocialSisterYi/bilibili-API-collect/blob/master/docs/search/suggest.md)。

上传会根据以上模版生成上传视频对应的 `yaml` 配置文件，也可在 `src/upload/extract_video_info.py` 中自定义相关配置。