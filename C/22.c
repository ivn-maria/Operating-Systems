#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

int comp(const void* a, const void* b);

int comp(const void* a, const void* b) {
    return (*(uint32_t*)a - *(uint32_t*)b);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        errx(1, "ERROR: invalid arguments count");
    }

    struct stat s1;
    if (stat(argv[1], &s1) == -1) {
        err(2, "ERROR: cannot stat %s", argv[1]);
    }

    int numbers_count = s1.st_size / sizeof(uint16_t);
    if (numbers_count > 65535) {
        errx(3, "ERROR: file countain too many elements");
    }

    int fd1;
    if ((fd1 = open(argv[1], O_RDONLY, S_IRUSR | S_IWUSR)) == -1) {
	err(4, "ERROR: cannot open %s", argv[1]);
    }
    int fd2;
    if ((fd2 = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) == -1) {
	err(5, "ERROR: cannot open %s", argv[2]);
    }

    uint16_t* numbers = malloc(numbers_count * sizeof(uint16_t));
    uint16_t number;
    int bytes_count;
    int index = 0;
    while((bytes_count = read(fd1, &number, sizeof(number))) == sizeof(number)) {
        numbers[index] = number;
        index++;
    }

    if (bytes_count == -1) {
        err(6, "ERROR: cannot read from %s", argv[1]);
    }

    qsort(numbers, numbers_count, sizeof(uint16_t), comp);

    for (int i = 0; i < numbers_count; i++) {
        if (write(fd2, &numbers[i], sizeof(numbers[i])) == -1) {
            err(7, "ERROR: cannot write to %s", argv[2]);
        }
    }
    
    free(numbers);
    
    close(fd1);
    close(fd2);

    return 0;
}
