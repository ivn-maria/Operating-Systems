#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdint.h>
#include <sys/stat.h>

struct header_data {
    uint32_t magic;
    uint32_t count;
};

struct header_comp {
    uint32_t magic1;
    uint16_t magic2;
    uint16_t _;
    uint64_t count;
};

struct elements_comp {
    uint16_t type;
    uint16_t _;
    uint16_t __;
    uint16_t ___;
    uint32_t offset1;
    uint32_t offset2;
};

int main(int argc,char* argv[]) {
    if (argc != 3) {
       	errx(1, "ERROR: invalid arguments count");
    }
	
    int fd_data, fd_comp;
    if ((fd_data = open(argv[1], O_RDWR)) < 0) {
       	err(2, "ERROR: open data file failed");
    }
    if ((fd_comp = open(argv[2], O_RDWR)) < 0) {
       	err(3, "ERROR: open comprator file failed");
    }

    struct header_data data_hd;
    if (read(fd_data, &data_hd, sizeof(data_hd)) == -1) {
       	err(4, "ERROR: read from data file failed");
    }
    if (data_hd.magic != 0x21796F4A) { 
       	errx(5, "ERROR: invalid data file content");
    }

    struct header_comp comp_hd;
    if (read(fd_comp, &comp_hd, sizeof(comp_hd)) == -1) {
    	err(6, "ERROR: read from comprator file failed");
    }
    if (comp_hd.magic1 != 0xAFBC7A37 || comp_hd.magic2 != 0x1C27) {
       	errx(7, "ERROR: invalid comprator file content");
    }

    struct elements_comp comp_buf;
    uint64_t first_num, second_num;
    for (uint64_t i = 0; i < comp_hd.count; i++) {
	if (read(fd_comp, &comp_buf, sizeof(comp_buf)) == -1) {
            err(8, "ERROR: read from comprator file failed");
	}
    	
    	if ((comp_buf.type != 0 && comp_buf.type != 1) || comp_buf._ != 0 || comp_buf.__ != 0 || comp_buf.___ != 0) {
            errx(9, "ERROR: invalid comprator file content");
    	}
        if (sizeof(data_hd) + comp_buf.offset1 * sizeof(uint64_t) >= 
		sizeof(comp_hd) + comp_hd.count * sizeof(struct elements_comp) || 
            sizeof(data_hd) + comp_buf.offset2 * sizeof(uint64_t) >= 
		sizeof(comp_hd) + comp_hd.count * sizeof(struct elements_comp)) {
            errx(10, "ERROR: data and comprator file mismatch");
        }

    	if (lseek(fd_data, sizeof(data_hd) + comp_buf.offset1 * sizeof(uint64_t), SEEK_SET) < 0) {
            err(14, "ERROR: lseek data file failed");
        }
        if (read(fd_data, &first_num, sizeof(first_num)) == -1) {
            err(9, "ERROR: read from data file failed ");
        }
	if (lseek(fd_data, sizeof(data_hd) + comp_buf.offset2 * sizeof(uint64_t), SEEK_SET) < 0) {
            err(14, "ERROR: lseek data file failed");
     	}
        if (read(fd_data, &second_num, sizeof(second_num)) == -1) {
            err(9, "ERROR: read from data file failed");
        }
         
        if ((comp_buf.type == 0 && first_num > second_num) || (comp_buf.type == 1 && first_num < second_num)) { 
            if (lseek(fd_data, sizeof(data_hd) + comp_buf.offset1 * sizeof(uint64_t), SEEK_SET) < 0) {
            	err(14, "ERROR: lseek data file failed");
            }
            if (write(fd_data, &second_num, sizeof(second_num)) == -1) {
               	err(15, "ERROR: write to data file failed");
            }
	    if (lseek(fd_data, sizeof(data_hd) + comp_buf.offset2 * sizeof(uint64_t), SEEK_SET) < 0) {
               	err(14, "ERROR: lseek data file failed");
            }
            if (write(fd_data, &first_num, sizeof(first_num)) == -1) {
               	err(15, "ERROR: write to data file failed");
            }
        }
    }

    close(fd_data);
    close(fd_comp);

    return 0;
}
