#include "mythread.h"
#include "logreader.h"
#include "logsender.h"
/*线程处理函数*/
void mythread::run()
{
	LogReader logread;
	LogSender logsend;
	/*把数据发送出去 发送一条显示一条*/
	list<MatchedLogRec> match = logread.readLogs();
	logsend.initNetWork();
	MatchedLogRec  mlog;
	while(match.size()>0)
	{
		sleep(1);
		mlog = *(match.begin());
		logsend.sendMatch(mlog);
		emit sigUi(mlog.logip);
		match.erase(match.begin());
	}
}
