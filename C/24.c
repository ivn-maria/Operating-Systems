#include <fcntl.h>
#include <sys/stat.h>
#include <err.h>
#include <stdint.h>
#include <unistd.h>

struct patch_entry {
    uint16_t offset;
    uint8_t original_byte;
    uint8_t new_byte;
};

int main(int argc, char** argv) {
    if (argc != 4) {
        errx(1, "ERROR: invalid arguments count");
    }

    int f1_fd, f2_fd, patch_fd;
    if ((f1_fd = open(argv[1], O_RDONLY)) == -1) {
        err(2, "ERROR: cannot open %s", argv[1]);
    }
    if ((f2_fd = open(argv[2], O_RDONLY)) == -1) {
        err(3, "ERROR: cannot open %s", argv[2]);
    }
    if ((patch_fd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU)) == -1) {
        err(4, "ERROR: cannot open %s", argv[3]);
    }

    struct stat s;
    if (fstat(f1_fd, &s) == -1) {
        err(5, "ERROR: cannot stat %s", argv[1]);
    }
    int f1_size = s.st_size;

    if (fstat(f2_fd, &s) == -1) {
        err(6, "ERROR: cannot stat %s", argv[2]);
    }
    int f2_size = s.st_size;

    if (f1_size != f2_size) {
        errx(7, "ERROR: files sizes mismatch");
    }

    uint8_t f1_buf, f2_buf;
    struct patch_entry entry;
    for (int offset = 0; offset < f1_size; offset++) {
        if (lseek(f1_fd, offset, SEEK_SET) == -1) {
            err(8, "ERROR: cannot lseek %s", argv[1]);
        }
        if (lseek(f2_fd, offset, SEEK_SET) == -1) {
            err(9, "ERROR: cannot lseek %s", argv[2]);
        }

        if (read(f1_fd, &f1_buf, sizeof(f1_buf)) == -1) {
            err(10, "ERROR: cannot read from %s", argv[1]);
        }
        if (read(f2_fd, &f2_buf, sizeof(f2_buf)) == -1) {
            err(11, "ERROR: cannot read from %s", argv[2]);
        }

        if (f1_buf != f2_buf) {
            entry.offset = (uint16_t)offset;
            entry.original_byte = f1_buf;
            entry.new_byte = f2_buf;

            if (write(patch_fd, &entry, sizeof(entry)) == -1) {
                err(12, "ERROR: cannot write to %s", argv[3]);
            }
        }
    }

    close(f1_fd);
    close(f2_fd);
    close(patch_fd);

    return 0;
}
