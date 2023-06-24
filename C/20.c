#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>

int comparator(const void*, const void*);

int comparator(const void* a, const void* b) {
    return (*(uint32_t*)a - *(uint32_t*)b);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        errx(1, "ERROR: invalid arguments count");
    }

    int fd;
    if ((fd = open(argv[1], O_RDWR)) == -1) {
        err(2, "ERROR: cannot open file %s", argv[1]);
    }

    struct stat s;
    if (fstat(fd, &s) == -1) {
        err(3, "ERROR: cannot get file size");
    }

    int num_elements = s.st_size / sizeof(uint32_t);
    if (num_elements > 100000000) {
        errx(4, "ERROR: too many elements in the file");
    }

    int left_half = num_elements / 2;

    uint32_t* first;
    if ((first = (uint32_t*) malloc(left_half * sizeof(uint32_t))) == NULL) {
        err(5, "ERROR: cannot malloc");
    }

    if (read(fd, first, left_half * sizeof(uint32_t)) == -1) {
       err(6, "ERROR: cannot read");
    }

    qsort(first, left_half, sizeof(uint32_t), comparator);

    int fd1;
    if ((fd1 = open("first_file", O_RDWR | O_CREAT | O_TRUNC, 0666)) == -1) {
       err(7, "ERROR: cannot open file");
    }
    if (write(fd1, first, left_half * sizeof(uint32_t)) == -1) {
        err(8, "ERROR: cannot write");
    }
    if (lseek(fd1, 0, SEEK_SET) == -1) {
        err(9, "ERROR: cannot lseek");
    }

    free(first);

    int right_half = num_elements - left_half;

    uint32_t* second;
    if ((second = (uint32_t*) malloc(right_half * sizeof(uint32_t))) == NULL) {
        err(5, "ERROR: cannot malloc");
    }

    if (read(fd, second, right_half * sizeof(uint32_t)) == -1) {
       err(6, "ERROR: cannot read");
    }

    qsort(second, right_half, sizeof(uint32_t), comparator);

    int fd2;
    if ((fd2 = open("second_file", O_RDWR | O_CREAT | O_TRUNC, 0666)) == -1) {
       err(7, "ERROR: cannot open file");
    }
    if (write(fd2, second, right_half * sizeof(uint32_t)) == -1) {
        err(8, "ERROR: cannot write");
    }
    if (lseek(fd2, 0, SEEK_SET) == -1) {
        err(9, "ERROR: cannot lseek");
    }

    free(second);

    if (lseek(fd, 0, SEEK_SET) == -1) {
        err(9, "ERROR: cannot lseek");
    }

    uint32_t a,b;
    int read_bytes1 = 0, read_bytes2 = 0;
    while ((read_bytes1 = read(fd1, &a, sizeof(a))) > 0 && (read_bytes2 = read(fd2, &b, sizeof(b))) > 0) {
        if (a <= b) {
           if (write(fd, &a, sizeof(a)) == -1) {
              err(8, "ERROR: cannot write");
           }
           if (lseek(fd2, -sizeof(b), SEEK_CUR) == -1) {
              err(9, "ERROR: cannot lseek");
           }
        } else {
           if (write(fd, &b, sizeof(b)) == -1) {
              err(8, "ERROR: cannot write");
           }
           if (lseek(fd1, -sizeof(a), SEEK_CUR) == -1) {
              err(9, "ERROR: cannot lseek");
           }
        }
    }

    if (read_bytes1 == -1 || read_bytes2 == -1) {
       err(6, "ERROR: cannot read");
    }

    read_bytes1 = 0;
    while ((read_bytes1 = read(fd1, &a, sizeof(a))) > 0) {
        if (write(fd, &a, sizeof(a)) == -1) {
            err(8, "ERROR: cannot write");
        }
    }

    if (read_bytes1 == -1) {
        err(6, "ERROR: cannot read");
    }

    read_bytes2 = 0;
    while ((read_bytes2 = read(fd2, &b, sizeof(b))) > 0) {
        if (write(fd, &b, sizeof(b)) == -1) {
            err(8, "ERROR: cannot write");
        }
    }

    if (read_bytes2 == -1) {
        err(6, "ERROR: cannot read");
    }

    close(fd1);
    close(fd2);
    close(fd);

    return 0;
}
