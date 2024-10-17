from typing import Optional
from .core import BilibiliUploaderBase, VideoPart, login, login_by_access_token
import json


class BilibiliUploader(BilibiliUploaderBase):
    def __init__(self):
        self.access_token = None
        self.refresh_token = None
        self.sid = None
        self.mid = None

    def login(self, username, password):
        code, self.access_token, self.refresh_token, self.sid, self.mid, _ = login(
            username, password)
        if code != 0:  # success
            print("login fail, error code = {}".format(code))
            return -1
        return 0

    def login_by_access_token(self, access_token, refresh_token=None):
        self.access_token = access_token
        self.refresh_token = refresh_token
        self.sid, self.mid, _ = login_by_access_token(access_token)

    def login_by_access_token_file(self, file_name):
        with open(file_name, "r", encoding='utf-8') as f:
            login_data = json.loads(f.read())["token_info"]
        self.access_token = login_data["access_token"]
        self.refresh_token = login_data["refresh_token"]
        self.sid, self.mid, _ = login_by_access_token(self.access_token)

    def save_login_data(self, file_name=None):
        login_data = json.dumps(
            {
                "access_token": self.access_token,
                "refresh_token": self.refresh_token
            }
        )
        try:
            with open(file_name, "w+", encoding='utf-8') as f: #type:ignore
                f.write(login_data)
        finally:
            return login_data

    def set_videos_info(self,
                parts: list[VideoPart],
                copyright: int,
                title: str,
                tid: int,
                tag: str,
                desc: str,
                source: str = '',
                cover: str = '',
                no_reprint: int = 0,
                open_elec: int = 1,
                max_retry: int = 5,
                thread_pool_workers: int = 1,
                video_list_json='',
                submit_mode:int = 1,
                avid: Optional[int] = None,
                bvid: Optional[str] = None,
                replace_tag: int = 0,
                upload_rate = 0,
                engine = None,
                title_format = None
               ):
        """
        Configure the uploader object based on the argument passed.

        Args:
        access_token: oauth2 access token.
        sid: session id.
        mid: member id.
        parts: VideoPart list.
        insert_index: new video index.
        copyright: 原创/转载.
        title: 投稿标题.
        tid: 分区id.
        tag: 标签.
        desc: 投稿简介.
        source: 转载地址.
        cover: cover url.
        no_reprint: 可否转载.
        open_elec: 充电.
        max_retry: max retry time for each chunk.
        thread_pool_workers: max upload threads.
        video_list_json: This file will be frequently checked for dynamic realtime parts uploading. 
        mode: 1(default): single submission, 2: submission after each video part
        avid: av number, give it None for creating new video post, otherwise it will append to the current post 
        bvid: bv string, give it None for creating new video post, otherwise it will append to the current post
        engine: For database connection, if not provided, operate in offline mode
        """
        self.avid = avid
        self.bvid = bvid
        if not isinstance(parts, list):
            parts = [parts]
        self.parts = parts
        self.copyright: int = copyright
        self.title: str = title
        self.tid: int = tid
        self.tag: str = tag
        self.desc: str = desc
        self.source: str =source
        self.cover: str = cover
        self.no_reprint: int = no_reprint
        self.open_elec: int = open_elec
        self.max_retry: int = max_retry
        self.thread_pool_workers: int =thread_pool_workers
        self.video_list_json=video_list_json
        self.submit_mode = submit_mode
        self.replace_tag = replace_tag

        self.engine = engine
        self.upload_rate = upload_rate

        self.title_format = title_format
        

    def upload_new(self, submit_mode = 1):
        self.avid = None
        self.bvid = None
        self.submit_mode = submit_mode

        return self._upload()

    def replace(self,avid=None, bvid=None, submit_mode = 1):
        if not avid and not bvid:
            print("please provide avid or bvid")
            return None, None
        self.avid = avid
        self.bvid = bvid
        self.submit_mode = submit_mode
        self.replace_tag = 1

        return self._upload()

    def append(self,avid=None, bvid=None, submit_mode = 1):
        if not avid and not bvid:
            print("please provide avid or bvid")
            return None, None
        self.avid = avid
        self.bvid = bvid
        self.submit_mode = submit_mode

        return self._upload()