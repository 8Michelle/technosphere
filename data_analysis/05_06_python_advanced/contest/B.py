from functools import reduce
import re
import operator

def solution1(arg): # complete
    return list(map(lambda x: int(''.join(re.findall(r'\d{1}', x))[::-1]), arg))

def solution2(arg): # complete
    return list(map(lambda x: reduce(lambda prev, now: now * prev, x),
               list(arg)))

def solution3(arg): # complete
    return list(filter(lambda x: x % 6 in [0, 2, 5], arg))

def solution4(arg): # complete
    return list(filter(lambda x: x, arg))

def solution5(arg): # complete
    list(map(operator.setitem,
        arg, ['square' for _ in range(len(arg))],
        [d['length'] * d['width'] for d in arg],
    ))
    return arg

def solution6(arg): # complete
    return list(map(lambda x: dict(x, square=x['length'] * x['width']), arg))

def solution7(arg): # complete
    return set(reduce(lambda set1, set2: set1 & set2, arg))

def solution8(arg):
    return {i: reduce(lambda x, y: x + 1 if y == i else x, arg, 0) for i in set(arg)}

def solution9(arg): # complete
    return list(map(lambda x: x['name'], filter(lambda x: x['gpa'] > 4.5, arg)))

def solution10(arg): #complete
    return list(filter(lambda x: sum([int(i) for i in x[::2]]) == sum([int(i) for i in x[1::2]]), arg))

solutions = {
    'solution1': solution1,
    'solution2': solution2,
    'solution3': solution3,
    'solution4': solution4,
    'solution5': solution5,
    'solution6': solution6,
    'solution7': solution7,
    'solution8': solution8,
    'solution9': solution9,
    'solution10': solution10,
}

if __name__ == "__main__":
    seq = [1, 2, 1, 1, 3, 2, 3, 2, 4, 2, 4]
    print(solution8(seq))