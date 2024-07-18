import os
import time,datetime
import sys
from multiprocessing import get_context
from threading import Thread
import logging

# 这个类的主要特点是在进程启动时重定向输出和错误到一个指定的日志文件，这对于调试和记录进程的运行情况非常有用。
# 此外，通过post_run方法，可以在进程结束后执行一些清理工作，而不会阻塞主线程。
class Myproc(get_context('spawn').Process):
    def __init__(self, group=None, target=None, name=None, args=(), kwargs={},
                 *, daemon=None):
        """
        Overwritten Process class with stdout, stderr redirection.
        """

        super().__init__(group, target, name, args, kwargs, daemon=daemon)  # type: ignore
        self.logfile = ""
        self.path = "."  

    # 重写了Process类的run方法。
    def run(self):
        time.sleep(0.1)
        print("===========Myproc==========")
        print(datetime.datetime.now())
        print("Process ", self.name, "has started.")
        print("Process spawned at PID: ",os.getpid())
        print("===========================",flush=True)
        time.sleep(0.1)
        # 打开日志文件用于写入，并设置UTF-8编码。
        with open(self.logfile, "w", encoding='utf-8') as f:
            # 将标准输出和标准错误重定向到日志文件。
            sys.stdout = f
            sys.stderr = f
            print("PID: ", os.getpid(),flush=True)
            # 检查是否有目标函数要执行。
            if self._target:  # type: ignore
                try:
                    # 执行目标函数，传入参数。
                    self._target(*self._args, **self._kwargs)  # type: ignore
                except Exception as e:
                    logging.exception(e)

    # 在进程结束后执行的私有方法。
    def _post_run(self):
        # 等待进程结束。
        self.join()
        print("===========Myproc==========")
        print(datetime.datetime.now())
        print("Process ", self.name, "has terminated, exit code: ", self.exitcode)
    
    # 启动一个新线程来执行_post_run方法，以便在进程结束后执行清理工作。
    def post_run(self):
        t = Thread(target = self._post_run)
        t.start()
    
    # 允许外部代码设置日志文件的路径。
    def set_output_err(self,logfile):
        self.logfile = logfile

    # 允许外部代码设置进程的工作目录。
    def set_path(self,path):
        self.path = path