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
#define QTY_MOVIES   10
#define TITLE_LEN    30

void initializeMovies(char titles[][TITLE_LEN], char times[][TITLE_LEN]);
void initializeMovieList(char titles[][TITLE_LEN], char times[][TITLE_LEN]);

int main(void){
    char titles[QTY_MOVIES][TITLE_LEN] = {"The Aristocats           ", "Space Jam                ", "Shrek                    ", "Garfield                 ", "The Lion King            ", "Jurassic Park            ", "The Parent Trap          ",
                       "Matilda                  ", "Casper                   ", "Honey I shrunk the kids  "}; 
    char times[QTY_MOVIES][TITLE_LEN] = {"Monday 13:00", "Monday 15:00", "Monday 16:45", 
                      "Monday 19:00", "Monday 22:00", "Tuesday 0:30",
                      "Tuesday 12:00", "Tuesday 14:30","Tuesday 17:00",
                      "Tuesday 19:30"};
    initializeMovies(titles,times);
    initializeMovieList(titles,times);
    return 0;
}

void initializeMovieList(char titles[][TITLE_LEN], char times[][TITLE_LEN]){
     char movies[QTY_MOVIES][60];   
     FILE * file = fopen("movie_list", "wb");
     if ( file == NULL ){
            printf("error while creating movie_list file\n");
            exit(1);
     }
     int i=0;
     for(i=0; i<10;i++){
            strcpy(movies[i],titles[i]);
            strcat(movies[i]," ");
            strcat(movies[i],times[i]);
     }        

     if( fwrite(&movies, sizeof(movies), 1, file) != 1 ){
            printf("error while writing movie_list structure\n", i);
            exit(1);
     }
    
     fclose(file);
}

void initializeMovies(char titles[][TITLE_LEN], char times[][TITLE_LEN]){
    int i, j;
    char movieName[40];
    Movie movie[10];
    
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


