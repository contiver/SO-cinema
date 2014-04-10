#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include "../include/client.h"
#include "../include/backend.h"

#define COMMAND_SIZE 4
#define ROWS 6
#define COLS 10

Client login(void);
void execute_command(char *command, Client c);
void buy_ticket(Client c);
int check_command(char * com);
void printSeats(char seats[][MAX_LENGTH]);
void list_movies(void);
void cancel_ticket(Client c);

string commands[] = { "ListMovies", "BuyMovie", "CancelMovie", "Exit" };

int main(void){
    char command[20]="";
    printf("\nWelcome to RetroMovies :)\n");
    Client c = login();
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
        case -1: printf("Invalid command\n");
                 return;
        case 0:  list_movies();
                 break;
        case 1:  buy_ticket(c);
                 break;
        case 2:  cancel_ticket(c);
                 break;
        case 3:  exit(1);
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

/** TODO
 ********* Hacerla wrapper de una funcion del back ****
 ********* como buy_ticket ! ************************/
void cancel_ticket(Client c){
    int movieID;
    char movieName[MAX_NAME_LENGTH];
    char sts[MAX_DISPLAY]="Your current seats are:";

    printf("Insert movie code:\n");
    scanf("%d", &movieID);    
    sprintf(movieName, "./src/database/movie_%d", movieID);

    //search_client(c,movieName,sts);
    printf("%s\n",sts);
}

void buy_ticket(Client c){
    int movieID, seat, aux = 1;
    printf("Insert movie code:\n");
    scanf("%d", &movieID);

    Movie m;
    get_movie(&m, movieID);    

    if( m.th.seats_left == 0 ){
        printf("Sorry, no seats left for this movie");
        return;
    }
    while(aux){
        printf("Please choose a seat\n"); 
        printf("%s %s\n", m.name, m.time);
        printSeats(m.th.seats);
        scanf("%d", &seat); 
        aux = reserve_seat(c, m, seat);
        if( aux == -1 ){
            printf("Sorry, that seat is taken\n");
        }else if(aux==-2){
            printf("Invalid number of seat\n");
        }
    //si devuelve 0 es que ya reservo ese asiento
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
