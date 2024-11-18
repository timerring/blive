import json
import os
from datetime import datetime
import collections

# import the FileLock, for handling the file write lock, to avoid data conflict
from filelock import FileLock
class Live():
    '''
    Provide the mapping between python dictionary and json object save in files. 
    '''
    def __init__(self, filename = None):
        # constructor, initialize the instance of Live class
        # if the filename is provided, load the data from the file
        if filename is not None:
            with open(filename, 'r', encoding='utf-8') as f:
                self._data = json.load(f)
        else:
            # if the filename is not provided, initialize an empty dictionary
            self._data = {}
        # save the filename
        self.filename = filename

    # create_v1 method is used to create a new live record, including the basic information of the live and the timestamp.
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

    # add a new video record to the temporary video list (video_list_now) of the current live.
    def add_video_now_v1(
            self,
            start_time:str,
            filename:str
            ):
        # root is the name part of the filename, ext is the extension part of the filename
        (root, ext) = os.path.splitext(filename)
        # video is a dictionary, containing the basic information of the video
        # here the root key is associated with another dictionary, with three key-value pairs
        video = {root:
                {
                "start_time": start_time, # ISO time format
                "filename": filename,
                "live_title": self._data["live_title_now"]
            }
        }
        # update to the temporary video list
        self._data['video_list_now'].update(video)
        return video
    
    # move a video from the temporary video list (video_list_now) to the final video list (video_list), and update the filename.
    def finalize_video_v1(self, filename:str):
        """
        Move the video from video_list_now to video_list. 
        The video may have different extension, so only the root is used to find the video.
        """
        # root is the name part of the filename, ext is the extension part of the filename
        (root, ext) = os.path.splitext(filename)
        # remove the video from the temporary video list
        video = self._data["video_list_now"].pop(root)
        # update the filename in case the extension is changed
        video["filename"] = filename
        # add to the final video list
        self._data["video_list"].append(video)

    # update the title of the current live
    def update_live_title_now(self, live_title_now:str):
        """
        Update the live_title_now in the data.
        """
        self._data["live_title_now"] = live_title_now

    # update the status of the live
    def update_live_status(self, status:str):
        self._data["status"] = status

    # dump the live data to the file
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
        # use the FileLock context manager to lock the file, to prevent conflict during writing
        # this is achieved by creating a .lock file with the same name as the target file
        with FileLock(f"{filename}.lock"):
            with open(filename, 'w', encoding='utf-8') as f:
                # use json.dump to serialize the self._data dictionary to JSON format, and write to the file
                # the indent=4 parameter specifies the output indentation level, making the JSON file formatted, for easy reading
                json.dump(self._data, f, indent=4)
        return filename

    # load the data from the file to the instance of Live class
    def load(self, filename):
        """
        Load the data from the file.
        """
        # use the FileLock to ensure that no writing operation occurs while reading the file, to avoid data corruption
        with FileLock(f"{filename}.lock"):
            with open(filename, 'r', encoding='utf-8') as f:
                # use json.load to read the JSON data from the file object f, and parse it to a Python dictionary
                # then assign this dictionary to the instance variable self._data
                self._data = json.load(f)

    # check if the live is ongoing
    def islive(self):
        if self._data["version"] == "v1":
            return self._data["status"] == "Living"
    
    # update the server name of the video
    def update_server_name(self, filename:str, server_name:str):
        """
        Find the dictionary in video_list with filename, and update the server_name.
        """
        for item in self._data["video_list"]:
            if item["filename"] == filename:
                item["server_name"] = server_name
                break


# example data, may be used to initialize the instance of Live class
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

