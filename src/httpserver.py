# 导入HTTP服务器和请求处理器
from http.server import SimpleHTTPRequestHandler, HTTPServer
# 导入JSON模块，用于解析JSON数据
import json
# 导入日志模块，用于记录日志
import logging
# 导入操作系统接口模块
import os
# 从相对路径导入Live模块
from .Live import Live
# 从相对路径导入自定义进程模块
from .Myproc import Myproc
# 从相对路径导入上传配置函数
from .blive_upload import configured_upload
# 从相对路径导入获取直播标题的函数
from .api import get_title

# 定义MyHandler类，继承自SimpleHTTPRequestHandler
class MyHandler(SimpleHTTPRequestHandler):
    # 类属性定义

    # 存储房间ID和对应的视频列表文件路径的字典
    # dict room_id: list_file path
    room_ids = {}
    # 存储活动视频文件和对应列表文件路径的字典
    # dict active video_file path (not full path, root only, without extension): list_file path
    videos_active = {}
    # 存储等待所有视频完成的列表文件路径的集合
    # set of list_file path for each list_file that is waiting for all videos to be finalized
    lists_fin_wait = set()
    
    # 视频列表文件存储目录
    _video_list_directory = ""
    # 上传日志文件存储目录
    _upload_log_dir = ""
    # 需要上传的房间ID列表
    _upload_list = []
    
    # 类方法，用于配置目录路径
    @classmethod
    def config(cls, video_list_path,upload_log_dir, upload_config_path):
        # 设置视频列表文件目录
        cls._video_list_directory = video_list_path
        # 设置上传日志目录
        cls._upload_log_dir = upload_log_dir
        # 设置上传配置文件路径
        cls.upload_config_path = upload_config_path

     # 类方法，用于从配置文件加载上传列表
    @classmethod
    def set_upload_list(cls):
        with open("upload_config.json", 'r', encoding='utf-8') as f:
            # 加载JSON配置
            config = json.load(f)
        # 将配置文件中的键（房间ID）列表赋值给上传列表
        cls._upload_list = list(config.keys())

    # 处理POST请求的方法
    def do_POST(self):
        # 获取请求体长度
        content_length = int(self.headers['Content-Length'])
        # 读取并解码请求体数据
        post_data = self.rfile.read(content_length).decode('utf-8')

        try:
            # Deserialize the JSON payload
            # 将字符串解析为JSON对象
            event = json.loads(post_data)
            # 记录事件
            self.log_event(event)
            # 处理事件
            self.handle_event(event)
        # 记录异常
        except Exception as e:
            logging.exception(e)


        # You might want to send a response back to the client
        # Here, we send a simple "OK" text.
        # 发送200状态码
        self.send_response_only(200)
        # 设置响应头部
        self.send_header('Content-type', 'text/plain')
        # 结束头部
        self.end_headers()
        # 发送响应体
        self.wfile.write(b'OK')

    # 记录事件的方法
    def log_event(self, event):
        # 获取事件类型
        event_type = event.get("type", "")
        # 根据事件数据获取房间ID
        if event['data'].get('room_id'):
            room_id = event['data']['room_id']
        elif event['data'].get('room_info'):
            room_id = event['data']['room_info']['room_id']
        else:
            room_id = ""
        # 格式化日志信息
        format = f"[{room_id}] {event_type}"
        # 记录日志信息
        self.log_message(format)

    # 静态方法，用于处理事件
    @staticmethod
    def handle_event(event):
        # Extract the event type
        event_type = event.get("type", "")
        # 根据事件类型调用相应的处理函数
        # Handle the event according to its type      
        if event_type == "RecordingStartedEvent":
            MyHandler.handle_recording_started(event)
            MyHandler.set_upload_list()
            MyHandler.upload(event)
        elif event_type == "RoomChangeEvent":
            MyHandler.handle_room_change(event)
        elif event_type in ["RecordingFinishedEvent", "RecordingCancelledEvent"]:
            MyHandler.handle_recording_finished(event)
        elif event_type == "VideoFileCreatedEvent":
            MyHandler.handle_video_create(event)
        elif event_type == "VideoPostprocessingCompletedEvent":
            MyHandler.handle_video_file_completed(event)
        elif event_type == "Error":
            logging.error(f"Error event occurred at {event['date']}. Error message: {event['data']}")
        else:
            # 忽略其他类型的事件
            pass

    # 类方法定义，用于上传视频、处理直播开始、房间变更、视频创建和视频处理完成等事件
    @classmethod
    def upload(cls, event):
        """
        Upload the video to Bilibili, if room_id is in upload_list.
        """
        # 上传视频到Bilibili，如果房间ID在上传列表中
        room_id = event['data']['room_info']['room_id']

        if str(room_id) in cls._upload_list:
            # 创建日志文件路径
            logfile = os.path.join(
                cls._upload_log_dir,
                os.path.basename(cls.room_ids[room_id]).split('.')[0] + '.log'
                )
            # 创建上传进程
            p = Myproc(target = configured_upload, args = (cls.room_ids[room_id], cls.upload_config_path), name="[{}]Uploader".format(room_id))
            # 设置日志文件
            p.set_output_err(logfile)
            # 启动进程
            p.start()
            # 设置进程结束后的操作
            p.post_run()
            print("=============================", flush=True)
            print("开始上传"+ str(room_id), flush=True)
            print("=============================", flush=True)
        else:
            # 如果房间ID不在上传列表中，记录警告日志
            logging.warning(f"upload: Room {room_id} not found in upload list{cls._upload_list}")
    
    # 处理直播开始事件，创建一个新的视频列表文件，并记录房间ID
    @classmethod
    def handle_recording_started(cls, event:dict):
        """
        Create a new video list file, and add dumped filename to room_ids.
        """
        # 创建Live对象
        live = Live()
        # 从事件中获取房间ID
        room_id = event['data']['room_info']['room_id']
        
        # 尝试获取直播标题
        title = get_title(room_id)
        if title is None:
            # 如果获取失败，则从事件数据中获取
            title = event['data']['room_info']['title']

        # 使用事件数据创建直播记录
        live.create_v1(
                room_id = room_id,
                start_time = event['date'],
                live_title = title,
                status="Living"
                )
        # 保存直播记录并添加到room_ids字典
        cls.room_ids[room_id] = live.dump(path = MyHandler._video_list_directory)
    
    # 处理直播结束或取消事件，从字典中移除房间ID，并根据是否有活跃视频决定是等待还是直接完成列表文件。
    @classmethod
    def handle_recording_finished(cls, event:dict):
        """
        Pop the video list file from room_ids. 
        Put it into lists_fin_wait if there is video active, otherwise finalize it.
        """
        room_id = event['data']['room_info']['room_id']
        try:
            # 从room_ids字典中弹出视频列表文件路径
            list_file = cls.room_ids.pop(room_id)
            # 如果还有活跃的视频
            if list_file in cls.videos_active.values():
                # 将列表文件添加到等待完成的集合中
                cls.lists_fin_wait.add(list_file)
            else:
                # 调用完成列表文件的处理
                cls.finalize_list(list_file)

        except Exception as e:
            logging.exception(e)

    # 完成视频列表文件的处理，设置状态为完成，并保存更改。
    @classmethod
    def finalize_list(cls, list_file):
        """
        Finalize the video list file, set status to Done.
        """
        # 创建Live对象并加载视频列表文件
        live = Live(filename = list_file)
        # 更新直播状态为完成
        live.update_live_status("Done")
        # 保存更改
        live.dump()
        # 记录日志
        logging.warning(f"Video list '{list_file}' is finished.")        
        
    @classmethod
    def handle_room_change(cls, event:dict):
        # 提取房间ID
        room_id = event['data']['room_info']['room_id']
        # 尝试获取房间的新标题
        title = get_title(room_id)
        # 如果获取不到，则使用事件中提供的标题
        if title is None:
            title = event['data']['room_info']['title']
        # 如果房间ID存在于room_ids字典中，说明我们有这个房间的记录
        if room_id in cls.room_ids:
            # 加载对应的Live对象
            live = Live(filename = cls.room_ids[room_id])
            # 更新直播标题
            live.update_live_title_now(title)
            # 保存Live对象的更改
            live.dump()
        else:
            # 如果room_ids中没有这个房间ID，记录一条警告日志
            logging.warning(f"handle_room_change: Room {room_id} not found in data store")
    
    @classmethod
    def handle_video_create(cls, event:dict):
        # 通过将新视频添加到活动视频来处理视频创建事件并更新相应的活动对象。
        # 提取房间ID
        room_id = event['data']['room_id']
        # 提取视频文件路径
        filename = event['data']['path']
        # 尝试获取房间标题
        title = get_title(room_id)

        # 如果room_ids字典中有这个房间ID，说明我们有这个房间的记录
        if room_id in cls.room_ids:
            # 加载Live对象
            live = Live(filename = cls.room_ids[room_id])
            if title is not None:
                # 如果获取到了标题，则更新
                live.update_live_title_now(title)
            
            # 添加视频到Live对象
            live.add_video_now_v1(
                    start_time = event['date'],
                    filename = filename
                    )
            # 保存Live对象的更改
            list_file = live.dump()
            # 将视频文件名（不含扩展名）与列表文件路径关联起来
            (root, ext) = os.path.splitext(filename)
            cls.videos_active[root] = list_file
        else:
            # 如果room_ids中没有这个房间ID，记录一条警告日志
            logging.warning(f"handle_video_create: Room {room_id} not found in data store")

    # 当一个视频文件完成后，将其最终确定在相应的live对象中，并检查家长名单是否也可以最终确定
    @classmethod
    def handle_video_file_completed(cls, event:dict):
        """
        Finalize the video file, and remove it from videos_active.
        Then check if the paraent list is in lists_fin_wait, finalize it if no more video in the list is active.
        """
        # 提取视频文件路径
        filename = event['data']['path']
        # 从视频文件名中提取文件名和扩展名
        (root, ext) = os.path.splitext(filename)
        # 从活跃视频中移除该视频文件
        list_file = cls.videos_active.pop(root)
        
        # 加载Live对象
        live = Live(filename = list_file)
        # 完成视频文件的记录
        live.finalize_video_v1(
                filename = filename
                )
        # 保存Live对象的更改
        live.dump()

        # 如果列表文件在等待完成的集合中，并且没有活跃的视频了，完成列表文件
        # if the list is in lists_fin_wait fin_wait, finalize it if no video is active
        if list_file in cls.lists_fin_wait:
            if list_file not in cls.videos_active.values():
                # 完成列表文件
                cls.finalize_list(list_file)
                # 从等待完成的集合中移除
                cls.lists_fin_wait.remove(list_file)
            
    # shutdown方法确保在服务器关闭时，所有直播记录的状态都被正确设置为完成，这是一个清理过程，以确保数据的一致性。
    @classmethod
    def shutdown(cls):
        """
        Set status of all video list files to Done.
        """
        # 遍历所有房间ID
        for room_id in cls.room_ids:
            # 加载Live对象
            live = Live(filename = cls.room_ids[room_id])
            # 设置状态为Done
            live.update_live_status("Done")
            # 保存更改
            live.dump()
        # 遍历等待完成的列表文件
        for list_file in cls.lists_fin_wait:
            # 加载Live对象
            live = Live(filename = list_file)
            # 设置状态为Done
            live.update_live_status("Done")
            # 保存更改
            live.dump()

