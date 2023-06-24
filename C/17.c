#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char** argv) {
    if (argc == 1) {
        char buff;
        int bytes_read = 0;
        while ((bytes_read = read(0, &buff, sizeof(buff))) > 0) {
            if (write(1, &buff, sizeof(buff)) == -1) {
                err(1, "ERROR: cannot write to stdout");
            }
        }

        if (bytes_read == -1) {
            err(2, "ERROR: cannot read from stdin");
        }
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-") == 0) {
            char buff;
            int bytes_read = 0;
            while ((bytes_read = read(0, &buff, sizeof(buff))) > 0) {
                if (write(1, &buff, sizeof(buff)) == -1) {
                    err(1, "ERROR: cannot write to stdout");
                }
            }

            if (bytes_read == -1) {
                err(2, "ERROR: cannot read from stdin");
            }
        } else {
            int fd;
            if ((fd = open(argv[i], O_RDONLY)) == -1) {
                err(3, "ERROR: cannot not open file %s", argv[i]);
            }

            char buff;
            int bytes_read = 0;
            while ((bytes_read = read(fd, &buff, sizeof(buff))) > 0) {
                if (write(1, &buff, sizeof(buff)) == -1) {
                    err(1, "ERROR: cannot write to stdout");
                }
            }

            if (bytes_read == -1) {
                err(4, "ERROR: cannot read from %s", argv[i]);
            }

            close(fd);
        }
    }

    return 0;
}
