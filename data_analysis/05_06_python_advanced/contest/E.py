def chain_loop(args):
    instance_gen = [(i for i in arg) for arg in args]
    i = -1
    length = len(instance_gen)
    while length:
        i = (i + 1) % length
        try:
            yield next(instance_gen[i])
        except StopIteration:
            instance_gen.pop(i)
            length -= 1
