from time import sleep
import datetime
import logging
from typing import Any, Tuple

class Retry:
    def __init__(self, max_retry: int, check_func = lambda r: r, interval: int = 10, default = None):
        '''
        :param max_retry: 最大重试次数
        :param check_func: 检查函数，接受返回值，返回True表示成功，False表示失败
        :param interval: 重试间隔
        :param default: 默认返回值
        '''
        self.max_retry = max_retry
        # self.success_return_value = success_return_value
        self.check_func = check_func
        self.interval = interval
        self.default = default

    def run(self, func, *args, **kwargs) -> Tuple[bool, Any]:
        status = (False, self.default)
        for i in range(0, self.max_retry):
            # print("Trials {}/{} :".format(i+1,self.max_retry))
            try:
                return_value = func(*args, **kwargs)
                if self.check_func(return_value):
                    status = (True,return_value)
                    break
            except Exception as e:
                print("================================")
                print(datetime.datetime.now())
                print("Exceptions in trial {}/{} :".format(i+1,self.max_retry), e, flush=True)
                logging.exception(e)
                sleep(self.interval)

        return status

    def decorator(self, func):
        def _(*args, **kwargs):
            return self.run(func, *args, **kwargs)[1]
        return _


