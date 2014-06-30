#include "logreader.h"
#include <iostream>
#include <cstdlib>
#include <fcntl.h>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <cstring>
#include <fstream>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;
LogReader::LogReader()
{
	//strcpy(backFileName,"wtmpt");
	strcpy(logFileName,"/home/tarena/傅振文/案例/DMS/程序/client/wtmpx");
	strcpy(failLoginsFileName,"wppwj");
}

LogReader::~LogReader()
{
}

list<MatchedLogRec>& LogReader::readLogs()
{
	backup();
	readFailLogins();
	readBackupFile();
	cout << "logins size = " << logins.size() << endl;
	cout << "logouts size = " << logouts.size() << endl;
	rename(backFileName,logFileName);
	matchLogRec();
	cout << "matches size = " << matches.size() << endl;
	saveFailLogins();
	return matches;
}

void LogReader::backup()
{
	cout << "1.备份日志文件" << endl;
	ostringstream  oss;
	time_t t=time(NULL);
	struct tm *mytime = localtime(&t);
	oss << "/home/tarena/傅振文/案例/DMS/程序/client/" << setfill('0') << mytime->tm_year+1900 << setw(2) << mytime->tm_mon+1 << setw(2) << mytime->tm_mday << setw(2) << mytime->tm_hour << ':' <<setw(2) << mytime->tm_min << ':' << setw(2) << mytime->tm_sec << "wtmpx";
	string file = oss.str();
	cout << file.size() << endl;
	strcpy(backFileName,file.c_str());
	//改文件名
	rename(logFileName,backFileName);
}
void LogReader::readFailLogins()
{

	cout << "2.读取上次未匹配的数据" << endl;
	//c++
	ifstream ifs(failLoginsFileName);
	if(!ifs)perror("open failed");
	LogRec log;
	while(ifs.read(((char*)&log),sizeof log))
	{
		logins.push_back(log);
	}
	ifs.close();
/*  //uc
	int fd = open(failLoginsFileName,O_RDONLY);
	if(fd==-1)perror("open failloginsfile failed"),exit(-1);
	
	struct stat fs;
	fstat(fd,&fs);
	int loopcount = fs.st_size/372;
	for(int i=0;i<loopcount;++i)
	{
		LogRec log;
		read(fd,&log.logname,32);
		lseek(fd,36,SEEK_CUR);
		read(fd,&log.pid,4);
		read(fd,&log.logtype,2);
	
		lseek(fd,6,SEEK_CUR);
		read(fd,&log.logtime,4);
		lseek(fd,30,SEEK_CUR);
		read(fd,&log.logip,20);
		logins.push_back(log);

	}
*/
}

void LogReader::readBackupFile()
{
	cout << "3.读取备份日志文件" << endl;
	int fd=open(backFileName,O_RDONLY);
	if(fd==-1)perror("open backfile failed"),exit(-1);
	
	struct stat fs;
	fstat(fd,&fs);
	cout << "file size=" << fs.st_size << endl;	
	int loopcount = fs.st_size/372;
	for(int i=0;i<loopcount;++i)
	{
		LogRec log;
		Logstruct str;
		read(fd,&str,372);
		strcpy(log.logname,str.logname);
		log.pid = str.pid;
		log.pid = htonl(log.pid);
		log.logtype = str.logtype;
		log.logtype = htons(log.logtype);
		log.logtime = str.logtime;
		log.logtime = htonl(log.logtime);
		strcpy(log.logip,str.logip);
/*		read(fd,&log.logname,32);
		lseek(fd,36,SEEK_CUR);
		read(fd,&log.pid,4);
		log.pid = htonl(log.pid);
		read(fd,&log.logtype,2);
		log.logtype = htons(log.logtype);
		lseek(fd,6,SEEK_CUR);
		read(fd,&log.logtime,4);
		log.logtime = htonl(log.logtime);
		lseek(fd,30,SEEK_CUR);
		read(fd,&log.logip,257);
		lseek(fd,1,SEEK_CUR);
*/		
		/*输出不是.开头的数据*/
		if(!(log.logname[0]=='.'))
//		cout << log.logname << " " << log.pid << " " << log.logtype  << " " << log.logtime << " " << log.logip << endl;
		if(7==log.logtype)
		{
			logins.push_back(log);
		}else if(8==log.logtype){
			logouts.push_back(log);
		}else{
			;
		}
	}
	close(fd);
}

void LogReader::matchLogRec()
{
	cout << "4.匹配" << endl;
/****1.使用迭代器循环的从登出集合中取出数据
 ****2.在登入集合中循环查找，如果找到匹配(logname,pid,logip),就构建匹配记录对象并赋值放入匹配集合，删除登入记录 终止本次查找
 ****3.如果找不到就继续 直到查找结束，清空登出集合*/
	list<LogRec>::iterator outlog;
	list<LogRec>::iterator inlog; 
	MatchedLogRec matchlog;
	for(outlog=logouts.begin();outlog!=logouts.end();++outlog)
	{
		for(inlog=logins.begin();inlog!=logins.end();++inlog)
		{
			if(strcmp(inlog->logname,outlog->logname)==0 && inlog->pid==outlog->pid && strcmp(inlog->logip,outlog->logip)==0)
			{
				strcpy(matchlog.logname,inlog->logname);
				matchlog.pid = inlog->pid;
				matchlog.logintime = inlog->logtime;
				matchlog.logoutime = outlog->logtime;
				matchlog.durations = outlog->logtime - inlog->logtime;
				strcpy(matchlog.logip,inlog->logip);

				matches.push_back(matchlog); 
				logins.erase(inlog);	
				break;
			}
		}
	}
	logouts.clear();	
}

void LogReader::saveFailLogins()
{
		cout << "5.保存" << endl;
	/*未匹配成功的登入记录*/
		ofstream ofs(failLoginsFileName);
		if(!ofs)perror("open file failed"),exit(-1);
		while(logins.size()>0)
		{
			ofs.write((const char*)(&(*logins.begin())),sizeof(LogRec));
			logins.erase(logins.begin());
		}
		ofs.close();
	/*	
		list<LogRec>::iterator inlog; 
		int fd = open(failLoginsFileName,O_WRONLY|O_CREAT|O_APPEND,0600);
		if(fd==-1)perror("open failloginsfilename"),exit(-1);
		for(inlog=logins.begin();inlog!=logins.end();++inlog)
			{
				write(fd,&inlog->logname,32);
				write(fd,&inlog->pid,4);

				write(fd,&inlog->logtype,2);
				write(fd,&inlog->logtime,4);
				write(fd,&inlog->logip,257);
			}
		close(fd);
		logins.clear();
	*/
}




















