from collections import defaultdict
import copy

def smartdict_nan(key):
    return lambda: key * 10

N = 10

smartdict = {}
for key in range(N):
    val = defaultdict(smartdict_nan(key))
    smartdict[key] = val

if __name__ == '__main__':
    print(smartdict[5]['key_unknown'])
    print(smartdict[7]['key_unknown'])
    print(smartdict[2]['key_unknown'])


# В исходной реализации на каждой итерации цикла происходило
# добавление defauldict в smatdict по текущему значению ключа.
# При этом в качестве генератора дефолтного значения в каждом
# defaultdict устанавливалась lambda функция, которая возвращает
# результат применения smartdict_nan к переменной key. Эта
# переменная по сути является ссылкой на текущее значение key
# в цикле. После выхода из цикла key остается равно 9, и к этому
# значению функция в defaultdict будет обращаться при каждом
# вызове (lambda функции во всех элементах smartdict будут
# обращаться к одному и тому же значению key).
# Можно переписать функцию smartdict_nan, чтобы она возвращала
# lamba функцию с параметром key, который определяется на данной
# итерации цикла, тогда будет достигнуто требуемое поведение.


def function():
    pass

# Комментарии комментарии
# комментарии комментарии




