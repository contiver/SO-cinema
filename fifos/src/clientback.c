#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../../include/clientback.h"

#define FIFO_PATH "./fifo";

void
fatal(char *s){
    perror(s);
    exit(1);
}

void get_movie(Movie *m, int movieID){
    pid_t pid = getpid();
    int fd;

    if( mkfifo(FIFO_PATH, S_IRUSR | S_IWUSR | S_IWGRP) == -1 
            && errno != EEXIST){
        exit(1);
    }

    while(true){


}

int get_movies_list(char *movies[10][60]){
	return 0; // IMPLEMENTAR !!
}

int cancel_seat(Client c, int movieID, int seat){
	return 0; // IMPLEMENTAR !!
}

int reserve_seat(Client c, Movie m, int seat){
	return 0; // IMPLEMENTAR !!
}
