#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include "clientfront.h"
#include "clientback.h"
#include "dbAccess.h"

int main(void){
    char command[20]="";
    printf("\nWelcome to RetroMovies :)\n");
    Client c = login();
    initializeClient();
    while(true){
        printf("\nPlease type the desired option:\n");  
        printf("ListMovies\nBuyMovie\nCancelMovie\nExit\n\n");
        scanf("%s",command);
        execute_command(command, c);
    }
}

Client login(void){
    char email[MAX_LENGTH],
         pw[MAX_LENGTH];

    printf("Log In\nEmail:");
    scanf("%s", email);
    printf("Password:");
    scanf("%s", pw);
    /* TODO Check values: if invalid, print error and return NULL */
    Client c;
    strncpy(c.email, email, MAX_LENGTH);
    strncpy(c.pw, pw, MAX_LENGTH);
    strncpy(c.name, "DEFAULT_NAME", MAX_LENGTH);
    return c;
}

void execute_command(char *command, Client c){
    int ans = check_command(command);
    switch( ans ){
        case 0:  list_movies();
                 break;
        case 1:  buy_ticket(c);
                 break;
        case 2:  cancel_ticket(c);
                 break;
        case 3:  terminateClient();
                 break;
        default: printf("Invalid command\n");
                 return;
    }
}

int check_command(char *com){
    int i;
    for(i = 0; i < COMMAND_SIZE; i++){
        if( strcmp(commands[i],com) == 0 ){
            return i;
        }
    }
    return -1;
}

void cancel_ticket(Client c){
    int movieID, seat;
    
    printf("Insert movie code:\n");
    scanf("%d", &movieID);    
    printf("Insert seat ticket to cancel:\n");
    scanf("%d", &seat);    

    if( cancel_seat(c, movieID, seat) == 1){
        printf("Sorry, we couldn't cancel the seat because it isn't yours\n");
    }
	else{
		printf("Your seat has been cancelled\n");
	}
}

void buy_ticket(Client c){
    int movieID, seat, aux = 1;
    printf("Insert movie code:\n");
    scanf("%d", &movieID);

    Movie m;
    m.name[0] = 0;
    m = get_movie(movieID);    

    if(m.name[0] == 0){
        printf("Sorry, either the movie code is invalid, or an error\
                has ocurred while trying to access the database\n");
        return;
    }

    if( m.th.seats_left == 0 ){
        printf("Sorry, no seats left for this movie\n");
        return;
    }

    while(aux){
        printf("Please choose a seat\n"); 
        printf("%s %s\n", m.name, m.time);
        printSeats(m.th.seats);

	char buf[1024];
	fgets(buf,sizeof(buf),stdin);

        if(scanf("%d", &seat)!=1){
		aux = INVALID_SEAT;
		printf("Invalid number of seat\n");
	}
	else{
        	aux = reserve_seat(c, movieID, seat);
        	if( aux == SEAT_TAKEN ){
            	printf("Sorry, that seat is taken, please try with another one\n");
        	}else if(aux == INVALID_SEAT){
            	printf("Invalid number of seat\n");
        	}
	}
    }
    printf("The purchase has been successful\n");
}

void list_movies(void){
    int code = 1, i = 0, error;
    char movies[10][60];
    if ( (error = get_movies_list(&movies)) != 0){
        switch( error ){
            case -1: printf("ListMovies file not found in database\n");
                     break;
            case -2: printf("Error reading from file\n"); 
                     break;
        }
	return;
    }
    printf("------------------------------------------------------------\n");
    for(i = 0; i < 10; i++){
        printf("%s\t\tCode:%d\n",movies[i],code++);
    }
    printf("------------------------------------------------------------\n");
}

void printSeats(char seats[][MAX_LENGTH]){
    int row, col, seatNumber;
    for(row = 0; row < ROWS; row++){
        for(col = 0; col < COLS; col++){
            if( strcmp(seats[seatNumber = row*10 + col], "\0") == 0 ){
                printf("%4d", seatNumber+1);
            }else{
                printf("%4s", "X");
            }
        }
        printf("\n");
   } 
}
