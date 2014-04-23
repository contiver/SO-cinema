// Implementar el backend que funcionara con el front ya creado
//cuando el servidor atrapa una senial lo primero que hace es bloquearla,
//luego atiende la senial, osea va a leer TODOS los mensajes que hayan en el directorio
//y luego desbloquea la senial y se queda esperando otra  (enmascarar)
//para que los clientes conozcan el pid del servidor, hacer un archivo que contenga su pid
//para mandar las senales funcion kill
//para leer los directorios readdir, opendir

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <signal.h>
#include "../../common/dbAccess.h"
#include "../../common/shared.h"
#include "../include/files-signals.h"
#include "../../common/clientback.h"
#include "../../common/ipc.h"

static Request req;
static Response resp;
static char clientFile[CLIENT_FILE_NAME_LEN];
int server_int=0;

struct sigaction sig;

void
initializeClient(){
    req.pid = (long)getpid();

	snprintf(clientFile, CLIENT_FILE_NAME_LEN, CLIENT_FILE_TEMPLATE,
                (long) getpid());
	signal(SIGINT, onSigInt);

	//no signals masked
	sigemptyset(&sig.sa_mask);
	sig.sa_flags=0;
	sig.sa_handler=sig_usr2_handler;

}


void sig_usr2_handler(void){

	server_int=1;

	sigset_t signal_set;
	sigemptyset(&signal_set);
	sigaddset(&signal_set,SIGUSR1);
	sigaddset(&signal_set,SIGUSR2);

	if(sigprocmask(SIG_BLOCK,&signal_set,NULL)==-1){
		printf("error blocking signals\n");
		fatal("block");s
	}

	snprintf(clientFile, SERVER_FILE_NAME_LEN, SERVER_FILE_TEMPLATE,
                (long) getpid());

	FILE * file=fopen(clientFile,"rb");
	if(file==NULL){
		printf("error while opening %s file\n",clientFile);				
		fatal("fopen");
	}
	else{
		if(fread(&resp,sizeof(Response),1,file)==-1){
			printf("error while reading from %s file\n",clientFile);
			fatal("fread");
		}

		fclose(file);
		
		if(remove(clientFile)==-1){
			printf("error while removing %s file\n",clientFile);
			fatal("remove");
		}
	}
	
	if(sigprocmask(SIG_UNBLOCK,&signal_set,NULL)==-1){
		printf("error unblocking signals\n");
		fatal("unblock");
	}

}

void
terminateClient(void){
    int exit_status = EXIT_SUCCESS;
    exit(exit_status);
}

void onSigInt(int sig){
    terminateClient();
}


int
cancel_seat(Client c, int movieID, int seat){
 	req.comm = CANCEL_SEAT;
    req.movieID = movieID;
    req.seat = seat;

	create_client_file();
    communicate();
	sigaction(SIGUSR2,&sig,NULL);
	
	while(!server_int){
	}

	server_int=0;
    return resp.ret;
}


int
get_movies_list(char *movies[10][60]){
	return 0; // IMPLEMENTAR !!
}

Movie
get_movie(int movieID){

	req.comm = GET_MOVIE;
    req.movieID = movieID;

	create_client_file();
    communicate();
	sigaction(SIGUSR2,&sig,NULL);
	
	while(!server_int){
	}

	server_int=0;
    return resp.m;
}

void create_client_file(void){

	snprintf(clientFile, CLIENT_FILE_NAME_LEN, CLIENT_FILE_TEMPLATE,
                (long) getpid());
	FILE *file=fopen(clientFile, "wb");
	if(file==NULL){
		printf("error while creating %s file\n",clientFile);
		fatal("fopen");
	}
	if(fwrite(&req,sizeof(Request),1,file)==-1){
		printf("error while writing %s file\n",clientFile);
		fatal("fwrite");
	}
	fclose(file);
}


void communicate(void){
	int s;
	long serverpid;

	FILE *file = fopen(SERVER_FILE_PID, "rb");
   	if ( file == NULL ){
		printf("error while opening server_pid file\n");
        fatal("fopen");
    }
	fread(&serverpid,sizeof(serverpid),1,file);
	fclose(file);

	s= kill(serverpid, SIGUSR1);

	if(s==-1){
		if(errno==EPERM)
			printf("Server process exists, but we dont have permission to send it a signal\n");
		else if(errno==ESRCH)
			printf("Server process does not exist\n");
		fatal("kill");
	}
	if(s==0){
		//el proceso existe y le podemos mandar una senial
	}
}

int
reserve_seat(Client c, int movieID, int seat){
    req.comm = RESERVE_SEAT;
    req.client = c;
    req.movieID = movieID;
    req.seat = seat;

	create_client_file();
    communicate();

	sigaction(SIGUSR2,&sig,NULL);
	while(!server_int){
	}

	server_int=0;
    return resp.ret;
}

void
fatal(char *s){
    perror(s);
    exit(EXIT_FAILURE);
}
