#include <fcntl.h>
#include <stdio.h>
#include "../../include/client.h"
#include "../../include/common.h"

#define NO_SEATS -1
#define MLIST_PATH "../../database/db/movie_list"

/* Las funciones de filelocking son especificas de esta implementacion,
   por lo que los prototipos no deberian ir en el header */
struct flock fl = {.l_start = 0, .l_whence = SEEK_SET, .l_len = 0};
int unlockFile(int fd);
int wrlockFile(int fd);
int rdlockFile(int fd);

int lockedfd = -1;
FILE *openedf = NULL;

void get_movie(Movie *m, int movieID){
    char movieName[MAX_NAME_LENGTH];
    sprintf(movieName, MOVIE_PATH, movieID);

    FILE *file = fopen(movieName, "rb+");
    if( file == NULL ){
        printf("Invalid movie code: not found in database\n");
        exit(1);
    }

    int fd = fileno(file);
    
    // TODO hace falta usar un lock aca? 
    // lock de la movie para que nadie mas pueda leerla ni escribirla
    if( wrlockFile(fd) == -1 ){
        printf("Error locking file\n");
        exit(1);
    }

    if( fread(m, sizeof(Movie), 1, file) != 1 ){
        printf("Error reading from file\n"); 
        exit(1);
    }
    lockedfd = fd;
}

int reserve_seat(Client c, Movie m, int seat){
    char movieName[40];
    int i = 0, fd;
   
    if( seat > STD_SEAT_QTY || seat < 1 ) return INVALID_SEAT;
    if( strcmp(m.th.seats[seat-1],"") != 0 ) return SEAT_TAKEN; // asiento ocupado
    
    m.th.seats_left--;
    strncpy(m.th.seats[seat-1], c.email, MAX_LENGTH);
    
    sprintf(movieName, MOVIE_PATH, m.id); 

    FILE *file = fopen(movieName, "wb");
    if ( file == NULL ){
        printf("error while creating movie_%d file\n", m.id);
        exit(1);
    }

    if( fwrite(&m, sizeof(Movie), 1, file) != 1 ){
        printf("error while writing movie_%d structure\n", m.id);
        exit(1);
    }
    
    unlockFile(fd);
    fclose(file);
    if(lockedfd != -1){
        unlockFile(lockedfd);
        lockedfd = -1;
    }
    if(openedf != NULL){
        fclose(openedf);
        openedf = NULL;
    }
    return 0; //reserva ok
}

int get_movies_list(char *movies[10][60]){
    FILE *file = fopen(MLIST_PATH, "rb");
    if( file == NULL ) return -1;
    if( fread(movies, sizeof(char[10][60]), 1, file) != 1) return -2;
    
    return 0;
}

int cancel_seat(Client c, int movieID, int seat){
    int fd;
    char movieName[40];
    Movie m;
    get_movie(&m, movieID);
    sprintf(movieName, MOVIE_PATH, m.id); 

    if( strncmp(m.th.seats[seat-1], c.email, strlen(c.email)) == 0 ){
        strncpy(m.th.seats[seat-1], "\0", MAX_LENGTH);
        m.th.seats_left++;
        
        FILE *file = fopen(movieName, "wb");
        if( file == NULL ){
            printf("Invalid movie code: not found in database\n");
            exit(1);
        }

        // TODO hace falta usar un lock aca? 
        // lock de la movie para que nadie mas pueda leerla ni escribirla
        
       if( wrlockFile(fileno(file)) == -1 ){
            printf("Error locking file\n");
            exit(1);
        }
        
        if( fwrite(&m, sizeof(Movie), 1, file) != 1 ){
            printf("error while writing movie_%d structure\n", m.id);
            exit(1);
        }
        unlockFile(fd);
        fclose(file);
        return 0;
    }
    unlockFile(fd);
    return 1;
}

int rdlockFile(int fd){
    fl.l_type = F_RDLCK;
    return fcntl(fd, F_SETLKW, &fl);
}

int wrlockFile(int fd){
    fl.l_type = F_WRLCK;
    return fcntl(fd, F_SETLKW, &fl);
}

int unlockFile(int fd){
    fl.l_type = F_UNLCK;
    return fcntl(fd, F_SETLKW, &fl);
}
