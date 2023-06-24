#include <fcntl.h>
#include <sys/stat.h>
#include <err.h>
#include <stdint.h>
#include <unistd.h>

struct pos {
    uint16_t pos;
    uint8_t len;
    uint8_t _;
};

int main(int argc, char** argv) {
    if (argc != 5) {
        errx(1, "ERROR: invalid arguments count");
    }

    int dat_fd, idx_fd, dat_out, idx_out;
    if ((dat_fd = open(argv[1], O_RDONLY)) == -1) {
        err(2, "ERORR: cannot open %s", argv[1]);
    }
    if ((idx_fd = open(argv[2], O_RDONLY)) == -1) {
        err(3, "ERORR: cannot open  %s", argv[2]);
    }
    if ((dat_out = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU)) == -1) {
        err(4, "ERROR: cannot open %s", argv[3]);
    }
    if ((idx_out = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU)) == -1) {
        err(5, "ERROR: cannot opne %s", argv[4]);
    }

    struct stat s;
    if (fstat(dat_fd, &s) == -1) {
        err(6, "ERROR: cannot stat");
    }

    int bytes_count = 0;
    struct pos pos;
    uint16_t offset = 0;
    while ((bytes_count = read(idx_fd, &pos, sizeof(pos))) > 0) {
        if (pos.pos + pos.len > s.st_size) {
            errx(7, "ERROR: incosistent files");
        }

        if (lseek(dat_fd, pos.pos, SEEK_SET) == -1) {
            err(8, "ERROR: cannot lseek");
        }

        uint8_t buf;
        if (read(dat_fd, &buf, sizeof(buf)) == -1) {
            err(9, "ERROR: cannot read from %s", argv[1]);
        }

        if (buf < 0x41 || buf > 0x5A) {
            continue;
        }

        if (write(dat_out, &buf, sizeof(buf)) == -1) {
            err(10, "ERROR: cannot write to %s", argv[3]);
        }

        struct pos out_pos;
        out_pos.pos = offset;
        out_pos.len = pos.len;
        out_pos._ = 0;

        if (write(idx_out, &out_pos, sizeof(out_pos)) == -1) {
            err(11, "ERROR: cannot write to %s", argv[4]);
        }

        for (int i = 0; i < pos.len - 1; ++i) {
            if (read(dat_fd, &buf, sizeof(buf)) == -1) {
                err(9, "ERROR: cannot read from %s", argv[1]);
            }
            if (write(dat_out, &buf, sizeof(buf)) == -1) {
                err(10, "ERROR: cannot write to %s", argv[3]);
            }
        }

        offset += pos.len;
    }

    if (bytes_count == -1) {
        err(11, "ERROR: cannot read from %s", argv[2]);
    }

    close(idx_fd);
    close(dat_fd);
    close(dat_out);
    close(idx_out);

    return 0;
}
