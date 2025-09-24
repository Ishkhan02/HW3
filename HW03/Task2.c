#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

static void explain_status(int status, const char *label) {
    if (WIFEXITED(status)) {
        printf("%s: exited normally with code %d\n", label, WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("%s: killed by signal %d%s\n", label, WTERMSIG(status),
#ifdef WCOREDUMP
               WCOREDUMP(status) ? " (core dumped)" : ""
#else
               ""
#endif
        );
    } else if (WIFSTOPPED(status)) {
        printf("%s: stopped by signal %d\n", label, WSTOPSIG(status));
    } else if (WIFCONTINUED(status)) {
        printf("%s: continued\n", label);
    } else {
        printf("%s: unknown status: %d\n", label, status);
    }
}

int main(void) {
    pid_t c1 = fork();
    if (c1 < 0) { perror("fork c1"); return EXIT_FAILURE; }

    if (c1 == 0) { // child #1
        printf("[child1] PID=%d\n", getpid());
        _exit(21);
    }

    pid_t c2 = fork();
    if (c2 < 0) { perror("fork c2"); return EXIT_FAILURE; }

    if (c2 == 0) { // child #2
        printf("[child2] PID=%d\n", getpid());
        _exit(42);
    }

    int status = 0;
    pid_t w = waitpid(c2, &status, 0); // wait specifically for child 2
    if (w == -1) { perror("waitpid c2"); return EXIT_FAILURE; }
    explain_status(status, "[parent] child2");

    w = wait(&status);
    if (w == -1) { perror("wait"); return EXIT_FAILURE; }
    // Distinguish which child finished second
    if (w == c1) explain_status(status, "[parent] child1");
    else         explain_status(status, "[parent] (unexpected PID)");

    return 0;
}

