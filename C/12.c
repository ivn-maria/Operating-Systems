#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <err.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        errx(1, "ERROR: invalid arguments count");
    }

    int successful = 0;
    int error = 0;

    for (int i = 1; i < argc; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            err(2, "ERROR: cannot fork");
        }

        int status;
        if (pid > 0) {
            wait(&status);
            if (WIFEXITED(status)) {
                if (WEXITSTATUS(status) == 0) {
                    successful++;
                } else {
                    error++;
                }   
            }   
        } else {
            execlp(argv[i], argv[i], NULL);
	}   
    }   
    
    dprintf(1, "Successful commands: %d\n", successful);
    dprintf(1, "Error commands: %d\n", error);
    
    return 0;
}   
