#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_SIZE 4096

int main(int argc, const char *argv[])
{
	key_t key = ftok("/home/tydeng/Workspace/IO/day07/sharedMemory", 'k');
	if (key == -1) {
		perror("ftok failed");
		return 1;
	}

	int id = shmget(key, MAX_SIZE, IPC_CREAT | 0666);
	if (id == -1) {
		perror("shmget failed");
		return 1;
	}

	void *address = shmat(id, NULL, 0);
	if (address == (void *)-1) {
		perror("shmat failed");
		return 1;
	}

	while (1) {
		sleep(1);
		if (strncmp(address, "quit", 4) == 0) {
			break;
		}
		printf("Read Data %s\n", (char *)address);
	}

	if (shmdt(address) == -1) {
		perror("shmdt failed");
		return 1;
	}

	return 0;
}
