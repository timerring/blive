import os
import logging
from http.server import HTTPServer

# import the toml module, for parsing the TOML configuration file
import toml

# import the custom HTTP request processor from the relative path
from src.httpserver import MyHandler


PORT = 22333

if __name__ == "__main__":
    # open and read the TOML configuration file
    with open('settings.toml', 'r', encoding='utf-8') as f:
        settings = toml.load(f)
    # get the video list file path
    video_list_path = os.path.join(settings['output']['out_dir'], "_list")
    # create the video list file directory, if it already exists, ignore
    os.makedirs(video_list_path, exist_ok=True)
    # get the upload log file path
    upload_log_dir = os.path.join(settings['logging']['log_dir'],"uploadNoDanmakuLog")
    # create the upload log directory, if it already exists, ignore
    os.makedirs(upload_log_dir, exist_ok = True)

    # configure the paths and configuration file for MyHandler
    MyHandler.config(video_list_path, upload_log_dir, "upload_config.json")
    # set the server listening address and port
    server_address = ('', PORT)
    # create the HTTP server instance, using MyHandler as the request processor
    httpd = HTTPServer(server_address, MyHandler)
    # print the server listening port information
    print(f"Listening to port {PORT}...", flush=True)
    try:
        # run the server until there is an error or interruption
        httpd.serve_forever()
    except Exception as e:
        # if there is an exception, record the exception information
        logging.exception(e)
    finally:
        # call the shutdown method of MyHandler to clean up
        MyHandler.shutdown()
        # close the server
        httpd.server_close()
        print("Terminated Gracefully!")
