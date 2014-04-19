#ifndef MUTUAL_H
#define MUTUAL_H

#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include "../../common/ipc.h"
 
#define SIZE sizeof(Request)
 
void fatal(char *s);
Request *getmem(void);
void initmutex(void);
void enter1(void);
void enter2(void);
void enter3(void);
void leave1(void);
void leave2(void);
void leave3(void);

#endif
