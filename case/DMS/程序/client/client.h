#ifndef CLIENT_H
#define CLIENT_H
#include "logsender.h"
#include "logreader.h"
class Client
{
	LogReader logread;
	LogSender logsend;	
	public:
		void sendlog();
};
#endif
