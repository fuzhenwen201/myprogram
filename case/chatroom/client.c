#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
int fd;
char buf[200] ={};//发送消息数组
char rec[100] = {};//接收消息数组
void* task()
{
	while(1)
	{
		memset(rec,0,strlen(rec));
		int res = read(fd,rec,sizeof(rec));
		if(res ==-1)perror("read"),exit(-1);
		printf("%s\n",rec);
	}
}

int main()
{
	int ch = 0;
	printf("请输入连接服务器：");
	scanf("%d",&ch);
    char ipadd[20]={};
	sprintf(ipadd,"172.16.7.%d",ch);
	fd = socket(AF_INET,SOCK_STREAM,0);
	if(fd==-1)perror("socket"),exit(-1);
	
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(2222);
	addr.sin_addr.s_addr = inet_addr(ipadd);

	int res = connect(fd,(struct sockaddr*)&addr,sizeof(addr));
	if(res==-1)perror("connect"),exit(-1);
	printf("您已上线\n");

	pthread_t id ;
	pthread_create(&id,0,task,NULL);


	while(strcmp(buf,"bye"))
	{

		gets(buf);

		res = write(fd,buf,strlen(buf));
		if(res==-1)printf("发送失败\n"),exit(-1);
	}
	printf("您已下线\n");
	pthread_detach(id);
	return 0;
}

