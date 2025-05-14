/*
 *	Enzo Nunes		ist1106336
 *	Tomás Monteiro	ist1106211
 */

#include "threads.h"

int main(int argc, char *argv[]) {
	unsigned int state_access_delay_ms = STATE_ACCESS_DELAY_MS;

	int MAX_PROC = argc > 2 ? atoi(argv[2]) : 1;
	int MAX_THREADS = argc > 3 ? atoi(argv[3]) : 1;

	if (argc > 4) {
		char *endptr;
		unsigned long int delay = strtoul(argv[3], &endptr, 10);

		if (*endptr != '\0' || delay > UINT_MAX) {
			fprintf(stderr, "Invalid delay value or value too large\n");
			return 1;
		}

		state_access_delay_ms = (unsigned int)delay;
	}

	if (ems_init(state_access_delay_ms)) {
		fprintf(stderr, "Failed to initialize EMS\n");
		return 1;
	}

	DIR *jobs_dir;
	struct dirent *entry;
	int status = 0, proc_num = 0;
	jobs_dir = opendir("jobs");

	while ((entry = readdir(jobs_dir)) != NULL) {
		if (strstr(entry->d_name, ".jobs") != NULL) {
			if (proc_num >= MAX_PROC) {
				int proc_id = wait(&status);
				printf("Process %d exited with status %d\n", proc_id, status);
				proc_num--;
			}
			if (fork() == 0) {
				processFile(entry->d_name, MAX_THREADS);
				exit(0);
			} else {
				proc_num++;
			}
		}
	}

	while (proc_num > 0) {
		int proc_id = wait(&status);
		printf("Process %d exited with status %d\n", proc_id, status);
		proc_num--;
	}

	closedir(jobs_dir);
	ems_terminate();
}