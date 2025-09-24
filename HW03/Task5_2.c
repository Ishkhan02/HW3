#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return EXIT_FAILURE; }

    if (pid == 0) {
        _exit(0);
    }

    int status;
    if (waitpid(pid, &status, 0) == -1) { perror("waitpid"); return EXIT_FAILURE; }
    (void)status;

    sleep(5);
    return 0;
}

