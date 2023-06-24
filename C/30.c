#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>

int main(void) {
    int fd_scl = open("input.scl", O_RDONLY);
    if (fd_scl == -1) {
        err(1, "ERROR: open input.scl failed");
    }

    int fd_sdl = open("input.sdl", O_RDONLY);
    if (fd_sdl == -1) {
        err(2, "ERROR: open input.sdl failed");
    }

    int fd_output = open("output.sdl", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_output == -1) {
        err(3, "ERROR: open output.sdl failed");
    }

    uint8_t byte;
    uint16_t position = 0;
    while (read(fd_scl, &byte, sizeof(uint8_t)) > 0) {
	for (int i = 0; i < 8; i++) {
	    if ((byte >> i) & 1) {
                uint16_t element;
                if (lseek(fd_sdl, position * sizeof(uint16_t), SEEK_SET) == -1) {
                    err(4, "ERROR: lseek input.sdl failed");
                }
                if (read(fd_sdl, &element, sizeof(uint16_t)) == -1) {
                    err(5, "ERROR: read from input.sdl failed");
                }
                if (write(fd_output, &element, sizeof(uint16_t)) == -1) {
                    err(6, "ERROR: write to output.sdl failed");
                }
	    }
        }
        position++;
    }

    close(fd_scl);
    close(fd_sdl);
    close(fd_output);

    return 0;
}
