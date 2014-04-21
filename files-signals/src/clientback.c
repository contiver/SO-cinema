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

	char clientFile[MAX_NAME_LENGTH];
	snprintf(clientFile, CLIENT_FILE_NAME_LEN, CLIENT_FILE_TEMPLATE,
                (long) getpid());

   	FILE *file = fopen(clientFile, "wb");
    	if ( file == NULL ){
            printf("error while creating %s file\n",clientFile);
            exit(1);
    }

}

void
terminateClient(void){
return;
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
return m;
}

