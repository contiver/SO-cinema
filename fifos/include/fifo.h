#ifndef FIFO_H
#define FIFO_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../../common/dbAccess.h"
#include "../../common/ipc.h"

// Well-known name for server's FIFO
#define SERVER_FIFO "/tmp/cinema_sv"

#define CLIENT_FIFO_TEMPLATE "/tmp/client_fifo.%ld"

// Space for the FIFO pathname (+20 as a generous allowance for the PID)
#define CLIENT_FIFO_NAME_LEN (sizeof(CLIENT_FIFO_TEMPLATE) + 20)

#endif
