#include "threads.h"

struct ThreadData {
	int thread_id, MAX_THREADS;
	int fd_jobs, fd_out;
} ThreadData;

void *processThread(void *arg) {
	unsigned int event_id, delay;
	size_t num_rows, num_columns, num_coords;
	size_t xs[MAX_RESERVATION_SIZE], ys[MAX_RESERVATION_SIZE];
	struct ThreadData *threadData = (struct ThreadData *)arg;

	for (int line = 0;;line++) {
		fflush(stdout);
		switch (get_next(threadData->fd_jobs)) {
		case CMD_CREATE:
			if (parse_create(threadData->fd_jobs, &event_id, &num_rows, &num_columns) != 0) {
				fprintf(stderr, "Invalid command. See HELP for usage\n");
				break;
			}

			if (line % threadData->MAX_THREADS != threadData->thread_id) {
				break;
			}

			if (ems_create(event_id, num_rows, num_columns)) {
				fprintf(stderr, "Failed to create event\n");
			}

			break;

		case CMD_RESERVE:
			num_coords = parse_reserve(threadData->fd_jobs, MAX_RESERVATION_SIZE, &event_id, xs, ys);

			if (num_coords == 0) {
				fprintf(stderr, "Invalid command. See HELP for usage\n");
				break;
			}

			if (line % threadData->MAX_THREADS != threadData->thread_id) {
				break;
			}

			if (ems_reserve(event_id, num_coords, xs, ys)) {
				fprintf(stderr, "Failed to reserve seats\n");
			}

			break;

		case CMD_SHOW:
			if (parse_show(threadData->fd_jobs, &event_id) != 0) {
				fprintf(stderr, "Invalid command. See HELP for usage\n");
				break;
			}

			if (line % threadData->MAX_THREADS != threadData->thread_id) {
				break;
			}

			if (ems_show(event_id, threadData->fd_out)) {
				fprintf(stderr, "Failed to show event\n");
			}

			break;

		case CMD_LIST_EVENTS:
			if (line % threadData->MAX_THREADS != threadData->thread_id) {
				break;
			}

			if (ems_list_events(threadData->fd_out)) {
				fprintf(stderr, "Failed to list events\n");
			}

			break;

		case CMD_WAIT:
			unsigned int thread_id;
			if (parse_wait(threadData->fd_jobs, &delay, &thread_id) == -1) {
				fprintf(stderr, "Invalid command. See HELP for usage\n");
				break;
			}

			if (delay > 0) {
				if (thread_id == 0 || (int)thread_id == threadData->thread_id + 1) {
					printf("Waiting...\n");
					ems_wait(delay);
				}
			}

			break;

		case CMD_INVALID:
			if (line % threadData->MAX_THREADS != threadData->thread_id) {
				break;
			}

			fprintf(stderr, "Invalid command. See HELP for usage\n");
			break;

		case CMD_HELP:

			if (line % threadData->MAX_THREADS != threadData->thread_id) {
				break;
			}

			printf("Available commands:\n"
				   "  CREATE <event_id> <num_rows> <num_columns>\n"
				   "  RESERVE <event_id> [(<x1>,<y1>) (<x2>,<y2>) ...]\n"
				   "  SHOW <event_id>\n"
				   "  LIST\n"
				   "  WAIT <delay_ms> [thread_id]\n"
				   "  BARRIER\n"
				   "  HELP\n");

			break;

		case CMD_BARRIER:
			pthread_exit((void *)1);

		case CMD_EMPTY:
			break;

		case EOC:
			close(threadData->fd_jobs);
			pthread_exit(0);
		}
	}
}

void processFile(char *name, const int MAX_THREADS) {
	char *path_jobs = (char *)malloc((6 + strlen(name)) * sizeof(char));
	char *path_out = (char *)malloc((6 + strlen(name)) * sizeof(char));
	strcpy(path_jobs, "jobs/");
	strcpy(path_out, "jobs/");
	strcat(path_jobs, name);
	strcat(path_out, name);
	int threadNum;
	pthread_t threads[MAX_THREADS];
	struct ThreadData *threadDataArray = malloc((long unsigned int)MAX_THREADS * sizeof(struct ThreadData));

	char *dot = strrchr(path_out, '.');
	long int dot_pos = dot - path_out;
	strcpy(path_out + dot_pos, ".out");
	int fd_out = open(path_out, O_CREAT | O_RDWR | O_TRUNC, S_IRUSR);
	free(path_out);

	for (threadNum = 0; threadNum < MAX_THREADS; threadNum++) {
		int fd_jobs = open(path_jobs, O_RDONLY, S_IRUSR);
		threadDataArray[threadNum].fd_jobs = fd_jobs;
		threadDataArray[threadNum].fd_out = fd_out;
		threadDataArray[threadNum].thread_id = threadNum;
		threadDataArray[threadNum].MAX_THREADS = MAX_THREADS;
		if (pthread_create(&threads[threadNum], NULL, processThread, (void *)&threadDataArray[threadNum]) != 0) {
			fprintf(stderr, "Failed to create thread\n");
			break;
		}
	}

	for (int barrier = 1; barrier == 1;) {
		barrier = 0;
		void *value;
		for (int i = 0; i < threadNum; i++) {
			pthread_join(threads[i], &value);
			if (value == (void *)1)
				barrier = 1;
		}

		if (barrier == 1) {
			for (int i = 0; i < threadNum; i++) {
				pthread_create(&threads[i], NULL, processThread, (void *)&threadDataArray[i]);
			}
		} else {
			break;
		}
	}
	free(threadDataArray);
	free(path_jobs);
	close(fd_out);
}