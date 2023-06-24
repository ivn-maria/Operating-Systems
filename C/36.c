#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define uint128_t __uint128_t

struct header {
    uint64_t magic;
    uint32_t cfsb;
    uint32_t cfsu;
    uint32_t ofsb;
    uint32_t ofsu;
    uint32_t _;
    uint32_t cksum;
    uint128_t section_key;
    uint32_t sections[4];
};

struct section {
    int64_t offset;
    uint64_t length;
    uint128_t data_key;
};

void decrypt_section(struct section*, uint128_t);
void decrypt_unit(uint128_t*, uint128_t);

void decrypt_section(struct section* s, uint128_t key) {
    uint128_t* first = (uint128_t*) s;
    uint128_t* second = first + 1;

    *first = *first ^ key;
    *second = *second ^ key;
}

void decrypt_unit(uint128_t* unit, uint128_t key) {
    *unit = *unit ^ key;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        errx(1, "ERROR: invalid arguments count");
    }

    int fd_src, fd_dest;
    if ((fd_src = open(argv[1], O_RDONLY)) == -1) {
        err(2, "ERROR: open input file failed");
    }
    if ((fd_dest = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666))== -1) {
        err(3, "ERROR: open output file failed");
    }

    struct header h;
    if (read(fd_src, &h, sizeof(struct header)) == -1) {
	err(4, "ERROR: read from input file failed");
    } 

    struct stat st;
    if (fstat(fd_src, &st) == -2) {
        err(5, "ERROR: stat input file failed");
    }
    if (st.st_size % sizeof(uint64_t) != 0 || st.st_size != h.cfsb) {
        errx(6, "ERROR: inavlid input file size");
    }

    char tmp_name[256] = "tmp_";
    strcat(tmp_name, argv[2]);
    int fd_tmp = open(tmp_name, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd_tmp < 0) {
        err(7, "ERROR: open %s file failed", tmp_name);
    }

    for (int i = 0; i < 4; i++) {
    	if (lseek(fd_src, h.sections[i] * 16, SEEK_SET) == -1) {
            err(8, "ERROR: lseek input file failed");
    	}

        struct section s;
    	if (read(fd_src, &s, sizeof(struct section)) == -1) {
    	    err(4, "ERROR: read from input file failed");
	}
        
        decrypt_section(&s, h.section_key);

        if (h.sections[i] == 0) {
            continue;
        }

   	if (lseek(fd_src, (h.sections[i] + s.offset) * 16, SEEK_SET) == -1) {
            err(8, "ERROR: lseek input file failed");
    	}

        for (uint64_t j = 0; j < s.length; ++j) {
            uint128_t unit;

    	    if (read(fd_src, &unit, sizeof(uint128_t)) == -1) {
    		err(4, "ERROR: read from input file failed");
	    }

            decrypt_unit(&unit, s.data_key);

    	    if (write(fd_tmp, &unit, sizeof(uint128_t)) == -1){
		err(9, "ERROR: write to %s file failed", tmp_name);
	    }
        }
    }

    if (lseek(fd_tmp, 0, SEEK_SET) < 0) {
        err(10, "ERROR: lseek %s file failed", tmp_name);
    }

    uint32_t checksum = 0;

    int bytes_read1;
    uint32_t current;
    while ((bytes_read1 = read(fd_tmp, &current, sizeof(current))) > 0) {
        checksum = checksum ^ current;
    }

    if (bytes_read1 == -1) {
    	err(11, "ERROR: read from %s file failed", tmp_name);
    }

    if (checksum != h.cksum) {
        errx(12, "ERROR: invalid checksum");
    }

    if (lseek(fd_tmp, 0, SEEK_SET) < 0) {
        err(10, "ERROR: lseek %s file failed", tmp_name);
    }

    char c;
    int bytes_read2;
    for (uint32_t i = 0; (bytes_read2 = read(fd_tmp, &c, sizeof(c))) > 0 && i < h.ofsb; ++i) {
    	if (write(fd_dest, &c, sizeof(c)) == -1) {
    	    err(13, "ERROR: write to output file failed");
	}
    }

    if (bytes_read2 == -1) {
    	err(11, "ERROR: read from %s file failed", tmp_name);
    }
    
    if (unlink(tmp_name) == -1) {
        err(14, "ERROR: unlik %s file failed", tmp_name);
    }

    close(fd_src);
    close(fd_dest);
    close(fd_tmp);

    return 0;
}
