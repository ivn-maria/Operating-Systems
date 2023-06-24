#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char** argv) {
    if (argc != 3 && argc != 5) {
        errx(1, "ERROR: invalid arguments count");
    }

    char delimeter = '\0';
    char option;
    if (strcmp(argv[1], "-c") == 0) {
	if (argc != 3) {
	    errx(1, "ERROR: invalid arguments count");
	}

   	if (strlen(argv[2]) != 1 && strlen(argv[2]) != 3) {
            errx(2, "ERROR: invalid arguments");
        }

    	int start = atoi(&argv[2][0]);
	int end;
	if (strlen(argv[2]) == 3) end = atoi(&argv[2][2]);
    	if (start == 0 || (strlen(argv[2]) == 3 && (start > (end)))) {
            errx(2, "ERROR: invalid arguments");
    	}

        option = 'c';
    } else if (strcmp(argv[1], "-d") == 0) {
	if (argc != 5) {
	    errx(1, "ERROR: invalid arguments count");
	}

        delimeter = argv[2][0];
        
 	if (strcmp(argv[3], "-f") != 0) {
            errx(2, "ERROR: invalid arguments");
        }
        
  	if (strlen(argv[4]) != 1 && strlen(argv[4]) != 3) {
            errx(2, "ERROR: invalid arguments");
        }
        
        int start = atoi(&argv[4][0]);
        int end;
        if (strlen(argv[4]) == 3) end = atoi(&argv[4][2]);
        if (start == 0 || (strlen(argv[4]) == 3 && (start > (end)))) {
            errx(2, "ERROR: invalid arguments");
        }

        option = 'd';
    } else {
        errx(2, "ERROR: invalid arguments");
    }

    char buff[4096];
    if (read(0, buff, sizeof(buff)) == -1) {
	err(3, "ERROR: cannot read from stdin");
    }

    if (option == 'c') {
    	int str_len = strlen(buff) - 1;
    	int option_len = strlen(argv[2]);

    	if (option_len == 1) {
      	    int index = atoi(&argv[2][0]);
	    index -= 1;
            if (index < str_len) {
    		if (write(1, &buff[index], sizeof(buff[index])) == -1) {
        	    err(4, "ERROR: cannot write to stdin");
    		}
            }
    	} else if (option_len == 3) {
            int start = atoi(&argv[2][0]);
	    start -= 1;
 	    int end = atoi(&argv[2][2]); 
	    end -= 1;
            if (start < str_len && start <= end) {
            	if (end >= str_len) {
                    end = str_len - 1;
            	}
            	for (int i = start; i <= end; i ++) {
    		    if (write(1, &buff[i], sizeof(buff[i])) == -1) {
        		err(4, "ERROR: cannot write to stdin");
    		    }
            	}
            }
    	} else {
            errx(2, "ERROR: invalid option");
    	}

	char c = '\n';
    	if (write(1, &c, sizeof(c)) == 0) {
            err(4, "ERROR: cannot write to stdin");
    	}
    } else if (option == 'd') {
    	int str_len = strlen(buff) - 1;
    	int index = 0;
    	int option_len = strlen(argv[4]);

   	if (option_len == 1) {
            int pos = atoi(&argv[4][0]);
            while (index < str_len) {
            	if (buff[index] == delimeter) {
                    pos--;
                    if (pos == 1) {
                    	index++;
                    	while (index < str_len && buff[index] != delimeter) {
   			    if (write(1, &buff[index], sizeof(buff[index])) == -1) {
        			err(4, "ERROR: cannot write to stdin");
    			    }
                       	    index++;
                    	}
                    }
            	}
            	index++;
            }
    	} else if (option_len == 3) {
            int pos = atoi(&argv[4][0]);
	    int end = atoi(&argv[4][2]);
            while (index < str_len) {
            	if (buff[index] == delimeter) {
                    pos--;
                    if (pos == 1) {
                    	index++;
                    	while (index < str_len && pos < end) {
                            if (buff[index] == delimeter) {
                            	pos++;
                            	if (pos == end) {
                                    break;
                            	}
                            }
                            if (write(1, &buff[index], sizeof(buff[index])) == -1) {
                                err(3, "ERROR: cannot write to stdin");
                            }
                            index++;
                    	}
                    }	
            	}
            	index++;
            }
        } else {
            errx(2, "ERROR: invalid option");
    	}

	char c = '\n';
    	if (write(1, &c, sizeof(c)) == 0) {
            err(4, "ERROR: cannot write to stdin");
    	}
    }

    return 0;
}
