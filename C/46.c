#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <pwd.h>

int main(int argc, char** argv) {
    if (argc < 2) {
    	errx(1, "ERROR: invalid arguments count");
    }

    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    struct tm* time_info = localtime(&current_time.tv_sec);
    char formatted_time[27];
    strftime(formatted_time, sizeof(formatted_time), "%Y-%m-%d %H:%M:%S", time_info);
    int microseconds = current_time.tv_usec;

    uid_t uid = getuid();
    struct passwd* pw = getpwuid(uid);
    char* username = pw->pw_name;

    int fd;
    if ((fd = open("foo.log", O_WRONLY | O_CREAT | O_APPEND, 0644)) == -1) {
        err(2, "ERROR: open foo.log failed");
    }

    dprintf(fd, "%s.%06d %s ", formatted_time, microseconds, username);
    for (int i = 1; i < argc; i++) {
        dprintf(fd, "%s ", argv[i]);
    }
    dprintf(fd, "\n"); 

    int a[2];
    if (pipe(a) == -1) {
    	err(3, "ERROR: first pipe failed");
    }

    pid_t pid = fork();
    if (pid == -1) {
    	err(4, "ERROR: first fork failed");
    }

    if (pid == 0) {
        close(a[1]);
        if (dup2(a[0], 0) == -1) {
            err(5, "ERROR: first dup failed");
        }
        if (execlp("passwd", "passwd", "-l", username, (const char*)NULL) == -1) {
            err(6, "ERROR: exec passwd failed");
        }
    } else {
        close(a[1]);
    } 

    int b[2];
    if (pipe(b) == -1) {
        err(7, "ERROR: second pipe failed");
    }

    pid = fork();
    if (pid == -1) {
        err(8, "ERROR: second fork failed");
    }

    if (pid == 0) {
        close(b[1]);
        if (dup2(b[0], 0) == -1) {
            err(9, "ERROR: second dup failed");
        }
        if (execlp("pkill", "pkill", "-u", username, (const char*)NULL) == -1) {
            err(10, "ERROR: exec pkill failed");
        }
    } else {
        close(b[1]);
    }    	

    return 0;
}

/*
За да се настрои програмата, така че студентите да я използват вместо оригиналното sudo, могат да се изпълнят следните стъпки:
1. Програмата се компилира, за да се създаде изпълним файл.
2. Променят се разрешенията на изпълнимия файл, така че да бъде достъпен за студентите (chmod +x fake_sudo).
3. Замества се оригиналното sudo, като се постави изпълнимия файл в директорията, която е включена в системния път за търсене на изпълними файлове (sudo cp fake_sudo /usr/local/bin/).
След извършването на тези стъпки студентите ще използват програмата fake_sudo вместо оригиналното sudo.
*/
