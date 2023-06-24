#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        errx(1, "ERROR: invalid arguments count");
    }

    int fd;
    if ((fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, S_IRWXU)) == -1) {
        err(2, "ERROR: cannot open file");
    }

    pid_t pid = fork();
    if (pid == -1) {
        err(3, "ERROR: cannot fork");
    }

    if (pid > 0) {
        char buf[2];
        wait(NULL);
        lseek(fd, 0, SEEK_SET);
        while (read(fd, &buf, 2) > 0) {
            write(1, &buf, 2);
            write(1, " ", 1);
        }   
    } else {
        if (write(fd, "foobar", 6) == -1) {
            err(4, "ERROR: cannot write");
        }   
    }   

    return 0;
}   
