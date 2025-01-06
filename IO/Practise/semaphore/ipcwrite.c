#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define MAX_SIZE 4096

union semun {
	int		val;	/* Value for SETVAL */
	struct semid_ds *buf;	/* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;	/* Array for GETALL, SETALL */
	struct seminfo  *__buf;	/* Buffer for IPC_INFO
							   (Linux-specific) */
};

union semun mysem;
struct sembuf mysembuf;

int main(int argc, const char *argv[])
{
	//创建一个key值
	key_t key = ftok("/home/tydeng/Workspace/IO/day07/semaphore/", 'k');
	if (key == -1) {
		perror("ftok failed");
		return 1;
	}

	//获得一个信号灯集的ID
	//我们设置了这个集合中有两个信号灯，分别为读与写
	int semid = semget(key, 2, IPC_CREAT | IPC_EXCL | 0666);
	if (semid == -1) {
		perror("semget failed");
		return 1;
	}
	
	//设置写信号灯的数值为1
	mysem.val = 1;
	int ret = semctl(semid, 1, SETVAL, mysem);
	if (ret == -1) {
		perror("semctl failed");
		return 1;
	}

	//获得共享内存的ID
	key = ftok("/home/tydeng/Workspace/IO/day07/semaphore/", 's');
	if (key == -1) {
		perror("ftok failed");
		return 1;
	}
	int shmid = shmget(key, MAX_SIZE, IPC_CREAT | IPC_EXCL | 0666);
	if (shmid == -1) {
		shmid = shmget(key, MAX_SIZE, IPC_CREAT | 0666);
		if (shmid == -1) {
			perror("shmget failed");
			return 1;
		} 
	}

	//映射共享内存
	void *shmaddr = shmat(shmid, NULL, 0);

	while (1) {
		//分别对读写信号灯进行PV操作
		//申请写资源
		mysembuf.sem_num = 1;
		mysembuf.sem_op = -1;
		mysembuf.sem_flg = 0;
		if (semop(semid, &mysembuf, 1) == -1) {
			perror("write semop failed");
			exit(EXIT_FAILURE);
		}

		memset(shmaddr, 0, MAX_SIZE);
		fgets((char *)shmaddr, MAX_SIZE, stdin);

		//释放读资源
		mysembuf.sem_num = 0;
		mysembuf.sem_op = 1;
		mysembuf.sem_flg = 0;
		if (semop(semid, &mysembuf, 1) == -1) {
			perror("read semop failed");
			exit(EXIT_FAILURE);
		}

		if (strncmp(shmaddr, "quit", 4) == 0) {
			break;
		}
	}

	//由于使用IPC_RMID删除信号灯集
	//因此不需要对集合中信号灯一一删除
	//当删除发生后，所有等待该集合中任意信号灯的进程全部被唤起
	if (semctl(semid, 0, IPC_RMID, 0) == -1) {
		perror("remove write sem failed");
		return 1;
	}

	//分离共享内存
	if (shmdt(shmaddr) == -1) {
		perror("shmdt failed");
		return 1;
	}
	//删除共享内存
	if (shmctl(shmid, IPC_RMID, NULL) == -1) {
		perror("shmctl failed");
		return 1;
	}

	return 0;
}
