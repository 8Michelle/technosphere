def brackets(n):
    n *= 2
    sequence = ['(' for _ in range(n // 2)] + [')' for _ in range(n // 2)]
    yield ''.join(sequence)
    while True:
        ind = n - 1
        diff = 0
        while ind >= 0:
            diff = diff - 1 if sequence[ind] == ')' else diff + 1
            if diff < 0 and sequence[ind] == '(':
                break
            ind -= 1
        if ind < 0:
            break
        sequence[ind] = ')'
        for i in range(ind + 1, n):
            sequence[i] = '(' if i <= (n - ind + diff) / 2 + ind else ')'
        yield ''.join(sequence)

if __name__ == '__main__':
    n = int(input())
    for seq in brackets(n):
        print(seq)
