#ifndef SHARED_H
#define SHARED_H

#define MAX_LENGTH 40
#define MAX_NAME_LENGTH 30
#define STD_SEAT_QTY 60

typedef struct{
    char m[10][60];
} Matrix;

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

#endif
