import operator

def sum_sort(n, a):
    sum_list = [(sum([int(digit) for digit in list(str(number))]), number) for number in a]
    sum_list.sort(key=operator.itemgetter(0, 1))
    return [item[1] for item in sum_list]

if __name__ == '__main__':
    n = int(input())
    a = input().split()
    print(*sum_sort(n, a))
