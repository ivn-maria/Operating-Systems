#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <err.h>

/* cut -d: -f7 /etc/passwd | sort | uniq -c | sort -n */

int main(void) {		
    int a[2];
    if (pipe(a) == -1) {
	err(1, "ERROR: first pipe failed");
    }

    pid_t pid;
    if ((pid = fork()) == -1) {
	err(2, "ERROR: first fork failed");
    }

    if (pid == 0) {
	close(a[0]);	
	if (dup2(a[1], 1) == -1) {
	    err(3, "ERROR: dup cut failed");
	}
	if (execlp("cut", "cut", "-d:", "-f7", "/etc/passwd", (char *) NULL) == -1) {
	    err(4, "ERROR: cut exec failed");
	}
    } else {
	close(a[1]);
    }

    int b[2];
    if (pipe(b) == -1) {
	err(5, "ERROR: second pipe failed");
    }

    if ((pid = fork()) == -1) {
	err(6, "ERROR: sort fork failed");
    }

    if (pid == 0) {
	close(b[0]);
	if (dup2(a[0], 0) == -1) {
	    err(7, "ERROR: dup read end sort failed");
	}
	if (dup2(b[1], 1) == -1) {
	    err(8, "ERROR: dup write end sort failed");
	}
	if (execlp("sort", "sort", (char *) NULL) == -1) {
	    err(9, "ERROR: sort exec failed");
	}
    } else {
	close(b[1]);
    }

    close(a[0]);

    int c[2];
    if (pipe(c) == -1){
	err(10, "ERROR: third pipe failed");
    }

    if ((pid = fork()) == -1) {
	err(11, "ERROR: uniq fork failed");
    }
	
    if (pid == 0) {
	close(c[0]);
	if (dup2(b[0], 0) == -1) {
	    err(12, "ERROR: dup read end uniq failed");
	}
	if (dup2(c[1], 1) == -1) {
	    err(13, "ERROR: dup read end uniq failed");
	}
	if (execlp("uniq", "uniq", "-c", (char *)NULL) == -1) {
	    err(14, "EEROR: uniq exec failed");
	}
    } else {
	close(c[1]);
    }

    close(b[0]);

    while (wait(NULL) > 0);

    if (dup2(c[0], 0) == -1){
	err(15, "ERROR: dup read end sort failed");
    }
    if (execlp("sort", "sort", "-n", (char *)NULL) == -1){
	err(16, "ERROR: exec sort falied");
    }

    return 0;
}
