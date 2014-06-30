#include <stdio.h>
#include <signal.h>//信号
#include <pthread.h>//多线程
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
struct sockaddr_in clien;
socklen_t len = sizeof(clien);
int buf[40];
int fd;

char rec[100]={}/*接收消息数组*/;
char sen[200] = {}/*发送信息数组*/;
char* poin[40] = {};/*存储ip地址的指针数组*/
/*自定义信号处理函数*/
void fa(int signum)
{
	printf("服务器关闭中，请稍候...\n");
	sleep(1);
	close(fd);
	printf("服务器已关闭。\n");
	exit(0);
}

/*子线程进行接收/发送信息*/
void* task(void* i)
{
	while(1){
	int fd2 = (int)i,j = 0;

	/*接收信息*/
	int res1 = read(fd2,rec,sizeof(rec));
	
	/*判断是否接收到信息*/
	if(res1<=0)
	{
		buf[fd2]=0;
		printf("%s已下线\n",poin[fd2]);
		char buf1[10] = {};
		sprintf(buf1,"%s已下线",poin[fd2]);
		for(j=0;j<40;j++)
		{
			if(buf[j]==1 && j!=fd2)
				sendto(j,buf1,strlen(buf1),0,(struct sockaddr*)&clien,len);
		}
		break;
	}
	/*将接收到的信息发送给在线客户端*/
	else
	{
		memset(sen,0,strlen(sen));
		sprintf(sen,"%s:%s",poin[fd2]/*inet_ntoa(clien.sin_addr)*/,rec);
		for(j=0;j<40;j++)
		{
			if(buf[j]==1 && j!=fd2)/*判断客户端是否在线*/
			{
				res1 = sendto(j,sen,strlen(sen),0,(struct sockaddr*)&clien,len);
				if(res1==-1)perror("sento"),exit(-1);
			}
			
		}
		memset(rec,0,strlen(rec));
	
	}
	}
}

int main()
{
	printf("按ctrl+C退出\n");
	signal(SIGINT,fa);
	
	/*创建socket*/
	fd = socket(AF_INET,SOCK_STREAM,0);
	if(fd==-1)perror("socket"),exit(-1);

	/*准备通信地址*/
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(2222);
	addr.sin_addr.s_addr = inet_addr("172.16.7.181");
	
	/*防止通信地址被使用*/
	int status =1;
	setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&status,sizeof(status));

	/*服务器绑定*/
	int res = bind(fd,(struct sockaddr*)&addr,sizeof(addr));
	if(res==-1)perror("绑定失败"),exit(-1);
	printf("绑定成功\n");

	/*监听客户端是否上线*/
	if(listen(fd,40))perror("监听失败"),exit(-1);
	int i = 0;
	for(i=4;i<44;i++)	
	{
		/*生成新的socket*/
		int fd2 = accept(fd,(struct sockaddr*)&clien,&len);
		if(fd2==-1)perror("accept"),exit(-1);
		poin[fd2]=inet_ntoa(clien.sin_addr);
		printf("%s已连接:%d\n",poin[fd2]/*inet_ntoa(clien.sin_addr)*/,fd2);
		buf[fd2] = 1;
	

		/*创建子线程*/
		pthread_t id;
		pthread_create(&id,0,task,(void*)fd2);
		pthread_detach(id);
	}	
	return 0;
}

