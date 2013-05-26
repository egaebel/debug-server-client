#include <pthread.h>
#include "pi_client.h"

int main() {

	run_client();
	print_from_server();
	return 1;
}