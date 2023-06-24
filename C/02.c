#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <err.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        errx(1, "ERROR: invalid arguments count");
    }

    for (int i = 1; i < argc; i++) {
        int fd;
        if ((fd = open(argv[i], O_RDONLY)) == -1) {
            err(2, "ERROR: cannot open file %d", i);
        }

        uint8_t buf;
        int bytes_read, bytes_written;
        while ((bytes_read = read(fd, &buf, sizeof(buf))) > 0) {
            bytes_written = write(1, &buf, bytes_read);
            if (bytes_written == -1) {
                err(3, "ERROR: cannot write to stdin");
            }
        }

        if (bytes_read == -1) {
            err(4, "ERROR: cannot read from file %d", i);
        }

        close(fd);
    }

    return 0;
}
