#ifndef BACKEND_H
#define BACKEND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STD_SEAT_QTY 60
#define MAX_NAME_LENGTH 60
#define MAX_LENGTH 40
#define MAX_DISPLAY 500
#define MOVIE_PATH "../../database/db/movie_%d"
#define INVALID_SEAT -1
#define SEAT_TAKEN -2

typedef struct{
    char email[MAX_LENGTH];
    char pw[MAX_LENGTH];
    char name[MAX_LENGTH];
} Client;

typedef struct{
    int number;
    int seats_left;
    char seats[STD_SEAT_QTY][MAX_LENGTH]; /* emails array */
} Theatre;

typedef struct{
    int id;
    char name[MAX_NAME_LENGTH];
    char time[20];
    Theatre th;
} Movie;

typedef struct{
    Movie movie;
    int fd;
}MovieFile;

void get_movie(Movie *m, int movieID);

int get_movies_list(char *movies[10][60]);

int cancel_seat(Client c, int movieID, int seat);

int reserve_seat(Client c, Movie m, int seat);

void search_client(Client c, char movieName[MAX_NAME_LENGTH],char sts[MAX_DISPLAY]);

#endif
