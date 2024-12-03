# -*- coding: UTF-8 -*-  

import os
from pathlib import Path
import configparser
import platform
import stat

def get_settings_config():
    settings_config = configparser.ConfigParser()
    settings_config.read(SETTINGS_PATH, encoding='utf-8')
    return settings_config


def get_model_path():
    model_dir = os.path.join(BASE_DIR, 'models')
    model_path = os.path.join(model_dir, f'{get_settings_config()["DEFAULT"]["Mode"]}.pt')
    return model_path


def get_interface_config():
    interface_config = configparser.ConfigParser()
    interface_file = os.path.join(os.path.dirname(os.path.abspath(__file__)), f"en.ini")
    interface_config.read(interface_file, encoding='utf-8')
    return interface_config


def init_settings_config():
    if not os.path.exists(SETTINGS_PATH):
        with open(os.path.join(os.path.dirname(os.path.dirname(__file__)), 'settings.ini'), mode='w', encoding='utf-8') as f:
            f.write('[DEFAULT]\n')
            f.write('Interface = English\n')
            f.write('Language = auto\n')
            f.write('Mode = small')


BASE_DIR = str(Path(os.path.abspath(__file__)).parent)
SETTINGS_PATH = os.path.join(os.path.dirname(os.path.dirname(__file__)), 'settings.ini')

INTERFACE_KEY_NAME_MAP = {
    'English': 'en'
}

init_settings_config()

ffmpeg_bin = os.path.join('linux_x64', 'ffmpeg')
FFMPEG_PATH = os.path.join(BASE_DIR, '', ffmpeg_bin)
os.chmod(FFMPEG_PATH, stat.S_IRWXU+stat.S_IRWXG+stat.S_IRWXO)

SILENCE_THRESH = -70           # silence below -70dBFS is considered silence
MIN_SILENCE_LEN = 700          # if silence is longer than 700ms, split
LENGTH_LIMIT = 60 * 1000       # split into segments no longer than 1 minute
ABANDON_CHUNK_LEN = 500        # discard chunks shorter than 500ms