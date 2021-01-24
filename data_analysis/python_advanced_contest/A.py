import string
import math

def solution1(arg): # complete
    return [char * 4 for char in arg]

def solution2(arg): # complete
    return [char * (i + 1) for i, char in enumerate(arg)]

def solution3(arg): # complete
    return [item for item in arg if math.gcd(item, max(arg)) != 1]

def solution4(arg): # complete
    return [item for item_list in arg for item in item_list]

def solution5(arg): # complete
    return [(a, b, c)
            for a in range(3, arg + 1)
            for b in range(4, arg + 1)
            for c in range(5, arg + 1)
            if (a ** 2 + b ** 2 == c ** 2) and (a < b) and (b < c)]

def solution6(arg): # complete
    return [[item + shift for item in arg[1]] for shift in arg[0]]

def solution7(arg): # complete
    return [[arg[i][j] for i in range(len(arg))]
            for j in range(len(arg[0]))]

def solution8(arg): # complete
    return [[int(item) for item in string.split()] for string in arg]

def solution9(arg): # complete
    return {string.ascii_lowercase[i]: i ** 2 for i in arg}

def solution10(arg): # complete
    return {string[0].upper() + string[1:].lower() for string in arg if len(string) > 3}

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