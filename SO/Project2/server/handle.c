#include "handle.h"

void handle_create(int req_fd, int resp_fd) {
	int session_id;
	if (read(req_fd, &session_id, sizeof(int)) == -1) {
		perror("Failed to read session_id from pipe");
		return;
	}

	unsigned int event_id;
	size_t num_rows, num_cols;
	if (read(req_fd, &event_id, sizeof(unsigned int)) == -1) {
		perror("Failed to read event_id from pipe");
		return;
	}
	if (read(req_fd, &num_rows, sizeof(size_t)) == -1) {
		perror("Failed to read num_rows from pipe");
		return;
	}
	if (read(req_fd, &num_cols, sizeof(size_t)) == -1) {
		perror("Failed to read num_cols from pipe");
		return;
	}

	int result = ems_create(event_id, num_rows, num_cols);
	if (write(resp_fd, &result, sizeof(int)) == -1) {
		perror("Failed to write result to pipe");
		return;
	}
}

void handle_reserve(int req_fd, int resp_fd) {
	int session_id;
	if (read(req_fd, &session_id, sizeof(int)) == -1) {
		perror("Failed to read session_id from pipe");
		return;
	}

	unsigned int event_id;
	size_t num_seats, xs[MAX_RESERVATION_SIZE], ys[MAX_RESERVATION_SIZE];
	if (read(req_fd, &event_id, sizeof(unsigned int)) == -1) {
		perror("Failed to read event_id from pipe");
		return;
	}
	if (read(req_fd, &num_seats, sizeof(size_t)) == -1) {
		perror("Failed to read num_seats from pipe");
		return;
	}
	if (read(req_fd, xs, num_seats * sizeof(size_t)) == -1) {
		perror("Failed to read row from pipe");
		return;
	}
	if (read(req_fd, ys, num_seats * sizeof(size_t)) == -1) {
		perror("Failed to read col from pipe");
		return;
	}

	int result = ems_reserve(event_id, num_seats, xs, ys);
	if (write(resp_fd, &result, sizeof(int)) == -1) {
		perror("Failed to write result to pipe");
		return;
	}
}

void handle_show(int req_fd, int resp_fd) {
	int session_id;
	if (read(req_fd, &session_id, sizeof(int)) == -1) {
		perror("Failed to read session_id from pipe");
		return;
	}

	unsigned int event_id;
	if (read(req_fd, &event_id, sizeof(unsigned int)) == -1) {
		perror("Failed to read event_id from pipe");
		return;
	}

	ems_show(resp_fd, event_id);
}

void handle_list_events(int req_fd, int resp_fd) {
	int session_id;
	if (read(req_fd, &session_id, sizeof(int)) == -1) {
		perror("Failed to read session_id from pipe");
		return;
	}

	ems_list_events(resp_fd);
}

void handle_quit(ThreadData *data) {
	int session_id;
	if (read(data->req_fd, &session_id, sizeof(int)) == -1) {
		perror("Failed to read session_id from pipe");
		return;
	}

	if (unlink(data->req_path) == -1) {
		perror("Failed to unlink requests pipe");
		return;
	}
	if (unlink(data->resp_path) == -1) {
		perror("Failed to unlink responses pipe");
		return;
	}
	close(data->req_fd);
	close(data->resp_fd);
}