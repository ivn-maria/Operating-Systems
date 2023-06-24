#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdint.h>

struct header {
    uint16_t magic;
    uint16_t filetype;
    uint32_t count;
};

int main(int argc,char* argv[]) {
    if (argc != 4) {
      	errx(1, "ERROR: invalid arguments count");
    }

    int fd_list, fd_data, fd_out;
    if ((fd_list = open(argv[1], O_RDONLY)) == -1) {
      	err(2, "ERROR: open list file failed");
    }
    if ((fd_data = open(argv[2], O_RDONLY)) < 0) {
       	err(3, "ERROR: open data file failed");
    }
    if ((fd_out = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) {
       	err(4, "ERROR: open out file failed");
    }

    struct header list_hd;
    if (read(fd_list, &list_hd, sizeof(list_hd)) == -1) {
       	err(5, "ERROR: read from list file failed");
    }
    if (list_hd.magic != 0x5A4D || list_hd.filetype != 1) {
    	errx(6, "ERROR: invalid list file content");
    }
	
    struct header data_hd;
    if (read(fd_data, &data_hd, sizeof(data_hd)) == -1) {
       	err(7, "ERROR: read from data file failed");
    }
    if (data_hd.magic != 0x5A4D || data_hd.filetype != 2) {
      	errx(8, "ERROR: invalid data file content");
    }

    struct header out_hd;
    out_hd.magic = 0x5A4D;
    out_hd.filetype = 3;
    out_hd.count = list_hd.count;
    if (write(fd_out, &out_hd, sizeof(out_hd)) == -1) {
        err(9, "ERROR: write to out file failed");
    }
    
    uint16_t list_buf;
    uint32_t data_buf;
    for (uint32_t i = 0; i < list_hd.count; i++) {
    	if (read(fd_list, &list_buf, sizeof(list_buf)) == -1) {
            err(5, "ERROR: read from list file failed");
       	}

       	int position;
       	if ((position = lseek(fd_list, 0 ,SEEK_CUR)) == -1) {
            err(10, "ERROR: lseek list file failed");
       	}
	position -= sizeof(struct header);
	position /= sizeof(uint32_t);
       	if (sizeof(data_hd) + position * sizeof(uint32_t) > sizeof(list_hd) + data_hd.count * sizeof(uint16_t)) {
	    errx(11, "ERROR: list and data file mismatch");
       	}

       	if (lseek(fd_data, sizeof(data_hd) + position * sizeof(uint32_t), SEEK_SET) < 0) {
            err(12, "ERROR: lseek data file failed");
       	}
       	if (read(fd_data, &data_buf, sizeof(data_buf)) == -1) {
            err(7, "ERROR: read from data file failed");
       	}

       	if (lseek(fd_out, sizeof(out_hd) + list_buf * sizeof(uint64_t), SEEK_SET) < 0) {
            err(13, "ERROR: lseek out file failed");
       	}
    	uint64_t out_data = (uint64_t)data_buf;
	if (write(fd_out, &out_data, sizeof(out_data)) == -1) {
    	    err(9, "ERROR: write to out file failed");
	}
    }

    close(fd_list);
    close(fd_data);
    close(fd_out);

    return 0;
}
