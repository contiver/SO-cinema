#ifndef FILELOCK_H
#define FILELOCK_H
#define MAX_DISPLAY 500
#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MovieFile access_movie(char movieName[MAX_NAME_LENGTH]);
int buy_seat(int seat, Client c, MovieFile mf);
void search_client(Client c, char movieName[MAX_NAME_LENGTH],char sts[MAX_DISPLAY]);

#endif

