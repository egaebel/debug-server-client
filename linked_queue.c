#include "linked_queue.h"

//init_queue, init_qnode, add(from rear), remove(from front), isEmpty

//Functions--------------------------------------
//Initializes a Queue struct's sentinal nodes
void init_queue(Queue *q) {

	q->head.next = &(q->tail);
	q->tail.prev = &(q->head);

	q->head.prev = NULL;
	q->tail.next = NULL;
}

//Returns an initialized Queue struct
Queue* create_queue() {

	Queue *q = malloc(sizeof(Queue));
	init_queue(q);

	return q;
}

//Initializes a QNode to have all NULL ptrs
void init_qnode(QNode *node) {

	node->next = NULL;
	node->prev = NULL;
}

//Adds the passed in Qnode to the END of the passed in Queue
void add_queue(Queue *q, QNode *node) {

	node->next = &(q->tail);
	node->prev = q->tail.prev;

	q->tail.prev->next = node;
	q->tail.prev = node;
}

//Pops the element at the front of the queue off and returns it
//Return the removed node if successful
//Return NULL if the Queue is empty
QNode* remove_queue(Queue *q) {

	//if queue is empty
	if (is_empty(q)) {

		//ERROR
		return NULL;
	}
	else {

		QNode *temp = q->head.next;

		//new linkage
		q->head.next = q->head.next->next;
		q->head.next->prev = &(q->head);

		//kill temp's access to the list
		temp->prev = NULL;
		temp->next = NULL;

		return temp;
	}
}

//Takes in a Queue, returns if its empty
//Return 1 if empty
//Return 0 if NOT empty
int is_empty (Queue *q) {

	//if the head's next points to the tail
	if (q->head.next == &(q->tail)) {

		return 1;
	}

	return 0;
}

//Takes in a queue, returns its size.
//Returns size of -1 on error.
int size_queue(Queue * q) {

	if (!q)
		return -1;

	int count = 0;
	QNode * it = q->head.next;
	while (it != &q->tail)
		count++, it = it->next;

	return count;
}

//Takes a Queue and prints all the addresses of the nodes
void print_queue(Queue * q) {

	if (!q)
		return;

	QNode * it = q->head.next;
	printf("head: %p\n", &q->head);
	while (it != &(q->tail))
		printf("%p\n", it), it = it->next;
	printf("tail: %p\n", &q->tail);
}