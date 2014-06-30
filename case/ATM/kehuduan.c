#include "head.h"
int main()
{
    key_t key = ftok(".",25);
    int msgid = msgget(key,0);
    if(msgid==-1)perror("zmsgget"),exit(-1);
    printf("*************************************\n");
    printf("*             客  户  端            *\n");
    printf("*                                   *\n");
    printf("*        1.开户        2.存款       *\n");
    printf("*                                   *\n");
    printf("*        3.转账        4.取款       *\n");
    printf("*                                   *\n");
    printf("*        5.查询        6.销户       *\n");
    printf("*                                   *\n");
    printf("*        7.退出     请选择服务项目  *\n");
    printf("*************************************\n");
    int ch = 0,res = 0;
    scanf("%d",&ch);
    getchar();
    struct Message mess;
    struct MSG msg;
    switch(ch)
    {
        case 1:
            msg.type = OPENAC;
            printf("姓名：");
            scanf("%s",msg.mess.name);
            scanf("%*[^\n]");
            scanf("%*c");
            printf("密码：");
            scanf("%s",msg.mess.password);
            scanf("%*[^\n]");
            scanf("%*c");
            printf("金额：");
            scanf("%d",&msg.mess.amout);
            scanf("%*[^\n]");
            scanf("%*c");

            res = msgsnd(msgid,&msg,sizeof(mess),0);
            if(!res)printf("发送成功,请稍候...\n");
            else printf("发送失败\n");
    }
    return 0;
}
