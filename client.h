#include <fcntl.h>
#define BUF_SIZE 512
#define STD_SEAT_QTY 60
#define MAX_NAME_LENGTH 60
#define MAX_LENGTH 40

/*
struct flock rwlock;
rwlock.l_type    = F_WRLCK;
rwlock.l_start   = 0;
rwlock.l_whence  = SEEK_SET;
rwlock.l_len     = 0;
*/
typedef char* string;

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

