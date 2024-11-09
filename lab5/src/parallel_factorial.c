#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <getopt.h>

#define MAX_THREADS 100

// Структура для хранения параметров каждого потока
typedef struct {
  int k;
  int start;
  int end;
  int mod;
  long long *result;
} thread_args_t;

pthread_mutex_t mutex; // Мьютекс для синхронизации результатов

// Функция, которую будет выполнять каждый поток
void *calculate_factorial_part(void *args) {
  thread_args_t *thread_args = (thread_args_t *)args;
  long long partial_result = 1;

  for (int i = thread_args->start; i <= thread_args->end; i++) {
    partial_result = (partial_result * i) % thread_args->mod;
  }

  // Блокировка мьютекса перед обновлением общего результата
  pthread_mutex_lock(&mutex);
  *(thread_args->result) = (*(thread_args->result) * partial_result) % thread_args->mod;
  // Разблокировка мьютекса
  pthread_mutex_unlock(&mutex);

  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  int k = 0;
  int pnum = 1;
  int mod = 0;

  // Парсинг аргументов командной строки
  while (1) {
    static struct option long_options[] = {
      {"k", required_argument, 0, 'k'},
      {"pnum", required_argument, 0, 'p'},
      {"mod", required_argument, 0, 'm'},
      {0, 0, 0, 0}
    };

    int option_index = 0;
    int c = getopt_long(argc, argv, "k:p:m:", long_options, &option_index);

    if (c == -1) {
      break;
    }

    switch (c) {
      case 'k':
        k = atoi(optarg);
        break;
      case 'p':
        pnum = atoi(optarg);
        break;
      case 'm':
        mod = atoi(optarg);
        break;
      default:
        fprintf(stderr, "Invalid option: %c\n", optopt);
        return 1;
    }
  }

  // Проверка валидности входных данных
  if (k <= 0 || pnum <= 0 || mod <= 0) {
    fprintf(stderr, "Invalid input parameters!\n");
    return 1;
  }

  if (pnum > MAX_THREADS) {
    fprintf(stderr, "Maximum allowed threads exceeded!\n");
    return 1;
  }

  // Инициализация мьютекса
  pthread_mutex_init(&mutex, NULL);

  // Создание потоков
  pthread_t threads[pnum];
  thread_args_t thread_args[pnum];
  long long result = 1;

  // Распределение работы между потоками
  int chunk_size = k / pnum;
  int start = 1;
  int end = chunk_size;

  for (int i = 0; i < pnum; i++) {
    thread_args[i].k = k;
    thread_args[i].start = start;
    thread_args[i].end = end;
    thread_args[i].mod = mod;
    thread_args[i].result = &result;

    // Запуск потока
    pthread_create(&threads[i], NULL, calculate_factorial_part, (void *)&thread_args[i]);

    // Обновление значений start и end для следующего потока
    start = end + 1;
    end += chunk_size;
  }

  // Ожидание завершения всех потоков
  for (int i = 0; i < pnum; i++) {
    pthread_join(threads[i], NULL);
  }

  // Удаление мьютекса
  pthread_mutex_destroy(&mutex);

  // Вывод результата
  printf("Factorial of %d modulo %d is: %lld\n", k, mod, result);
  return 0;
}