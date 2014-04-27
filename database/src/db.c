#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "../../common/db.h"
#include "../../common/dbAccess.h"
#include "../../common/shared.h"

void initializeMovies(char titles[][TITLE_LEN], char times[][TITLE_LEN]);
void initializeMovieList(char titles[][TITLE_LEN], char times[][TITLE_LEN]);

int
main(void){
    char titles[MOVIES_QTY][TITLE_LEN] = {"The Aristocats           ",
        "Space Jam                ", "Shrek                    ",
        "Garfield                 ", "The Lion King            ",
        "Jurassic Park            ", "The Parent Trap          ",
        "Matilda                  ", "Casper                   ",
        "Honey I shrunk the kids  "}; 
    char times[MOVIES_QTY][TITLE_LEN] = {
        "Monday 13:00", "Monday 15:00", "Monday 16:45",
        "Monday 19:00", "Monday 22:00", "Tuesday 0:30",
        "Tuesday 12:00", "Tuesday 14:30","Tuesday 17:00",
        "Tuesday 19:30"};
    initializeMovies(titles,times);
    initializeMovieList(titles,times);
    return 0;
}

void
initializeMovieList(char titles[][TITLE_LEN], char times[][TITLE_LEN]){
     int i;
     Matrix mList;
     FILE *file = fopen(MLIST_PATH, "wb");
     if ( file == NULL ){
            printf("error while creating movie_list file\n");
            exit(EXIT_FAILURE);
     }
     for(i = 0; i < 10; i++){
            strcpy(mList.m[i], titles[i]);
            strcat(mList.m[i], " ");
            strcat(mList.m[i], times[i]);
     }        
     if( fwrite(&mList, sizeof(mList), 1, file) != 1 ){
            printf("Error while writing movie_list file\n");
            exit(EXIT_FAILURE);
     }
     fclose(file);
}

void
initializeMovies(char titles[][TITLE_LEN], char times[][TITLE_LEN]){
    int i, j;
    char movieName[40];
    Movie movie[10];
    
    for(i = 0; i < 10; i++){
        movie[i].id = i+1;
        strncpy(movie[i].name, titles[i], MAX_LENGTH); 
        strncpy(movie[i].time, times[i], MAX_LENGTH);
        movie[i].th.number = 1;
        movie[i].th.seats_left = STD_SEAT_QTY;
        for(j = 0; j < STD_SEAT_QTY; j++){
            strncpy((movie[i].th.seats)[j], "\0", MAX_LENGTH);
        }

        sprintf(movieName, MOVIE_PATH, movie[i].id);
        FILE *file = fopen(movieName, "wb");
        if ( file == NULL ){
            printf("Error while creating movie_%d file\n", i);
            exit(EXIT_FAILURE);
        }
        if( fwrite(&movie[i], sizeof(Movie), 1, file) != 1 ){
            printf("error while writing movie_%d structure\n", i);
            exit(EXIT_FAILURE);
        }
        fclose(file);
    }
}
