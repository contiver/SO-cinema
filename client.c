#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include "client.h"
#include "fileLockImpl.h"

#define COMMAND_SIZE 4

Client login(void);
void buy_movie(Client c);
void initialize_rwflock(struct flock * rwlock);
void finish_rwlock(struct flock * rwlock);
void execute_command(char * command, Client c);
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
        printf("ListMovies\nBuyMovie\nChooseSeat\nCancelMovie\nExit\n\n");
        scanf("%s",command);
        execute_command(command,c);
    }
}

void execute_command(char * command, Client c){
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
    int movieID;
    char movieName[MAX_NAME_LENGTH];
    int seat;
    printf("Insert movie code:\n");
    scanf("%d", &movieID);
    sprintf(movieName, "movie_%d", movieID);

    MovieFile movie_file=buy_ticket(movieName);
    if(movie_file.movie.th.seats_left==0){
         printf("Sorry, this movie is full\n");
         //llamar a unlock
    }
    else{
        printf("Please choose a seat\n");
        printSeats(movie_file.movie.th.seats);
        scanf("%d",&seat);
    }
    //al terminar unlockear en el back
    unlockFile(movie_file.fd);
}
void list_movies(void){
 
    char movies[10][60];
    int i=0;
    FILE * file = fopen("movie_list", "rb");
    if( file == NULL ){
        printf("Invalid movie code: not found in database\n");
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
        printf("%s\n",movies[i]);
    }
    for(i=0; i<60; i++){
        printf("-");
    }
    printf("\n");
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
    int rows, cols, seatNumber;
    for(rows = 0; rows < 6; rows++){
        for(cols = 0; cols < 10; cols++){
            if(strcmp(seats[seatNumber = rows*10 + cols],"")==0){
                printf("%4d", seatNumber+1);
            }else{
                printf("%.4s", "X");
            }
        }
        printf("\n");
   } 
}
