#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void cleanup1(void) { puts("cleanup1: runs at process exit"); }
static void cleanup2(void) { puts("cleanup2: runs at process exit"); }

int main(void) {
    if (atexit(cleanup1) != 0) { perror("atexit cleanup1"); return EXIT_FAILURE; }
    if (atexit(cleanup2) != 0) { perror("atexit cleanup2"); return EXIT_FAILURE; }

    puts("main: before conditional exit");

    if (getpid() % 2 == 0) {
        puts("main: calling exit(0) early...");
        exit(0);
    }

    puts("main: normal return (also triggers atexit handlers)");
    return 0; // same effect as exit(0) for atexit handlers
}

