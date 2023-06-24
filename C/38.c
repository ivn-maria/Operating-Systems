#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <sys/wait.h>

int main(void) {
    const char *cmd = "Enter command: ";
    if (write(1, cmd, strlen(cmd)) == -1) {
        err(1, "ERROR: write to stdout failed");
    }

    int bytes_read;
    char buff[4096];
    while ((bytes_read = read(1, buff, sizeof(buff))) > 0) {
	buff[bytes_read - 1] = '\0';

        if (strcmp(buff, "exit") == 0) {
            return 0;
        }

        pid_t pid = fork();
        if (pid == -1) {
            err(2, "ERROR: fork failed");
        }

        if (pid == 0) {
            char path[4096];
            strcpy(path, "/bin/");
            strcat(path, buff);
            if (execl(path, buff, (char*)NULL) == -1) {
                err(3, "ERROR: %s exec failed", buff);
            }
        }

        int status;
        if (wait(&status) == -1) {
            err(4, "ERROR: wait failed");
	}

        if (WIFEXITED(status)) {
 	    if (write(1, cmd, strlen(cmd)) == -1) {
        	err(1, "ERROR: write to stdout failed");
      	    }
      	}
    }

    if (bytes_read == -1) {
        err(5, "ERROR: read from stdin failed");
    }

    return 0;
}
