import os
import logging
from http.server import HTTPServer

# 导入toml模块，用于解析TOML配置文件
import toml

# 从相对路径导入自定义的HTTP请求处理器
from src.httpserver import MyHandler


PORT = 22333

if __name__ == "__main__":
    # 打开并读取TOML配置文件
    with open('settings.toml', 'r', encoding='utf-8') as f:
        settings = toml.load(f)
    # 根据配置获取视频列表文件路径
    video_list_path = os.path.join(settings['output']['out_dir'], "_list")
    # 创建视频列表文件目录，如果已存在则忽略
    os.makedirs(video_list_path, exist_ok=True)
    # 获取上传日志文件路径
    upload_log_dir = os.path.join(settings['logging']['log_dir'],"_upload_log")
    # 创建上传日志目录，如果已存在则忽略
    os.makedirs(upload_log_dir, exist_ok = True)

    # 配置MyHandler所需的路径和配置文件
    MyHandler.config(video_list_path, upload_log_dir, "upload_config.json")
    # 设置服务器监听的地址和端口
    server_address = ('', PORT)
    # 创建HTTP服务器实例，使用MyHandler作为请求处理器
    httpd = HTTPServer(server_address, MyHandler)
    # 打印服务器监听端口信息
    print(f"Listening to port {PORT}...", flush=True)
    try:
        # 运行HTTP服务器，直到遇到错误或中断
        httpd.serve_forever()
    except Exception as e:
        # 如果有异常则记录异常信息
        logging.exception(e)
    # 无论是否出现异常，都会执行以下代码
    finally:
        # 调用MyHandler的shutdown方法进行清理
        MyHandler.shutdown()
        # 关闭HTTP服务器
        httpd.server_close()
        print("Terminated Gracefully!")
