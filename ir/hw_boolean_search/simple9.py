import array
import struct

size_code = {
    1: 1,
    2: 2,
    3: 3,
    4: 4,
    5: 5,
    7: 6,
    9: 8,
    14: 10,
    28: 11
}

code_size = {size_code[i]: i for i in size_code}


def get_bit_size(batch):
    for i in [1, 2, 3, 4, 5, 7, 9, 14, 28]:
        if batch // 2 ** i == 0:
            return i


def encode_batch(batch, size):
    code = size_code[size]
    buffer = 0
    for ind, item in enumerate(batch[::-1]):
        buffer = item << (ind * size) | buffer
    buffer = buffer << (28 - len(batch) * size)
    buffer = code << ((ind + 1) * size + (28 - len(batch) * size)) | buffer

    return buffer


def decode_batch(batch):
    code = batch >> 28
    size = code_size[code]
    data = (batch & 268435455)
    buffer = array.array('i')
    i = 28 - size
    flag_start = True
    while i >= 0:
        batch = (data >> i) & (2 ** size - 1)
        if batch != 0 or flag_start:
            buffer.append((data >> i) & (2 ** size - 1))
        flag_start = False
        i -= size
    return buffer


def encode(data):
    length = len(data)
    i = 0
    j = 1
    result = array.array("I")
    while i < length:
        max_size = get_bit_size(data[i])
        while j < length:
            if max(max_size, get_bit_size(data[j])) * (j + 1 - i) > 28:
                break
            max_size = max(max_size, get_bit_size(data[j]))
            j += 1
        result.append(encode_batch(data[i:j], max_size))
        i = j
        j += 1

    return struct.pack(str(len(result)) + 'I', *result)


def decode(data):
    result = array.array('i')
    counts = array.array("I")
    counts.frombytes(data)
    for count in counts:
        result += decode_batch(count)

    return result
