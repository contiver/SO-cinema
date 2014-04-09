#ifndef FILELOCK_H
#define FILELOCK_H
#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MovieFile buy_ticket(char movieName[MAX_NAME_LENGTH]);
int buy_seat(int seat, Client c, MovieFile mf);

#endif

