#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
  pid_t pid = fork();

  if (pid == 0) {
    // Дочерний процесс
    printf("Дочерний процесс: PID = %d\n", getpid());
    sleep(5); // Дочерний процесс завершится через 5 секунд
    exit(0); // Завершаем дочерний процесс
  } else if (pid > 0) {
    // Родительский процесс
    printf("Родительский процесс: PID = %d\n", getpid());
    sleep(10); // Родительский процесс будет ждать 10 секунд
    //wait(NULL); // Ждем завершения дочернего процесса
  } else {
    // Ошибка при создании процесса
    perror("fork");
    exit(1);
  }

  return 0;
}