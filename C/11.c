#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <err.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        errx(1, "ERROR: invalid arguments count");
    }

    pid_t pid = fork();
    if (pid == -1) {
        err(2, "ERROR: cannot fork");
    }

    int status;
    if (pid > 0) {
        wait(&status);
        if (WIFEXITED(status)) {
            execlp(argv[2], argv[2], NULL);
        }
    } else {
        if (execlp(argv[1], argv[1], NULL) == -1) {
            err(42, "ERROR: cannot exec %s", argv[1]);
        }
    }

    return 0;
}
