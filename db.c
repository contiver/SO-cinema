#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "client.h"

#define STD_SEAT_QTY 60

void initializeMovies(void);

int main(void){
    initializeMovies();
    return 0;
}

void initializeMovies(void){
    int i, fd;
    char movieName[40];

    Movie m1;
    m1.id = 1;
    m1.name = "Los aristogatos";
    m1.time = "Monday 12:30";

    m1.th.number = 1;
    m1.th.size = STD_SEAT_QTY;
    
    for(i = 0; i < STD_SEAT_QTY; i++){
        m1.th.seats[i] = NULL;
    }

    sprintf(movieName, "movie_%d", m1.id);

    if ( (fd = open(movieName, O_RDWR | O_CREAT | O_EXCL, 0644)) == -1){
        printf("error while creating database\n");
       // errExit("open");
       exit(1);
    }

    if( write(fd, &m1, sizeof(Movie)) == -1){
        printf("error while writing structure");
        exit(1);
    }

    if(close(fd) == -1) exit(1);//errExit("close");
    return;
}
