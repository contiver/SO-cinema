#include <fcntl.h>
#include <stdio.h>
#include "dbAccess.h"

#define MOVIE_PATH "../../database/db/movie_%d"
#define MLIST_PATH "../../database/db/movie_list"

/* File locking auxiliary functions particular to the backend implementation */
struct flock fl = {.l_start = 0, .l_whence = SEEK_SET, .l_len = 0};
int unlockFile(int fd);
int wrlockFile(int fd);
int rdlockFile(int fd);

Movie
get_movie(int movieID){
    Movie m;
    m.name[0] = 0;
    char movieName[MAX_NAME_LENGTH];
    sprintf(movieName, MOVIE_PATH, movieID);

    FILE *file = fopen(movieName, "rb+");
    if( file == NULL ){
        printf("Invalid movie code: not found in database\n");
        return m;
    }

    int fd = fileno(file);
    
    // lock de la movie para que nadie mas pueda leerla ni escribirla
    if( wrlockFile(fd) == -1 ){
        printf("Error locking file 1\n");
        return m;
    }

    if( fread(&m, sizeof(Movie), 1, file) != 1 ){
        printf("Error reading from file\n"); 
        return m;
    }
   
    unlockFile(fd);
    fclose(file);
    return m;
}

int
reserve_seat(Client c, int movieID, int seat){
    int fd;
    Movie m;
    char movieName[MAX_NAME_LENGTH];
     
    if( seat < 1 || seat > STD_SEAT_QTY ){
	    return INVALID_SEAT;
    }

    sprintf(movieName, MOVIE_PATH, movieID); 
    FILE *file = fopen(movieName, "rb+");
    if ( file == NULL ){
        printf("error while opening movie_%d file\n", movieID);
        return INVALID_MOVIE_ID;
    }
    
    fd = fileno(file);
    if( wrlockFile(fd) == -1 ){
        printf("Error locking file 2\n");
        fclose(file);
        return -3;
    }
    
    if( fread(&m, sizeof(Movie), 1, file) != 1 ){
        printf("Error reading from file\n"); 
        unlockFile(fd);
        fclose(file);
        return -4;
    }

    if( strcmp(m.th.seats[seat-1],"") != 0 ){
        unlockFile(fd);
        fclose(file);
        return SEAT_TAKEN;
    }
    
    m.th.seats_left--;
    strncpy(m.th.seats[seat-1], c.email, MAX_LENGTH);
 
    fclose(file);
    file = fopen(movieName, "wb");
    if ( file == NULL ){
        printf("error while opening movie_%d file\n", movieID);
        return INVALID_MOVIE_ID;
    }

    if( fwrite(&m, sizeof(Movie), 1, file) != 1 ){
        printf("error while writing movie_%d structure\n", movieID);
        unlockFile(fd);
        fclose(file);
        exit(1);
    }
    unlockFile(fd);
    fclose(file);
    return 0; //reserva ok
}

int
get_movies_list(char *movies_list[10][60]){
    //char movie_list[10][60];
    FILE *file = fopen(MLIST_PATH, "rb");
    if( file == NULL ) return -1;
    if( fread(movies_list, sizeof(char[10][60]), 1, file) != 1) return -2;
    
    return 0;
}

int
cancel_seat(Client c, int movieID, int seat){
    int fd;
    char movieName[MAX_NAME_LENGTH];
    Movie m;
    m = get_movie(movieID);
    sprintf(movieName, MOVIE_PATH, movieID); 

    if( strncmp(m.th.seats[seat-1], c.email, strlen(c.email)) != 0 ){
        return 1;
    }

    strncpy(m.th.seats[seat-1], "\0", MAX_LENGTH);
    m.th.seats_left++;

    FILE *file = fopen(movieName, "wb");
    if( file == NULL ){
        printf("Invalid movie code: not found in database\n");
        exit(1);
    }

    fd = fileno(file); 
    if( wrlockFile(fd) == -1 ){
        printf("Error locking file 3\n");
        return INVALID_MOVIE_ID; 
    }

    if( fwrite(&m, sizeof(Movie), 1, file) != 1 ){
        printf("error while writing movie_%d structure\n", m.id);
        exit(1);
    }
    unlockFile(fd);
    fclose(file);
    return 0;
}

int
rdlockFile(int fd){
    fl.l_type = F_RDLCK;
    return fcntl(fd, F_SETLKW, &fl);
}

int
wrlockFile(int fd){
    fl.l_type = F_WRLCK;
    return fcntl(fd, F_SETLKW, &fl);
}

int
unlockFile(int fd){
    fl.l_type = F_UNLCK;
    return fcntl(fd, F_SETLKW, &fl);
}

bool noSeatsLeft(Movie m){
    return m.th.seats_left == 0;
}
