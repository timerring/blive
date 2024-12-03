# Copyright (c) 2024 bilive.

import os
from pathlib import Path
from datetime import datetime

SRC_DIR = str(Path(os.path.abspath(__file__)).parent)
BILIVE_DIR = str(Path(SRC_DIR).parent)
DanmakuFactory_bin = os.path.join('utils', 'DanmakuFactory')
DanmakuFactory_PATH = os.path.join(SRC_DIR, DanmakuFactory_bin)

WHISPER_LOG_PATH = os.path.join(BILIVE_DIR, 'logs', 'burningLog', f'whisper-{datetime.now().strftime("%Y%m-%d-%H%M%S")}.log')
BURN_LOG_PATH = os.path.join(BILIVE_DIR, 'logs', 'burningLog', f'burn-{datetime.now().strftime("%Y%m-%d-%H%M%S")}.log')
MERGE_LOG_PATH = os.path.join(BILIVE_DIR, 'logs', 'mergeLog', f'merge-{datetime.now().strftime("%Y%m-%d-%H%M%S")}.log')
GPU_EXIST=True
