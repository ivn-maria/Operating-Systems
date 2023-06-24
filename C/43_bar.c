#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    if(argc != 2) {
	errx(1, "ERROR: invalid arguments count");
    }

    int fd = open("temp", O_RDONLY);
    if (fd == -1) {
	err(2, "ERROR: open fifo failed");	
    }

    if (dup2(fd, 0) == -1) {
	err(3, "ERROR: dup failed");
    }
    if (execl(argv[1], argv[1], (char*) NULL) == -1) {
	err(4, "ERROR: exec failed");
    }
	
    return 0;
}
