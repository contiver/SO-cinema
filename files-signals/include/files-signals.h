
#define CLIENT_FILE_TEMPLATE "../ctos/client_file.%ld"
#define SERVER_FILE_TEMPLATE "../stoc/client_file.%ld"
#define CLIENT_FILE_NAME_LEN (sizeof(CLIENT_FILE_TEMPLATE) + 20)
#define SERVER_FILE_NAME_LEN (sizeof(SERVER_FILE_TEMPLATE) + 20)
#define SERVER_FILE_PID	"../stoc/server_pid"
#define CTOS_PATH "../ctos"
#define STOC_PATH "../stoc"


void communicate(void);
void fatal(char *s);


void read_client_messages(void);
void sig_usr1_handler(void);

void sig_usr2_handler(void);
void create_client_file(void);
void onSigInt(int sig);
void get_answer(void);

void server_communicate(long clientpid);
void create_server_file(char * clientFile);

//Response execRequest(Request r);
