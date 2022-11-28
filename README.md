# Практическое задание #1.

Нужно реализовать quicksort. От Вас требуется написать последовательную версию алгоритма  (sequential) и параллельную версию (
parallel). Взять случайный массив из 10^8 элементов и отсортировать. (Усреднить по 5 запускам) Сравнить время работы parallel на 4
процессах и sequential на одном процессе - у Вас должно быть раза в 3 быстрее.  (Если будет медленнее, то выставление баллов
оставляется на моё усмотрение.)

Дедлайн где-то в начале декабря. Нужен код на гитхабе и результаты запусков в README.md. Код, который запускает, тоже
должен лежать в репо.

# Результаты запусков
started sequential quicksort

REPEAT[0]TIME[37708 in ms]<br />
REPEAT[1]TIME[36274 in ms]<br />
REPEAT[2]TIME[37484 in ms]<br />
REPEAT[3]TIME[38704 in ms]<br />
REPEAT[4]TIME[36794 in ms]<br />
AVERAGE[37392 in ms]<br />

started parallel quicksort<br />
REPEAT[0]TIME[11575 in ms]<br />
REPEAT[1]TIME[10885 in ms]<br />
REPEAT[2]TIME[10513 in ms]<br />
REPEAT[3]TIME[11298 in ms]<br />
REPEAT[4]TIME[11450 in ms]<br />
AVERAGE[11144 in ms]<br />

Итого, ускорение 37392 / 11144 = 3.36