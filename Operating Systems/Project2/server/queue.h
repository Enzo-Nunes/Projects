#ifndef QUEUE_H
#define QUEUE_H

#include "common/constants.h"

/**
 * @brief Structure representing the data associated with a thread.
 */
typedef struct ThreadData {
	char req_path[MAX_PIPENAME_SIZE];  /**< The path of the request pipe. */
	char resp_path[MAX_PIPENAME_SIZE]; /**< The path of the response pipe. */
	int req_fd;						   /**< The file descriptor of the request pipe. */
	int resp_fd;					   /**< The file descriptor of the response pipe. */
} ThreadData;

/**
 * @brief Structure representing a node in the queue.
 */
typedef struct Node {
	ThreadData *data;  /**< Pointer to the thread data. */
	struct Node *next; /**< Pointer to the next node in the queue. */
} Node;

/**
 * @brief Structure representing a queue.
 */
typedef struct Queue {
	Node *head;			   /**< Pointer to the head of the queue. */
	Node *tail;			   /**< Pointer to the tail of the queue. */
	pthread_mutex_t mutex; /**< Mutex for thread synchronization. */
	pthread_cond_t cond;   /**< Condition variable for thread synchronization. */
} Queue;

/**
 * Initializes the queue.
 *
 * @param q The queue to be initialized.
 */
void queue_init(Queue *q);

/**
 * Terminates the queue and frees any allocated memory.
 *
 * @param q The queue to be terminated.
 */
void queue_terminate(Queue *q);

/**
 * Checks if the queue is empty.
 *
 * @param q The queue to be checked.
 * @return 1 if the queue is empty, 0 otherwise.
 */
int is_queue_empty(Queue *q);

/**
 * Removes and returns the first element from the queue.
 *
 * @param q The queue from which to dequeue.
 * @return The dequeued element, or NULL if the queue is empty.
 */
ThreadData *dequeue(Queue *q);

/**
 * Adds an element to the end of the queue.
 *
 * @param q The queue to which to enqueue.
 * @param data The data to be enqueued.
 */
void enqueue(Queue *q, ThreadData *data);

#endif // QUEUE_H