#include "logsender.h"
#include "clientexception.h"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <fstream>
using namespace std;
LogSender::LogSender()
{
	port = 8899;
	strcpy(serverip,"172.16.9.188");
	strcpy(failSendFileName,"sendfail.dat");
}

LogSender::~LogSender(){}
/*发送一条数据*/
void LogSender::sendMatch(MatchedLogRec match)throw(DmsSenderDataException)
{
	match.pid = htonl(match.pid);
	match.logintime = htonl(match.logintime);
	match.logoutime = htonl(match.logoutime);
	match.durations = htonl(match.durations);
	int sfd = send(fd,&match,sizeof(match),0);
	if(sfd<=0){
		throw DmsSenderDataException("send match to server failed");
	}
}

void LogSender::sendMatchs(list<MatchedLogRec>* matches)throw(DmsInitNetWorkException,DmsSenderDataException)
{
	try{
		initNetWork();
	}catch(DmsInitNetWorkException& e){	
		saveSendFailed(matches);
		throw;
	}
	readSendFailed(matches);

	/*循环发送数据,成功发送一条就删除一条*/
	while(matches->size()>0)
	{
		int sfd = send(fd,&(*(matches->begin())),sizeof(MatchedLogRec),0);
		if(sfd<=0){
				saveSendFailed(matches);
			throw DmsSenderDataException("send data to server failed");
		}else{
			matches->erase(matches->begin());
		}
	}
}

void LogSender::initNetWork()throw(DmsInitNetWorkException)
{
	fd = socket(AF_INET,SOCK_STREAM,0);
	if(-1==fd){
		throw DmsInitNetWorkException("init socket failed");
	}
	struct sockaddr_in caddr;
	caddr.sin_family = AF_INET;
	caddr.sin_port = htons(port);
	caddr.sin_addr.s_addr = inet_addr(serverip);

	int cfd = connect(fd,(struct sockaddr*)&caddr,sizeof(caddr));
	if(-1 == cfd){
		throw DmsInitNetWorkException("connect server failed");
	}
}

void LogSender::readSendFailed(list<MatchedLogRec>* matches) 
{
	ifstream ifs(failSendFileName);
	MatchedLogRec mlog;
	while(ifs.read((char*)&mlog,sizeof mlog))
	{
		matches->push_back(mlog);
	}
}

void LogSender::saveSendFailed(list<MatchedLogRec>* matches)
{	
	ofstream ofs(failSendFileName,ios::trunc);
	if(!ofs)perror("open failsendfile failed"),exit(-1);
	//MatchedLogRec mlog;
	while(matches->size()>0)
	{
		ofs.write((const char*)&(*(matches->begin())),sizeof(MatchedLogRec));
		matches->erase(matches->begin());
	}
}


















