#include <fcntl.h>
#include <stdio.h>
#include "../include/fileLockImpl.h"
#include "../include/client.h"


//lock de write, nadie lo puede ni leer ni escribir   
//lock de read, lo pueden leer pero no lo pueden escribir   

void buy_ticket(void){
    int movieID, fd, error = 0, seatNumber = -1;
    char movieName[MAX_NAME_LENGTH];
    Movie requested_movie;
    
    printf("Insert movie code:\n");
    scanf("%d", &movieID);
    sprintf(movieName, "movie_%d", movieID);
  
    FILE * file = fopen(movieName, "rb+");
    if( file == NULL ){
        printf("Invalid movie code: not found in database\n");
        return;
    }
    fd= fileno(file);
    
    //lock de la movie para que nadie mas pueda leerla
    //ni escribirla

    if(wrlockFile(fd) == -1){
        printf("error fcntl\n");
        return;
    }

    if( fread(&requested_movie, sizeof(Movie), 1, file) != 1){
        printf("Error reading from file\n"); 
        return;
    }

    //checkear si esta llena
    if(requested_movie.th.seats_left ==0){
        printf("Sorry, this movie is full\n");
    }
    else{
        //modificar la peli
        printf("modificando la peli\n");
        printf("ID=%d, Name = %s\n", requested_movie.id, requested_movie.name);
        //sleep(20);
    }   
    //unlockear la peli
    unlockFile(fd);

   // printf("ID=%d, Name = %s", requested_movie.id, requested_movie.name);
    return;

    /* F_SETLKW lockea, o unlockea, si ya esta lockeado espera.
    struct flock rwlock;
    fcntl(file, F_SETLKW, &rwlock);

    if( (error = printSeats(code)) == -1){
        printf("Invalid code");
        return;    
    }else if(error == -2){
        printf("Sorry, Theatre is full");
        return;    
    }
    printf("Insert desired seats");
    scanf("%d", seatNumber);
    chooseSeat(seatNumber); */
}

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
