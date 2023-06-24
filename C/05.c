#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <string.h>

int main(int argc, char** argv) {
    if (argc != 3) {
        errx(1, "ERROR: invalid arguments count");
    }

    int fd = open(argv[2], O_RDONLY);
    if (fd == -1) {
        err(2, "ERROR: cannot open file");
    }

    if (strcmp(argv[1], "--min") == 0) {
        uint16_t min_val = UINT16_MAX;
        uint16_t num;
        int bytes_read;
        while ((bytes_read = read(fd, &num, sizeof(num))) > 0) {
            if (num < min_val) {
                min_val = num;
            }
        }

        if (bytes_read == -1) {
            err(3, "ERROR: cannot read from file");
        }
        write(1, &min_val, sizeof(min_val));
    } else if (strcmp(argv[1], "--max") == 0) {
        uint16_t max_val = 0;
        uint16_t num;
        int bytes_read;
        while ((bytes_read = read(fd, &num, sizeof(num))) > 0) {
            if (num > max_val) {
                max_val = num;
            }
        }

        if (bytes_read == -1) {
            err(3, "ERROR: cannot read from file");
        }
        write(1, &max_val, sizeof(max_val));
    } else if (strcmp(argv[1], "--print") == 0) {
        uint16_t num;
        int bytes_read;
        while ((bytes_read = read(fd, &num, sizeof(num))) > 0) {
            write(1, &num, sizeof(num));
        }

        if (bytes_read == -1) {
            err(3, "ERROR: cannot read from file");
        }
    } else {
        errx(4, "ERROR: invalid option");
    }

    close(fd);

    return 0;
}
