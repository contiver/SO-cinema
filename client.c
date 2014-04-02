#include <fcntl.h>
#include <stdbool.h>

#define DATABASE 
#define BUF_SIZE 512
#define FILEPATH 
#define STD_SEAT_QTY 60

typedef char* string;

string[] seats;

typedef struct{
    string email;
    string pw;
    string name;
} Client;

typedef struct{
    int id;
    string name;
    string time;
    Theatre th;
} Movie;

typedef struct{
	int number;
	int size;
    string seats[];
} Theatre;

//struct flock {
//    short l_type; /* Lock type: F_RDLCK, F_WRLCK, F_UNLCK */
//    short l_whence; /* How to interpret 'l_start': SEEK_SET, SEEK_CUR, SEEK_END */
//    off_t l_start; /* Offset where the lock begins */
//    off_t l_len; /* Number of bytes to lock; 0 means "until EOF" */
//    pid_t l_pid; /* Process preventing our lock (F_GETLK only) */
//};

struct flock rwlock;
rwlock.l_type    = F_WRLCK;
rwlock.l_start   = 0;
rwlock.l_whence  = SEEK_SET;
rwlock.l_len     = 0;


int main(void){
    string command;
    Client c = NULL;

    initializeTheatres();

    while( c == NULL ){
        c = login();
    }

    while(true){
        scanf("%s", command);
        if( execCommand(command) ) break;
   }
   return;
}

void initializeTheatres(void){
    string seat[STD_SEAT_QTY];
    int i;
    for(i = 0; i < STD_SEAT_QTY; i++){
        seat[i] = NULL;
    }
    Theatre t1 = {1, STD_SEAT_QTY, seat};
    /* TODO use write() syscall to save t1 variable to file */
}

Client login(void){
    string email, pw;

    printf("email:");
    scanf("%s", name);
    printf("\nPassword:");
    scanf("%s", pw);
    /* TODO Check values: if invalid, print error and return NULL */
    Client c = { email, pw, "DEFAULT_NAME"}
    return c;
}

int execCommand(string command){
    if( !strcmp("exit", command) || !strcmp("quit", command) ) return 1; 

    /* TODO Search command and execute it. On success return 0 */
    return 0; 
} 
    
void checkMovies(){ 
    int fd;
	ssize_t nread;
	char buffer[BUF_SIZE];

	if( (fd = open(DATABASE, O_RDONLY | 0644) == -1){
		exit(1);		
	}

	while((nread = read(fd, buffer, BUF_SIZE)) > 0){
		/* imprimir en pantallas las peliculas */	
	}
}

void buyTicket(void){
	string code;
    int fd, error = 0;

	printf("Insert movie code:");
	scanf("%s", code);
    if( (fd = open(FILEPATH, O_RDWR | 0644) == -1 ){
        printf("Invalid movie code: not found in database");
        return;
    }
    fcntl(fd, F_SETLK, &rwlock);

    if( (error = printSeats(code)) == -1){
        printf("Invalid code");
        return;    
    }else if(error == -2){
        printf("Sorry, Theatre is full");
        return;    
    }
    printf("Insert desired seats");
    scanf(/* completar */);
    chooseSeat(); /*recibe los asientos y trata de comprarlos */
}

void cancelPurchase(){
    /* TODO */
    return;
}
