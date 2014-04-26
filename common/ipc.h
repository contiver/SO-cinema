#ifndef IPC_H
#define IPC_H
#include <sys/types.h> 
#include "shared.h"

typedef enum{RESERVE_SEAT, CANCEL_SEAT, GET_MOVIE, MOVIE_LIST} command;

typedef struct  {
    pid_t pid;   // client's pid
    command comm; // number representing a command
    Client client;
    int movieID;
    int seat;
} Request;

typedef struct  {
    int ret;  // Value to interpret depending of what the client requested
    Movie m;
    Matrix matrix;
} Response;

#endif
