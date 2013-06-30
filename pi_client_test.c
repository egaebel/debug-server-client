#include <pthread.h>
#include "pi_client.h"

int main(int argc, char * argv[]) {

	run_client(argc, argv);
	print_from_server();
	return 1;
}
