#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../include/files-signals.h"
#include "../../common/shared.h"
#include "../../common/dbAccess.h"

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
	return 0;
}
