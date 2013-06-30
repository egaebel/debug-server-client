#include <pthread.h>
#include "pi_client.h"

int main(int argc, char * argv[]) {

	if (argc == 3) {

		printf("Running client on server: %s on port %s\n", argv[1], argv[2]);
		run_client_args(argv[1], atoi(argv[2]));
	}
	else {

		printf("Running client on default server settings!\n");
	}
	print_from_server();
	return 1;
}
