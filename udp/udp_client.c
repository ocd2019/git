#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "top_comm.h"

#define BUFF_LENGTH  1024

/*==================================================

编译完成之后，在可执行程序后面加上ip地址就可以
运行起来了如下:
./udp_client 192.168.0.130
===================================================*/
int main(int argc, char *argv[])
{
     S32 iSockfd;
     S32 iSendLen = 0;
     S32 strLen = 0;
     struct sockaddr_in stAddr;  /*网络地址结构*/
     S8 *pcIpString = argv[1];
     U16 usiPort = 8000;
     S8 sendBuf[BUFF_LENGTH];
     size_t iReceiveLen;

     if(argc < 2)
     {
        /*用户必须输入一个ip地址参数*/
        printf("argument err!\n");
        exit(1);
     }
     if(argc > 2)
        usiPort = atoi(argv[2]);

    /*初始化函数*/
     memset(&stAddr,0, sizeof(stAddr));
    /*AF_INET网际协议，因特网(internet )就是一个网际网*/
     stAddr.sin_family = AF_INET;
     stAddr.sin_port = htons(usiPort);/*把主机字节序转换为网络字节序*/
     /*将ip地址字符串转换为网络地址*/
     inet_pton(AF_INET, pcIpString, &stAddr.sin_addr.s_addr);
     /*SOCK_DGRAM,表示数据报套接字，即UDP套接字*/
     iSockfd = socket(AF_INET, SOCK_DGRAM, 0);

     if(iSockfd < 0)
     {
         printf("Invalid socket\n");
         exit(1);
     }
     while(1)
     {
        printf("please input>");
        if (fgets(sendBuf, BUFF_LENGTH, stdin) == NULL) 
        {
            printf("fgets error\n");
            return RET_FAIL;
        }
        strLen = strlen(sendBuf);
        /*把数据发送到服务器*/
        iSendLen = sendto(iSockfd, sendBuf, strLen, 0, (struct sockaddr*)&stAddr, sizeof(stAddr));
        if(iSendLen != strLen)
        {
            printf("error : send length :%d,%d\n",iSendLen,strLen);
            break;
        }
        memset(sendBuf,0,BUFF_LENGTH);
        /*接收来自服务器的数据*/
        iReceiveLen = recvfrom(iSockfd, sendBuf, BUFF_LENGTH, 0, NULL, NULL);
        if(iReceiveLen <= 0)
        {
            printf("error : receive  length :%d\n",iReceiveLen);
            break;            
        }
        printf("receive from server:%s\r\n", sendBuf);
     }

     close(iSockfd);
     return 0;
}

