#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstdio>
#include "data.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <deque>
#include "userdata.h"
using namespace std;
/*uc线程处理函数 完成接收数据的功能*/
/*
void* recivedata(void* par)
{
	int newfd = *((int*)par);
	while(1)
	{
		MatchedLogRec mlog = {0};
		int rfd = recv(newfd,&mlog,sizeof(mlog),0);
		if(rfd<=0)break;
	cout << mlog.logname << " " << mlog.logip << endl;
	}
	close(newfd);
}
*/
/*数据缓冲池  队列*/
//deque<MatchedLogRec>  datapool;
UserData userdata;
/*包装c++生成者线程类*/
class producterthread
{
	pthread_t thid;
	int newfd;
    public:
	producterthread(int newfd):newfd(newfd){}
	//线程处理函数
	static void* recivedata(void* par){
		((producterthread*)par)->run();
	}
	/*真正完成线程功能的函数,继承后只需覆盖run()函数*/
	virtual void run()
	{
		while(1)
		{
			MatchedLogRec mlog = {0};
			/*如果没有接收满一个结构体 就继续接收直到接满一个结构体大小*/
			int rlen = 0;
			for(int len=sizeof(mlog);len;len=len-rlen){
				 rlen = recv(newfd,((char*)&mlog)+(sizeof(mlog)-len),len,0);
				 if(rlen<=0){
				 goto outloop;
				 }
			}
			/*正常退出for 代表接满一个结构体大小*/
			userdata.push_data(mlog);			
		}
		outloop:
		close(newfd);
		delete this;
	}
	void start(){		
		pthread_create(&thid,0,recivedata,this);
	}
};

int main()
{
	int fd= socket(AF_INET,SOCK_STREAM,0);
	if(-1==fd)perror("socket failed"),exit(-1);
	
	struct sockaddr_in addr,newaddr= {0};
	socklen_t len = sizeof(newaddr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8899);
	addr.sin_addr.s_addr = INADDR_ANY;//宏,自定替换IP地址
	
	int bfd = bind(fd,(struct sockaddr*)&addr,sizeof(addr));
	if(-1==bfd)perror("bind fail"),exit(-1);

	listen(fd,10);
	while(1)
	{
		int newfd = accept(fd,(struct sockaddr*)&newaddr,&len);
		if(-1==newfd)perror("accept failed"),exit(-1);
	/*创建线程uc*/
	//	pthread_t thid;
	//	pthread_create(&thid,0,recivedata,&newfd);
   /*创建线程c++*/
		producterthread *product = new producterthread(newfd);
		product->start();
	}
	close(fd);
	return 0;
}
