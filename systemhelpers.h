#ifndef SYSTEM_HELPERS_H_
#define SYSTEM_HELPERS_H_

#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>

//Socket related
void Setsockopt(int fd, int level, int optname);
int Socket(int domain, int type, int protocol);
void Listen (int sock_fd, int queue_length);
void Bind(int sock_fd, struct sockaddr * addr, int addr_len);
int Accept(int sockfd, struct sockaddr * addr, unsigned int * addrlen);
void Connect(int sockfd, const struct sockaddr * addr, socklen_t addrlen);
void Getaddrinfo(const char * node, const char * service, const struct addrinfo * hints, struct addrinfo ** res);
//general system
int Dup2(int fd, int fd2);
void Execve(char *filename, char * argv[], char * envp[]);
//files/fds open/close
int Close(int fd);
void Fclose(FILE * file);
FILE * Fopen(const char * filename, const char * mode);
int Open(const char *pathname, int flags, mode_t mode);

#endif
