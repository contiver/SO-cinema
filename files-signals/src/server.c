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
Response resp;

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

	//signal(SIGINT,
	

	//no signals masked
	sigemptyset(&sig.sa_mask);
	sig.sa_flags=0;
	sig.sa_handler=sig_usr1_handler;
	sigaction(SIGUSR1,&sig,NULL);

	while(1){
	}

}

void sig_usr1_handler(void){
	sigset_t signal_set;
	sigemptyset(&signal_set);
	sigaddset(&signal_set,SIGUSR1);
	sigaddset(&signal_set,SIGUSR2);

	if(sigprocmask(SIG_BLOCK,&signal_set,NULL)==-1){
		printf("error blocking signals\n");
	}

	read_client_messages();
	
	if(sigprocmask(SIG_UNBLOCK,&signal_set,NULL)==-1){
		printf("error unblocking signals\n");
	}
}

void read_client_messages(void){
	
	char clientFile[CLIENT_FILE_NAME_LEN];

	DIR *dirp;
	struct dirent *direntp;
	if((dirp= opendir(CTOS_PATH))==NULL){
		printf("error opening ctos directory\n");
	}
	while((direntp=readdir(dirp))!=NULL){
		if(direntp->d_name[0] != '.'){
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
				if(remove(clientFile)==-1){
					printf("error while removing %s file\n",clientFile);
				}
				resp= execRequest(req);

				strncpy(clientFile,STOC_PATH,CLIENT_FILE_NAME_LEN);
				strcat(clientFile,"/");
				strcat(clientFile,direntp->d_name);
				printf("%s\n",clientFile);

				create_server_file(clientFile);
				server_communicate(req.pid);

			}
		}
	}
	closedir(dirp);
}

void server_communicate(long clientpid){
	int s;

	s= kill(clientpid, SIGUSR2);

	if(s==-1){
		if(errno==EPERM)
			printf("Server process exists, but we dont have permission to send it a signal\n");
		else if(errno==ESRCH)
			printf("Server process does not exist\n");
		//fatal("kill");
	}
	if(s==0){
		//el proceso existe y le podemos mandar una senial
	}

}

void create_server_file(char * clientFile){
	FILE *file=fopen(clientFile, "wb");
	if(file==NULL){
		printf("error while creating %s file\n",clientFile);
		//fatal("fopen");
	}
	if(fwrite(&resp,sizeof(Response),1,file)==-1){
		printf("error while writing %s file\n",clientFile);
		//fatal("fwrite");
	}
	fclose(file);
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
