#ifndef SERVER_EVENT_LIST_H
#define SERVER_EVENT_LIST_H

#include "common/constants.h"
#include "common/io.h"

struct Event {
	unsigned int id;		   /// Event id
	unsigned int reservations; /// Number of reservations for the event.

	size_t cols; /// Number of columns.
	size_t rows; /// Number of rows.

	unsigned int *data;	   /// Array of size rows * cols with the reservations for each seat.
	pthread_mutex_t mutex; // Mutex to protect the event
};

struct ListNode {
	struct Event *event;
	struct ListNode *next;
};

// Linked list structure
struct EventList {
	struct ListNode *head; // Head of the list
	struct ListNode *tail; // Tail of the list
	pthread_rwlock_t rwl;  // Mutex to protect the list
};

/// Creates a new event list.
/// @return Newly created event list, NULL on failure
struct EventList *create_list();

/// Appends a new node to the list.
/// @param list Event list to be modified.
/// @param data Event to be stored in the new node.
/// @return 0 if the node was appended successfully, 1 otherwise.
int append_to_list(struct EventList *list, struct Event *data);

/// Removes a node from the list.
/// @param list Event list to be modified.
/// @return 0 if the node was removed successfully, 1 otherwise.
void free_list(struct EventList *list);

/// Retrieves an event in the list.
/// @param list Event list to be searched
/// @param event_id Event id.
/// @param from First node to be searched.
/// @param to Last node to be searched.
/// @return Pointer to the event if found, NULL otherwise.
struct Event *get_event(struct EventList *list, unsigned int event_id, struct ListNode *from, struct ListNode *to);

#endif // SERVER_EVENT_LIST_H
