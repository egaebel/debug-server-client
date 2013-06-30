#ifndef WEBSERVER_H_
#define WEBSERVER_H_

#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <signal.h>
#include <pthread.h>

#include "systemhelpers.h"
#include "string_node.h"

/* A struct wrapping up a string and its size */
typedef struct {
	char * str;
	int str_length;
} string_wrap;

typedef void (*sa_sigaction_t)(int signum, siginfo_t * info, void * stuff);

//Function Prototypes
void * run_server_args(void * port_number);
void * run_server();
void add_message(char * message, int message_length);

//Static Function Prototypes
static int init(int portnum, struct sockaddr * sock_addr);
static int init_listen(struct sockaddr * sock_addr);
static void wait_for_listen(int sockfd, struct sockaddr * addr);
static void run(int the_fd);
static void send_messages(int fd, char * version);

#endif
