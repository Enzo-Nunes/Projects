#ifndef HANDLE_H
#define HANDLE_H

#include "operations.h"
#include "queue.h"

/**
 * Handles the create request.
 *
 * @param req_fd The file descriptor for the request.
 * @param resp_fd The file descriptor for the response.
 */
void handle_create(int req_fd, int resp_fd);

/**
 * Handles the reserve request.
 *
 * @param req_fd The file descriptor for the request.
 * @param resp_fd The file descriptor for the response.
 */
void handle_reserve(int req_fd, int resp_fd);

/**
 * Handles the show request.
 *
 * @param req_fd The file descriptor for the request.
 * @param resp_fd The file descriptor for the response.
 */
void handle_show(int req_fd, int resp_fd);

/**
 * Handles the list events request.
 *
 * @param req_fd The file descriptor for the request.
 * @param resp_fd The file descriptor for the response.
 */
void handle_list_events(int req_fd, int resp_fd);

/**
 * Handles the quit request.
 *
 * @param data The thread data. (fd's are later extracted from it)
 */
void handle_quit(ThreadData *data);

#endif // HANDLE_H