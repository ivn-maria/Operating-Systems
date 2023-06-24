#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    if (argc != 3) {
        errx(1, "ERROR: invalid arguments count");
    }

    int pd[2];
    if (pipe(pd) == -1) {
        err(2, "ERROR: pipe failed");
    }

    pid_t pid = fork();
    if (pid == -1) {
        err(3, "ERROR: fork failed");
    }

    if (pid == 0) {
        close(pd[0]);
        if (dup2(pd[1], 1) == -1) {
        	err(4, "ERROR: dup failed");
	}
        if (execlp("xxd", "xxd", argv[1], (char*)NULL) == -1) {
            err(5, "ERROR: exec failed");
        }
    } else {
   	 close(pd[1]);
    }

    int fd;
    int bytes_read;
    if ((fd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR)) == -1) {
        err(6, "ERROR: open %s failed", argv[2]);
    }

    unsigned char escape_char = 0x7D;
    unsigned char key = 0x20;
    unsigned char current_byte;	
    while ((bytes_read = read(pd[0], &current_byte, sizeof(current_byte))) > 0) {
        if (current_byte == escape_char) {
            if ((bytes_read = read(pd[0], &current_byte, sizeof(current_byte))) > 0) {
                unsigned char original_byte = current_byte ^ key;
                
		if (!(original_byte == 0x00 || original_byte == 0xFF || 
		    original_byte == 0x55 || original_byte == 0x7D)) {
                    errx(7, "ERROR: invalid file content");
                }

                if (write(fd, &original_byte, sizeof(original_byte)) == -1) {
                    err(8, "ERROR: write to %s failed", argv[2]);
                }
            }
        } else {
            if (write(fd, &current_byte, sizeof(current_byte)) == -1) {
             	err(8, "ERROR: write to %s failed", argv[2]);
            }
        }
    }

    if (bytes_read == -1) {
        err(9, "ERROR: read failed");
    }

    close(fd);
    
    return 0;
}
