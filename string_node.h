#ifndef STRING_NODE_H
#define STRING_NODE_H

#include <string.h>
#include <stdlib.h>
#include "linked_queue.h"

typedef struct {
	
	char * str;
	int str_length;
	QNode node;

} string_node;

string_node * alloc_string_node(char * message, int message_length);
void free_string_node(string_node * node);

#endif