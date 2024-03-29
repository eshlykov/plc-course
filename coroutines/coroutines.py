# -*- coding: utf-8 -*-
"""Coroutines.ipynb

Automatically generated by Colaboratory.

Original file is located at
    https://colab.research.google.com/drive/1Za_Q_9TlV-qy0ALSq-F63yNK55Bbtko_
"""

from collections import Counter, defaultdict
import numpy as np

"""# Корутины

С помощью `yield` можно легко писать бесконечные генераторы.

Например, бесконечный поток чисел Фибоначчи:
"""

def fibonacci_generator():
    a, b = 1, 1
    while True:
        yield a;
        a, b = b, a + b
        
gen = fibonacci_generator()
print(next(gen))
print(next(gen))
print(next(gen))

"""А если хочется получить сразу несколько первых элементов из генератора?

**Задание** Напишите функцию-генератор, которая будет выдавать элементы до тех пор, пока не наберется нужное количество (`n`).
"""

def take(generator, n):
    for i in range(n):
      yield next(generator)
           
list(take(fibonacci_generator(), 5))

"""**Задание** Напишите функцию, которая будет выдавать только четные числа Фибоначчи."""

def even_num_generator(num_generator):
    for item in num_generator:
        if item % 2 == 0:
            yield item

list(take(even_num_generator(fibonacci_generator()), 5))

"""Другая доступная операция - `send`:  
![send](https://image.ibb.co/dbKyLS/2018_03_29_13_58_49.png=x300)
"""

def receiver():
    while True:
        item = yield
        print('Got', item)

recv = receiver()
next(recv)
recv.send('Hello')
recv.send('World')

"""## N-граммная языковая модель"""

!wget -qq -O perashki.txt https://share.abbyy.com/index.php/s/Y86O2aRLOcdnNWv/download
  
!head perashki.txt

"""Давайте посчитаем частотности слов в файле:"""

word_counts = Counter()
with open('perashki.txt', encoding='utf-8') as f:
    for line in f:
        for word in line.strip().split():
            if len(word) != 0:
                word_counts[word] += 1
                
print(word_counts.most_common(100))

"""Почему бы не разнести чтение файла и получение отдельных токенов и их обработку?

Тогда при чтении легко можно будет добавлять некоторую предобработку - например, токенизацию не по пробелам, а с помощью nltk, не изменяя работу обработчика.

**Задание** Напишите `parser` - функцию, которая будет выдавать поток токенов из файла.
"""

def parser(path):
    with open(path, encoding='utf-8') as f:
        for line in f:
            for word in line.strip().split():
                yield word
                    
list(take(parser('perashki.txt'), 10))

"""Раз у нас есть такая удобная функция, почему бы не использовать её для чего-то интересного.

Напишем N-граммную языковую модель.

Языковая модель - это штука, которая умеет оценивать вероятности $\mathbf{P}(w_1, \ldots, w_n) = \prod_k \mathbf{P}(w_k|w_{k-1}, \ldots, w_{1})$.

N-граммная языковая модель приближает эту вероятность, используя предположение, что вероятность токена зависит только от недавней истории: $\mathbf{P}(w_k|w_1, \ldots, w_{k-1}) = \mathbf{P}(w_k|w_{k-1}, \ldots, w_{k-N + 1})$.

Для начала нужно собрать статистику. Для простоты будем работать с триграммной моделью, а значит - нужно собрать информацию:
- о триграммных частотностях $(w_{i-2}, w_{i-1}) \to C(w_i)$ - то есть о числе раз, когда слово $w_i$ шло за парой $w_{i-2}, w_{i-1}$
- о биграммных частотностях $(w_{i-1}) \to C(w_i)$
- об униграммных частотностях $() \to C(w_i)$

**Задание** Напишите функцию, которая будет из потока токенов формировать и выдавать наружу пары (ngram, next_word).
"""

def compose_ngram(tokens_stream):
    predpred, pred = None, None
    for curr in tokens_stream:
      if len(curr) == 0:
          # Хотим забыть предудешие два токена, так как это, скорее всего,
          # перенос строки либо пустая строка, и слова более вероятно на самом
          # деле друг с другом не связаны
          predpred, pred = None, None
          continue
      yield (), curr
      if pred is not None:
          yield (pred,), curr
      if predpred is not None:
          yield (predpred, pred), curr
      predpred, pred = pred, curr
          
list(take(compose_ngram(parser('perashki.txt')), 10))

"""Соберем статистику:"""

def collect_stat(path):
    ngrams_counter = defaultdict(Counter)
    for ngram in compose_ngram(parser(path)):
        ngrams_counter[ngram[0]][ngram[1]] += 1
    
    return ngrams_counter

ngrams_counter = collect_stat('perashki.txt')
ngrams_counter

"""Теперь генерировать будем так: пусть есть некоторый уже сгенерированный набор слов (возможно, пустой).

Тогда проверяем, есть ли статистика для последних двух слов - если есть, генерируем с помощью неё новое. Нет - тогда смотрим статистику для только одного слова. И так далее.
"""

def sample_token(ngrams_counter, ngram):
    probs = np.array(list(ngrams_counter[ngram].values()))
    probs = probs / np.sum(probs)
    return np.random.choice(list(ngrams_counter[ngram]), p=probs)  
  
def generate_token(ngrams_counter):
    predpred, pred = None, None
    while True:
        if predpred is None and pred is None:
            ngram = ()
        elif predpred is None:
            ngram = (pred, )
            if ngram not in ngrams_counter:
                ngram = ()
        else:
            ngram = (predpred, pred)
            if ngram not in ngrams_counter:
                ngram = (pred, )
                if ngram not in ngrams_counter:
                    ngram = ()
        token = sample_token(ngrams_counter, ngram)
        yield token
        predpred, pred = pred, token

list(take(generate_token(ngrams_counter), 30))

