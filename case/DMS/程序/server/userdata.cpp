#include "userdata.h"

UserData::UserData()
{
	/*互斥量初始化*/
	pthread_mutex_init(&mutex,0);
	/*生产条件量的初始化*/
	pthread_cond_init(&p_con,0);
	/*消费条件量的初始化*/
	pthread_cond_init(&c_con,0);
}
UserData::~UserData()
{
	/*销毁互斥量*/
	pthread_mutex_destroy(&mutex);
	/*条件量的销毁*/
	pthread_cond_destroy(&p_con);
	pthread_cond_destroy(&c_con);
}

void UserData::push_data(MatchedLogRec mlog)
{
	/*上锁*/
	pthread_mutex_lock(&mutex);	
	while(datas.size()>10240)
	{
		/*等待直到可以生产*/
		pthread_cond_wait(&p_con,&mutex);
	}
	datas.push_back(mlog);
	//肯定可以消费,通知全部消费者
	pthread_cond_broadcast(&c_con);
	/*解锁*/
	pthread_mutex_unlock(&mutex);	
}

MatchedLogRec UserData::pop_data()
{
	pthread_mutex_lock(&mutex);	
	while(datas.empty()){
		/*等待消费条件直到被通知*/
	 pthread_cond_wait(&c_con,&mutex);
	}
	MatchedLogRec mlog = datas.front();
	datas.pop_front();
	/*肯定可以生产*/
	//pthread_cond_signal(&p_con);//只通知一个生产者
	pthread_cond_broadcast(&p_con);//全部通知,但只选一个

	pthread_mutex_unlock(&mutex);	
	return mlog;
}


















