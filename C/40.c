#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char** argv) {
    if (argc > 2) {
	errx(1, "ERROR: invalid arguments count");
    }

    if (argc == 2) {
    	if (strlen(argv[1]) > 4) {
            errx(2, "ERROR: invalid argument length");
    	}

    	char buff[4096];
    	int bytes_read;
    	char c;
    	int index = 0;
   	while ((bytes_read = read(0, &c, sizeof(c))) == sizeof(c)) {
            if (c == ' ' || c == '\n' || c == '\t') {
            	buff[index] = '\0';
            	index = 0;

            	if (strlen(buff) > 0) {
		    if (strlen(buff) > 4) {
			errx(3, "ERROR: invalid string length");
		    }

                    const pid_t pid = fork();
                    if (pid == -1) {
                    	err(4, "ERROR: fork failed");
                    }

                    if (pid == 0) {
                    	if (execlp(argv[1], buff, buff, (char *)NULL) == -1) {
                            err(5, "ERROR: exec failed");
                    	}
                    }

                    if (wait(NULL) == -1) {
                    	err(6, "ERROR: wait failed");
                    }
            	}
            } else {
            	buff[index] = c;
            	index++;
            }
    	}

    	if (bytes_read == -1) {
            err(7, "ERROR: read from stdin failed");
    	}
    } else {
    	char c;
    	int bytes_read;
    	while ((bytes_read = read(0, &c, sizeof(c))) == sizeof(c)) {
            if (write(1, &c, sizeof(c)) == -1) {
            	err(8, "ERROR: write to stdout failed");
            }
    	}

    	if (bytes_read == -1) {
            err(7, "ERROR: read from stdin failed");
    	}
    }
	
    return 0;
}
