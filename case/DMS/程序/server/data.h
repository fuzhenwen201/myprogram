#ifndef DATA_H
#define DATA_H
	/*登入登出记录类型*/
	struct LogRec
	{
		char logname[32];
		int pid;
		short logtype;
		int logtime;
		char logip[257];
	};
	
	/*匹配记录类型*/
	struct MatchedLogRec
	{
		char logname[32];
		int pid;
		int logintime;
		int logoutime;
		int durations;
		char logip[257];
		/*出租的服务器的ip*/
		char serverip[20];
	};

struct Logstruct
{
	char logname[32];
	int initid;
	char deviname[32];
	int pid;
	short logtype;
	short process;
	short exsta;
	int logtime;
	int mic;
	int sessin;
	char reserved[20];
	short signifi;
	char logip[257];
};
#endif
