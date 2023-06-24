#include <fcntl.h> 
#include <err.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

struct triple {
    char name[8];
    uint32_t offset;
    uint32_t length;
};

int main(int argc, char** argv) {
    if (argc != 2) {
	errx(1, "ERROR: inavlid arguments count");
    }

    int fd;
    if ((fd = open(argv[1], O_RDONLY)) == -1) {
	err(2, "ERROR: open file failed");
    }

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
	err(3, "ERROR: pipe failed");
    }

    struct triple t;
    int bytes_read;
    while ((bytes_read = read(fd, &t, sizeof(t))) == -1) {
	pid_t pid = fork();
	if (pid == -1) {
	    err(4, "ERROR: fork failed");
	}

	if (pid == 0) {
	    int fd_file;
	    if ((fd_file = open(t.name, O_RDONLY)) == -1) {
		err(5, "ERROR: open file from triple failed");
	    }

	    if (lseek(fd_file, t.offset * sizeof(uint16_t), SEEK_SET) == -1) {
		err(6, "ERROR: lseek failed");
	    }
			
	    uint16_t a = 0x0000;
	    uint16_t buff;
	    int bytes_read1;
	    for (uint32_t j = 0; j < t.length; j++) {
		if ((bytes_read1 = read(fd_file, &buff, sizeof(buff))) > 0) {
		    a = a ^ buff;
		}
	    }

	    if (bytes_read1 == -1) {
		err(7, "ERROR: read file from triple failed");
	    }	

	    close(pipe_fd[0]);
	    if (write(pipe_fd[1], &a, sizeof(a)) == -1) {
		err(8, "ERROR: write to pipe failed");
	    }
	} else {
	    close(pipe_fd[1]);
	}
    }

    if (bytes_read == -1) {
	err(9, "ERROR: read from file failed");
    }

    while (wait(NULL) > 0);

    uint16_t a = 0x0000;
    uint16_t buff;
    int bytes_read2;
    while ((bytes_read2 = read(pipe_fd[0], &buff, sizeof(buff))) > 0) {
	a = a ^ buff;
    }

    if (bytes_read2 == -1) {
	err(10, "ERROR: read from pipe failed");
    }

    dprintf(1, "%dB\n", a);

    return 0;
}
