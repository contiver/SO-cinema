#ifndef CLIENTFRONT_H
#define CLIENTFRONT_H

#include "dbAccess.h"
#define COMMAND_SIZE 4
#define ROWS 6
#define COLS 10

// ANSI colors definitions
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

char *commands[] = { "ListMovies", "BuyMovie", "CancelMovie", "Exit" };

Client login(void);

int check_command(char * com);

void execute_command(char *command, Client c);

void buy_ticket(Client c);

void printSeats(char seats[][MAX_LENGTH]);

void list_movies(void);

void cancel_ticket(Client c);

#endif
