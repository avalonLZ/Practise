/*************************************************************************
	> File Name: simplesocket.c
	> Author:lizhong 
	> Mail: 
	> Created Time: Thu 09 Mar 2017 11:02:12 PM PST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>//路径:/usr/include/
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

#define DPORT 3333
#define SIZE 1000

typedef struct
{
    char *prdata[1024];
    int index;
    int selstate;
    int reerror;
    int lastsize;
}Redata_t;

int Select(int confd, int outtime)
{
    fd_set fd;
    struct timeval time;
    int ts  = 0;

    time.tv_sec = 0;//0秒
    time.tv_usec = 100;//500毫秒

    //初始化文件描述符集合
    FD_ZERO(&fd);

    //向文件描述符集合中增加一个文件描述符
    FD_SET(confd, &fd);

    if(outtime == 0)
    {
        /*最后一个time参数设置为空意味着阻塞，直到有收到东西*/
        ts = select(confd + 1, &fd, NULL, NULL, NULL);
        //printf("Select Return:%d\r\n", ts);
    }
    else if(outtime == 1)
    {
        ts = select(confd + 1, &fd, NULL, NULL, &time);
        //printf("Select Outtime\r\n");
    }
    else
    {
        printf("Select Param Error\r\n");
    }
    
    if(ts)
        return 1;
    else
        return 0;
}

Redata_t My_Recv(int tempfd)
{
    Redata_t temredata = {0};
    int temsize = 0;
    int tempsel = 0;
    struct timeval time;
    char *tempdata;

    //接收超时机制，判断一帧数据结束
    time.tv_sec = 0;//0秒
    time.tv_usec = 500;//500毫秒
    setsockopt(tempfd, SOL_SOCKET, SO_RCVTIMEO, &time, sizeof(time));

    while(1)
    {
        tempdata = malloc(SIZE);
        memset(tempdata, 0, SIZE);

        temredata.selstate = Select(tempfd, 0); 
        temsize = recv(tempfd, tempdata, SIZE, MSG_WAITALL);

        if(temsize == -1)
        {
            free(tempdata);
            printf("Recv Error free tempdata\r\n");
            temredata.reerror = 1;
            return temredata;
        }
        temredata.prdata[temredata.index++] = tempdata;

        if (temsize < SIZE)
        {
            temredata.index--;
            temredata.lastsize = temsize;
            printf("Received Packed End\r\n");
            return temredata;
        }
        else if(temsize == SIZE)
        {
            //防止刚好对方发送了SIZE的整数倍数据
            tempsel = Select(tempfd, 1);

            if(tempsel == 0)
            {
                temredata.index--;
                temredata.lastsize = temsize;
                printf("Select Outtime Received Packed End\r\n");
                return temredata;
            }
        }
        else
        {
            printf("Recv Error\r\n");
        }
    }
}

static void FreeRedata(Redata_t *temredata)
{
    int i = 0;

    for(i = 0;i < temredata->index + 1; ++i)
    {
        free(temredata->prdata[i]);
        printf("free myrecv.prdata[%d]\r\n", i);
    }
}

static unsigned long int CountDataSize(const Redata_t *temredata)
{
    int i = 0;
    unsigned long int temlen = 0;
    
    for(i = 0;i < temredata->index; ++i)
        temlen += SIZE; 
    temlen += temredata->lastsize;
                
    printf("index = %d, lastsize = %d, temlen = %ld\r\n", 
            temredata->index, temredata->lastsize, temlen);
    
    return temlen;
}

static void CopyData(char *temsedata, const Redata_t *temredata)
{
    int i = 0;
    char *temseindex = temsedata;
    
    for(i = 0; i < temredata->index; ++i)
    {
        memcpy(temseindex, temredata->prdata[i], SIZE);
        temseindex += SIZE;
    }
    memcpy(temseindex, temredata->prdata[temredata->index], temredata->lastsize);
}

int main()
{
    int sfd,confd;
    struct sockaddr_in serveradd;
    char *rebuff;
    char *sebuff;
    int j = 0,k = 0, sel = 0;
    unsigned long int relen = 0;
    Redata_t myrecv = {0};
    
    /*初始化Socket,返回socket的文件描述符*/
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if( sfd == -1 )
    {
        printf("Created Socket Error:%s\r\n", strerror(errno));
        exit(0);
    }
    /*配置本服务器地址参数*/
    memset(&serveradd, 0, sizeof(struct sockaddr_in));
    serveradd.sin_family = AF_INET;
    /*系统自动获取本机IP*/
    serveradd.sin_addr.s_addr = htonl(INADDR_ANY);
    /*设置监听端口为DPORT*/
    serveradd.sin_port = htons(DPORT);
    
    /*Socket和端口绑定*/
    j = bind(sfd, (struct sockaddr*)&serveradd, sizeof(struct sockaddr_in));
    
    if(j == -1)
    {
        printf("Bind Error :%s\r\n", strerror(errno));
        exit(0);
    }
    j = 0;
    /*开启监听客户端请求，（开闸）*/
    j = listen(sfd, 10);
    
    if(j == -1)
    {
        printf("Listen Error:%s\r\n", strerror(errno));
        exit(0);
    }
    j= 0;
    printf("***********************Wait Request**********************\r\n");
       
    /*接受客户端连接,此条语句有阻塞效果*/
     confd = accept(sfd, (struct sockaddr*)NULL, NULL);

    if(confd == -1)
    {
        printf("Accept Error:%s\r\n", strerror(errno));
        /*出错则结束*/
        return 0;
    }
    while(1)
    {
        relen = 0;
        memset(&myrecv, 0, sizeof(myrecv));
        myrecv = My_Recv(confd);

        if(myrecv.reerror != 1)
        {
            printf("*****************Received Data*****************\r\n");

            /*可以接收数据，但是数据长度却为0说明客户端断开了TCP连接*/
            if(myrecv.selstate == 1 && strlen(myrecv.prdata[0]) == 0)
            {
                printf("Socket is close!\r\n");
                free(myrecv.prdata[0]);
                printf("Socket is close free myrecv.prdata[0]\r\n");
                break;
            }
            else
            {
                //统计需要malloc多少内存
                relen = CountDataSize(&myrecv);
                sebuff = malloc(relen);
                
                //拷贝数据
                CopyData(sebuff, &myrecv);
                printf("Receive Data is :%s\r\n", sebuff);  
                FreeRedata(&myrecv);

                //一次性send大数据时系统会分段发送,和segsend测得的效果类似
                //效率没有segsend高
                send(confd, sebuff, relen, 0);
                    
                //free大动态内存时出错，打印查看发现有一段内存数据异常,验证发现是strcat的问题
                free(sebuff);
                printf("free sebuff\r\n");//free大数组时报错                
            }
        }
        else
        {
            printf("Receive Error is\r\n");
            break;
        }
        printf("***********************End*************************\r\n");
    }
    return 0;
}

