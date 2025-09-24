#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

static void report(pid_t child, int status) {
    if (WIFEXITED(status)) {
        printf("Child %d exited normally with code %d\n", child, WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("Child %d terminated by signal %d\n", child, WTERMSIG(status));
    } else {
        printf("Child %d ended with non-standard status %d\n", child, status);
    }
}

int main(void) {
    pid_t c1 = fork();
    if (c1 < 0) { perror("fork c1"); return EXIT_FAILURE; }
    if (c1 == 0) _exit(3); // child 1 exits with code 3

    pid_t c2 = fork();
    if (c2 < 0) { perror("fork c2"); return EXIT_FAILURE; }
    if (c2 == 0) _exit(7); // child 2 exits with code 7


    int status;
    pid_t w = waitpid(c1, &status, 0);
    if (w == -1) { perror("waitpid c1"); return EXIT_FAILURE; }
    report(w, status);

    w = waitpid(c2, &status, 0);
    if (w == -1) { perror("waitpid c2"); return EXIT_FAILURE; }
    report(w, status);

    return 0;
}

