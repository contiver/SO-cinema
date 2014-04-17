#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../../common/dbAccess.h"

#define SERVER_FIFO "/tmp/cinema_sv" // Well-known name for server's FIFO
#define CLIENT_FIFO_TEMPLATE "/tmp/client_fifo.%ld"
// Space for the FIFO pathname (+20 as a generous allowance for the PID)
#define CLIENT_FIFO_NAME_LEN (sizeof(CLIENT_FIFO_TEMPLATE) + 20)

typedef enum{RESERVE_SEAT, CANCEL_SEAT, GET_MOVIE, MOVIE_LIST, TEST_CONNECTION} command;

typedef struct  {
    pid_t pid;   // client's pid
    command comm; // number representing a command
    Client client;
    int movieID;
    int seat;
} Request;

typedef struct  {
    int ret;  // Value to interpret depending of what the client requested
    Movie m;
} Response;

