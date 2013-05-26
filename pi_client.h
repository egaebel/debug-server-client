#ifndef PI_CLIENT_H_
#define PI_CLIENT_H_

#include "systemhelpers.h"
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>

void * run_client ();
void * run_client_args(int port, char * server);
int print_from_server();

#endif