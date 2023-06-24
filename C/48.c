#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

const char L[3][5] = { "tic ", "tac ", "toe\n" };

int main(int argc, char** argv) {
    if (argc != 3) {
	errx(1, "ERROR: invalid arguments count");
    }

    int NC = atoi(argv[1]);
    int WC = atoi(argv[2]);

    if (NC < 1 || NC > 7 || WC < 1 || WC > 35) {
        errx(2, "ERROR: invalid arguments");
    }

    for (int i = 0; i < WC; i++) {
    	const char* word = L[i % 3];
	if (write(0, word, strlen(word)) == -1) {
            errx(3, "ERROR: write to stdout failed");
    	}
        i++;

        for (int j = 0; j < NC && i < WC; j++) {
            const pid_t pid = fork();
            if (pid == -1) {
                err(4, "ERROR: fork failed");
            }

            if (pid == 0) {
            	word = L[i % 3];
               	if (write(0, word, strlen(word)) == -1) {
        	    errx(3, "ERROR: write to stdout failed");
    		} 
            }

            if (wait(NULL) == -1) {
                err(5, "ERROR: wait failed");
            }

            i++;
        }
    }

    return 0;
}
