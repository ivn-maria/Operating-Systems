#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <err.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        errx(1, "ERROR: invalid argument count");
    }

    int fd1;
    if ((fd1 = open(argv[1], O_RDONLY)) == -1) {
        err(2, "ERROR: cannot open file 1");
    }

    int fd2;
    if ((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
        err(3, "ERROR: cannot open file 2");
    }

    uint8_t buf;
    int bytes_read, bytes_written;
    while ((bytes_read = read(fd1, &buf, sizeof(buf))) > 0) {
        bytes_written = write(fd2, &buf, bytes_read);
        if (bytes_written == -1) {
            err(4, "ERROR: cannot writo to file 2");
        }
    }

    if (bytes_read == -1) {
        err(5, "ERROR: cannot read from file 1");
    }

    close(fd1);
    close(fd2);

    return 0;
}                                             
