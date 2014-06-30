//聊天室客户端
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

int sockfd;//客户端socket
char *IP="127.0.0.1";//本地IP
short PORT=10222;//服务器服务端口
typedef struct sockaddr SA;
char name[30];//用于存放聊天人的昵称

//客户端也分为几步:
//启动客户端，连接服务器
void init(){
	printf("聊天室客户端开始启动\n");
	sockfd=socket(PF_INET,SOCK_STREAM,0);
	struct sockaddr_in addr;
	addr.sin_family = PF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(IP);
	if(connect(sockfd,(SA*)&addr,sizeof(addr))==-1){
		perror("无法连接到服务器");
		printf("客户端启动失败\n");
		exit(-1);
	}
	printf("客户端启动成功\n");
}
//开始通信
void start(){
	//发送消息之前
	//启动一个线程，用来接受服务器发过来的消息
	pthread_t pid;
	void* recv_thread(void*);//线程函数的声明
	pthread_create(&pid,0,recv_thread,0);
	while(1){
		char buf[100] = {};
		scanf("%s",buf);//读取客户端的输入信息
		char msg[100] = {};
		sprintf(msg,"%s说:%s",name,buf);//拼接成一句话.存到msg中
		send(sockfd,msg,strlen(msg),0);//发送消息给服务器
	}
}
void * recv_thread(void* p){
	while(1){
		char buf[100]={};//存储读到的消息
		if(recv(sockfd,buf,sizeof(buf),0)<=0){//接收出错
			return ;//结束线程
		}
		printf("%s\n",buf);
	}
}
int main(){
	printf("请输入您的名字:");
	scanf("%s",name);
	//char buf[100];
	//sprintf(buf,"%s登录了聊天室..",name);
	init();//启动客户端，连接服务器
	send(sockfd,name,strlen(name),0);//把名字给服务器
	start();//通信
}

