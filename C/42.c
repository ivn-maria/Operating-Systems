#include <err.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <time.h>

int time_cond(time_t, time_t, int);

int time_cond(time_t t_start, time_t t_end, int seconds) {
   return seconds > (t_end - t_start);
}

int main(int argc, char** argv) {
    if (argc < 3) {
	errx(1, "ERROR: invalid arguments count");
    }

    int fd = open("run.log", O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
    if (fd == -1) {
	err(2, "ERROR: open run.log failed");
    }

    int seconds = atoi(argv[1]);
    char* Q = argv[2];
    char** args = argv + 2;
    int condition = 0;	
    while (1) {
        time_t t_start = time(NULL);

	pid_t pid = fork();
	if (pid == -1) {
	    err(3, "ERROR: fork failed");
	}

	if (pid == 0) {
	    if (execvp(Q, args) == -1) {
		err(4, "ERROR: exec failed");	
 	    }
	} else {
	    int status;	
	    if (wait(&status) == -1) {
		err(5, "ERROR: wait failed");
	    }

	    time_t t_end = time(NULL);
	    int exit_code;
	    if (WIFEXITED(status)) {
		exit_code = WEXITSTATUS(status);
	    } else {
		exit_code = 129;		
	    }

	    dprintf(fd, "%ld %ld %d\n", t_start, t_end, exit_code);
			
	    if (exit_code != 0 && time_cond(t_start, t_end, seconds)) {
		if (condition == 1) {
		    break;
		} else {
		    condition = 1;
		}
	    } else {
		condition = 0;
	    }
	}
    }
	
    close(fd);
	
    return 0;
}
