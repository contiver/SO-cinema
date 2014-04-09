#include <fcntl.h>
#include <stdio.h>
#include "../include/backend.h"
#include "../include/client.h"

/* Las funciones de filelocking son especificas de esta implementacion,
   por lo que los prototipos no deberian ir en el header */
int unlockFile(int fd);
int wrlockFile(int fd);
int rdlockFile(int fd);

MovieFile access_movie(char movieName[MAX_NAME_LENGTH]){
    int fd;
    Movie requested_movie;
    MovieFile mf;    

    FILE *file = fopen(movieName, "rb+");
    if( file == NULL ){
        printf("Invalid movie code: not found in database\n");
        exit(1);
    }
    fd = fileno(file);
    
    //lock de la movie para que nadie mas pueda leerla
    //ni escribirla

    if(wrlockFile(fd) == -1){
        printf("Error locking file\n");
        exit(1);
    }

    if(fread(&requested_movie, sizeof(Movie), 1, file) != 1){
        printf("Error reading from file\n"); 
        exit(1);
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

int buy_seat(int seat, Client c, MovieFile mf){
    char movieName[40];
    int i=0;
    if(seat>60 || seat<1){
        return -2;
    }
    if(strcmp(mf.movie.th.seats[seat-1],"")!=0){
        return -1; //ese asiento esta ocupado
    }
    strncpy(mf.movie.th.seats[seat-1],c.email,MAX_LENGTH);

    Theatre newth;
    newth.number= mf.movie.th.number;
    newth.seats_left= (mf.movie.th.seats_left)-1;
    for(i=0;i<STD_SEAT_QTY;i++){
        strncpy(newth.seats[i],mf.movie.th.seats[i],MAX_LENGTH);
    }
                    
    Movie newmovie;
    newmovie.id= mf.movie.id;
    strncpy(newmovie.name, mf.movie.name,MAX_NAME_LENGTH);
    strncpy(newmovie.time, mf.movie.time,20);
    newmovie.th= newth;

    sprintf(movieName, "./src/database/movie_%d", mf.movie.id); 
   
    //lo abre y lo trunca a long 0   
    FILE *file = fopen(movieName, "wb");
    if ( file == NULL ){
        printf("error while creating movie_%d file\n", mf.movie.id);
        exit(1);
    }
    if( fwrite(&newmovie, sizeof(Movie), 1, file) != 1 ){
            printf("error while writing movie_%d structure\n",mf.movie.id);
            exit(1);
    }
    fclose(file);
    return 0; //reserva ok
}

void search_client(Client c, char movieName[MAX_NAME_LENGTH],char sts[MAX_DISPLAY]){
    MovieFile mf= access_movie(movieName);
    int s=1;
    char buff[10];
    
    while(s<=60){
        if(strcmp(c.email, mf.movie.th.seats[s++])==0){
            sprintf(buff, "%d", s);
            strcat(sts,buff);   
            strcat(sts,"  ");
        }
    }
}
