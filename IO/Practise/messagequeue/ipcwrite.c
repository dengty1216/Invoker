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
    int msgid = msgget(key, IPC_CREAT | IPC_EXCL | 0666);
    if (msgid == -1) {
        perror("msgget failed");
        return 1;
    }

    while (1) {
        //发送消息
        MSG_T msg;
        msg.msgtype = WARNING;
        memset(msg.msgbuf, 0, sizeof(msg.msgbuf));
        fgets(msg.msgbuf, sizeof(msg.msgbuf), stdin);
        if (msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0) == -1) {
            perror("msgsnd failed");
            exit(EXIT_FAILURE);
        }

        if (strncmp(msg.msgbuf, "quit", 4) == 0) {
            break;
        }
    }

    //删除消息队列
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl failed");
        return 1;
    }

	return 0;
}
