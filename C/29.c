#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>

struct Interval {
    uint8_t start;
    uint8_t length;
};

struct Set {
    struct Interval post;
    struct Interval pre;
    struct Interval in;
    struct Interval suf;
};

int main(int argc, char** argv) {
    if (argc != 7) {
        errx(1, "ERROR: invalid arguments count");
    }

    int fd_affix, fd_postfix, fd_prefix, fd_infix, fd_suffix, fd_crucifixus;
    if ((fd_affix = open(argv[1], O_RDONLY)) == -1 ||
        (fd_postfix = open(argv[2], O_RDONLY)) == -1 ||
        (fd_prefix = open(argv[3], O_RDONLY)) == -1 ||
        (fd_infix = open(argv[4], O_RDONLY)) == -1 ||
        (fd_suffix = open(argv[5], O_RDONLY)) == -1) {
        err(2, "ERROR: open input files failed");
    }

    if ((fd_crucifixus = open(argv[6], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
        err(3, "ERROR: open output file failed");
    }

    uint16_t count;
    if (lseek(fd_affix, 4, SEEK_SET) == -1) {
    	err(4, "ERRPR: lseek affix file failed");
    }
    if (read(fd_affix, &count, sizeof(uint16_t)) == -1) {
        err(5, "ERROR: read from affix file failed");
    }

    if (lseek(fd_affix, 16, SEEK_SET) == -1 ||
        lseek(fd_postfix, 16, SEEK_SET) == -1 ||
        lseek(fd_prefix, 16, SEEK_SET) == -1 ||
        lseek(fd_infix, 16, SEEK_SET) == -1 ||
        lseek(fd_suffix, 16, SEEK_SET) == -1) {
        err(6, "ERROR: lseek input files failed");
    }

    for (uint16_t i = 0; i < count; i++) {
        struct Set set;
        if (read(fd_affix, &set, sizeof(struct Set)) == -1) {
            err(7, "ERROR: read from affix file failed");
        }

        uint32_t postfix[set.post.length];
        uint8_t prefix[set.pre.length];
        uint16_t infix[set.in.length];
        uint64_t suffix[set.suf.length];
        if (lseek(fd_postfix, set.post.start, SEEK_SET) == -1 ||
	    lseek(fd_prefix, set.pre.start, SEEK_SET) == -1 || 
            lseek(fd_infix, set.in.start, SEEK_SET) == -1 || 
            lseek(fd_suffix, set.suf.start, SEEK_SET) == -1) {
            err(8, "ERROR: lseek files with elements failed");
	}
        if (read(fd_postfix, postfix, sizeof(uint32_t) * set.post.length) == -1 ||
            read(fd_prefix, prefix, sizeof(uint8_t) * set.pre.length) == -1 ||
            read(fd_infix, infix, sizeof(uint16_t) * set.in.length) == -1 ||
            read(fd_suffix, suffix, sizeof(uint64_t) * set.suf.length) == -1) {
            err(9, "ERROR: read from files with elements failed");
        }

        if (write(fd_crucifixus, postfix, sizeof(uint32_t) * set.post.length) == -1 ||
            write(fd_crucifixus, prefix, sizeof(uint8_t) * set.pre.length) == -1 ||
            write(fd_crucifixus, infix, sizeof(uint16_t) * set.in.length) == -1 ||
            write(fd_crucifixus, suffix, sizeof(uint64_t) * set.suf.length) == -1) {
            err(10, "ERROR: write to crucifixus file failed");
        }
    }

    close(fd_affix);
    close(fd_postfix);
    close(fd_prefix);
    close(fd_infix);
    close(fd_suffix);
    close(fd_crucifixus);

    return 0;
}
