import copy
import operator

class FragileDict:
    def __init__(self, data=None):
        if data is None:
            self._data = {}
        else:
            self._data = copy.deepcopy(data)
        self._lock = True

    def __getitem__(self, key):
        if self._lock:
            return copy.deepcopy(self._data[key])
        else:
            return self._data_tmp[key]

    def __setitem__(self, key, value):
        if self._lock:
            raise RuntimeError("Protected state")
        else:
            self._data_tmp[key] = value

    def __contains__(self, key):
        if self._lock:
            return key in self._data
        else:
            return key in self._data_tmp

    def __enter__(self):
        self._data_tmp = copy.deepcopy(self._data)
        self._lock = False
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self._lock = True
        error = exc_type is not None
        if error:
            print("Exception has been suppressed.")
        else:
            delattr(self, '_data')
            self._data = copy.deepcopy(self._data_tmp)
        delattr(self, '_data_tmp')
        return error


