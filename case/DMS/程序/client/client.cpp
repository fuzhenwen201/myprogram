#include "client.h"
#include <iostream>
using namespace std;

void Client::sendlog()
{
	list<MatchedLogRec> match = logread.readLogs();
    try{
       logsend.sendMatchs(&match);
       }catch(DmsInitNetWorkException& e){
	       cout << e.what() << endl;
	       cout << "以追加方式写日志文件" << endl;
	     }catch(DmsSenderDataException& e){
	        cout << e.what() << endl;
	        cout << "以截断方式写日志文件" << endl;
	     }
}

