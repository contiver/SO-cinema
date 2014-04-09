#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include "../include/client.h"
#include "../include/fileLockImpl.h"

#define COMMAND_SIZE 20

Client login(void);
void buy_ticket(void);
void initialize_rwflock(struct flock * rwlock);
void execute_command(char * command);
int check_command(char * com);

string commands[] = { "ListMovies", "BuyMovie", "ChooseSeat",
                      "CancelMovie", "Exit" };

int main(void){
    char command[20] = "";
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
