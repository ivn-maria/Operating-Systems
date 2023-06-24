#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <stdint.h>

int main(void) {
    int fd_read = open("/etc/passwd", O_RDONLY);
    if (fd_read == -1) {
        err(1, "ERROR: cannot open /etc/passwd");
    }

    int fd_write = open("passwd_copy", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_write == -1) {
        err(2, "ERROR: cannot open destination file");
    }

    uint8_t buffer;
    int bytes_read, bytes_written;
    while ((bytes_read = read(fd_read, &buffer, sizeof(buffer))) > 0) {
        bytes_written = write(fd_write, &buffer, bytes_read);
        if (bytes_written == -1) {
            err(3, "ERROR: cannot write to destination file");
        }
    }

    if (bytes_read == -1) {
        err(4, "ERROR: cnnot read from /etc/passwd");
    }

    lseek(fd_read, 0, SEEK_SET);

    uint8_t replace = '?';
    uint8_t delimiter = ':';
    while ((bytes_read = read(fd_read, &buffer, sizeof(buffer))) > 0) {
        if (buffer == delimiter) {
            buffer = replace;
        }

        bytes_written = write(fd_write, &buffer, bytes_read);
        if (bytes_written == -1) {
            err(5, "ERROR: cannot write to destination file");
        }
    }

    if (bytes_read == -1) {
        err(6, "ERROR: cannot read from /etc/passwd");
    }

    close(fd_read);
    close(fd_write);

    return 0;
}
