def group_by(arr, key=lambda x: x):
    arr = sorted(arr, key=key)
    groups = []
    prev = None
    for elem in arr:
        if prev is not None and key(elem) == key(prev):
            groups[-1][1].append(elem)
        else:
            groups.append((key(elem), [elem]))
        prev = elem
    return groups


class Sequence:
    def __init__(self, *, gen=None, arr=None):
        if gen is not None and arr is not None:
            raise Exception('Both generator and list arguments are given.')
        if gen is None and arr is None:
            raise Exception('Both generator and list arguments are not given.')
        if gen is not None:
            self.gen = gen
        if arr is not None:
            def gen():
                for elem in arr:
                    yield elem
            self.gen = gen

    def map(self, fun):
        old = self.gen

        def gen():
            for elem in old():
                yield fun(elem)
        self.gen = gen
        return self

    def where(self, fun):
        old = self.gen

        def gen():
            for elem in old():
                if fun(elem):
                    yield elem
        self.gen = gen
        return self

    def take(self, num):
        old = self.gen()

        def gen():
            for _ in range(num):
                yield next(old)
        self.gen = gen
        return self

    def flatten(self):
        old = self.gen

        def gen():
            for _, seq in old():
                for elem in seq.gen():
                    yield elem
        self.gen = gen
        return self

    def sort_by(self, fun=lambda x: x):
        arr = list(self.gen())

        def gen():
            for elem in sorted(arr, key=fun):
                yield elem
        self.gen = gen
        return self

    def group_by(self, fun=lambda x: x):
        groups = group_by(list(self.gen()), fun)

        def gen():
            for key, group in groups:
                yield key, Sequence(arr=group)
        self.gen = gen
        return self

    def to_list(self):
        return list(self.gen())
