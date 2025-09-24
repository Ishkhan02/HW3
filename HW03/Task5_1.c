#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return EXIT_FAILURE; }

    if (pid == 0) {
        printf("[child] PID=%d exiting now...\n", getpid());
        _exit(0);
    }

    printf("[parent] PID=%d, child PID=%d. Sleeping 20s without wait()...\n",
           getpid(), pid);
    sleep(20);
    puts("[parent] exiting without reaping child");
    return 0;
}
