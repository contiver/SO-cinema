#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "../include/files-signals.h"
#include "../../common/shared.h"
#include "../../common/dbAccess.h"
#include "../../common/ipc.h"

Response execRequest(Request r);

struct sigaction sig;
Request req;
Response res;

int main(void){

	long p = (long)getpid();

   	FILE *file = fopen(SERVER_FILE_PID, "wb");
    	if ( file == NULL ){
            printf("error while creating server_pid file\n");
            exit(1);
    	}
	if( fwrite(&p, sizeof(p), 1, file) != 1 ){
            printf("error while writing server_pid file\n");
            exit(1);
     	}
     	fclose(file);

	initialize_sigactions();

	while(1){

		if(sigaction(SIGUSR1,&sig,NULL)==-1){
			printf("error\n");
		}

	}

}

void sig_handler(void){
	sigset_t signal_set;
	sigemptyset(&signal_set);
	sigaddset(&signal_set,SIGUSR1);
	sigaddset(&signal_set,SIGUSR2);

	printf("in signal handler\n");
	//bloquear las demas seniales
	if(sigprocmask(SIG_BLOCK,&signal_set,NULL)==-1){
		printf("error blocking signals\n");
	}

	//llamar a read messages hasta que este vacio

	//while(
	read_client_messages();
		//si devuelve cero es que no hay mensajes para leer
	//};
	printf("con seniales bloqueadas\n");
	sleep(20);

	//desbloquear las seniales

	if(sigprocmask(SIG_UNBLOCK,&signal_set,NULL)==-1){
		printf("error unblocking signals\n");
	}
	printf("seniales desbloqueadas\n");
}

void read_client_messages(void){
	char clientFile[CLIENT_FILE_NAME_LEN];

	printf("reading messages\n");

	DIR *dirp;
	struct dirent *direntp;
	if((dirp= opendir(CTOS_PATH))==NULL){
		printf("error opening ctos directory\n");
	}
	while((direntp=readdir(dirp))!=NULL){
		if(direntp->d_name[0] != '.'){
			//decidir que comando tiene que ejecutar
			printf("ejecutar comando\n");
			
			strncpy(clientFile,CTOS_PATH,CLIENT_FILE_NAME_LEN);
			strcat(clientFile,"/");
			strcat(clientFile,direntp->d_name);

			printf("%s\n",clientFile);

			FILE * file=fopen(clientFile,"rb");
			if(file==NULL){
				printf("error while opening %s file\n",clientFile);
				//fatal("fopen");
			}
			else{
				if(fread(&req,sizeof(Request),1,file)==-1){
					printf("error while reading from %s file\n",clientFile);
				}
				printf("%d movie id\n",req.movieID);
				fclose(file);
				//res= execRequest(req);
			}
			//luego borrar el archivo que acaba de leer
		}
	}
	closedir(dirp);
}

void initialize_sigactions(void){
	//no signals masked
	sigemptyset(&sig.sa_mask);
	sig.sa_flags=0;
	sig.sa_handler=sig_handler;
}


Response execRequest(Request r){
    Response resp;

    switch(r.comm){
        case RESERVE_SEAT:
            resp.ret = reserve_seat(r.client, r.movieID, r.seat);
            break;
        case CANCEL_SEAT:
            resp.ret = cancel_seat(r.client, r.movieID, r.seat);
            break;
        case GET_MOVIE:
            resp.m = get_movie(r.movieID);
            break;
        case MOVIE_LIST:
            //TODO IMPLEMENTAR
            break;
        case TEST_CONNECTION:
            resp.ret = 0;
            break;
    }
    return resp;
}
