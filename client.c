#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h>
#include "client.h"

Client login(void);
void buyTicket(void);
void initialize_rwflock(struct flock * rwlock);
void finish_rwlock(struct flock * rwlock);

int main(void){
    string command;
    //Client c = login();

    buyTicket();
/*
    while(true){
        scanf("%s", command);
        if( execCommand(command) ) break;
   }
   return;
   */
}

//Client login(void){
  //  string email, pw;

//    printf("email:");
 //   scanf("%s", email);
  //  printf("\nPassword:");
   // scanf("%s", pw);
    /* TODO Check values: if invalid, print error and return NULL */
   // Client c = { email, pw, "DEFAULT_NAME"};
    //return c;
//}

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

void buyTicket(void){
    int movieID, fd, error = 0, seatNumber = -1;
    char movieName[MAX_NAME_LENGTH];
    Movie requested_movie;
    struct flock rwlock;
    
    initialize_rwflock(&rwlock);

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
    if(fcntl(fd,F_SETLKW, &rwlock)==-1){
        printf("error fcntl\n");
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
    finish_rwlock(&rwlock);
    fcntl(fd,F_SETLKW,&rwlock);    

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

void initialize_rwflock(struct flock * rwlock){
    //lock de write, nadie lo puede ni leer ni escribir   
    //lock de read, lo pueden leer pero no lo pueden escribir   
    rwlock->l_type    = F_WRLCK; 
    rwlock->l_start   = 0;
    rwlock->l_whence  = SEEK_SET;
    rwlock->l_len     = 0;
}

void finish_rwlock(struct flock * rwlock){
    rwlock->l_type    = F_UNLCK; 
}

void cancelPurchase(){
    /* TODO */
    return;
}
