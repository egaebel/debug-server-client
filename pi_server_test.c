#include <pthread.h>
#include "pi_server.h"

void * add_test_messages() {

	while (!is_server_init())
		pthread_yield();

	char * str = "TESTING STRING##############################################\n";
	int str_length = strlen(str);

	add_message(str, str_length);
	add_message(str, str_length);
	add_message(str, str_length);
	add_message(str, str_length);

	return NULL;
}

int main (int argc, char * argv[]) {

	pthread_t server_thread;
	pthread_t msg_thread;

	int portnum;
	if (argc == 1) {
	
		portnum = 19484;
	}
	else if (argc == 2) {
	
		portnum = argv[1];
	}

	if (pthread_create(&server_thread, 0, run_server, (void *) &portnum) != 0)
		return -1;

	if (pthread_create(&msg_thread, 0, add_test_messages, (void *) NULL) != 0)
		return -1;

	pthread_join(msg_thread, NULL);
	pthread_join(server_thread, NULL);

	return 1;
}
