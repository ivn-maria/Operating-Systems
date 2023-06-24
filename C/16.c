#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <stdbool.h>

uint8_t read_fd(int, uint8_t, bool);

uint8_t read_fd(int fd, uint8_t line_count, bool flag) {
    uint8_t buff;
    int bytes_count;
    bool flag2 = true;
    while ((bytes_count = read(fd, &buff, sizeof(buff))) > 0) {
        if (flag == true && flag2 == true) {
            dprintf(1, "%d ", line_count);
            line_count++;
        }

        if ((write(1, &buff, sizeof(buff))) == -1) {
            err(2, "ERROR: cannot write to stdout");
        }

        if (buff == '\n') {
            flag2 = true;
        } else {
            flag2 = false;
        }

    }

    if (bytes_count == -1) {
        err(3, "ERROR: cannot read from file");
    }

    return line_count;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        read_fd(0, 1, false);
    } else if (argc == 1 && strcmp(argv[1], "-n") == 0) {
	read_fd(0, 1, true);
    } else {
	uint8_t arg_count = 0;
        bool flag = false;
        if (strcmp(argv[1], "-n") == 0) {
            arg_count += 1;
            flag = true;
        }
        int line_count = 1;
        for (int i = 1 + arg_count; i < argc; i++) {
            int fd;
            if (strcmp(argv[i], "-") != 0) {
                fd = open(argv[i], O_RDONLY);
                if (fd == -1) {
                    err(1, "ERROR: cannot open %s", argv[i]);
                }
            } else {
                fd = 0;
            }
            line_count = read_fd(fd, line_count, flag);
        }
    }

    return 0;
}
