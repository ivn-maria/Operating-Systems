#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <stdint.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char** argv) {
    if (argc != 3) {
        errx(1, "ERROR: invalid arguments count");
    }

    int N = atoi(argv[1]);
    int D = atoi(argv[2]);

    if (!(N >= 0 && N <= 9) || !(D >= 0 && D <= 9)) {
	errx(2, "ERROR: invalid arguments");
    }

    for (int i = 0; i < N; i++) {
    	if (write(1, "DING\n", strlen("DING\n")) == -1) {
            err(3, "ERROR: write to stdout failed");
	}

        pid_t pid = fork();
        if (pid == -1) {
            err(4, "ERROR: fork failed");
        }

        if (pid == 0) {
    	    if (write(1, "DONG\n", strlen("DONG\n")) == -1) {
        	err(3, "ERROR: write to stdout failed");
    	    }
        }

        if (wait(NULL) == -1) {
            err(5, "ERROR: wait failed");
        }

        sleep(D);
    }

    return 0;
}
