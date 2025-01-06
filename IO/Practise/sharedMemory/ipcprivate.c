#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 4096

int main(int argc, const char *argv[])
{
	int id = shmget(IPC_PRIVATE, MAX_SIZE, IPC_CREAT | IPC_EXCL | 0666);
	if (id == -1) {
		perror("shmget failed");
		return 1;
	}

	void *address = shmat(id, NULL, 0);
	if (address == (void *)-1) {
		perror("shmat failed");
		return 1;
	}

	pid_t pid = fork();
	if (pid < 0) {
		perror("fork failed");
		return 1;
	} else if(pid == 0) {
		//child_process(buf, MAX_SIZE);	
		while (1) {
			sleep(3);
			if (strncmp(address, "quit", 4) == 0) {
				break;
			}
			printf("Read Data %s\n", (char *)address);
			memset(address, 0, MAX_SIZE);
		}
		exit(EXIT_SUCCESS);
	} else {
		//parent_process(buf, MAX_SIZE);
		while (1) {
			fgets(address, MAX_SIZE, stdin);
			printf("Write Data %s\n", (char *)address);
			if (strncmp(address, "quit", 4) == 0) {
				break;
			}
		}
	}

	if (shmdt(address) == -1) {
		perror("shmdt failed");
		return 1;
	}

	if (shmctl(id, IPC_RMID, NULL) == -1) {
		perror("shmctl failed");
		return 1;
	}

	return 0;
}
