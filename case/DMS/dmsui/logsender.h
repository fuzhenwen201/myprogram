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
	void sendMatchs(list<MatchedLogRec>*)throw(DmsInitNetWorkException,DmsSenderDataException);
	void initNetWork()throw(DmsInitNetWorkException);
	void readSendFailed(list<MatchedLogRec>*);
	void saveSendFailed(list<MatchedLogRec>*);
};
#endif

