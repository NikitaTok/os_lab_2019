#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>
#include <signal.h>

#include "find_min_max.h"
#include "utils.h"

int timeout = -1; //Таймаут по умолчанию

void handle_timeout(int sig){
  // Посылаем SIGKILL всем дочерним процессам
  if (sig == SIGALRM){
    printf("Timeout reached! Terminating child processes...\n");
    kill(0, SIGKILL);
  }
}

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  bool with_files = false;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {"timeout", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            break;
          case 1:
            array_size = atoi(optarg);
            break;
          case 2:
            pnum = atoi(optarg);
            break;
          case 3:
            with_files = true;
            break;
          case 4:
            timeout = atoi(optarg); // Считываем таймаут
            break;
          default:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        with_files = true;
        break;

      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;

  struct timeval start_time;
  gettimeofday(&start_time, NULL);
  
  // Устанавливаем обработчик сигнала таймаута
  struct sigaction sa;
  sa.sa_handler = handle_timeout;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  if (sigaction(SIGALRM, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  // Запускаем таймаут
  if (timeout > 0) {
    alarm(timeout); // Устанавливаем таймаут
  }

  for (int i = 0; i < pnum; i++) {
    pid_t child_pid = fork();
    if (child_pid >= 0) {
      // successful fork
      active_child_processes += 1;
      if (child_pid == 0) {
        // child process
        // Параллельная работа с массивом
                int start = i * (array_size / pnum);
                int end = (i == pnum - 1) ? array_size : (i + 1) * (array_size / pnum);
                struct MinMax local_minmax = GetMinMax(array, start, end);


        // parallel somehow

        if (with_files) {
          // use files here
          // Запись в файл
                    char filename[20];
                    sprintf(filename, "result_%d.txt", i);
                    FILE *fp = fopen(filename, "w");
                    fprintf(fp, "%d %d\n", local_minmax.min, local_minmax.max);
                    fclose(fp);
        } else {
          // use pipe here
          // Поскольку мы уже находимся в дочернем процессе,
          // создаем доступ к pipe для передачи данных
          write(STDOUT_FILENO, &local_minmax, sizeof(local_minmax));
        }
        return 0;
      }

    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }

  while (active_child_processes > 0) {
    // your code here
    wait(NULL); // Ожидание завершения дочерних процессов
    
    active_child_processes -= 1;
  }

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  for (int i = 0; i < pnum; i++) {
    int min = INT_MAX;
    int max = INT_MIN;

    if (with_files) {
      // read from files
      // Чтение из файла
      char filename[20];
      sprintf(filename, "result_%d.txt", i);
      FILE *fp = fopen(filename, "r");
      if (fp) {
          fscanf(fp, "%d %d", &min, &max);
          fclose(fp);
          remove(filename); // Удаление файла после чтения
        }
    } else {
      // read from pipes
      // Чтение из pipe
      struct MinMax local_minmax;
      read(STDIN_FILENO, &local_minmax, sizeof(local_minmax));
      min = local_minmax.min;
      max = local_minmax.max;
    }

    if (min < min_max.min) min_max.min = min;
    if (max > min_max.max) min_max.max = max;
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);

  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}
