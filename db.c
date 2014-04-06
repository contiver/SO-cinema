#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#define STD_SEAT_QTY 60
#define DB_PATH "t1"

void initializeTheatres(void);

typedef char* string;

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

int main(void){
    initializeTheatres();
    return 0;
}

void initializeTheatres(void){
    int i, fd;

    Theatre t1;
    t1.number = 1;
    t1.size = STD_SEAT_QTY;
    
    for(i = 0; i < STD_SEAT_QTY; i++){
        t1.seats[i] = NULL;
    }

    if ( (fd = open(DB_PATH, O_RDWR | O_CREAT | O_EXCL, 0644)) == -1){
        printf("error while creating database\n");
       // errExit("open");
       exit(1);
    }

    if( write(fd, &t1, sizeof(Theatre)) == -1){
        printf("error while writing structure");
        exit(1);
    }

    if(close(fd) == -1) exit(1);//errExit("close");
    return;
}
