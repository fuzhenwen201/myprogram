#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>

class mythread:public QThread
{
	Q_OBJECT
	public:
		virtual void run();
		/*自定义信号通知界面发送了哪些数据*/
	public:signals:
	void sigUi(QString);
};

#endif
