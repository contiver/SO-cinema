#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../../common/ipc.h"
#include "../../common/shared.h"
#include "../../common/dbAccess.h"
#include "../../common/server.h"
#include "mutual.h"

int
main(void){
    Request *req;
    Response *resp;
    req = (Request *)getmem();
    memset(req, 0, SHM_SIZE);
    resp = (Response *)req;

    initmutex();
    signal(SIGINT, onSigInt);

    for(;;){
        enter(2);
        *resp = execRequest(*req);
        leave(3);
    }
    return 0;
}

void
onSigInt(int sig){
    terminateServer();
}
