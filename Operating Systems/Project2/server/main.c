#include "handle.h"

Queue thread_data;
char server_path[MAX_PIPENAME_SIZE];
int client_fd;
pthread_t threads[MAX_SESSION_COUNT];
volatile sig_atomic_t sigint_received = 0;
volatile sig_atomic_t sigusr1_received = 0;

int server_init(char *pathname) {
	if (mkfifo(pathname, 0666) == -1) {
		perror("Failed to create pipe");
		return 1;
	}

	return 0;
}

int server_terminate() {
	if (unlink(server_path) == -1) {
		perror("Failed to unlink server pipe");
		return 1;
	}

	if (close(client_fd) == -1) {
		perror("Failed to close server pipe");
		return 1;
	}

	queue_terminate(&thread_data);

	return 0;
}

void handle_sigint() {
	if (server_terminate()) {
		fprintf(stderr, "Failed to terminate server\n");
		return;
	}

	if (ems_terminate()) {
		fprintf(stderr, "Failed to terminate EMS\n");
		return;
	}
	exit(EXIT_SUCCESS);
}

void signal_handler(int sig) {

	if (signal(sig, signal_handler)) {
		exit(EXIT_FAILURE);
	}

	switch (sig) {
	case SIGINT:
		printf("\nSIGINT received.\n");
		sigint_received = 1;
		break;
	case SIGUSR1:
		printf("SIGUSR1 received.\n");
		sigusr1_received = 1;
		break;
	}
}

void *handle_client(void *arg) {
	// Block SIGUSR1 in worker threads
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	if (pthread_sigmask(SIG_BLOCK, &set, NULL) != 0) {
		perror("Failed to block SIGUSR1 in worker thread");
		return NULL;
	}

	(void)arg; // No arg needed in worker threads
	// Loop until the server is terminated
	while (1) {
		ThreadData *data = dequeue(&thread_data);

		if (data == NULL) {
			pthread_exit(EXIT_SUCCESS);
		}

		// Loop until the client is done
		while (1) {
			char char_op_code;
			if (read(data->req_fd, &char_op_code, sizeof(char)) == -1) {
				perror("Failed to read op_code from pipe");
				return NULL;
			}

			int client_done = 0; // Used to break out of the main loop
			int op_code = atoi(&char_op_code);
			switch (op_code) { // Handle the request
			case OP_CREATE:
				handle_create(data->req_fd, data->resp_fd);
				break;

			case OP_RESERVE:
				handle_reserve(data->req_fd, data->resp_fd);
				break;

			case OP_SHOW:
				handle_show(data->req_fd, data->resp_fd);
				break;

			case OP_LIST:
				handle_list_events(data->req_fd, data->resp_fd);
				break;

			case OP_QUIT:
				handle_quit(data);
				client_done = 1;
				break;

			default:
				fprintf(stderr, "Invalid op_code received\n");
				return NULL;
			}
			if (client_done) {
				break;
			}
		}
	}
}

int main(int argc, char *argv[]) {
	// Set up signal handlers
	signal(SIGINT, signal_handler);
	signal(SIGUSR1, signal_handler);

	printf("Process ID: %d\n", getpid()); // Printed for better User Experience. No need to search for it in the terminal.
	if (argc < 2 || argc > 3) {
		fprintf(stderr, "Usage: %s\n <pipe_path> [delay]\n", argv[0]);
		return 1;
	}

	char *endptr;
	unsigned int state_access_delay_us = STATE_ACCESS_DELAY_US;
	if (argc == 3) {
		unsigned long int delay = strtoul(argv[2], &endptr, 10);

		if (*endptr != '\0' || delay > UINT_MAX) {
			fprintf(stderr, "Invalid delay value or value too large\n");
			return 1;
		}

		state_access_delay_us = (unsigned int)delay;
	}

	if (ems_init(state_access_delay_us)) {
		fprintf(stderr, "Failed to initialize EMS\n");
		return 1;
	}

	memset(server_path, 0, sizeof(server_path));
	strncpy(server_path, argv[1], sizeof(server_path) - 1);

	if (server_init(server_path)) {
		fprintf(stderr, "Failed to initialize server\n");
		return 1;
	}
	queue_init(&thread_data);
	for (int i = 0; i < MAX_SESSION_COUNT; i++) {
		if (pthread_create(&threads[i], NULL, handle_client, NULL) != 0) {
			fprintf(stderr, "Failed to create thread\n");
			return 1;
		}
	}

	client_fd = open(server_path, O_CREAT | O_RDWR);
	if (client_fd == -1) {
		perror("Failed to open pipe");
		return 1;
	}

	int session_id = -1; // Session ID for each client
	// Loop until the server is terminated
	while (1) {
		if (sigusr1_received) {
			sigusr1_received = 0;
			ems_show_all();
		}

		if (sigint_received) {
			handle_sigint();
			return 0;
		}

		// Wait for a client setup request
		char char_op_code;
		if (read(client_fd, &char_op_code, sizeof(char)) == -1) {
			perror("Failed to read op_code from pipe");
			if (errno == EINTR) {
				continue;
			}
			return 1;
		}

		int op_code = atoi(&char_op_code);
		if (op_code != OP_SETUP) {
			fprintf(stderr, "Invalid op_code received\n");
			return 1;
		}

		// Read and open client's pipes
		ThreadData *new_data = malloc(sizeof(ThreadData));
		memset(new_data->req_path, 0, MAX_PIPENAME_SIZE);
		memset(new_data->resp_path, 0, MAX_PIPENAME_SIZE);

		if (read(client_fd, new_data->req_path, MAX_PIPENAME_SIZE) == -1) {
			perror("Failed to read req_pipe_path from pipe");
			return 1;
		}
		if (read(client_fd, new_data->resp_path, MAX_PIPENAME_SIZE) == -1) {
			perror("Failed to read resp_pipe_path from pipe");
			return 1;
		}

		int req_fd = open(new_data->req_path, O_RDONLY);
		int resp_fd = open(new_data->resp_path, O_WRONLY);
		if (req_fd == -1) {
			perror("Failed to open requests pipe");
			return 1;
		}
		if (resp_fd == -1) {
			perror("Failed to open responses pipe");
			return 1;
		}

		new_data->req_fd = req_fd;
		new_data->resp_fd = resp_fd;

		// Send the session ID to the client and enqueue the data to the thread data queue
		session_id++;
		int id_to_thread = session_id % MAX_SESSION_COUNT;
		if (write(resp_fd, &id_to_thread, sizeof(int)) == -1) {
			perror("Failed to write  to pipe");
			return 1;
		}
		enqueue(&thread_data, new_data);
	}
}