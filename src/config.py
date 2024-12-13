# Copyright (c) 2024 bilive.

import os
from pathlib import Path
from datetime import datetime
import configparser

# ============================ Your configuration ============================
GPU_EXIST=True
# Can be pipeline, append, merge
MODEL_TYPE = "pipeline"
Inference_Model = "small"

# ============================ Basic configuration ============================
SRC_DIR = str(Path(os.path.abspath(__file__)).parent)
BILIVE_DIR = str(Path(SRC_DIR).parent)
VIDEOS_DIR = os.path.join(BILIVE_DIR, 'Videos')
DanmakuFactory_bin = os.path.join('utils', 'DanmakuFactory')
DanmakuFactory_PATH = os.path.join(SRC_DIR, DanmakuFactory_bin)

WHISPER_LOG_PATH = os.path.join(BILIVE_DIR, 'logs', 'burningLog', f'whisper-{datetime.now().strftime("%Y%m-%d-%H%M%S")}.log')
BURN_LOG_PATH = os.path.join(BILIVE_DIR, 'logs', 'burningLog', f'burn-{datetime.now().strftime("%Y%m-%d-%H%M%S")}.log')
MERGE_LOG_PATH = os.path.join(BILIVE_DIR, 'logs', 'mergeLog', f'merge-{datetime.now().strftime("%Y%m-%d-%H%M%S")}.log')

def get_model_path():
    SRC_DIR = str(Path(os.path.abspath(__file__)).parent)
    model_dir = os.path.join(SRC_DIR, 'subtitle', 'models')
    model_path = os.path.join(model_dir, f'{Inference_Model}.pt')
    return model_path

def get_interface_config():
    interface_config = configparser.ConfigParser()
    interface_dir = os.path.join(SRC_DIR, 'subtitle')
    interface_file = os.path.join(interface_dir, "en.ini")
    interface_config.read(interface_file, encoding='utf-8')
    return interface_config