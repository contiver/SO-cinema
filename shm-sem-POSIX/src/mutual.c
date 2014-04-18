#include "common.h"
 
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
 
#define SIZE 1000
 
void fatal(char *s);
char *getmem(void);
void initmutex(void);
void enter(void);
void leave(void);

void
fatal(char *s)
{
    perror(s);
    exit(1);
}
 
char *
getmem(void)
{
    int fd;
    char *mem;
     
    if ( (fd = shm_open("/message", O_RDWR|O_CREAT, 0666)) == -1  )
        fatal("sh_open");
    ftruncate(fd, SIZE);
    if ( !(mem = mmap(NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0))  )
        fatal("mmap");
    close(fd);
    return mem;
}
 
static sem_t *sd;
 
void
initmutex(void)
{
    if ( !(sd = sem_open("/mutex", O_RDWR|O_CREAT, 0666, 1))  )
        fatal("sem_open");
}
 
void
enter(void)
{
    sem_wait(sd);
}
 
void
leave(void)
{
    sem_post(sd);
}
