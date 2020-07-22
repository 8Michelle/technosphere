import functools

def counter(func):
    ncalls = 0
    depth = 0
    rdepth = 0
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        nonlocal ncalls
        nonlocal depth
        nonlocal rdepth
        ncalls += 1
        depth += 1
        rdepth = max(rdepth, depth)
        result = func(*args, **kwargs)
        depth -= 1
        if depth == 0:
            wrapper.ncalls = ncalls
            wrapper.rdepth = rdepth
            ncalls = 0
            rdepth = 0
        return result
    return wrapper
