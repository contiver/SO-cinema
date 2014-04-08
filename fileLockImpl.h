#ifndef FILELOCK_H
#define FILELOCK_H

int rdlockFile(int fd);
int wrlockFile(int fd);
int unlockFile(int fd);

#endif

