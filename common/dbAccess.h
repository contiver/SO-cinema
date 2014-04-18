#ifndef DBACCESS_H
#define DBACCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "shared.h"

#define MAX_DISPLAY 500
#define MOVIE_PATH "../../database/bin/movie_%d"
#define MLIST_PATH "../../database/bin/movie_list"
#define INVALID_SEAT -1
#define SEAT_TAKEN -2
#define NO_SEATS -1
#define INVALID_MOVIE_ID -3

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
