#ifndef LOGDAO_H
#define LOGDAO_H

#include "data.h"
/*负责把数据放入数据库的类*/
class LogDao 
{
	public:
		void connectDb(const char* userpasswd);//连接数据库
		void saveData(MatchedLogRec mlog);//保存数据
		void commit();//确认
		void disconnect();//断开连接
};
#endif
