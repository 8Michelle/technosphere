def min_sequence(n, k):

    if n == k:
        return [i for i in range(1, k + 1)]

    sequence = []
    i = 0
    while len(sequence) < n - k + 2:
        sequence.append(1)
        if len(sequence) < n - k + 2:
            sequence.append(2)
    sequence += [i for i in range(3, k + 1)]
    return sequence


if __name__ == '__main__':
    n, k = input().split()
    print(*min_sequence(int(n), int(k)))
