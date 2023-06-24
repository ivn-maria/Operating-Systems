#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char** argv) {
    if (argc != 4) {
        errx(1, "ERROR: invalid arguments count");
    }

    for (int i = 1; i < argc; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            err(2, "ERROR: cannot fork");
        }

        int status;
        if (pid > 0) {
            wait(&status);
            if (WIFEXITED(status)) {
                dprintf(1, "PID: %d\n", pid);
                dprintf(1, "EXIT STATUS: %d\n", WEXITSTATUS(status));
            }
        } else {
            execlp(argv[i], argv[i], (char*)NULL);
        }
    }
    
    return 0;
}
