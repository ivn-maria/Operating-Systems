#include <unistd.h>
#include <err.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>

struct pair {
    uint32_t x;
    uint32_t y;
};

int main(int argc, char** argv) {
    if (argc != 4) {
        errx(1, "ERROR: invalid arguments count");
    }

    int fd1;
    if ((fd1 = open(argv[1], O_RDONLY)) == -1) {
        err(2, "ERROR: cannot open %s", argv[1]);
    }

    int fd2;
    if ((fd2 = open(argv[2], O_RDONLY)) == -1) {
        err(3, "ERROR: cannot open %s", argv[2]);
    }

    int fd3;
    if ((fd3 = open(argv[3], O_WRONLY | O_CREAT, S_IRWXU) ) == -1) {
        err(4, "ERROR: cannot open file %s", argv[3]);
    }

    struct pair p;
    int bytes_count = 0;
    while ((bytes_count = read(fd1, &p, sizeof(p))) > 0) {
        if (lseek(fd2, p.x, SEEK_SET) == -1) {
            err(5, "ERROR: cannot lseek");
        }

        uint32_t buf;
        for (uint32_t i = 0; i < p.y; ++i) {
            if (read(fd2, &buf, sizeof(buf)) == -1) {
                err(6, "ERROR: cannot read from %s", argv[2]);
            }
            if (write(fd3, &buf, sizeof(buf)) == -1) {
                err(7, "ERROR: cannot write to %s", argv[2]);
            }
        }
    }

    if (bytes_count == -1) {
        err(8, "ERROR: cannot read from %s", argv[1]);
    }

    return 0;
}
