#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include "client.h"
#define COMMAND_SIZE 20

Client login(void);
void buy_ticket(void);
void initialize_rwflock(struct flock * rwlock);
void finish_rwlock(struct flock * rwlock);
void execute_command(char * command);
int check_command(char * com);

char * commands[]={"ListMovies","BuyMovie", "ChooseSeat", "CancelMovie", "Exit"};

int main(void){

    char command[20]="";
    //Client c = login();
    printf("\nWelcome to MovieLand :)\n");
    Client c = login();
    printf("\nPlease type the desired option:\n");
    printf("ListMovies\nBuyMovie\nChooseSeat\nCancelMovie\nExit\n\n");
    scanf("%s",command);
    execute_command(command);

}


void execute_command(char * command){
    int ans=check_command(command);
    if(ans==-1){
        printf("Invalid command\n");
    }
    else{
        switch(ans){
            case 0:
                    //list_movies(); break;
            case 1:
                    buy_ticket(); break;
            case 2:
                   // choose_seat(); break;
            case 3:
                   // cancel_ticket(); break;
            case 4:
                    exit(1);
        }
    }
}

int check_command(char * com){
    int i;
    for(i=0;i<COMMAND_SIZE;i++){
        if(strcmp(commands[i],com)==0){
            return i;
        }
    }
    return -1;
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

//lock de write, nadie lo puede ni leer ni escribir   
//lock de read, lo pueden leer pero no lo pueden escribir   
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
