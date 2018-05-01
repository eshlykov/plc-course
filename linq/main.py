from sequence import Sequence

print(Sequence(arr=range(10))
      .where(lambda x: x % 2 == 0)
      .map(lambda x: x ** 2)
      .take(2)
      .to_list())


def fibonacci_generator():
    a, b = 1, 1
    while True:
        yield a
        a, b = b, a + b


print(Sequence(gen=fibonacci_generator)
      .where(lambda x: x % 3 == 0)
      .map(lambda x: x ** 2 if x % 2 == 0 else x)
      .take(5)
      .to_list())


def line_generator():
    with open('tolstoy.txt', mode='r', encoding='utf-8') as file:
        for line in file:
            yield line


print(Sequence(gen=line_generator)
      .map(lambda x: x.split())
      .where(lambda x: len(x) > 0)
      .map(lambda x: (None, Sequence(arr=x)))
      .flatten()
      .group_by()
      .map(lambda x: (x[0], len(x[1].to_list())))
      .sort_by(lambda x: -x[1])
      .to_list())
