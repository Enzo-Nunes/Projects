#include "queue.h"

void queue_init(Queue *q) {
	q->head = q->tail = NULL;
	pthread_mutex_init(&q->mutex, NULL);
	pthread_cond_init(&q->cond, NULL);
}

void queue_terminate(Queue *q) {

	if (q->head != NULL) {
		Node *current = q->head;
		while (current != NULL) {
			Node *temp = current;
			current = current->next;
			free(temp->data);
			free(temp);
		}
	}
}

int is_queue_empty(Queue *q) { return q->head == NULL; }

ThreadData *dequeue(Queue *q) {
	pthread_mutex_lock(&q->mutex);

	while (is_queue_empty(q)) {
		pthread_cond_wait(&q->cond, &q->mutex);
	}

	Node *temp = q->head;
	ThreadData *data = temp->data;

	q->head = q->head->next;
	if (q->head == NULL)
		q->tail = NULL;

	free(temp);

	pthread_mutex_unlock(&q->mutex);
	return data;
}

void enqueue(Queue *q, ThreadData *data) {
	pthread_mutex_lock(&q->mutex);

	Node *new_node = malloc(sizeof(Node));
	new_node->data = data;
	new_node->next = NULL;

	if (q->tail != NULL) {
		q->tail->next = new_node;
		q->tail = new_node;
	} else {
		q->head = q->tail = new_node;
	}

	pthread_cond_signal(&q->cond);
	pthread_mutex_unlock(&q->mutex);
}
