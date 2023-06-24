#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <err.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        errx(1, "ERROR: invalid arguments count");
    }

    pid_t pid1 = fork();
    if (pid1 == -1) {
        err(2, "ERROR: cannot fork");
    }

    if (pid1 == 0) {
        if (execlp(argv[1], argv[1], NULL) == -1) {
            err(3, "ERROR: cannot execlp %s", argv[1]);
        }
    }

    pid_t pid2 = fork();
    if (pid2 == -1) {
        err(2, "ERROR: cannot fork");
    }

    if (pid2 == 0) {
        if (execlp(argv[2], argv[2], NULL) == -1) {
            err(4, "ERROR: cannot execlp %s", argv[2]);
        }
    }

    int status1, status2;
    pid_t result;

    while ((result = wait(&status1)) > 0) {
        if (WIFEXITED(status1)) {
            dprintf(1, "%d\n", result);
            return 0;
        }
    }

    if (result == -1) {
        err(5, "ERROR: cannot wait");
    }

    while ((result = wait(&status2)) > 0) {
        if (WIFEXITED(status2)) {
            dprintf(1, "%d\n", result);
            return 0;
        }
    }

    if (result == -1) {
        err(5, "ERROR: cannot wait");
    }

    dprintf(1, "-1\n");

    return 0;
}
