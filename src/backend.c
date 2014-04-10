#include <fcntl.h>
#include <stdio.h>
#include "../include/backend.h"
#include "../include/client.h"

#define MOVIE_PATH "./src/database/movie_%d"
#define NO_SEATS -1

/* Las funciones de filelocking son especificas de esta implementacion,
   por lo que los prototipos no deberian ir en el header */
struct flock fl = {.l_start = 0, .l_whence = SEEK_SET, .l_len = 0};
int unlockFile(int fd);
int wrlockFile(int fd);
int rdlockFile(int fd);

/* En principio esta es una funcion "privada", con lo que tampoco
   deberia formar parte del header */
int reserve_seat(Client c, Movie m, int seat);

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
}
/*
        //sale y va a unlock
        //falta ver donde cerrar el archivo
    }else{
        while( aux != 0 ){
            printf("Please choose a seat\n");
            printf("%s %s\n", m.name, m.time);
            printSeats(m.th.seats);
            scanf("%d",&seat);
            aux = reserve_seat(c, m, seat);
            if( aux == -1 ){
                printf("Sorry, that seat is taken\n");
            }
            else if(aux==-2){
                printf("Invalid number of seat\n");
            }
            //si devuelve 0 es que ya reservo ese asiento
        }
        printf("The purchase has been successful\n");
    }
    //al terminar unlockear en el back
}
*/
// lock de write, nadie lo puede ni leer ni escribir   
// lock de read, lo pueden leer pero no lo pueden escribir   

int reserve_seat(Client c, Movie m, int seat){
    char movieName[40];
    int i = 0, fd;
   
    if( seat > STD_SEAT_QTY || seat < 1 ) return -2;
    if( strcmp(m.th.seats[seat-1],"") != 0 ) return -1; // asiento ocupado
    
    m.th.seats_left = -1;
    strncpy(m.th.seats[seat-1], c.email, MAX_LENGTH);
    
    sprintf(movieName, MOVIE_PATH, m.id); 

    FILE *file = fopen(movieName, "wb");
    if ( file == NULL ){
        printf("error while creating movie_%d file\n", m.id);
        exit(1);
    }

    fd = fileno(file);

    // TODO esta bien este file lock, no? o va en otro lado?
    if( wrlockFile(fd) == -1 ){
        printf("Error locking file\n");
        exit(1);
    }

    if( fwrite(&m, sizeof(Movie), 1, file) != 1 ){
        printf("error while writing movie_%d structure\n", m.id);
        exit(1);
    }
    
    unlockFile(fd);
    fclose(file);
    return 0; //reserva ok
}

int get_movies_list(char *movies[10][60]){
    FILE *file = fopen("./src/database/movie_list", "rb");
    if( file == NULL ) return -1;
    if( fread(movies, sizeof(char[10][60]), 1, file) != 1) return -2;
    
    return 0;
}

/*
void search_client(Client c, char movieName[MAX_NAME_LENGTH], char sts[MAX_DISPLAY]){
    FILE *file = fopen(movieName, "rb");
    if( file == NULL ) 
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
*/

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
