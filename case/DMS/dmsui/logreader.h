#ifndef LOGREADER_H
#define LOGREADER_H
#include "data.h"
#include <list>
using namespace std;
class LogReader
{
	char logFileName[100];
	char backFileName[100];
	char failLoginsFileName[100];
	list<LogRec> logins;
	list<LogRec> logouts;
	list<MatchedLogRec> matches;
	public:
	LogReader();
	~LogReader();
	list<MatchedLogRec>& readLogs();
	private:
	void backup();
	void readFailLogins();
	void readBackupFile();
	void matchLogRec();
	void saveFailLogins();
};

#endif



















