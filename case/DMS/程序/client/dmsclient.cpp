#include "dmsclient.h"
DmsClient::DmsClient()
{

	this->resize(400,500);
	showdata = new QTextBrowser(this);
	bsend = new QPushButton("send",this);
	bclose = new QPushButton("close",this);
	showdata->resize(360,400);
	showdata->move(20,20);
	bsend->move(50,450);
	bclose->move(250,450);
	/*创建线程*/
	myth = new mythread();
	/*绑定线程和接收数据的槽函数*/
	connect(myth,SIGNAL(sigUi(QString)),this,SLOT(recivedata(QString)));
	//点击发送按钮 显示发送的数据
	connect(bsend,SIGNAL(clicked()),this,SLOT(sendAndClose()));
	connect(bclose,SIGNAL(clicked()),this,SLOT(close()));
}

DmsClient::~DmsClient()
{
	delete showdata;
	delete bsend;
	delete bclose;
}
 void DmsClient::recivedata(QString data)
{
	showdata->append(data);
}

void DmsClient::sendAndClose()
{
	/*启动线程*/
	myth->start();
}
