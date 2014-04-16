
#define CLIENT_FIFO_SIZE        51
#define CLIENT_FIFO_TEMPLATE    "/tmp/fifo_cl.%ld"
#define SERVER_PATHNAME         "/tmp/fifo_sv"


typedef struct  {
    pid_t pid;
    command com;
    Client client;
    int movieID;
    int seat;
} Request;

typedef struct  {
    int ret;  // Value to interpret depending of what the client requested
    Movie m;
} Response;
