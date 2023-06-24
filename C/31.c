#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <err.h>

int main(int argc, char** argv) {
    if (argc != 3) {
        errx(1, "ERROR: invalid arguments count");
    }

    int fd_input = open(argv[1], O_RDONLY);
    if (fd_input == -1) {
        err(2, "ERROR: open input file failed");
    }

    int fd_output = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_output == -1) {
        err(3, "ERROR: open output file failed");
    }

    struct stat st;
    if (fstat(fd_input, &st) == -1) {
        err(4, "ERROR: stat input failed failed");
    }
    
    int num_elements = st.st_size / sizeof(uint16_t);
    if (num_elements > 524288) {
        errx(5, "ERROR: input file contains too much numbers");
    }
    
    dprintf(fd_output, "#ifndef OUTPUT_H\n");
    dprintf(fd_output, "#define OUTPUT_H\n\n");
    dprintf(fd_output, "#include <stdint.h>\n\n");
    dprintf(fd_output, "const uint16_t arr[] = {\n");

    uint16_t element;
    int bytes_read;
    while ((bytes_read = read(fd_input, &element, sizeof(uint16_t))) > 0) {
        if (bytes_read > 1) {
            dprintf(fd_output, ", ");
        }
        dprintf(fd_output, "%u", element);
    }

    if (bytes_read == -1) {
    	err(6, "ERROR: read from input file failed");
	}

    dprintf(fd_output, "\n};\n\n");
    dprintf(fd_output, "const uint32_t arrN = %d;\n\n", bytes_read);
    dprintf(fd_output, "#endif\n");

    close(fd_input);
    close(fd_output);

    return 0;
}
