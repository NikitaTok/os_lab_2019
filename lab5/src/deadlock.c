#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex1, mutex2;
int resource1 = 0;
int resource2 = 0;

void *thread1_func(void *arg) {
  pthread_mutex_lock(&mutex1); // Блокирует mutex1
  printf("Thread 1: Acquired mutex1\n");
  sleep(1); // Задержка для имитации работы
  pthread_mutex_lock(&mutex2); // Пытается заблокировать mutex2
  printf("Thread 1: Acquired mutex2\n");
  // ... (продолжить работу)
  pthread_mutex_unlock(&mutex2); // Разблокирует mutex2
  pthread_mutex_unlock(&mutex1); // Разблокирует mutex1
  return NULL;
}

void *thread2_func(void *arg) {
  pthread_mutex_lock(&mutex2); // Блокирует mutex2
  printf("Thread 2: Acquired mutex2\n");
  sleep(1); // Задержка для имитации работы
  pthread_mutex_lock(&mutex1); // Пытается заблокировать mutex1
  printf("Thread 2: Acquired mutex1\n");
  // ... (продолжить работу)
  pthread_mutex_unlock(&mutex1); // Разблокирует mutex1
  pthread_mutex_unlock(&mutex2); // Разблокирует mutex2
  return NULL;
}

int main() {
  pthread_t thread1, thread2;

  // Инициализация мьютексов
  pthread_mutex_init(&mutex1, NULL);
  pthread_mutex_init(&mutex2, NULL);

  // Создание потоков
  pthread_create(&thread1, NULL, thread1_func, NULL);
  pthread_create(&thread2, NULL, thread2_func, NULL);

  // Ожидание завершения потоков
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  // Удаление мьютексов
  pthread_mutex_destroy(&mutex1);
  pthread_mutex_destroy(&mutex2);

  return 0;
}