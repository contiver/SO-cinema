#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdbool.h>
#include "clientfront.h"
#include "clientback.h"
#include "dbAccess.h"

int
main(void){
    char command[20]="";
    printf("\nWelcome to RetroMovies :)\n");
    Client c = login();
    initializeClient();
    while(true){
        printf("\nPlease type the desired option: \n");  
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
    for(i = 0; com[i]; i++){
        com[i] = (char) tolower(com[i]);
    }
    for(i = 0; i < COMMAND_SIZE; i++){
        if( strcmp(commands[i], com) == 0 ){
            return i;
        }
    }
    return -1;
}

void cancel_ticket(Client c){
    int movieID, seat;

    printf("Insert movie code: ");
    fflush(stdout);
    scanf("%d", &movieID);    
    if(movieID > MOVIES_QTY || movieID < 1){
        printf("Invalid movie code\n");
        return;
    }
    printf("\n Insert seat ticket to cancel: ");
    fflush(stdout);
    scanf("%d", &seat);    

    if( cancel_seat(c, movieID, seat) == 1){
        printf(RED "Error: The seat you are trying to cancel isn't yours\n" RESET); 
    }
    else{
        printf(GREEN "Seat successfully cancelled\n" RESET);
    }
}

void buy_ticket(Client c){
    int movieID, seat, aux = 1;
    char buffer[1024];

    printf("Insert movie code:\n");
    fgets(buffer,sizeof(buffer),stdin);
    scanf("%d", &movieID);

    if(movieID > MOVIES_QTY || movieID < 1){
        printf("Invalid movie code\n");
        return;
    }

    Movie m;
    m.name[0] = 0;
    m = get_movie(movieID);    

    if(m.name[0] == 0){
        printf("Sorry, an error has ocurred while trying to access the database\n");
        return;
    }

    if( m.th.seats_left == 0 ){
        printf("Sorry, no seats left for this movie\n");
        return;
    }

    char buf[1024];
    printf("Please choose a seat\n"); 
    printf("%s %s\n", m.name, m.time);
    printSeats(m.th.seats);
    while(aux){
        fgets(buf, sizeof(buf), stdin);

        if(scanf("%d", &seat) != 1){
            aux = INVALID_SEAT;
            printf(RED "Invalid number of seat\n" RESET);
            continue;
        }
        aux = reserve_seat(c, movieID, seat);
        if( aux == SEAT_TAKEN ){
            printf(YELLOW "Sorry, that seat is already taken."
                    " Please try with another one.\n" RESET);
        }else if(aux == INVALID_SEAT){
            printf("Error: Invalid seat number\n");
        }
    }
    printf(GREEN "The purchase has been successful\n" RESET);
}

void
list_movies(void){
    int code = 1, i = 0;
    Matrix movies;
    movies = get_movies_list();
    if(movies.ret != 0){
        switch( movies.ret ){
            case -1:
                printf(RED "Error: movie list not found in database.\n" RESET);
                break;
            case -2:
                printf(RED "Error reading from file\n" RESET); 
                break;
            default:
                printf("ERROR: ret = %d\n", movies.ret);
        }
        return;
    }
    printf("------------------------------------------------------------\n");
    for(i = 0; i < 10; i++){
        printf("%s\t\tCode:%d\n",movies.m[i],code++);
    }
    printf("------------------------------------------------------------\n");
}

void
printSeats(char seats[][MAX_LENGTH]){
    int row, col, seatNumber;
    for(row = 0; row < ROWS; row++){
        for(col = 0; col < COLS; col++){
            if( strcmp(seats[seatNumber = row*10 + col], "\0") == 0 ){
                printf("%4d", seatNumber+1);
            }else{
                printf(YELLOW "%4s" RESET, "X");
            }
        }
        printf("\n");
    } 
}
