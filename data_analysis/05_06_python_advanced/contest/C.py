import time
import functools
import random

def timeout(rps):
    def decorator(func):
        @functools.wraps(func)
        def wrapper(*args, **kwargs):
            call_time = time.time()
            result = func(*args, **kwargs)
            time.sleep(max(1 / rps - (time.time() - call_time), 0))
            return result
        return wrapper
    return decorator
