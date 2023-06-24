#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

int main(int argc, char** argv) {
    if (argc != 3) {
        errx(1, "ERROR: inavalid arguments count");
    }

    char str[4096];
    if (read(0, &str, sizeof(str)) == -1) {
        err(2, "ERROR: cannot read from stdin");
    }

    char set1[4096];
    char set2[4096];
    bool buff[256];
    for (int i = 0; i < 256; i++) {
	buff[i] = 0;
    }
    char sub[256];
    if (strcmp(argv[1],"-d") == 0) {
        strcpy(set1, argv[2]);

        for (unsigned long i = 0; i < strlen(str); i++) {
            buff[(int)set1[i]] = 1;
        }

        for (unsigned long i = 0; i < strlen(str); i++) {
            if (buff[(int)str[i]] == 0) {
                if (write(1, &str[i], sizeof(str[i])) == -1) {
                    err(3, "ERROR: cannot write to stdout");
                }
            }
        }
    } else if  (strcmp(argv[1],"-s") == 0) {
        strcpy(set1, argv[2]);

        for (unsigned long i = 0; i < strlen(str); i++) {
            buff[(int)set1[i]] = 1;
        }

        bool written[256];
	for (int i = 0; i < 256; i++) {
 	    written[i] = 0;
 	}
        for (unsigned long i = 0; i < strlen(str); i++) {
            if (buff[(int)str[i]] == 0) {
                if (write(1, &str[i], sizeof(str[i])) == -1) {
                    err(3, "ERROR: cannot write to stdout");
                }
            } else if (buff[(int)str[i]] == 1 && written[(int)str[i]] == 0) {
                written[(int)str[i]] = 1;
                if (write(1, &str[i], sizeof(str[i])) == -1) {
                    err(3, "error: cannot write to stdout");
                }
            }
        }
    } else {
        if (strlen(argv[1]) != strlen(argv[2])) {
            errx(4, "ERROR: lenght of SET1 and SET2 mismatch");
        }
        strcpy(set1, argv[1]);
        strcpy(set2, argv[2]);

        for (unsigned long i = 0; i < strlen(str); i++) {
            buff[(int)set1[i]] = 1;
        }

        for (unsigned long i = 0; i < strlen(set1); i++) {
            sub[(int)set1[i]] = set2[i];
        }

        for (unsigned long i = 0; i < strlen(set1); i++) {
            if (buff[(int)str[i]] == 0) {
                if (write(1, &str[i], sizeof(str[i])) == -1) {
                    err(3, "ERROR: cannot write to stdout");
                }
            } else {
                if (write(1, &sub[(int)str[i]], sizeof(sub[(int)str[i]])) == -1) {
                    err(3, "ERROR: cannot write to stdout");
                }
            }
        }
    }

    return 0;
}
