#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <err.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        errx(1, "ERROR: invalid arguments count");
    }

    int fd;
    if ((fd = open(argv[3], O_RDWR | O_CREAT | O_TRUNC, S_IRWXU)) == -1) {
        err(2, "ERROR: cannot open file");
    }

    pid_t pid1 = fork();
    if (pid1 == -1) {
        err(3, "ERROR: cannot fork");
    }

    int status1;
    if (pid1 > 0) {
        wait(&status1);
        if (WIFEXITED(status1)) {
            if (write(fd, argv[1], sizeof(argv[1])) == -1) {
                err(4, "ERROR: cannot write");
            }

            pid_t pid2 = fork();
            if (pid2 == -1) {
                err(3, "ERROR: cannot fork");
            }

            int status2;
            if (pid2 > 0) {
                wait(&status2);
            } else {
                if (execlp(argv[2], argv[2], NULL) == -1) {
                    err(5, "ERROR: cannot exec %s", argv[2]);
                }
            }
        }
    } else {
        if (execlp(argv[1], argv[1], NULL) == -1) {
            err(6, "ERROR: cannot exec %s", argv[1]);
        }
    }

    close(fd);

    return 0;
}
