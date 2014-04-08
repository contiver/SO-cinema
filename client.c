#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include "client.h"
#include "fileLockImpl.h"

Client login(void);
void buyTicket(void);
int unlockFile(int fd);
int wrlockFile(int fd);
int rdlockFile(int fd);

int main(void){
    string command;
   // Client c = login();

    buyTicket();
/*
    while(true){
        scanf("%s", command);
        if( execCommand(command) ) break;
   }
   return;
   */
}

Client login(void){
    char email[MAX_LENGTH],
         pw[MAX_LENGTH];

    printf("email:");
    scanf("%s", email);
    printf("\nPassword:");
    scanf("%s", pw);
    /* TODO Check values: if invalid, print error and return NULL */
    Client c;
    strncpy(c.email, email, MAX_LENGTH);
    strncpy(c.pw, pw, MAX_LENGTH);
    strncpy(c.name, "DEFAULT_NAME", MAX_LENGTH);
    return c;
}

int execCommand(string command){
    if( !strcmp("exit", command) || !strcmp("quit", command) ) return 1; 

    /* TODO Search command and execute it. On success return 0 */
    return 0; 
} 

/*
    
void checkMovies(){ 
    int fd;
	ssize_t nread;
	char buffer[BUF_SIZE];

	if( (fd = open(DATABASE, O_RDONLY | 0644)) == -1){
		exit(1);		
	}

	while((nread = read(fd, buffer, BUF_SIZE)) > 0){
		// imprimir en pantallas las peliculas 	
	}
}
*/

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
        sleep(20);
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

void cancelPurchase(){
    /* TODO */
    return;
}

void printSeats(char seats[][MAX_LENGTH], int seatsAmount){
    int rows, cols, seatNumber;
    for(rows = 0; rows < 6; rows++){
        for(cols = 0; cols < 10; cols++){
            if(seats[seatNumber = rows*10 + cols] == NULL){
                printf("%.4d", seatNumber);
            }else{
                printf("%.4s", "X");
            }
        }
        printf("\n");
   } 
}
