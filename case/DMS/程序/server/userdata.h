#ifndef USERDATA_H
#define USERDATA_H

#include "data.h"
#include <deque>
#include <pthread.h>
using namespace std;
/*用户自定义的数据缓冲池类型 具备线程安全的能力 和 生产消费协调的能力*/
class UserData
{
	/*真正存储数据的数据池*/
	deque<MatchedLogRec> datas;
	/*保证线程安全的互斥量*/
	pthread_mutex_t mutex;
	/*保证生产和消费协调的条件量*/
	pthread_cond_t p_con;
	pthread_cond_t c_con;
	public:
	UserData();
	~UserData();
	/*提供线程安全的 放入数据和取数据的函数*/
	void push_data(MatchedLogRec mlog);
	MatchedLogRec pop_data();

};

#endif
