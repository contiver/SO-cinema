#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../include/client.h"

#define STD_SEAT_QTY 60

void initializeMovies(void);

int main(void){
    initializeMovies();
    return 0;
}

void initializeMovies(void){
    int i, j;
    char movieName[40];
    Movie movie[10];
    string titles[] = {"Los aristogatos", "n2", "n3", "n10",
                       "n4", "n5", "n6",
                       "n7", "n8", "n9"}; 
    string times[] = {"Monday 13:00", "Monday 15:00", "Monday 16:45", 
                      "Monday 19:00", "Monday 22:00", "Tuesday 0:30",
                      "Tuesday 12:00", "Tuesday 14:30","Tuesday 17:00",
                      "Tuesday 19:30"};

    for(i = 0; i < 10; i++){
        printf("%d\n", i);
        movie[i].id = i+1;
        strncpy(movie[i].name, titles[i], MAX_LENGTH); 
        strncpy(movie[i].time, times[i], MAX_LENGTH);
        movie[i].th.number = 1;
        movie[i].th.seats_left = STD_SEAT_QTY;
        for(j = 0; j < STD_SEAT_QTY; j++){
            strncpy((movie[i].th.seats)[j], "\0", MAX_LENGTH);
        }

        sprintf(movieName, "movie_%d", movie[i].id);
        FILE *file = fopen(movieName, "wb");
        if ( file == NULL ){
            printf("error while creating movie_%d file\n", i);
            exit(1);
        }
        if( fwrite(&movie[i], sizeof(Movie), 1, file) != 1 ){
            printf("error while writing movie_%d structure\n", i);
            exit(1);
        }
        fclose(file);
    }
}


