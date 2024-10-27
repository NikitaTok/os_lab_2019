#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "utils.h" // Включаем заголовочный файл для функции Sum

struct SumArgs {
  int *array;
  int begin;
  int end;
};

int Sum(int *array, int begin, int end) {
  int sum = 0;
  for (int i = begin; i < end; i++) {
    sum += array[i];
  }
  return sum;
}

void *ThreadSum(void *args) {
  struct SumArgs *sum_args = (struct SumArgs *)args;
  return (void *)(size_t)Sum(sum_args->array, sum_args->begin, sum_args->end);
}

int main(int argc, char **argv) {
  // Парсинг аргументов командной строки
  if (argc != 7) {
    printf("Usage: ./psum --threads_num <num> --seed <num> --array_size <num>\n");
    return 1;
  }
  uint32_t threads_num = 0;
  uint32_t array_size = 0;
  uint32_t seed = 0;
  for (int i = 1; i < argc; i += 2) {
    if (strcmp(argv[i], "--threads_num") == 0) {
      threads_num = atoi(argv[i + 1]);
    } else if (strcmp(argv[i], "--seed") == 0) {
      seed = atoi(argv[i + 1]);
    } else if (strcmp(argv[i], "--array_size") == 0) {
      array_size = atoi(argv[i + 1]);
    }
  }

  // Проверка на корректность входных данных
  if (threads_num == 0 || array_size == 0) {
    printf("Error: Invalid arguments.\n");
    return 1;
  }

  // Генерация массива
  int *array = malloc(sizeof(int) * array_size);
  if (array == NULL) {
    printf("Error: Memory allocation failed.\n");
    return 1;
  }
  GenerateArray(array, array_size, seed);

  // Замер времени
  clock_t start_time = clock();

  // Создание и запуск потоков
  pthread_t threads[threads_num];
  struct SumArgs args[threads_num];
  int chunk_size = array_size / threads_num;
  for (uint32_t i = 0; i < threads_num; i++) {
    args[i].array = array;
    args[i].begin = i * chunk_size;
    args[i].end = (i == threads_num - 1) ? array_size : (i + 1) * chunk_size;
    if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args[i])) {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }

  // Сбор результатов
  int total_sum = 0;
  for (uint32_t i = 0; i < threads_num; i++) {
    int sum = 0;
    pthread_join(threads[i], (void **)&sum);
    total_sum += sum;
  }

  // Окончание замера времени
  clock_t end_time = clock();
  double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

  // Освобождение памяти
  free(array);

  // Вывод результатов
  printf("Total: %d\n", total_sum);
  printf("Time taken: %f seconds\n", elapsed_time);

  return 0;
}