#include <fcntl.h>
#include <stdio.h>
#include "../include/backend.h"
#include "../include/client.h"

/* Las funciones de filelocking son especificas de esta implementacion,
   por lo que los prototipos no deberian ir en el header */
int unlockFile(int fd);
int wrlockFile(int fd);
int rdlockFile(int fd);

MovieFile buy_ticket(char movieName[MAX_NAME_LENGTH]){
    int fd, error = 0, seatNumber = -1;
    int ans = 0;
    Movie requested_movie;
    MovieFile mf;    

    FILE *file = fopen(movieName, "rb+");
    if( file == NULL ){
        printf("Invalid movie code: not found in database\n");
        return;
    }
    fd = fileno(file);
    
    //lock de la movie para que nadie mas pueda leerla
    //ni escribirla

    if(wrlockFile(fd) == -1){
        printf("error fcntl\n");
        return;
    }

    if(fread(&requested_movie, sizeof(Movie), 1, file) != 1){
        printf("Error reading from file\n"); 
        return;
    }
    
    mf.movie=requested_movie;
    mf.fd=fd;
    return mf;

    //unlockFile(fd);
}

// lock de write, nadie lo puede ni leer ni escribir   
// lock de read, lo pueden leer pero no lo pueden escribir   

int rdlockFile(int fd){
    struct flock fl = {.l_type = F_RDLCK, .l_start = 0,
                       .l_whence = SEEK_SET, .l_len = 0};
    return fcntl(fd, F_SETLKW, &fl);
}

int wrlockFile(int fd){
    struct flock fl = {.l_type = F_WRLCK, .l_start = 0,
                       .l_whence = SEEK_SET, .l_len = 0};
    return fcntl(fd, F_SETLKW, &fl);
}

int unlockFile(int fd){
    struct flock fl = {.l_type = F_UNLCK, .l_start = 0,
                       .l_whence = SEEK_SET, .l_len = 0};
    return fcntl(fd, F_SETLKW, &fl);
}
