#ifndef CUSTOMERTHREAD_H
#define CUSTOMERTHREAD_H

#include <pthread.h>
/*消费者线程 负责从数据池中取得数据并且调用相关的类LogDao 把数据存入数据库中*/
class customerthread
{
	pthread_t thid;
	public:
	static void* savedata(void* par);
	virtual void run();
	void start();
};

#endif
