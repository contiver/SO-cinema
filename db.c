#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
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
    strncpy(m1.name, "Los aristogatos", MAX_LENGTH);
    strncpy(m1.time, "Monday 12:30", MAX_LENGTH);

    m1.th.number = 1;
    m1.th.size = STD_SEAT_QTY;
    for(i = 0; i < STD_SEAT_QTY; i++){
        strncpy(m1.th.seats[i], "\0", MAX_LENGTH);
    }

    sprintf(movieName, "movie_%d", m1.id);

    FILE *file = fopen(movieName, "wb");
    if ( file == NULL ){
        printf("error while creating database\n");
        exit(1);
    }

    if( fwrite(&m1, sizeof(Movie), 1, file) != 1 ){
        printf("error while writing structure\n");
        exit(1);
    }
    fclose(file);
}
