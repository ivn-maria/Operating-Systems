#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <sys/wait.h>

/* find DIR_NAME -mindepth 1 -type f -printf "%T@ %f\n" | sort -nr | head -n1 | cut -f2 -d ' ' */

int main(int argc, char** argv) {
    if (argc != 2) {
        errx(1, "ERROR: invalid arguments count");
    }

    int a[2];
    if (pipe(a) == -1) {
        err(2, "ERROR: first pipe failed");
    }

    pid_t pid = fork();
    if (pid == -1) {
        err(3, "ERROR: first fork failed");
    }

    if (pid == 0) {
        close(a[0]);
        if (dup2(a[1], 1) == -1) {
            err(4, "ERROR: dup find failed");
        }
        if (execlp("find", "find", argv[1], "-mindepth", "1", "-type", "f", "-printf", "%T@ %f\n", (char *)NULL) == -1) {
            err(5, "ERROR: find exec failed");
        }
    } else {
    	close(a[1]);
    }

    int b[2];
    if (pipe(b) == -1) {
        err(6, "ERROR: second pipe failed");
    }

    if ((pid = fork()) == -1) {
        err(7, "ERROR: second fork failed");
    }

    if (pid == 0) {
        close(b[0]);
        if (dup2(a[0], 0) == -1) {
            err(8, "ERROR: dup read end sort failed");
	}
        if (dup2(b[1], 1) == -1) {
            err(9, "ERROR: dup write end sort failed");
        }
        if (execlp("sort", "sort", "-nr", (char*)NULL) == -1) {
            err(10, "ERROR: sort exec failed");
        }
    } else {
    	close(b[1]);
    }

    close(a[0]);

    int c[2];
    if (pipe(c) == -1) {
        err(11, "ERROR: third pipe failed");
    }

    if ((pid = fork()) == -1) {
        err(12, "ERROR: third fork failed");
    }

    if (pid == 0) {
        close(c[0]);
        if (dup2(b[0], 0) == -1) {
            err(13, "ERROR: dup read end head failed");
	}
	if (dup2(c[1], 1) == -1) {
            err(14, "ERROR: dup write end head failed");
        }
        if (execlp("head", "head", "-n1", (char*)NULL) == -1) {
            err(15, "ERROR: head exec failed");
        }
    } else {
    	close(c[1]);
    }

    close(b[0]);

    while (wait(NULL) > 0);

    if (dup2(c[0], 0) == -1) {
        err(16, "ERROR: dup read end cut failed");
    }
    if (execlp("cut", "cut", "-f2", "-d", " ", (char*)NULL) == -1) {
        err(17, "ERROR: cut exec failed");
    }

    return 0;
}
