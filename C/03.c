#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        errx(1, "ERROR: invalid arguments count");
    }

    int dest_fd;
    if ((dest_fd = open(argv[argc-1], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
        err(2, "ERROR: cannot open destination file");
    }

    uint8_t buffer;
    int bytes_read, bytes_written;
    for (int i = 1; i < argc - 1; i++) {
        int src_fd;
        if ((src_fd = open(argv[i], O_RDONLY)) == -1) {
            warn("ERROR: cannot open source file %s", argv[i]);
            continue;
        }

        while ((bytes_read = read(src_fd, &buffer, sizeof(buffer))) > 0) {
            if ((bytes_written = write(dest_fd, &buffer, sizeof(buffer))) == -1) { 
		err(3, "ERROR: cannot write to destination file");
            }
        }

        if (bytes_read == -1) {
            err(4, "ERROR: cannot read from source file %s", argv[i]);
        }

        close(src_fd);
    }

    close(dest_fd);

    return 0;
}
