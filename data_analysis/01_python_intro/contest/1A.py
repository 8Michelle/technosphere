def select_unique(n, a):
    unique = set([])
    result = []
    for i in a:
        if i not in unique:
            unique.add(i)
            result.append(i)
    difference = n - len(result)
    return result, difference

if __name__ == '__main__':
    n = int(input())
    a = input().split()
    result, difference = select_unique(n, a)
    print(*result)
    print(difference)
