import json
import os
from datetime import datetime
import collections

# 导入FileLock，用于处理文件写入时的锁，避免数据冲突
from filelock import FileLock
class Live():
    '''
    Provide the mapping between python dictionary and json object save in files. 
    '''
    def __init__(self, filename = None):
        # 构造函数，初始化Live类的实例
        # 如果提供了文件名，则从文件中加载数据
        if filename is not None:
            with open(filename, 'r', encoding='utf-8') as f:
                self._data = json.load(f)
        else:
            # 如果没有提供文件名，初始化一个空字典
            self._data = {}
        # 保存文件名
        self.filename = filename

    # create_v1方法用于创建一个新的直播记录，包括直播的基本信息和时间戳。
    def create_v1(
            self,
            room_id:str,
            start_time:str,
            live_title:str,
            status = "Done",
            is_uploaded = False,
            version = "v1",
            ):
        
        timestamp_dt = datetime.fromisoformat(start_time)
        self._data = {
            "version": version,
            "room_id": str(room_id),
            "time": start_time, # ISO time format
            "year": timestamp_dt.strftime("%Y"),
            "month": timestamp_dt.strftime("%m"),
            "day": timestamp_dt.strftime("%d"),
            "hour": timestamp_dt.strftime("%H"),
            "minute": timestamp_dt.strftime("%M"),
            "second": timestamp_dt.strftime("%S"),
            "live_title": live_title,
            "live_title_now": live_title,
            "status": status,
            "is_uploaded": is_uploaded,
            "video_list": [],
            #Uncessary part
            "video_list_now": {}
        }

    # 向当前直播的临时视频列表（video_list_now）中添加一个新的视频记录。
    def add_video_now_v1(
            self,
            start_time:str,
            filename:str
            ):
        # root是文件名的名称部分，ext是文件名的扩展部分
        (root, ext) = os.path.splitext(filename)
        # video是一个字典，包含视频的基本信息 这里root键关联的值是另一个字典，有三个键值对
        video = {root:
                {
                "start_time": start_time, # ISO time format
                "filename": filename,
                "live_title": self._data["live_title_now"]
            }
        }
        # 更新到临时视频列表中
        self._data['video_list_now'].update(video)
        return video
    
    # 将临时视频列表中的一个视频移动到最终视频列表（video_list），并更新文件名。
    def finalize_video_v1(self, filename:str):
        """
        Move the video from video_list_now to video_list. 
        The video may have different extension, so only the root is used to find the video.
        """
        # root是文件名的名称部分，ext是文件名的扩展部分
        (root, ext) = os.path.splitext(filename)
        # 从临时视频列表中移除视频
        video = self._data["video_list_now"].pop(root)
        # Update the filename in case the extension is changed.
        # 更新文件名以防扩展名被更改
        video["filename"] = filename
        # 添加进最终视频列表中
        self._data["video_list"].append(video)

    # 更新当前直播的标题
    def update_live_title_now(self, live_title_now:str):
        """
        Update the live_title_now in the data.
        """
        self._data["live_title_now"] = live_title_now

    # 更新直播的状态
    def update_live_status(self, status:str):
        self._data["status"] = status

    # 将直播数据保存到文件
    def dump(self, filename = None, path = ""):
        """
        Dump the data to the file. 将数据导出到文件。
        Return the filename. 返回文件名。
        args:
            filename: the path to save the data. If None, use the filename in the data. 保存数据的路径。如果为None，则使用数据中的文件名。
            path: the directory to save the data. If None, use the current directory. 保存数据的目录。如果为None，则使用当前目录。
        """
        if filename is None:
            if self.filename is not None:
                filename = self.filename
            else:
                timestamp = datetime.fromisoformat(self._data['time']).strftime("_%Y%m%d_%H-%M-%S")
                self.filename = os.path.join(path, f"{self._data['room_id']}{timestamp}.json")
                filename = self.filename
        # 使用FileLock上下文管理器来锁定文件，以防止在写入时发生冲突。这是通过创建一个与目标文件同名的.lock文件来实现的。
        with FileLock(f"{filename}.lock"):
            with open(filename, 'w', encoding='utf-8') as f:
                # 使用json.dump将self._data字典序列化为JSON格式，并写入到文件中。indent=4参数指定了输出的缩进级别，使得JSON文件格式化，便于阅读。
                json.dump(self._data, f, indent=4)
        return filename

    # 从文件加载数据到Live类的实例
    def load(self, filename):
        """
        Load the data from the file.
        """
        # 使用FileLock确保在读取文件时不会有写入操作发生，从而避免数据损坏。
        with FileLock(f"{filename}.lock"):
            with open(filename, 'r', encoding='utf-8') as f:
                # 使用json.load函数从文件对象f中读取JSON数据，并将其解析为Python字典。然后将这个字典赋值给类的实例变量self._data。
                self._data = json.load(f)

    # 检查直播状态
    def islive(self):
        if self._data["version"] == "v1":
            return self._data["status"] == "Living"
    
    # 更新视频的服务器名称
    def update_server_name(self, filename:str, server_name:str):
        """
        Find the dictionary in video_list with filename, and update the server_name.
        """
        for item in self._data["video_list"]:
            if item["filename"] == filename:
                item["server_name"] = server_name
                break


# 示例数据，可能用于初始化Live类的实例
data = {
    "version": "v1",
    "time_format": "_%Y%m%d_%H-%M-%S",
    "live_DB": {
        "nickname": "kaofish",
        "room_id": 22259479,
        "live_title": "\ud83d\udc1f\u7ec8\u5c06\u6210\u4e3a\u672f\ud83d\udc1f",
        "start_time": 1696315449,
        "live_id": 306,
        "is_uploaded": False,
        "end_time": 1696315487
    },
    "year": "2023",
    "month": "10",
    "day": "03",
    "hour": "14",
    "up_name": "kaofish",
    "live_title": "\ud83d\udc1f\u7ec8\u5c06\u6210\u4e3a\u672f\ud83d\udc1f",
    "status": "Done",
    "video_list": [
        {
            "start_time": 1696315467,
            "video_basename": "kaofish_20231003_14-44-27.flv",
            "video_directory": "Videos/kaofish",
            "subtitle_file": "Videos/kaofish/kaofish_20231003_14-44-27.ass",
            "is_live": False,
            "is_stored": True,
            "live_title": "\ud83d\udc1f\u7ec8\u5c06\u6210\u4e3a\u672f\ud83d\udc1f",
            "end_time": 1696315486,
            "duration": 19,
            "size": 23691264,
            "deletion_type": 2,
            "live_id": 306,
            "video_id": 4472
        }
    ]
}

