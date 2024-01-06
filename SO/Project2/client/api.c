#include "api.h"

int req_fd = -1;
int resp_fd = -1;
int session_id = -1;

int ems_setup(char const *req_pipe_path, char const *resp_pipe_path, char const *server_pipe_path) {

	if (mkfifo(resp_pipe_path, 0666) == -1) {
		perror("Failed to create responses pipe");
		return 1;
	}

	if (mkfifo(req_pipe_path, 0666) == -1) {
		perror("Failed to create requests pipe");
		return 1;
	}

	int server_fd = open(server_pipe_path, O_WRONLY);
	if (server_fd == -1) {
		perror("Failed to open server pipe");
		return 1;
	}

	char op_code = TO_CHAR(OP_SETUP);
	if (write(server_fd, &op_code, sizeof(char)) == -1) {
		perror("Failed to write op_code to pipe");
		return 1;
	}

	if (write(server_fd, req_pipe_path, MAX_PIPENAME_SIZE) == -1) {
		perror("Failed to write to pipe");
		return 1;
	}

	if (write(server_fd, resp_pipe_path, MAX_PIPENAME_SIZE) == -1) {
		perror("Failed to write to pipe");
		return 1;
	}

	if (close(server_fd) == -1) {
		perror("Failed to close server pipe");
		return 1;
	}

	req_fd = open(req_pipe_path, O_WRONLY);
	resp_fd = open(resp_pipe_path, O_RDONLY);
	if (req_fd == -1) {
		perror("Failed to open requests pipe");
		return 1;
	}
	if (resp_fd == -1) {
		perror("Failed to open responses pipe");
		return 1;
	}

	if (read(resp_fd, &session_id, sizeof(int)) == -1) {
		perror("Failed to read session_id");
		return 1;
	}

	return 0;
}

int ems_quit(void) {

	char op_code = TO_CHAR(OP_QUIT);
	if (write(req_fd, &op_code, sizeof(char)) == -1) {
		perror("Failed to write op_code to pipe");
		return 1;
	}

	if (write(req_fd, &session_id, sizeof(int)) == -1) {
		perror("Failed to write session_id to pipe");
		return 1;
	}

	if (close(req_fd) == -1) {
		perror("Failed to close requests pipe");
		return 1;
	}

	if (close(resp_fd) == -1) {
		perror("Failed to close responses pipe");
		return 1;
	}

	return 0;
}

int ems_create(unsigned int event_id, size_t num_rows, size_t num_cols) {

	char op_code = TO_CHAR(OP_CREATE);
	if (write(req_fd, &op_code, sizeof(char)) == -1) {
		perror("Failed to write op_code to pipe");
		return 1;
	}

	if (write(req_fd, &session_id, sizeof(int)) == -1) {
		perror("Failed to write session_id to pipe");
		return 1;
	}

	if (write(req_fd, &event_id, sizeof(unsigned int)) == -1) {
		perror("Failed to write event_id to pipe");
		return 1;
	}

	if (write(req_fd, &num_rows, sizeof(size_t)) == -1) {
		perror("Failed to write num_rows to pipe");
		return 1;
	}

	if (write(req_fd, &num_cols, sizeof(size_t)) == -1) {
		perror("Failed to write num_cols to pipe");
		return 1;
	}

	int response;
	if (read(resp_fd, &response, sizeof(int)) == -1) {
		perror("Failed to read return value");
		return 1;
	}

	if (response == 1) {
		perror("Failed to create event");
		return 1;
	}

	return 0;
}

