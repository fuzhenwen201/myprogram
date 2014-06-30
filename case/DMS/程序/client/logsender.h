#ifndef LOGSENDER_H
#define LOGSENDER_H
#include "data.h"
#include "clientexception.h"
#include <list>
using namespace std;
class LogSender
{
	char failSendFileName[50];
	int fd;
	unsigned short port;
	char serverip[20];
	public:
	LogSender();
	~LogSender();
	/*新增发送一条数据的一个函数*/
	void sendMatch(MatchedLogRec)throw(DmsSenderDataException);
	/*发送数据集合*/
	void sendMatchs(list<MatchedLogRec>*)throw(DmsInitNetWorkException,DmsSenderDataException);
	void initNetWork()throw(DmsInitNetWorkException);
	void readSendFailed(list<MatchedLogRec>*);
	void saveSendFailed(list<MatchedLogRec>*);
};
#endif

