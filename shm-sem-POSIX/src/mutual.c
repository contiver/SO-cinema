#include "mutual.h"
#include <stdio.h>
#include <signal.h>
#include <errno.h>

static sem_t *s1 = NULL;
static sem_t *s2 = NULL;
static sem_t *s3 = NULL;

void
fatal(char *s)
{
    perror(s);
    exit(1);
}
 
void *
getmem(void)
{
    int fd;
    Request *mem;
     
    if ( (fd = shm_open("/message", O_RDWR|O_CREAT, 0666)) == -1  )
        fatal("sh_open");
    if ( ftruncate(fd, SIZE) == -1){
        printf("Error during ftrunctate\n");
        exit(1);
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
onSigInt(int sig){
    sem_unlink("/mutex1");
    sem_unlink("/mutex2");
    sem_unlink("/mutex3");
    exit(1);
}


 
void
initmutex(void)
{
    signal(SIGINT, onSigInt);

    if( !(s1 = sem_open("/mutex1", O_RDWR|O_CREAT, 0666, 1)) )
        fatal("sem_open");
    if ( !(s2 = sem_open("/mutex2", O_RDWR|O_CREAT, 0666, 1)) )
        fatal("sem_open");
    if ( !(s3 = sem_open("/mutex3", O_RDWR|O_CREAT, 0666, 1)) )
        fatal("sem_open");
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
