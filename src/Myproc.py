import os
import time,datetime
import sys
from multiprocessing import get_context
from threading import Thread
import logging

# The main feature of this class is to redirect the output and error to a specified log file when the process starts, which is very useful for debugging and recording the process run.
# In addition, by the post_run method, some cleanup work can be done after the process ends, without blocking the main thread.
class Myproc(get_context('spawn').Process):
    def __init__(self, group=None, target=None, name=None, args=(), kwargs={},
                 *, daemon=None):
        """
        Overwritten Process class with stdout, stderr redirection.
        """

        super().__init__(group, target, name, args, kwargs, daemon=daemon)  # type: ignore
        self.logfile = ""
        self.path = "."  

    # rewrite the run method of the Process class
    def run(self):
        time.sleep(0.1)
        print("===========Myproc==========")
        print(datetime.datetime.now())
        print("Process ", self.name, "has started.")
        print("Process spawned at PID: ",os.getpid())
        print("===========================",flush=True)
        time.sleep(0.1)
        # open the log file for writing, and set the UTF-8 encoding.
        with open(self.logfile, "w", encoding='utf-8') as f:
            # redirect the standard output and standard error to the log file.
            sys.stdout = f
            sys.stderr = f
            print("PID: ", os.getpid(),flush=True)
            # check if there is a target function to execute.
            if self._target:  # type: ignore
                try:
                    # execute the target function, pass the parameters.
                    self._target(*self._args, **self._kwargs)  # type: ignore
                except Exception as e:
                    logging.exception(e)

    # the private method to execute after the process ends.
    def _post_run(self):
        # wait for the process to end.
        self.join()
        print("===========Myproc==========")
        print(datetime.datetime.now())
        print("Process ", self.name, "has terminated, exit code: ", self.exitcode)
    
    # start a new thread to execute the _post_run method, so that the cleanup work can be done after the process ends.
    def post_run(self):
        t = Thread(target = self._post_run)
        t.start()
    
    # allow the external code to set the path of the log file.
    def set_output_err(self,logfile):
        self.logfile = logfile

    # allow the external code to set the working directory of the process.
    def set_path(self,path):
        self.path = path