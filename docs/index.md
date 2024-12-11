---
# https://vitepress.dev/reference/default-theme-home-page
layout: home

hero:
  name: "bilive"
  text: "Official documentation"
  tagline: 7 x 24 小时无人监守录制、渲染弹幕、识别字幕、自动上传，启动项目，人人都是录播员。
  actions:
    - theme: brand
      text: 现在开始
      link: /getting-started
    - theme: alt
      text: 在 GitHub 上查看
      link: https://github.com/timerring/bilive

features:
  - title: 速度快
    details: 采用 pipeline 流水线处理视频，理想情况下录播与直播相差半小时以内，没有下播前就上传录播!
  - title: 多房间
    details: 同时录制多个直播间直播以及弹幕（包含普通弹幕，付费弹幕以及礼物上舰等信息）。
  - title: 占用小
    details: 自动删除本地已上传的视频，极小的空间也能运行。
  - title: 模版化
    details: 无需复杂配置，开箱即用，(🎉NEW)通过 b 站搜索建议接口自动抓取相关热门标签。
  - title: 多模式
    details: 除了 pipeline 模式，还支持 append 以及 merge 模式，对于网络问题或者直播连线导致的视频流分段，能够自动检测合并成为完整视频。
  - title: fine tune 渲染弹幕
    details: 自动转换 xml 为 ass 弹幕文件并且渲染到视频中形成有弹幕版视频并自动上传。根据不同分辨率的视频有 fine tune 的渲染参数。
  - title: 硬件要求低
    details: 即使无 GPU ，只用最基础的单核 CPU 搭配最低的运存即可完成录制，弹幕渲染，上传等等全部过程，无最低配置要求，10 年前的电脑或服务器依然可以使用！
  - title: (🎉NEW) 自动渲染字幕(需要Nvidia 显卡)
    details: 采用 OpenAI 的开源模型 whisper ，自动识别视频内语音并转换为字幕渲染至视频中。
---

