#include "string_node.h"

/* Takes a message and a message_length (which excludes the null terminator). 
 * Allocates a string_node struct, as well as the string contained in it.
 * Copies the message into the node.
 * Returns NULL on error, returns the allocated string_node on success.
 */
string_node * alloc_string_node(char * message, int message_length) {

	if (!message || message_length <= 0)
		return NULL;

	string_node * node = malloc(sizeof(string_node));
	if (!node)
		return NULL;

	node->str = malloc(message_length + 1);
	if (!node->str)
		return NULL;

	if (strlen(message) != message_length)
		return NULL;
	strncpy(node->str, message, message_length + 1);

	node->str_length = message_length;

	init_qnode(&node->node);

	return node;
}

/* Takes a string_node struct and frees it. */
void free_string_node(string_node * node) {

	if (!node->str)
		return;
	free(node->str);

	if (!node)
		return;
	free(node);
}