#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdio.h>

struct user_data {
    uint32_t uid;
    uint16_t _;
    uint16_t __;
    uint32_t start_session;
    uint32_t end_session;
};

struct user_session {
    uint32_t uid;
    uint32_t session_duration;
};

int main(int argc, char** argv) {
    if (argc != 2) {
        errx(1, "ERROR: invalid arguments count");
    }

    int fd;
    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        err(2, "ERRoR: open file failed");
    }

    int bytes_count;
    struct user_data ud;
    struct user_session sessions[16384];
    int counter = 0;
    uint32_t sum1 = 0;
    while ((bytes_count = read(fd, &ud, sizeof(struct user_data))) > 0) {
        if (ud.end_session < ud.start_session) {
            errx(3, "ERROR: invalid file content");
        }

        uint32_t duration = ud.end_session - ud.start_session;
        sessions[counter].uid = ud.uid;
        sessions[counter].session_duration = duration;
        sum1 += duration;
        counter++;
    }

    if (bytes_count == -1) {
        err(4, "ERROR: read from file failed");
    }

    double average = sum1 / counter;
    double sum2 = 0;
    for (int i = 0; i < counter; i++) {
        double diff = sessions[i].session_duration - average;
        sum2 += (diff * diff);
    }
    double dispersion = sum2 / counter;

    uint32_t user_longest_session[2048];
    for (int i = 0; i < 2048; i++) {
        user_longest_session[i] = 0;
    }

    for (int i = 0; i < counter; i++) {
        uint32_t user_duration_squared = (sessions[i].session_duration * sessions[i].session_duration);
        if (user_duration_squared > dispersion) {
            if (user_longest_session[sessions[i].uid] < user_duration_squared) {
                user_longest_session[sessions[i].uid] = user_duration_squared;
            }
        }
    }

    for (uint32_t i = 0; i < 2048; i++) {
        if (user_longest_session[i] > 0) {
            dprintf(1, "%d %d\n", i, user_longest_session[i]);
        }
    }

    close(fd);

    return 0;
}
