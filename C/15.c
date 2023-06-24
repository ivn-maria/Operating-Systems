#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        errx(1, "ERROR: invalid arguments count");
    }

    uint32_t bytes[256];
    for (int i = 0; i < 256; i++) {
        bytes[i] = 0;
    }

    int fd_read;
    if ((fd_read = open(argv[1], O_RDWR)) == -1) {
        err(2, "ERROR: cannot open file %s", argv[1]);
    }

    uint8_t buf;
    int bytes_count;
    while ((bytes_count = read(fd_read, &buf, sizeof(buf))) > 0) {
        bytes[buf] += 1;
    }

    if (bytes_count == -1) {
        err(3, "ERROR: cannot read from file");
    }

    close(fd_read);

    int fd_write;
    if ((fd_write = open(argv[1], O_WRONLY)) == -1) {
        err(3, "ERROR: cannot open file %s", argv[1]);
    }

    for (uint16_t i = 0; i < 256; i++) {
        uint8_t temp = i;
        for (uint32_t j = 0; j < bytes[i]; j++) {
            if (write(fd_write, &temp, sizeof(temp)) == -1) {
                err(4, "ERROR: cannot write to file");
            }
        }
    }

    close(fd_write);

    return 0;
}
