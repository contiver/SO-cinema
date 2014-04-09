#ifndef FILELOCK_H
#define FILELOCK_H
#include "client.h"

MovieFile buy_ticket(char movieName[MAX_NAME_LENGTH]);
int rdlockFile(int fd);
int wrlockFile(int fd);
int unlockFile(int fd);

#endif

