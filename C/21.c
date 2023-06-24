#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <stdint.h>
#include <sys/stat.h>

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
        err(2, "ERROR: cannot stat file %s", argv[1]);
    }

    int numbers_count = s1.st_size / sizeof(uint32_t);
    if (numbers_count > 4194304) {
	errx(3, "ERROR: file countains too many elements");
    }

    int fd1;
    if ((fd1 = open(argv[1], O_RDONLY)) == -1) {
        err(4, "ERROR: cannot open file %s", argv[1]);
    }

    int left_half = s1.st_size / 2;
    int right_half = left_half + (s1.st_size % 2);
    char temp1[11] = "tempXXXXXX";
    char temp2[11] = "tempXXXXXX";
    int fd2;
    int fd3;

    if ((fd2 = mkstemp(temp1)) == -1) {
        err(5, "ERROR: cannot open file %s", temp1);
    }
    uint32_t* half1 = malloc(left_half * sizeof(uint32_t));
    for (int i = 0; i < left_half; i++) {
        if (read(fd1, &half1[i], sizeof(uint32_t)) == -1) {
            err(6, "ERROR: cannot read from file %s", argv[1]);
        }
    }
    qsort(half1, left_half, sizeof(uint32_t) ,comp);
    for (int i = 0; i < left_half; i++) {
    	if (write(fd2, &half1[i], sizeof(half1[i])) == -1) {
             err(7, "ERROR: cannot write to file %s", temp1);
    	}
    }

    if ((fd3 = mkstemp(temp2)) == -1) {
        err(9, "ERROR: cannot open file %s", temp2);
    }
    uint32_t* half2 = malloc(right_half * sizeof(uint32_t));
    for (int i = 0; i < right_half; i++) {
        if (read(fd1, &half2[i], sizeof(uint32_t)) == -1) {
            err(6, "ERROR: cannot read from file %s", argv[1]);
        }
    }
    qsort(half2, right_half, sizeof(uint32_t) ,comp);
    for (int i = 0; i < right_half; i++) {
    	if (write(fd3, &half2[i], sizeof(half2[i])) == -1) {
            err(8, "ERROR: cannot write to file %s", temp2);
    	}
    }

    free(half1);
    free(half2);

    int fd4;
    if ((fd4 = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR)) == -1) {
        err(9, "ERROR cannot open file %s", argv[2]);
    }

    uint32_t num1;
    if (read(fd2, &num1, sizeof(num1)) == -1) {
        err(10, "ERROR: cannot read from file %s", temp1);
    }
    uint32_t num2;
    if (read(fd3, &num2, sizeof(num2)) == -1) {
        err(11, "ERROR: cannot read from file %s", temp2);
    }

    int counter = 0;

    int min_size;
    if (left_half < right_half) { min_size = left_half; }
    else { min_size = right_half; }
    while (counter < min_size) {
        if (num1 < num2) {
    	    if (write(fd4, &num1, sizeof(num1)) == -1) {
                err(12, "ERROR: cannot write to file %s", argv[2]);
    	    }
	    if (read(fd2, &num1, sizeof(num1)) == -1) {
        	err(10, "ERROR: cannot read from file %s", temp1);
    	    }	
        } else {
    	    if (write(fd4, &num2, sizeof(num2)) == -1) {
        	err(13, "ERROR: cannot write to file %s", argv[2]);
    	    }
    	    if (read(fd3, &num2, sizeof(num2)) == -1) {
        	err(11, "ERROR: cannot read from file %s", temp2);
    	    }
        }
        counter++;
    }

    while (counter < left_half) {
    	if (write(fd4, &num1, sizeof(num1)) == -1) {
            err(12, "ERROR: cannot write to file %s", argv[2]);
    	}
	if (read(fd2, &num1, sizeof(num1)) == -1) {
            err(10, "ERROR: cannot read from file %s", temp1);
    	}	
        counter++;
    }

    while (counter < right_half) {
    	if (write(fd4, &num2, sizeof(num2)) == -1) {
            err(12, "ERROR: cannot write to file %s", argv[2]);
    	}
    	if (read(fd3, &num2, sizeof(num2)) == -1) {
            err(11, "ERROR: cannot read from file %s", temp2);
    	}
        counter++;
    }

    if (unlink(temp1) == -1) {
        err(13, "ERROR: cannot unlink file %s", temp1);
    }
    if (unlink(temp2) == -1) {
        err(13, "ERROR: cannot unlink file %s", temp2);
    }

    close(fd1);
    close(fd2);
    close(fd3);
    close(fd4);

    return 0;
}
