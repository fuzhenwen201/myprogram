#ifndef __HEAD_H__
#define __HEAD_H__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <signal.h>

struct Message
{
	int id;
	char name[15];
	char password[15];
	int amout;
};

struct MSG
{
	long type;
	struct Message mess;
};
#define OPENAC      1 //开户
#define DELAC       6 //销户
#define FIND        5 //查询
#define CATCHER     4 //取钱
#define SAVECHER    2 //存钱
#define REVCHER     3 //转账
#define FAIL        9 //处理失败
#define SUCCEED     8 //处理成功
#define QUIT        7 //退出   
#endif //__HEAD_H__
