import json
import logging
from filelock import FileLock
import logging

from .bilibiliuploader.bilibiliuploader import BilibiliUploader
from .bilibiliuploader.core import VideoPart

from ..Live import Live

    
#Edit Information here, details in https://github.com/FortuneDayssss/BilibiliUploader
#此处修改上传内容，标题，简介，tag等，详见https://github.com/FortuneDayssss/BilibiliUploader
def config_gen(config_json: str, record_info: dict, up_name = None):
    """
    Generate the upload configuration for this python script, 
    based on the config_json(json file) and record_info(json file for videos and live info) provided.
    """
    #Specify the up_name
    if not up_name:
        up_name = record_info["room_id"]
    
    with open(config_json, 'r', encoding='utf-8') as f:
        config:dict = json.load(f)

    if "upload_args" not in config:
        if up_name in config:
            config = config[up_name]
        else:
            raise Exception("General configuration not provided and missing specific configuration for up_name %s"%up_name)

    upload_args = config["upload_args"]
    upload_args["tag"] = ",".join(upload_args["tag"])
    upload_args["desc"] = "\n".join(upload_args["desc"])

    for x in upload_args:
        if isinstance(upload_args[x], str) and x != "title_format":
            upload_args[x] = upload_args[x].format(**record_info)

    return config
    
def uploader_login(login_token_file, username: str = "username", password: str = "username"):
    uploader = BilibiliUploader()
    try:
        uploader.login_by_access_token_file(login_token_file)
        print("Successful! Logged in by token.")
    except Exception as e:
        logging.exception(e)
        print("Error with uploader.login_by_access_token_file")
        print(e)
        print("Try username/password login")
        rtncode = uploader.login(username, password)            #-449 error because of VPN required
        
        if rtncode != 0:
            raise Exception("username/password login fail")
        else:
            uploader.save_login_data(file_name=login_token_file)
    return uploader

# def parts_prepare(record_info):
#     # 处理视频文件
#     parts = []
#     file_list=record_info.get('video_list')
#     for item in file_list:
#         parts.append(VideoPart(
#             path=item.videoname,
#             title = item.live_title if item.live_title else item.basename.split('.')[0],
#             server_file_name= item.server_name
#         ))
#     return parts

def configured_upload(record_info_json: str, config_json: str, *args, **kwargs):
    '''
    record_info_json is provided to the _upload function. 
    '''
    live = Live(filename = record_info_json)
    record_info = live._data

    config = config_gen(config_json, record_info)
    config["upload_args"].update(kwargs)
    config["upload_args"].update({"video_list_json": record_info_json})    

    if 'status' not in record_info:
        print("Live status is not checked!")

    try:
        uploader = uploader_login(config["login_token_file"], config["username"], config["password"])
        # parts = parts_prepare(record_info)

        uploader.set_videos_info(parts=[], **config["upload_args"])
        avid, bvid = uploader._upload()
        if avid is None or bvid is None:
            print("Upload failed")
        else:
            print("Done! All video parts uploaded! Avid:{}, Bvid: {}".format(avid, bvid), flush = True)
    except Exception as e:
        logging.exception(e)
        print("Upload failed")
