#include <sys/wait.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    if (argc != 2) {
	errx(1, "ERROR: invalid arguments count");	
    }

    if (mkfifo("temp", S_IRUSR | S_IWUSR) == -1) {
	err(2, "ERROR: mkfifo failed");
    }
	
    int fd = open("temp", O_WRONLY);
    if (fd == -1) {
	err(3, "ERROR: open fifo failed");	
    }

    pid_t pid = fork();
    if (pid == -1) {
	err(4, "ERROR: fork failed");
    }

    if (pid == 0) {
	if (dup2(fd, 1) == -1) {
	    err(4, "ERROR: dup failed");
	}
	if (execlp("cat", "cat", argv[1], (char*) NULL) == -1) {
	    err(5, "ERROR: exec failed");
	}
    }

    wait(NULL);

    if (unlink("temp") == -1) {
	err(6, "ERROR: unlik failed");
    }

    close(fd);
	
    return 0;
}
