#include "mutual.h"
#include <stdio.h>
#include <signal.h>
#include <errno.h>


Request *mem;
int memid;
int semid;

void
fatal(char *s){
    perror(s);
    exit(EXIT_FAILURE);
}


void *
getmem(void){

	if( (memid= shmget(IPC_PRIVATE,SIZE,IPC_CREAT | 0666)) ==-1)
		fatal("shmget");
	if( !(mem = shmat(memid,NULL,0)) )
		fatal("shmat");
	return mem;



/////
 /*   int fd;
    Request *mem;
     
    if ( (fd = shm_open("/message", O_RDWR|O_CREAT, 0666)) == -1  )
        fatal("sh_open");
    if ( ftruncate(fd, SIZE) == -1){
        printf("Error during ftrunctate\n");
        exit(EXIT_FAILURE);
    }
    mem = mmap(NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (mem == MAP_FAILED) 
        fatal("mmap");
    if( close(fd) ){
        printf("Error closing fd");
    }
    return mem;*/
}

void
initmutex(void){
    if( (semid = semget(IPC_PRIVATE,3,IPC_CREAT | 0666))==-1 )
		fatal("semget");
	semctl(semid,0,SETVAL,1);
	semctl(semid,1,SETVAL,0);
	semctl(semid,2,SETVAL,0);
}

void
terminateServer(void){
    int exit_status = EXIT_SUCCESS;
    if(shmdt(mem) !=0) exit_status = EXIT_FAILURE;
    if(shmctl(memid,IPC_RMID,NULL) != 0); exit_status = EXIT_FAILURE;
    exit(exit_status);
}

void
terminateClient(void){
    int exit_status = EXIT_SUCCESS;
	if(shmdt(mem) !=0) exit_status = EXIT_FAILURE;
    exit(exit_status);
}

void
enter(int sem_num)
{
    struct sembuf sb;
	
	sb.sem_num=sem_num-1;
	sb.sem_op=-1;
	sb.sem_flg=SEM_UNDO;
	if(semop(semid,&sb,1)==-1){
		fatal("semop");
	}
}
 
void
leave(int sem_num)
{
	struct sembuf sb;
	
	sb.sem_num=sem_num-1;
	sb.sem_op=1;
	sb.sem_flg=SEM_UNDO;
	if(semop(semid,&sb,1)==-1){
		fatal("semop");
	}
}
