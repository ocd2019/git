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

�������֮���ڿ�ִ�г���������ip��ַ�Ϳ���
��������������:
./udp_client 192.168.0.130
===================================================*/
int main(int argc, char *argv[])
{
     S32 iSockfd;
     S32 iSendLen = 0;
     S32 strLen = 0;
     struct sockaddr_in stAddr;  /*�����ַ�ṹ*/
     S8 *pcIpString = argv[1];
     U16 usiPort = 8000;
     S8 sendBuf[BUFF_LENGTH];
     size_t iReceiveLen;

     if(argc < 2)
     {
        /*�û���������һ��ip��ַ����*/
        printf("argument err!\n");
        exit(1);
     }
     if(argc > 2)
        usiPort = atoi(argv[2]);

    /*��ʼ������*/
     memset(&stAddr,0, sizeof(stAddr));
    /*AF_INET����Э�飬������(internet )����һ��������*/
     stAddr.sin_family = AF_INET;
     stAddr.sin_port = htons(usiPort);/*�������ֽ���ת��Ϊ�����ֽ���*/
     /*��ip��ַ�ַ���ת��Ϊ�����ַ*/
     inet_pton(AF_INET, pcIpString, &stAddr.sin_addr.s_addr);
     /*SOCK_DGRAM,��ʾ���ݱ��׽��֣���UDP�׽���*/
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
        /*�����ݷ��͵�������*/
        iSendLen = sendto(iSockfd, sendBuf, strLen, 0, (struct sockaddr*)&stAddr, sizeof(stAddr));
        if(iSendLen != strLen)
        {
            printf("error : send length :%d,%d\n",iSendLen,strLen);
            break;
        }
        memset(sendBuf,0,BUFF_LENGTH);
        /*�������Է�����������*/
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

