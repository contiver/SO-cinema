
#define CLIENT_FILE_TEMPLATE "../ctos/client_file.%ld"
#define CLIENT_FILE_NAME_LEN (sizeof(CLIENT_FILE_TEMPLATE) + 20)
#define SERVER_FILE_PID	"../stoc/server_pid"
#define CTOS_PATH "../ctos"
#define STOC_PATH "../stoc"


void communicate(void);
void fatal(char *s);

void initialize_sigactions(void);

void read_client_messages(void);
void sig_handler(void);
void create_client_file(void);
void onSigInt(int sig);

//Response execRequest(Request r);
