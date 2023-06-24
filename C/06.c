#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        errx(1, "ERROR: invalid arguments count");
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        err(2, "ERROR: cannot open file");
    }

    uint16_t max_byte = 0;
    uint16_t byte;
    int bytes_count;
    while ((bytes_count = read(fd, &byte, sizeof(byte))) > 0) {
        uint8_t byte1 = (byte >> 8) & 0xFF;
        uint8_t byte2 = byte & 0xFF;

        if (byte1 > max_byte) {
            max_byte = byte1;
        }
        if (byte2 > max_byte) {
            max_byte = byte2;
        }
    }

    if (bytes_count == -1) {
        err(3, "ERROR: cannot read from file");
    }

    write(1, &max_byte, sizeof(max_byte));

    close(fd);

    return 0;
}
