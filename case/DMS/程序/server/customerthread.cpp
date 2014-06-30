#include "customerthread.h"
#include "data.h"
#include <deque>
#include "userdata.h"
#include "logdao.h"
using namespace std;
/*系统提供的数据池类型*/
//extern deque<MatchedLogRec> datapool;
/*自定义数据池类型*/
extern UserData userdata;
void* customerthread::savedata(void* par)
{
	((customerthread*)par)->run();
}

void customerthread::run()
{
	LogDao logdao;
	logdao.connect("openlab/open123");
	//从数据池中取得数据并删除取得的数据并放入数据库中
	while(1)
	{
		//MatchedLogRec mlog = datapool.front();
		//datapool.pop_front();
		MatchedLogRec mlog = userdata.pop_data();
			
		/*将数据存入数据库*/
		logdao.savedata();
	}	
	logdao.commit();
	logdao.desconnect();
}
//创建线程
void customerthread::start()
{
	pthread_create(&thid,0,savedata,this);
}
