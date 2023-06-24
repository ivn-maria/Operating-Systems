#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <err.h>
#include <sys/types.h>
#include <stdlib.h>

uint8_t set_bit(uint8_t x, int idx, bool bit);

uint8_t set_bit(uint8_t x, int idx, bool bit) {
    if (bit) {
        return (1 << idx) | x;
    } else {
        return (~(1 << idx)) & x;
    }
}

int main(int argc, char** argv) {
    if (argc != 3) {
        errx(1, "ERROR: invalid arguments count");
    }

    int fd1;
    if ((fd1 = open(argv[1], O_RDONLY)) == -1) {
        err(2, "ERROR: cannot open file %s", argv[1]);
    }
    int fd2;
    if ((fd2 = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR)) == -1) {
        err(3, "ERROR: cannot open file %s", argv[2]);
    }

    uint16_t in_buf;
    uint8_t out_buf;
    int bytes_read;
    while ((bytes_read = read(fd1, &in_buf, sizeof(uint16_t))) > 0) {
        out_buf = 0;

        for (int j = 0; j < 8; j++) {
            bool first = in_buf & (1 << j*2);
            bool second = in_buf & ( 1 << (j*2 + 1));

            if (!first && second) {
                out_buf = set_bit(out_buf, j, 0);
            } else if (first && !second) {
                out_buf = set_bit(out_buf, j, 1);
            } else {
                errx(4, "ERROR: invalid data");
            }
        }

        if (write(fd2, &out_buf, sizeof(uint8_t)) == -1) {
            err(5, "ERROR: cannot write to file %s", argv[2]);
        }
    }

    if (bytes_read == -1) {
        err(6, "ERROR: cannot read from %s", argv[1]);
    }

    close(fd1);
    close(fd2);

    return 0;
}
