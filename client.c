#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h>
#include "client.h"

Client login(void);
void buyTicket(void);

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

Client login(void){
    string email, pw;

    printf("email:");
    scanf("%s", email);
    printf("\nPassword:");
    scanf("%s", pw);
    /* TODO Check values: if invalid, print error and return NULL */
    Client c = { email, pw, "DEFAULT_NAME"};
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

void buyTicket(void){
    int movieID, fd, error = 0, seatNumber = -1;
    char movieName[MAX_NAME_LENGTH];

    Movie test;

	printf("Insert movie code:");
    scanf("%d", &movieID);
	sprintf(movieName, "movie_%d", movieID);
    FILE *file = fopen(movieName, "rb");
    if( file == NULL ){
    //if((fd = open(movieName, O_RDWR | 0644)) == -1){
        printf("Invalid movie code: not found in database");
        return;
    }

    if( fread(&test, sizeof(Movie), 1, file) != 1){
        printf("Error reading from file"); 
        return;
    }

    printf("ID=%d, Name = %s", test.id, test.name);
    return;

    /*
    fcntl(fd, F_SETLK, &rwlock);

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

void printSeats(char seats[][], int seatsAmount){
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
