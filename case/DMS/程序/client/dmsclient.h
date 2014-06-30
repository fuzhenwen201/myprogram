#ifndef DMSCLIENT_H
#define DMSCLIENT_H

#include <QTextBrowser>
#include <QDialog>
#include <QPushButton>
#include "mythread.h"
#include <QString>
class DmsClient:public QDialog
{
	Q_OBJECT
			QTextBrowser *showdata;
			QPushButton *bsend;
			QPushButton *bclose;
			/*操作界面的线程*/
			mythread *myth;
	public:
			DmsClient();
			~DmsClient();
			public slots:
				void sendAndClose();
			/*接收线程发送的数据*/
			public slots:
				void recivedata(QString data);
};
#endif
