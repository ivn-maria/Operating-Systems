#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <stdint.h>

struct header {
    uint32_t magic;
    uint8_t header_version;
    uint8_t data_version;
    uint16_t count;
    uint32_t _;
    uint32_t __;
};

struct data_first {
    uint16_t offset;
    uint8_t original_byte;
    uint8_t new_byte;
};

struct data_second {
    uint32_t offset;
    uint16_t original_word;
    uint16_t new_word;
};

int main(int argc, char** argv) {
    if (argc != 4) {
        errx(1, "ERROR: invalid argumetns count");
    }

    int fd1, fd2, fd3;
    if ((fd1 = open(argv[1], O_RDONLY)) == -1 || (fd2 = open(argv[1], O_RDONLY)) == -1 ||
    	(fd3 = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR)) == -1) {
    	    err(2, "ERROR: open files failed");
    }

    struct header h;
    if (read(fd1, &h, sizeof(struct header)) == -1) {
        err(3, "ERROR: read from patch file failed");
    }
    if (h.magic != 0xEFBEADDE || h.header_version != 0x01) {
        errx(4, "ERROR: invalid patch file content");
    }

    int bytes_read;
    if (h.data_version == 0x00) {
        uint8_t original_byte;
        while ((bytes_read = read(fd2, &original_byte, sizeof(original_byte))) > 0) {
    	    if (write(fd3, &original_byte, sizeof(original_byte)) == -1) {
        	err(5, "ERROR: write to f2 file failed");
    	    }
        }

        if (bytes_read == -1) {
            err(6, "ERROR: read from f1 file failed");
        }

        struct data_first data;
        while ((bytes_read = read(fd1, &data, sizeof(data))) > 0) {
    	    if (lseek(fd2, data.offset, SEEK_SET) == -1) {
        	err(7, "ERROR: lseek f1 file failed");
    	    }

            if (read(fd2, &original_byte, sizeof(original_byte)) == -1) {
                err(8, "ERROR: read from f1 file failed");
            }
			
	    if (original_byte != data.original_byte) {
    		err(9, "ERROR: patch file and f1 file content mismatch");
	    }

    	    if (lseek(fd3, data.offset, SEEK_SET) == -1) {
        	err(10, "ERROR: lseek f2 file failed");
    	    }

    	    if (write(fd3, &data.new_byte, sizeof(data.new_byte)) == -1) {
        	err(5, "ERROR: write to f2 file failed");
    	    }
        }

    	if (bytes_read == -1) {
    	    err(11, "ERROR: read from f1 file failed");
	}
    } else if (h.data_version == 0x01) {
        uint16_t original_word;
        while ((bytes_read = read(fd2, &original_word, sizeof(original_word))) > 0) {
    	    if (write(fd3, &original_word, sizeof(original_word)) == -1) {
        	err(5, "ERROR: write to f2 file failed");
    	    }
        }

    	if (bytes_read == -1) {
            err(6, "ERROR: read from f1 file failed");
        }

        struct data_second data;
        while ((bytes_read = read(fd1, &data, sizeof(data))) > 0) {
    	    if (lseek(fd2, data.offset, SEEK_SET) == -1) {
        	err(7, "ERROR: lseek f1 file failed");
    	    }

            if (read(fd2, &original_word, sizeof(original_word)) == -1) {
                err(8, "ERROR: read from f1 file failed");
            }
			
	    if (original_word != data.original_word) {
    		err(9, "ERROR: patch file and f1 file content mismatch");
	    }

    	    if (lseek(fd3, data.offset, SEEK_SET) == -1) {
        	err(10, "ERROR: lseek f2 file failed");
    	    }

    	    if (write(fd3, &data.new_word, sizeof(data.new_word)) == -1) {
        	err(5, "ERROR: write to f2 file failed");
    	    }
        }

    	if (bytes_read == -1) {
    	    err(11, "ERROR: read from f1 file failed");
	} 
    } else {
    	errx(4, "ERROR: invalid patch file content");
    }

    close(fd1);
    close(fd2);
    close(fd3);

    return 0;
}
