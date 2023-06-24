#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        errx(1, "ERROR: invalid arguments count");
    }

    int a[2];
    if (pipe(a) == -1) {
        err(2, "ERROR: pipe failed");
    }

    pid_t pid = fork();
    if (pid == -1) {
        err(3, "ERROR: fork failed");
    }

    if (pid == 0) {
        close(a[0]);
        if (dup2(a[1], 1) == -1) {
            err(4, "ERROR: dup cat failed");
        }
        if (execlp("cat", "cat", argv[1], (char *)NULL) == -1) {
            err(5, "ERROR: cat exec failed");
        }
    } else {
	close(a[1]);
    } 

    while (wait(NULL) > 0);

    if (dup2(a[0], 0) == -1) {
       err(6, "ERROR: dup sort failed");
    }
    if (execlp("sort", "sort", (char *)NULL) == -1) {
       err(7, "ERROR: sort exec failed");
    }

    return 0;
}
