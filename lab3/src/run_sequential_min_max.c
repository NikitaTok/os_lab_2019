#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    // Создание массива аргументов для передачи в sequential_min_max
    char *args[] = {"./sequential_min_max", "--seed", "42", "--array_size", "1000", "--pnum", "4", NULL};

    // Запуск sequential_min_max в отдельном процессе
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {
        // Мы находимся в дочернем процессе
        execvp(args[0], args);
        perror("Exec failed"); // Если exec вернул (ошибка)
        return 1;
    }

    // Мы находимся в родительском процессе
    int status;
    waitpid(pid, &status, 0); // Ожидание завершения дочернего процесса

    if (WIFEXITED(status)) {
        printf("Child process exited with code %d\n", WEXITSTATUS(status));
    } else {
        printf("Child process did not exit normally\n");
    }

    return 0;
}