#ifndef FILELOCK_H
#define FILELOCK_H

void buy_ticket(void);
int rdlockFile(int fd);
int wrlockFile(int fd);
int unlockFile(int fd);

#endif

