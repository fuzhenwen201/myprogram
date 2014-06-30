#include "head.h"

int main()
{
	while(1){
    printf("***************************\n");
    printf("**        服 务 器       **\n");
    printf("**                       **\n");
    printf("**     傅家银行欢迎您    **\n");
    printf("**                       **\n");
    printf("**                       **\n");
    printf("**     进入客户端请按1   **\n");
    printf("**                       **\n");
    printf("***************************\n");
    int ch = 0;
    scanf("%d",&ch);
    getchar();
    while(ch==1){
    key_t key = ftok(".",25);
    int msgid = msgget(key,IPC_CREAT|0600);
    if(msgid==-1)perror("msgget"),exit(-1);

    pid_t pid = vfork();
    if(!pid)
    {//子进程
        execl("./b.out","b.out",NULL);
        exit(0);
    }
    
    int status;
    pid_t pid1 = wait(&status);
    printf("回车键继续\n",pid1);
    getchar();
    struct MSG msg;
    struct Message mess;    
    int res = msgrcv(msgid,&msg,sizeof(msg.mess),0,0);
    if(res==-1)perror("msgrcv"),exit(-1);
    
    int fd = open("number.txt",O_RDWR);
    if(!fd)perror("open1"),exit(-1);
    char num[15] = {};
    read(fd,num,sizeof(num));
    sscanf(num,"%d",&mess.id);


    int fd1 = open(num,O_RDWR|O_CREAT|O_TRUNC,0600);
    if(!fd1)perror("open2"),exit(-1);
    char num1[60] = {};
    sprintf(num1,"id:%d,姓名:%s,密码:%s,金额:%d\n",mess.id,msg.mess.name,msg.mess.password,msg.mess.amout);
    write(fd1,num1,strlen(num1));
    close(fd1);

	sprintf(num,"%d",mess.id+1);
	lseek(fd,0,SEEK_SET);
	write(fd,num,strlen(num));
    close(fd);
    
   /* fd = open("number.txt",O_WRONLY|O_TRUNC);
    if(!fd)perror("open3"),exit(-1);
    sprintf(num,"%d",mess.id+1);
    write(fd,num,strlen(num));
    close(fd);
	*/
    msgctl(msgid,IPC_RMID,0);
    }
	}
    return 0;
}















