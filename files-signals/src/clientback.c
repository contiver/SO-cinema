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

void
initializeClient(){
    req.pid = (long)getpid();

	snprintf(clientFile, CLIENT_FILE_NAME_LEN, CLIENT_FILE_TEMPLATE,
                (long) getpid());
	signal(SIGINT, onSigInt);

}

void
terminateClient(void){
   return;
}

void onSigInt(int sig){
    terminateClient();
}


int
cancel_seat(Client c, int movieID, int seat){
return 0;
}


int
reserve_seat(Client c, int movieID, int seat){
return 0;
}


int
get_movies_list(char *movies[10][60]){
	return 0; // IMPLEMENTAR !!
}

Movie
get_movie(int movieID){
	Movie m;

	req.comm = GET_MOVIE;
    req.movieID = movieID;

	create_client_file();
    communicate();

    return m;
}

void create_client_file(void){
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
        exit(1);
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
		printf("server process exists and we can send signal\n");
		//el proceso existe y le podemos mandar una senial
	}
}

void
fatal(char *s){
    perror(s);
    exit(EXIT_FAILURE);
}
