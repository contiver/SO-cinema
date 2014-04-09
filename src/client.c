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
void buy_movie(Client c);
void execute_command(char *command, Client c);
int check_command(char * com);
void printSeats(char seats[][MAX_LENGTH]);
void list_movies(void);

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

void execute_command(char *command, Client c){
    int ans=check_command(command);
    if(ans==-1){
        printf("Invalid command\n");
    }
    else{
        switch(ans){
            case 0:
                    list_movies(); break;
            case 1:
                    buy_movie(c); break;
            case 2:
                    // cancel_ticket(); break;
            case 3:
                    exit(1);
        }
    }
}

void buy_movie(Client c){
    int movieID, seat, aux=-1;
    char movieName[MAX_NAME_LENGTH];
    printf("Insert movie code:\n");
    scanf("%d", &movieID);
    
    sprintf(movieName, "./src/database/movie_%d", movieID);

    MovieFile movie_file = buy_ticket(movieName);
    if(movie_file.movie.th.seats_left==0){
         printf("Sorry, this movie is full\n");
         //sale y va a unlock
            //falta ver donde cerrar el archivo
    }
    else{
        while(aux!=0){
            printf("Please choose a seat\n");
            printf("%s %s\n",movie_file.movie.name, movie_file.movie.time);
            printSeats(movie_file.movie.th.seats);
            scanf("%d",&seat);
            aux=buy_seat(seat,c,movie_file);
            if(aux==-1){
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
    unlockFile(movie_file.fd);
}
void list_movies(void){
    int code=1;
    char movies[10][60];
    int i=0;
    FILE * file = fopen("./src/database/movie_list", "rb");
    if( file == NULL ){
        printf("ListMovies file not found in database\n");
        return;
    }
    if( fread(&movies, sizeof(movies), 1, file) != 1){
        printf("Error reading from file\n"); 
        return;
    }
    for(i=0; i<60; i++){
        printf("-");
    }
    printf("\n");
    for(i=0; i<10; i++){
        printf("%s\t\tCode:%d\n",movies[i],code++);
    }
    for(i=0; i<60; i++){
        printf("-");
    }
    printf("\n");
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

void cancelPurchase(){
    /* TODO */
    return;
}

void printSeats(char seats[][MAX_LENGTH]){
    int row, col, seatNumber;
    for(row = 0; row < ROWS; row++){
        for(col = 0; col < COLS; col++){
            if(strcmp(seats[seatNumber = row*10 + col],"")==0){
                printf("%4d", seatNumber+1);
            }else{
                printf("%4s", "X");
            }
        }
        printf("\n");
   } 
}
