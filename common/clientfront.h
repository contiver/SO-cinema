#ifndef CLIENTFRONT_H
#define CLIENTFRONT_H

#include "dbAccess.h"
#define COMMAND_SIZE 4
#define ROWS 6
#define COLS 10

char *commands[] = { "ListMovies", "BuyMovie", "CancelMovie", "Exit" };

Client login(void);

int check_command(char * com);

void execute_command(char *command, Client c);

void buy_ticket(Client c);

void printSeats(char seats[][MAX_LENGTH]);

void list_movies(void);

void cancel_ticket(Client c);

#endif
