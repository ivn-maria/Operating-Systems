#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char** argv) {
    if (argc != 2) {
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
            write(1, argv[1], strlen(argv[1]));
        }
    } else {
        execlp(argv[1], argv[1], (char*)NULL);
    }

    return 0;
}
