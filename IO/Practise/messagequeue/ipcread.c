#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define NOTE 0
#define WARNING 1
#define ERROR 2

typedef struct msgbuf
{
    long msgtype;
    char msgbuf[1024];
}MSG_T;

int main(int argc, const char *argv[])
{
	//创建一个key值
	key_t key = ftok("/home/tydeng/Workspace/IO/day07/semaphore/", 'k');
	if (key == -1) {
		perror("ftok failed");
		return 1;
	}

    //获得消息队列的ID值
    int msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget failed");
        return 1;
    }

    while (1) {
        //获取消息
        MSG_T msg;
        if (msgrcv(msgid, &msg,
            sizeof(msg) - sizeof(long), WARNING, 0) == -1) {
            perror("msgrcv failed");
            exit(EXIT_FAILURE);
        }

        if (strncmp(msg.msgbuf, "quit", 4) == 0) {
            break;
        }
        printf("Read Data %s\n", msg.msgbuf);
    }

	return 0;
}
