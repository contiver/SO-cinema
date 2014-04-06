#include <fcntl.h>
#define BUF_SIZE 512
#define STD_SEAT_QTY 60
#define MAX_NAME_LENGTH 60

/*
struct flock rwlock;
rwlock.l_type    = F_WRLCK;
rwlock.l_start   = 0;
rwlock.l_whence  = SEEK_SET;
rwlock.l_len     = 0;
*/
typedef char* string;

typedef struct{
    string email;
    string pw;
    string name;
} Client;

typedef struct{
    int number;
    int size;
    string seats[STD_SEAT_QTY];
} Theatre;

typedef struct{
    int id;
    string name;
    string time;
    Theatre th;
} Movie;

