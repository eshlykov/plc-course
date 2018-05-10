def time_this(original_function):
    def new_function(*args,**kwargs):
        import datetime
        before = datetime.datetime.now()

        x = original_function(*args,**kwargs)

        after = datetime.datetime.now()
        print("Elapsed Time = {0}".format(after-before))
        return x

    return new_function


@time_this
def func_a(stuff):
    import time
    time.sleep(3)


func_a(1)  # Эквивалентно time_this(func_a(1)), если бы у func_a не было декоратора


def add_five(original_function):
    def new_function(*args, **kwargs):
        x = original_function(*args, **kwargs)
        return x + 5

    return new_function


@add_five
def foo():
    return 3


print(foo())  # 8


def add(val):
    def other_function(original_function):
        def new_function(*args, **kwargs):
              x = original_function(*args, **kwargs)
              return x + val

        return new_function

    return other_function

@add(5)  # Сначала применяется (5), а потом уже @
def bar():
  return 3


print(bar())  # 8


def log(original_class):
  class new_class(original_class):
      def __init__(self):
          super().__init__()

      def __getattribute__(self, name):  # Выполняется при доступе к методу или к классу
          # Причем этот метод должен выполнить доступ к этому атрибуту и вернуть его
          print('accessing {}'.format(name))
          # Чтобы не зациклится, пишим object.__getattribute__, то есть глобального базового класса.
          # object - не объект этого класса, а сам класс, то передать надо так же и self
          # Но так как мы делаем доступ к атрибутам данного объекта, то self надо передать именно текущий
          return object.__getattribute__(self, name)

  return new_class


@log
class Buz:
    def __init__(self):
        self.x = 4

    def foo(self, y):
      return self.x + y


buz = Buz()
print(buz.x)  # accessing x // 4
print(buz.foo(5))  # accessing foo // accessting x // 9
