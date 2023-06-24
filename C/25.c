#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>

struct patch_entry {
    uint16_t offset;
    uint8_t original_byte;
    uint8_t new_byte;
};

int main(int argc, char** argv) {
    if (argc != 4) {
        errx(1, "ERROR: invalid arguments count");
    }

    int patch_fd = open(argv[1], O_RDONLY);
    if (patch_fd == -1) {
        err(2, "ERROR: cannot open %s", argv[1]);
    }
    int input_fd = open(argv[2], O_RDONLY);
    if (input_fd == -1) {
        err(3, "ERROR: cannot open %s", argv[2]);
    }
    int output_fd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd == -1) {
        err(4, "ERROR: connot open %s", argv[3]);
    }

    uint8_t buffer;
    int bytes_read;
    while ((bytes_read = read(input_fd, &buffer, sizeof(buffer))) > 0) {
        if (write(output_fd, &buffer, bytes_read) == -1) {
            err(5, "ERROR: cannot write output file");
        }
    }

    if (bytes_read == -1) {
        err(6, "ERROR: cannot read input file");
    }

    struct patch_entry entry;
    while (read(patch_fd, &entry, sizeof(entry)) > 0) {
        int offset = entry.offset;
        if (lseek(input_fd, offset, SEEK_SET) == -1) {
            err(7, "ERROR: cannot lseek");
        }
        if (lseek(output_fd, offset, SEEK_SET) == -1) {
            err(8, "ERROR: cannot lseek");
        }

        uint8_t byte;
        if (read(input_fd, &byte, sizeof(byte)) != sizeof(byte)) {
            err(9, "ERROR: cannot read from file %s", argv[2]);
        }
        if (byte == entry.original_byte) {
            byte = entry.new_byte;
            if (write(output_fd, &byte, sizeof(byte)) != sizeof(byte)) {
                err(10, "ERROR: cannot write to file %s", argv[3]);
            }
        } else {
            errx(10, "ERROR: files contents mismatch");
        }
    }

    close(patch_fd);
    close(input_fd);
    close(output_fd);

    return 0;
}
