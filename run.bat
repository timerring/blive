@echo off
chcp 65001

set config=.\settings.toml

set host=0.0.0.0
set port=22334

start cmd /K "python main.py" 

blrec -c %config% --open --host %host% --port %port%

pause

