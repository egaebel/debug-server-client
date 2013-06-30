#include "pi_client.h"

//Constants
static const int MAX_BUF_LENGTH = 8192;
static const int SERVER_PORT_NUM = 19484;
static const char * STR_SERVER_PORT_NUM = "19484";
static char * SERVER_IP = "127.0.0.1";

//Variables
static int server_sock = -1;
static bool is_init = false;

//Functions
/* Setup a client fd, return -1 on error. */
static int init_client(char * server, int portnum) {

	struct sockaddr * saddr;
	struct addrinfo hints, * ai,  * it;
	int sockfd = -1;
	size_t addrlen;
	char strportnum[25];

	memset(&hints, '\0', sizeof(hints));
	hints.ai_flags = AI_ADDRCONFIG;
	hints.ai_socktype = SOCK_STREAM;

	if (strncmp(server, "0", 1) == 0)
		server = SERVER_IP;

	if (snprintf(strportnum, 10, "%d", portnum) < 5)
		strncpy(strportnum, STR_SERVER_PORT_NUM, 5);

	Getaddrinfo(server, strportnum, &hints, &ai);

	for (it = ai; it != NULL; it = it->ai_next) {

		if ((sockfd = Socket(it->ai_family, it->ai_socktype, it->ai_protocol)) != -1) {

			saddr = ai->ai_addr;
			addrlen = ai->ai_addrlen;

			Connect(sockfd, saddr, addrlen);

			break;			 
		}
	}

	freeaddrinfo(ai);

	return sockfd;
}

/* Getter for the is_init bool. Indicates client is setup.*/
bool is_client_init() {

	return is_init;
}

/* Starts the client with default values. 
 * Returns a number > 0 on success, -1 on fail.
 */
void * run_client(int argc, char * argv[]){

	int port;
	char * server;
		
	if (argc == 2) {
	
		server = argv[0];
		port = atoi(argv[1]);
	}
	//1 argument is for server ip
	else if (argc == 1) {
		
		server = argv[0]	
	}
	else {

		port = SERVER_PORT_NUM;
		server = SERVER_IP;
	}

	server_sock = init_client(server, port);

	is_init = true;
	return &server_sock;
}

/* Starts the client with passed values.
 * Returns a number > 0 on success, -1 on fail.
 */
void * run_client_args(int port, char * server) {

	server_sock = init_client(server, port);

	return &server_sock;
}

static void send_request_header(int fd, char * message) {

	char buf[MAX_BUF_LENGTH];

	snprintf(buf, MAX_BUF_LENGTH, "GET %s HTTP/1.1", message);

	write(fd, buf, sizeof(buf));
}

/* 
 * Fetches debug messages from the server.
 * Returns number of bytes read on success, -1 on fail.
 */
int print_from_server() {

	char buffer[MAX_BUF_LENGTH];
	int chars_read = 0;
	int return_val = 1;

	if (server_sock != -1) {

		send_request_header(server_sock, "/get_messages");

		while ((return_val = read(server_sock, buffer, MAX_BUF_LENGTH)) > 0) {
			
			//Skip over printing the header
			if (strstr(buffer, "Server: Pi Web Server"))
				continue;

			chars_read += return_val;
			printf("%s", buffer);
			buffer[0] = '\0';
		}
	}
	else  {
		printf("SERVER SOCK FAILURE\n");
	}

	return chars_read;
}

void kill_client() {

	if (server_sock > 0)
		Close(server_sock);
}
