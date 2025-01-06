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
	//创建一个key值
	key_t key = ftok("/home/tydeng/Workspace/IO/day07/sharedMemory", 'k');
	if (key == -1) {
		perror("ftok failed");
		return 1;
	}

	//可以加上IPC_EXCL，但是使用Ctrl + c会导致该共享内存无法删除
	//需要在终端上人为使用ipcs -m id进行共享内存删除
	int id = shmget(key, MAX_SIZE, IPC_CREAT | 0666);
	if (id == -1) {
		perror("shmget failed");
		return 1;
	}

	//进行映射
	void *address = shmat(id, NULL, 0);
	if (address == (void *)-1) {
		perror("shmat failed");
		return 1;
	}

	while (1) {
		fgets(address, MAX_SIZE, stdin);
		printf("Write Data %s\n", (char *)address);
		if (strncmp(address, "quit", 4) == 0) {
			break;
		}
	}

	//分离共享内存的映射关系
	if (shmdt(address) == -1) {
		perror("shmdt failed");
		return 1;
	}

	//删除共享内存
	if (shmctl(id, IPC_RMID, NULL) == -1) {
		perror("shmctl failed");
		return 1;
	}

	return 0;
}
