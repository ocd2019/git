#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "top_comm.h"
#define BUFF_LENGTH  1024
#define SENDE_TO_CLIENT_STRING     "ok, server have received"
#define SEVER_ADD_PORT    8000      /*服务器端口号*/
/*==================================================

编译完成之后，可用如下命令运行:
./udp_server
===================================================*/

int main( )
{
    S32 iSockfd;
    S32 iBackStringLen = 0;
    char szRecvBuf[BUFF_LENGTH];
    S8 szClientIp[INET_ADDRSTRLEN];
    size_t recvLen,sendLen;
    size_t cliAddrLen;
    struct sockaddr_in stClientAddr;
    struct sockaddr_in stBindAddr;
     
    iSockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(iSockfd < 0)
    {
        printf("Invalid socket\n");
        exit(1);
    }
    memset(&stBindAddr,0, sizeof(struct sockaddr_in));
    memset(&stClientAddr,0, sizeof(struct sockaddr_in));
    stBindAddr.sin_family = AF_INET;
    stBindAddr.sin_port = htons(SEVER_ADD_PORT);
    stBindAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    /*绑定网口*/
    if(bind(iSockfd, (struct sockaddr*)&stBindAddr, sizeof(stBindAddr)) != 0)
    {
        close(iSockfd);
        printf("binding err!\n");
        exit(1);
    }
    iBackStringLen = strlen(SENDE_BACK_TO_CLIENT_STRING);
    cliAddrLen = sizeof(stClientAddr);
    while(1)
    {
        memset(szRecvBuf,0,BUFF_LENGTH);
        recvLen = recvfrom(iSockfd, szRecvBuf, BUFF_LENGTH, 0, (struct sockaddr*)&stClientAddr, &cliAddrLen);
        if(0>=recvLen)
        {
            printf("error: receive failed,error:%d,%s\n",errno,strerror(errno));
            break;
        }
        /*把发送数据的ip地址、端口号、收到的数据打印在屏幕上*/
        printf("%s:%d>%s\r\n", inet_ntop(AF_INET, &stClientAddr.sin_addr.s_addr, szClientIp, INET_ADDRSTRLEN),
                                            ntohs(stClientAddr.sin_port),
                                            szRecvBuf);
        sendLen = sendto(iSockfd, SENDE_TO_CLIENT_STRING, iBackStringLen, 0, (struct sockaddr*)&stClientAddr, cliAddrLen);
        if(0 >= sendLen)
        {
            printf("error: send failed, error:%d,%s\n",errno,strerror(errno));
            break;
        }
    }
    //套接字不用的时候，记得关它
    close(iSockfd);
    return 0;
}