int ems_reserve(unsigned int event_id, size_t num_seats, size_t *xs, size_t *ys) {

	char op_code = TO_CHAR(OP_RESERVE);
	if (write(req_fd, &op_code, sizeof(char)) == -1) {
		perror("Failed to write op_code to pipe");
		return 1;
	}

	if (write(req_fd, &session_id, sizeof(int)) == -1) {
		perror("Failed to write session_id to pipe");
		return 1;
	}

	if (write(req_fd, &event_id, sizeof(unsigned int)) == -1) {
		perror("Failed to write event_id to pipe");
		return 1;
	}

	if (write(req_fd, &num_seats, sizeof(size_t)) == -1) {
		perror("Failed to write num_seats to pipe");
		return 1;
	}

	if (write(req_fd, xs, num_seats * sizeof(size_t)) == -1) {
		perror("Failed to write xs to pipe");
		return 1;
	}

	if (write(req_fd, ys, num_seats * sizeof(size_t)) == -1) {
		perror("Failed to write ys to pipe");
		return 1;
	}

	int response;
	if (read(resp_fd, &response, sizeof(int)) == -1) {
		perror("Failed to read return value");
		return 1;
	}

	if (response == 1) {
		perror("Failed to reserve seats");
		return 1;
	}

	return 0;
}

int ems_show(int out_fd, unsigned int event_id) {

	char op_code = TO_CHAR(OP_SHOW);
	if (write(req_fd, &op_code, sizeof(char)) == -1) {
		perror("Failed to write to pipe");
		return 1;
	}

	if (write(req_fd, &session_id, sizeof(int)) == -1) {
		perror("Failed to write session_id to pipe");
		return 1;
	}

	if (write(req_fd, &event_id, sizeof(unsigned int)) == -1) {
		perror("Failed to write to pipe");
		return 1;
	}

	int response;
	if (read(resp_fd, &response, sizeof(int)) == -1) {
		perror("Failed to read return value");
		return 1;
	}

	if (response == 1) {
		perror("Failed to show event");
		return 1;
	}

	size_t num_rows, num_cols;
	if (read(resp_fd, &num_rows, sizeof(size_t)) == -1) {
		perror("Failed to read num_rows from pipe");
		return 1;
	}
	if (read(resp_fd, &num_cols, sizeof(size_t)) == -1) {
		perror("Failed to read num_cols from pipe");
		return 1;
	}

	for (size_t i = 0; i < num_rows; i++) {
		for (size_t j = 0; j < num_cols; j++) {
			unsigned int seat;
			if (read(resp_fd, &seat, sizeof(unsigned int)) == -1) {
				perror("Failed to read response");
				return 1;
			}
			char seat_str[32];
			snprintf(seat_str, 32, "%u ", seat);
			if (write(out_fd, seat_str, strlen(seat_str)) == -1) {
				perror("Failed to write seat to file");
				return 1;
			}
		}
		if (write(out_fd, "\n", sizeof(char)) == -1) {
			perror("Failed to write newline to file");
			return 1;
		}
	}

	return 0;
}

int ems_list_events(int out_fd) {

	char op_code = TO_CHAR(OP_LIST);
	if (write(req_fd, &op_code, sizeof(char)) == -1) {
		perror("Failed to write op_code to pipe");
		return 1;
	}

	if (write(req_fd, &session_id, sizeof(int)) == -1) {
		perror("Failed to write session_id to pipe");
		return 1;
	}

	int response;
	if (read(resp_fd, &response, sizeof(int)) == -1) {
		perror("Failed to read return value");
		return 1;
	}

	if (response == 1) {
		perror("Failed to list events");
		return 1;
	}

	size_t num_events;
	if (read(resp_fd, &num_events, sizeof(size_t)) == -1) {
		perror("Failed to read num_events from pipe");
		return 1;
	}

	for (size_t i = 0; i < num_events; i++) {
		unsigned int event_id;
		if (read(resp_fd, &event_id, sizeof(unsigned int)) == -1) {
			perror("Failed to read event_id from pipe");
			return 1;
		}
		char event_id_str[32];
		snprintf(event_id_str, 32, "Event: %u\n", event_id);
		if (write(out_fd, &event_id_str, strlen(event_id_str)) == -1) {
			perror("Failed to write event_id to file");
			return 1;
		}
	}

	return 0;
}
