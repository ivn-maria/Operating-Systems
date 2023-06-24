#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void) {
    int fd;
    if ((fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) == -1) {
        err(1, "ERROR: cannot create file");
    }

    pid_t pid = fork();
    if (pid == -1) {
        err(2, "ERROR: cannot fork");
    }

    if (pid > 0) {
        wait(NULL);
        const char* str = "bar";
        if (write(fd, str, strlen(str)) == -1) {
            err(3, "ERROR: cannot write to file");
        }
    } else {
        const char* str = "foo";
        if (write(fd, str, strlen(str)) == -1) {
            err(3, "ERROR: cannot write to file");
        }
    }

    close(fd);

    return 0;
}
