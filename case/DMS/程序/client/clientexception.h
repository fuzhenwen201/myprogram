#ifndef CLIENTEXCEPTION_H
#define CLIENTEXCEPTION_H
#include <string>
using namespace std;
class DmsException
{
	string msg;
	public:
	DmsException(string msg="dms exception"):msg(msg){}
	virtual const char* what()const throw()
	{
		return msg.c_str();	
	}
};
//发送失败抛出错误
class DmsSenderException:public DmsException
{
	public:
		DmsSenderException(string msg="dms sender exception"):DmsException(msg){}
};
//初始化网络失败抛出错误
class DmsInitNetWorkException:public DmsSenderException
{
	public:
		DmsInitNetWorkException(string msg="dms initnetwork exception"):DmsSenderException(msg){}
};

class DmsSenderDataException:public DmsSenderException
{
	public:
		DmsSenderDataException(string msg="dms sender data exception"):DmsSenderException(msg){}
};
#endif
