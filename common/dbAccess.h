#ifndef BACKEND_H
#define BACKEND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define STD_SEAT_QTY 60
#define MAX_NAME_LENGTH 60
#define MAX_LENGTH 40
#define MAX_DISPLAY 500
#define MOVIE_PATH "../../database/db/movie_%d"
#define MLIST_PATH "../../database/db/movie_list"
#define INVALID_SEAT -1
#define SEAT_TAKEN -2
#define NO_SEATS -1
#define INVALID_MOVIE_ID -3

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

bool noSeatsLeft(Movie m);

//char** getSeats(Movie m){
//    return m.th.seats;
//}

/**
 * @param movieID 
 * @return The movie identified by the movieID
 */
Movie get_movie(int movieID);

/**
 * @param movieID 
 * @return The movie identified by the movieID
 */
int get_movies_list(char *movies[10][60]);

/**
 * @param c client making the request
 * @param movieID integer that identifies the movie 
 * @param seat seat number to cancel
 * @return 0 if no problems were encountered, negative value otherwise
 */
int cancel_seat(Client c, int movieID, int seat);

/**
 * @param c client making the request
 * @param movieID integer that identifies the movie 
 * @param seat seat number to cancel
 * @return 0 if no problems were encountered, negative value otherwise
 */
int reserve_seat(Client c, int movieID, int seat);

#endif
