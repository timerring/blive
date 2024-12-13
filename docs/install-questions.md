# 安装常见问题

> 如果没有找到遇到的问题，请及时在 [issues](https://github.com/timerring/bilive/issues/new/choose) 中提出。

## OSError: sndfile library not found

Reference: https://github.com/timerring/bilive/issues/106

解决方案：安装对应的库即可 `apt-get install libsndfile1`。

## Error /lib/x86_64-linux-gnu/libc.so.6: version `GLIBC_2.34’ not found

Reference: https://blog.csdn.net/huazhang_001/article/details/128828999

尽量使用 22.04+ 的版本，更早版本的 ubuntu 自带 gcc 版本无法更新至 DanmakuFactory 以及 biliup-rs 所需版本。

解决方案：手动更新版本，参照链接操作即可。