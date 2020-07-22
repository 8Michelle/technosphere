from collections import deque

class BinarySearchTree:
    def __init__(self, root=None):
        if root is not None:
            self._root = root

    def append(self, item):
        if item is None:
            return
        if not hasattr(self, '_root'):
            self._root = item
        elif item > self._root:
            if not hasattr(self, '_right'):
                self._right = BinarySearchTree(item)
            else:
                self._right.append(item)
        elif item < self._root:
            if not hasattr(self, '_left'):
                self._left = BinarySearchTree(item)
            else:
                self._left.append(item)
        else:
            self._root = item

    def __contains__(self, item):
        if item is None:
            return False
        if not hasattr(self, '_root'):
            return False
        elif item < self._root and hasattr(self, '_left'):
            return item in self._left
        elif item > self._root and hasattr(self, '_right'):
            return item in self._right
        else:
            return self._root == item

    def __iter__(self):
        iterator = []
        d = deque()
        if hasattr(self, '_root'):
            d.append(self)
        while d:
            tmp = d.popleft()
            if hasattr(tmp, '_left'):
                d.append(tmp._left)
            if hasattr(tmp, '_right'):
                d.append(tmp._right)
            iterator.append(tmp._root)
        return iter(iterator)
