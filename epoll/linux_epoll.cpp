#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <poll.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <errno.h>
#include <iostream>
#include <stdio.h>

using namespace std;

#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE      10

int setnonblocking(int fd)
{
	int old_option = fcntl(fd, F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl(fd, F_SETFL, new_option);
	return old_option;
}

void addfd(int epollfd, int fd, bool enable_et)
{
	epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLIN;
	if (enable_et)
	{
		event.events |= EPOLLET;
	}
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
	setnonblocking(fd);
}

void lt(epoll_event* events, int number, int epollfd, int listenfd)
{
	char buf[BUFFER_SIZE];
	for (int i = 0; i < number; i++)
	{
		int sockfd = events[i].data.fd;
		if (sockfd == listenfd)
		{
			struct sockaddr_in clientaddr;
			socklen_t clientaddrlen = sizeof(clientaddr);
			int connfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
			addfd(epollfd, connfd, false);		
		} 
		else if (events[i].events & EPOLLIN)
		{
			printf("event trigger once\n");
			memset(buf, 0, BUFFER_SIZE);
			int ret = recv(sockfd, buf, BUFFER_SIZE - 1, 0);
			if (ret <= 0)
			{
				close(sockfd);
				continue;
			}
			printf("get %d bytes of content: %s\n", ret, buf);
		}
		else
		{
			printf("something else happened\n");
		}
	}	
}

void et(epoll_event* events, int number, int epollfd, int listenfd)
{
	char buf[BUFFER_SIZE];
	for (int i = 0; i < number; i++)
	{
		int sockfd = events[i].data.fd;
		if (sockfd == listenfd)
		{
			struct sockaddr_in clientaddr;
			socklen_t clientaddrlen = sizeof(clientaddr);
			int connfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
			addfd(epollfd, connfd, false);		
		} 
		else if (events[i].events & EPOLLIN)
		{
			printf("event trigger once\n");
			while (1)
			{
				memset(buf, 0, BUFFER_SIZE);
				int ret = recv(sockfd, buf, BUFFER_SIZE - 1, 0);
				if (ret <= 0)
				{
					if (errno == EAGAIN || errno == EWOULDBLOCK)
					{
						printf("read later\n");
						break;
					}
					else if (errno == EINTR)
					{
						continue;
					}
					
					close(sockfd);
					break;
				}
				else if (ret == 0)
				{
					close(sockfd);
					break;
				}
				else
				{
					printf("get %d bytes of content: %s\n", ret, buf);
				}
			}
			
		}
		else
		{
			printf("something else happened\n");
		}
		
	}	
}



int main()
{
    //创建一个监听socket
    int listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(listenfd == -1)
    {
        cout << "create listen socket error" << endl;
        return -1;
    }
    //将监听socker设置为非阻塞的
    int oldSocketFlag = fcntl(listenfd, F_GETFL, 0);
    int newSocketFlag = oldSocketFlag | O_NONBLOCK;
    if(fcntl(listenfd,F_SETFL,newSocketFlag)==-1) 
    {
        close(listenfd);
        cout << "set listenfd to nonblock error" << endl;
        return -1;
    }

    //初始化服务器地址
    struct sockaddr_in bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bindaddr.sin_port = htons(3000);

    if(bind(listenfd, (struct sockaddr*)&bindaddr,sizeof(bindaddr))==-1)
    {
        cout << "bind listen socker error." << endl;
        close(listenfd);
        return -1;
    }
    
    //启动监听
    if(listen(listenfd,SOMAXCONN)==-1)
    {
        cout << "listen error." << endl;
        close(listenfd);
        return -1;
    }
     
    epoll_event events[MAX_EVENT_NUMBER];
	
	//创建epollfd
    int epollfd = epoll_create(1);
    if(epollfd == -1)
    {
        cout << "create epollfd error." << endl;
        close(listenfd);
        return -1;
    }
	addfd(epollfd, listenfd, true);
	
    while(true)
    {
        
        int ret = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        if(ret < 0)
        {
            //被信号中断
            if(errno == EINTR) 
				continue;
            //出错,退出
			printf("epoll failed\n");
            break;
        }
		
		//lt(events, ret, epollfd, listenfd);
		et(events, ret, epollfd, listenfd);
	}
	
	close(listenfd);
	return 0;
}