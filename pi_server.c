/*
* Raspberry Pi Server
*/
#include "pi_server.h"

//Constants
static const int LISTEN_PORT_NUM = 55000;
static const int QUEUE_LENGTH = 1024;
static const int MAX_LENGTH = 8192;
static const int MAX_BUF = 8192;

//Variables
static Queue q;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static bool is_init = false;

/* Adds a message to the message queue. */
void add_message(char * message, int message_length) {

	pthread_mutex_lock(&mutex);

	if (!is_init || !message || message_length <= 0)
		return;

	string_node * snode = alloc_string_node(message, message_length);
	add_queue(&q, &(snode->node));
	
	pthread_mutex_unlock(&mutex);
}

/* main function */
void * run_server_args(int * port_number) {

	int port;

	if (!port_number)
		port = LISTEN_PORT_NUM;
	else
		port = *port_number;

	if (port <= 1024)
		port = LISTEN_PORT_NUM;

	init_queue(&q);

	struct sockaddr sock_addr;
	int listen_fd = init(port, &sock_addr);

	is_init = true;

	wait_for_listen(listen_fd, &sock_addr);
	
	return NULL;
}

/* main function, no arguments, uses default port number. */
void * run_server() {

	int port_number = LISTEN_PORT_NUM;
	return run_server_args(&port_number);
}

/* Getter for the is_init bool. Indicates server is setup and listening */
bool is_server_init() {

	return is_init;
}

/*
 * Set up threadpool and listening socket for normal listening server.
 * Set up threadpool, commandcenter, and listening socket.
 */
static int init(int portnum, struct sockaddr * sock_addr) {
	
	char strportnum[10];
	sprintf(strportnum, "%d", portnum);

	int listening_fd = -1;
	struct addrinfo * it;

	//Setup and run getaddrinfo
	struct addrinfo hints, * ai;
	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;

	Getaddrinfo(NULL, strportnum, &hints, &ai);
	
	//iterate over results of getaddrinfo and bind ipvX address
	for (it = ai; it != NULL; it = it->ai_next) {

		//check if this addrinfo has the correct family
		if (it->ai_family == AF_INET) {

			//begin listening
			sock_addr = it->ai_addr;
			listening_fd = init_listen(sock_addr);
		}
	}

	freeaddrinfo(ai);

	return listening_fd;
}

/* Helper function that initializes the listening socket for ipv4 addresses. */
static int init_listen(struct sockaddr * sock_addr) {
	
	//Set vars for sockopt depending on IP family
	int optname = SO_REUSEADDR;
	int level = SOL_SOCKET;

	int listen_fd = Socket(AF_INET, SOCK_STREAM, 0);

	Setsockopt(listen_fd, level, optname);

	Bind(listen_fd, sock_addr, sizeof(struct sockaddr_in));
	Listen(listen_fd, QUEUE_LENGTH);

	return listen_fd;
}

/* Wait for a new connection. Takes a wrapper struct of sockfd and sockaddr_in as argument */
static void wait_for_listen(int sockfd, struct sockaddr * addr) {

	unsigned int addrlen = sizeof(addr);
	int connected_fd;

	// Infinite loop for constant acceptance
	while(true){
		connected_fd = Accept(sockfd, addr, &addrlen);
		run(connected_fd);
	}

	Close(sockfd);
}

/*
* Receives requests and places the messages in a queue.
*/
static void run(int fd) {
	
	char buf[MAX_LENGTH], method[MAX_LENGTH], uri[MAX_LENGTH], version[MAX_LENGTH], error[MAX_LENGTH];
	char * args = NULL;

	//clear variables to prevent corruption
	memset(error, '\0', MAX_LENGTH);
	memset(uri, '\0', MAX_LENGTH);
	memset(buf, '\0', MAX_LENGTH);
	memset(version, '\0', MAX_LENGTH);
	memset(method, '\0', MAX_LENGTH);

	// Parse and perform requests if we haven't hit timeout and if the headers are still valid
	while (read(fd, buf, MAX_LENGTH) > 0) {

		sscanf(buf, "%s %s %s", method, uri, version);

		//Check for non GET method call
		if (strncasecmp(method, "GET", 3)) {

			snprintf(error, sizeof(error), "501 error: %s not supported The server does not support the method provided %s", method, version);
			write(fd, error, sizeof(error));
			Close(fd);
			return;
		}
		//check if version is HTTP 1.0, if so, break.
		if (strcmp(version, "HTTP/1.0") == 0) {
			fprintf(stderr, "NON HTTP 1.0 MESSAGE RECEIVED\n");
			break;
		}

		if ((args = strstr(uri, "/get_messages")))
			send_messages(fd, version);

		//clear variables to prevent corruption
		memset(error, '\0', MAX_LENGTH);
		memset(uri, '\0', MAX_LENGTH);
		memset(buf, '\0', MAX_LENGTH);
		memset(version, '\0', MAX_LENGTH);
		memset(method, '\0', MAX_LENGTH);
	}

	Close(fd);
}

static void send_messages(int fd, char * version) {

    char buf[MAX_BUF];
    char msg[MAX_LENGTH];
    int message_size = 0;
    QNode * qnode;
    string_node * snode;

    pthread_mutex_lock(&mutex);

    while(!is_empty(&q)) {

    	qnode = remove_queue(&q);
    	snode = QUEUE_ENTRY(qnode, string_node, node);

    	strncat(msg, snode->str, snode->str_length);

    	message_size += snode->str_length;

    	free_string_node(snode);
    }
 	
    // Send response headers to client
    sprintf(buf, "%s 200 OK\r\n", version);
    sprintf(buf, "%sServer: Pi Web Server\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n", buf, message_size);
    sprintf(buf, "%sContent-type: text/html\r\n\r\n", buf);
   
	write(fd, buf, sizeof(buf));
	write(fd, msg, sizeof(msg));

	pthread_mutex_unlock(&mutex);
}
