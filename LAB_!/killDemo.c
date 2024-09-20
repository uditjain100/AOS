#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int n = argc;

    if (n < 2) {
        printf("Invalid Command !!");
        return -1;
    }

    int signal = SIGTERM; // 9
    int pIdx = 1;

    if (argv[1][0] == '-') {
        signal = atoi(argv[1] + 1);
        if (signal <= 0) {
            printf("Invalid signal: %s\n", argv[1]);
            return -1;
        }
        pIdx = 2; 
    }

    for (int i = pIdx; i < n; ++i) {
        pid_t pid = atoi(argv[i]);

        if (pid <= 0) {
            printf("Invalid PID: %s\n", argv[i]);
            continue;
        }

        if (kill(pid, signal) == -1) {
            printf("kill error !!");
            return -1;
        }
    }

    return 0;
}
