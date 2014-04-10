#ifndef BACKEND_H
#define BACKEND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"

#define MAX_DISPLAY 500

void buy_seat(Client c, int movieID);

int get_movies_list(char *movies[10][60]);

void search_client(Client c, char movieName[MAX_NAME_LENGTH],char sts[MAX_DISPLAY]);

#endif

