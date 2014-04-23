#include "mutual.h"
#include <stdio.h>
#include <signal.h>
#include <errno.h>


static sem_t *s1 = NULL;
static sem_t *s2 = NULL;
static sem_t *s3 = NULL;

void
fatal(char *s){
    perror(s);
    exit(EXIT_FAILURE);
}


void *
getmem(void){
    int fd;
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
    return mem;
}

void
initmutex(void){
    if( !(s1 = sem_open("/mutex1", O_RDWR|O_CREAT, 0666, 1)) )
        fatal("sem_open");
    if ( !(s2 = sem_open("/mutex2", O_RDWR|O_CREAT, 0666, 1)) )
        fatal("sem_open");
    if ( !(s3 = sem_open("/mutex3", O_RDWR|O_CREAT, 0666, 1)) )
        fatal("sem_open");
}

void
terminateServer(void){
    int exit_status = EXIT_SUCCESS;
    if(sem_close(s1) != 0) exit_status = EXIT_FAILURE;
    if(sem_close(s2) != 0) exit_status = EXIT_FAILURE;
    if(sem_close(s3) != 0) exit_status = EXIT_FAILURE;
    if(sem_unlink("/mutex1") != 0); exit_status = EXIT_FAILURE;
    if(sem_unlink("/mutex2") != 0); exit_status = EXIT_FAILURE;
    if(sem_unlink("/mutex3") != 0); exit_status = EXIT_FAILURE;
    exit(exit_status);
}

void
terminateClient(void){
    int exit_status = EXIT_SUCCESS;
    if(sem_close(s1) != 0) exit_status = EXIT_FAILURE;
    if(sem_close(s2) != 0) exit_status = EXIT_FAILURE;
    if(sem_close(s3) != 0) exit_status = EXIT_FAILURE;
    exit(exit_status);
}

void
enter1(void)
{
    sem_wait(s1);
}
 
void
leave1(void)
{
    sem_post(s1);
}
void
enter2(void)
{
    sem_wait(s2);
}
 
void
leave2(void)
{
    sem_post(s2);
}
void
enter3(void)
{
    sem_wait(s3);
}
 
void
leave3(void)
{
    sem_post(s3);
}
