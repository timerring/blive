# import the HTTP server and request handler
from http.server import SimpleHTTPRequestHandler, HTTPServer
# import the JSON module, for parsing JSON data
import json
# import the logging module, for recording logs
import logging
# import the operating system interface module
import os
# import the Live module from the relative path
from .Live import Live
# import the custom process module from the relative path
from .Myproc import Myproc
# import the function for uploading the video to the relative path
from .blive_upload import configured_upload
# import the function for getting the live title from the relative path
from .api import get_title

# define the MyHandler class, inheriting from SimpleHTTPRequestHandler
class MyHandler(SimpleHTTPRequestHandler):
    # class attribute definition

    # dict room_id: list_file path
    room_ids = {}
    # dict active video_file path (not full path, root only, without extension): list_file path
    videos_active = {}
    # set of list_file path for each list_file that is waiting for all videos to be finalized
    lists_fin_wait = set()
    
    # the directory for storing the video list files
    _video_list_directory = ""
    # the directory for storing the upload log files
    _upload_log_dir = ""
    # the list of room_ids that need to be uploaded
    _upload_list = []
    
    # class method, for configuring the directory paths
    @classmethod
    def config(cls, video_list_path,upload_log_dir, upload_config_path):
        # set the directory for the video list files
        cls._video_list_directory = video_list_path
        # set the directory for the upload log files
        cls._upload_log_dir = upload_log_dir
        # set the path for the upload config file
        cls.upload_config_path = upload_config_path

     # class method, for loading the upload list from the config file
    @classmethod
    def set_upload_list(cls):
        with open("upload_config.json", 'r', encoding='utf-8') as f:
            # load the JSON config
            config = json.load(f)
        # assign the list of room_ids from the config file to the upload list
        cls._upload_list = list(config.keys())

    def do_POST(self):
        # get the length of the request body
        content_length = int(self.headers['Content-Length'])
        # read and decode the request body data
        post_data = self.rfile.read(content_length).decode('utf-8')

        try:
            # Deserialize the JSON payload
            event = json.loads(post_data)
            # record the event
            self.log_event(event)
            # handle the event
            self.handle_event(event)
        # record the exception
        except Exception as e:
            logging.exception(e)


        # You might want to send a response back to the client
        # Here, we send a simple "OK" text.
        # send the 200 status code
        self.send_response_only(200)
        # set the response header
        self.send_header('Content-type', 'text/plain')
        # end the header
        self.end_headers()
        # send the response body
        self.wfile.write(b'OK')

    # class method, for recording the event
    def log_event(self, event):
        # get the event type
        event_type = event.get("type", "")
        # get the room_id from the event data
        if event['data'].get('room_id'):
            room_id = event['data']['room_id']
        elif event['data'].get('room_info'):
            room_id = event['data']['room_info']['room_id']
        else:
            room_id = ""
        # format the log message
        format = f"[{room_id}] {event_type}"
        # record the log message
        self.log_message(format)

    # static method, for handling the event
    @staticmethod
    def handle_event(event):
        # Extract the event type
        event_type = event.get("type", "")
        # handle the event according to its type      
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
            # ignore other types of events
            pass

    # class method, for uploading the video, handling the recording started, room change, video create and video processing completed events
    @classmethod
    def upload(cls, event):
        """
        Upload the video to Bilibili, if room_id is in upload_list.
        """
        # upload the video to Bilibili, if the room_id is in the upload list
        room_id = event['data']['room_info']['room_id']

        if str(room_id) in cls._upload_list:
            # create the log file path
            logfile = os.path.join(
                cls._upload_log_dir,
                os.path.basename(cls.room_ids[room_id]).split('.')[0] + '.log'
                )
            # create the upload process
            p = Myproc(target = configured_upload, args = (cls.room_ids[room_id], cls.upload_config_path), name="[{}]Uploader".format(room_id))
            # set the log file
            p.set_output_err(logfile)
            # start the process
            p.start()
            # set the operation after the process ends
            p.post_run()
            print("=============================", flush=True)
            print("开始上传"+ str(room_id), flush=True)
            print("=============================", flush=True)
        else:
            # if the room_id is not in the upload list, record a warning log
            logging.warning(f"upload: Room {room_id} not found in upload list{cls._upload_list}")
    
    # class method, for handling the recording started event, creating a new video list file, and recording the room_id
    @classmethod
    def handle_recording_started(cls, event:dict):
        """
        Create a new video list file, and add dumped filename to room_ids.
        """
        # create the Live object
        live = Live()
        # get the room_id from the event data
        room_id = event['data']['room_info']['room_id']
        
        # try to get the live title
        title = get_title(room_id)
        if title is None:
            # if failed, get the title from the event data
            title = event['data']['room_info']['title']

        # create the live record using the event data
        live.create_v1(
                room_id = room_id,
                start_time = event['date'],
                live_title = title,
                status="Living"
                )
        # save the live record and add it to the room_ids dictionary
        cls.room_ids[room_id] = live.dump(path = MyHandler._video_list_directory)
    
    # class method, for handling the recording finished or cancelled event, removing the video list file from the room_ids dictionary, and deciding whether to wait or directly finalize the list file according to whether there is active video
    @classmethod
    def handle_recording_finished(cls, event:dict):
        """
        Pop the video list file from room_ids. 
        Put it into lists_fin_wait if there is video active, otherwise finalize it.
        """
        room_id = event['data']['room_info']['room_id']
        try:
            # pop the video list file path from the room_ids dictionary
            list_file = cls.room_ids.pop(room_id)
            # if there is active video
            if list_file in cls.videos_active.values():
                # add the list file to the waiting to be finalized set
                cls.lists_fin_wait.add(list_file)
            else:
                # finalize the list file
                cls.finalize_list(list_file)

        except Exception as e:
            logging.exception(e)

    # class method, for finalizing the video list file, setting the status to Done, and saving the changes
    @classmethod
    def finalize_list(cls, list_file):
        """
        Finalize the video list file, set status to Done.
        """
        # create the Live object and load the video list file
        live = Live(filename = list_file)
        # update the live status to Done
        live.update_live_status("Done")
        # save the changes
        live.dump()
        # record the log
        logging.warning(f"Video list '{list_file}' is finished.")        
        
    @classmethod
    def handle_room_change(cls, event:dict):
        # get the room_id from the event data
        room_id = event['data']['room_info']['room_id']
        # try to get the new title of the room
        title = get_title(room_id)
        # if failed, use the title from the event data
        if title is None:
            title = event['data']['room_info']['title']
        # if the room_id is in the room_ids dictionary, we have the record of this room
        if room_id in cls.room_ids:
            # load the corresponding Live object
            live = Live(filename = cls.room_ids[room_id])
            # update the live title
            live.update_live_title_now(title)
            # save the changes of the Live object
            live.dump()
        else:
            # if the room_id is not in the room_ids dictionary, record a warning log
            logging.warning(f"handle_room_change: Room {room_id} not found in data store")
    
    @classmethod
    def handle_video_create(cls, event:dict):
        # get the room_id from the event data
        room_id = event['data']['room_id']
        # get the video file path from the event data
        filename = event['data']['path']
        # try to get the room title
        title = get_title(room_id)

        # if the room_id is in the room_ids dictionary, we have the record of this room
        if room_id in cls.room_ids:
            # load the Live object
            live = Live(filename = cls.room_ids[room_id])
            if title is not None:
                # if the title is got, update the live title
                live.update_live_title_now(title)
            
            # add the video to the Live object
            live.add_video_now_v1(
                    start_time = event['date'],
                    filename = filename
                    )
            # save the changes of the Live object
            list_file = live.dump()
            # associate the video file name (without extension) with the list file path
            (root, ext) = os.path.splitext(filename)
            cls.videos_active[root] = list_file
        else:
            # if the room_id is not in the room_ids dictionary, record a warning log
            logging.warning(f"handle_video_create: Room {room_id} not found in data store")

    # class method, for handling the video file completed event, finalizing the video file in the corresponding live object, and checking whether the parent list can also be finalized
    @classmethod
    def handle_video_file_completed(cls, event:dict):
        """
        Finalize the video file, and remove it from videos_active.
        Then check if the paraent list is in lists_fin_wait, finalize it if no more video in the list is active.
        """
        # get the video file path from the event data
        filename = event['data']['path']
        # get the file name and extension from the video file name
        (root, ext) = os.path.splitext(filename)
        # remove the video file from the active videos
        list_file = cls.videos_active.pop(root)
        
        # load the Live object
        live = Live(filename = list_file)
        # finalize the video file
        live.finalize_video_v1(
                filename = filename
                )
        # save the changes of the Live object
        live.dump()

        # if the list is in lists_fin_wait fin_wait, finalize it if no video is active
        if list_file in cls.lists_fin_wait:
            if list_file not in cls.videos_active.values():
                # finalize the list file
                cls.finalize_list(list_file)
                # remove the list file from the waiting to be finalized set
                cls.lists_fin_wait.remove(list_file)
            
    # class method, for shutting down the server, ensuring that the status of all video list files is set to Done when the server is shut down, which is a cleanup process to ensure data consistency
    @classmethod
    def shutdown(cls):
        """
        Set status of all video list files to Done.
        """
        # iterate all room_ids
        for room_id in cls.room_ids:
            # load the Live object
            live = Live(filename = cls.room_ids[room_id])
            # set the status to Done
            live.update_live_status("Done")
            # save the changes
            live.dump()
        # iterate the waiting to be finalized list files
        for list_file in cls.lists_fin_wait:
            # load the Live object
            live = Live(filename = list_file)
            # set the status to Done
            live.update_live_status("Done")
            # save the changes
            live.dump()

