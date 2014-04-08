#include "fileLockImpl.h"

//lock de write, nadie lo puede ni leer ni escribir   
//lock de read, lo pueden leer pero no lo pueden escribir   
int rdlockFile(int fd){
    struct flock fl = {.l_type = F_RDLCK, .l_start = 0,
                       .l_whence = SEEK_SET, .l_len = 0};
    return fcntl(fd, F_SETLKW, &fl);
}

int wrlockFile(int fd){
    struct flock fl = {.l_type = F_WRLCK, .l_start = 0,
                       .l_whence = SEEK_SET, .l_len = 0};
    return fcntl(fd, F_SETLKW, &fl);
}

int unlockFile(int fd){
    struct flock fl = {.l_type = F_UNLCK, .l_start = 0,
                       .l_whence = SEEK_SET, .l_len = 0};
    return fcntl(fd, F_SETLKW, &fl);
}
