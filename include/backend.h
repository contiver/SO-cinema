#ifndef BACKEND_H
#define BACKEND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"

#define MAX_DISPLAY 500
#define MOVIE_PATH "./src/database/movie_%d"
#define INVALID_SEAT -1
#define SEAT_TAKEN -2


void get_movie(Movie *m, int movieID);

int get_movies_list(char *movies[10][60]);

int cancel_seat(Client c, int movieID, int seat);

int reserve_seat(Client c, Movie m, int seat);

void search_client(Client c, char movieName[MAX_NAME_LENGTH],char sts[MAX_DISPLAY]);

#endif

