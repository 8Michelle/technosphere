import math

def print_by_cols(v, cols):
    for i in range(0, len(v), cols):
        print(*v[i:i+cols], sep='\t')

def concat(matrix1, matrix2):
    if not matrix2:
        return matrix1
    rows = len(matrix2)
    matrix = []
    for row in range(rows):
        matrix.append(matrix1[row] + matrix2[row])
    matrix.append(matrix1[-1])
    return matrix


def pseudo_transpose(matrix):
    if not matrix:
        return []
    new_matrix = []
    for row in range(len(matrix[0])):
        new_matrix.append([])
        for col in range(len(matrix)):
            new_matrix[row].append(matrix[col][row])
    return new_matrix

def reorder_list(v, cols):
    if cols == len(v) or cols == 1:
        return v
    stride = math.ceil(len(v) / cols)
    diff = stride * cols - len(v)
    v_new = []
    ind = 0
    for col in range(cols):
        v_new.append([])
        rows = stride if (col < cols - diff) else (stride - 1)
        for row in range(rows):
            v_new[col].append(v[ind])
            ind += 1
    v_new_1 = pseudo_transpose(v_new[:cols - diff])
    v_new_2 = pseudo_transpose(v_new[cols - diff:])
    v_new = concat(v_new_1, v_new_2)
    v_new_flatten = []
    for row in v_new:
        v_new_flatten += row
    return v_new_flatten

if __name__ == '__main__':
    v = list(range(1, 11))
    cols = 2
    print_by_cols(v, cols)
    v_new = reorder_list(v, cols)
    print()
    print_by_cols(v_new, cols)
