#include "systemhelpers.h"

/**************WRAPPER FUNCTIONS**********************/
/* error handling wrapper function for getaddrinfo */
void Getaddrinfo(const char * node, const char * service, const struct addrinfo * hints, struct addrinfo ** res) {

	int error;
	if ((error = getaddrinfo(node, service, hints, res)) != 0) 
		fprintf(stderr, "Failed to getaddrinfo. Produced ERROR: %s\n", gai_strerror(error)), exit(-1);
}

/* Error handling wrapper function for socket call */
int Socket(int domain, int type, int protocol) {
	
	int listen_fd;
	if ((listen_fd = socket(domain, type, protocol)) < 0)
		fprintf(stderr, "Failed to create socket, ERROR: %s\n", strerror(errno));
	return listen_fd;
}

/* Error handling wrapper function for setsockopt. */
void Setsockopt(int fd, int level, int optname) {

	int optval = 1;
	if (setsockopt(fd, level, optname, (const void *)&optval, sizeof(int)) < 0)
		fprintf(stderr, "Failed to set socket options for socket with descriptor %d produces ERROR: %s\n", fd, strerror(errno)), exit(-1);
}

/* Error handling wrapper function for Bind. */
void Bind(int sock_fd, struct sockaddr * addr, int addr_len) {

	if (bind(sock_fd, addr, addr_len) < 0) {
		
		Close(sock_fd);
		fprintf(stderr, "Failed to bind socket with descriptor %d produces ERROR: %s\n", sock_fd, strerror(errno));
		exit(-1);
	}
}

/* Error handling wrapper function for listen. */
void Listen (int sock_fd, int queue_length) {

	if (listen(sock_fd, queue_length) < 0)
		fprintf(stderr, "Failed to start listening on socket with descriptor %d produces ERROR: %s\n", sock_fd, strerror(errno)), exit(-1);
}

/* Error handling wrapper function for Connect. Exits on failure. */
void Connect(int sockfd, const struct sockaddr * addr, socklen_t addrlen) {

	if (connect(sockfd, addr, addrlen) != 0) 
		fprintf(stderr, "Connect on sockfd %d failed. produces ERROR: %s\n", sockfd, strerror(errno)), exit(-1);
}

/* Error handling wrapper function for accept. */
int Accept(int sockfd, struct sockaddr * addr, unsigned int * addrlen) {
	
	int connected_fd = accept(sockfd, addr, addrlen);
	if(connected_fd < 0){
		fprintf(stderr, "Accept arrived upon an error, with sockfd: %d.\n", sockfd);
		exit(-1);
	}
	else {
		return connected_fd;
	}
}

/* Error handling wrapper function for dup2. */
int Dup2(int fd, int fd2) {
	int new_fd = dup2(fd, fd2);
	if(new_fd < 0){
		fprintf(stderr, "Failed to dup2(%d, %d).\n", fd, fd2);
		exit(-1);
	}
	else {
		return new_fd;
	}

}

/* Error handling wrapper function for execve. */
void Execve(char *filename, char * argv[], char * envp[]){
	if(execve(filename, argv, envp) < 0){
		fprintf(stderr, "Failed to execve on given character arrays, for file %s.\n", filename);
		exit(-1);
	}
}

int Open(const char *pathname, int flags, mode_t mode){
	int new_fd = open(pathname, flags, mode);
	if(new_fd == -1){
		fprintf(stderr, "Failed to open on flags %d.\n", flags);
	}
	return new_fd;
}

int Close(int fd){
	int closed_descriptor = close(fd);
	if(closed_descriptor == -1){
		fprintf(stderr, "Close on file descriptor %d failed.\n", fd);
		exit(-1);
	}
	return closed_descriptor;
}

/* Error handling wrapper function that handles errors for fopen system call.*/
FILE * Fopen(const char * filename, const char * mode) {

	FILE * file;
	if ((file = fopen(filename, mode)) == NULL)
		fprintf(stderr, "Filename: %s could not be opened! StrError: %s\n", filename, strerror(errno)), exit(-1);

	return file;
}

void Fclose(FILE * file) {

	if (fclose(file) != 0) 
		fprintf(stderr, "File could not be closed! StrError: %s\n", strerror(errno)), exit(-1);
}
