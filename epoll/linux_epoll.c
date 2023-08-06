#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <poll.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE      10

int set_non_block(int fd)
{
	int old_option = fcntl(fd, F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl(fd, F_SETFL, new_option);

	return old_option;
}

void add_fd(int epollfd, int fd, bool enable_et)
{
	struct epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLIN;
	if (enable_et)
	{
		event.events |= EPOLLET;
	}
	
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);

	set_non_block(fd);

	return ;
}

void level_trigger(struct epoll_event* events, int number, int epollfd, int listenfd)
{
	char buf[BUFFER_SIZE] = {0};

	for (int i = 0; i < number; i++)
	{
		int sockfd = events[i].data.fd;
		if (sockfd == listenfd)
		{
			struct sockaddr_in clientaddr;
			socklen_t clientaddrlen = sizeof(clientaddr);
			int connfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
			add_fd(epollfd, connfd, false);	
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

	return ;
}

void edge_trigger(struct epoll_event* events, int number, int epollfd, int listenfd)
{
	char buf[BUFFER_SIZE] = {0};

	for (int i = 0; i < number; i++)
	{
		int sockfd = events[i].data.fd;
		if (sockfd == listenfd)
		{
			struct sockaddr_in clientaddr;
			socklen_t clientaddrlen = sizeof(clientaddr);
			int connfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
			add_fd(epollfd, connfd, false);		
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

	return ;
}



int main(void)
{
    //创建一个监听socket
    int listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(listenfd == -1)
    {
        printf("create listen socket error\n");
        return -1;
    }
    //将监听socker设置为非阻塞的
    int oldSocketFlag = fcntl(listenfd, F_GETFL, 0);
    int newSocketFlag = oldSocketFlag | O_NONBLOCK;
    if(fcntl(listenfd,F_SETFL,newSocketFlag)==-1) 
    {
        close(listenfd);
        printf("set listenfd to nonblock error\n");
        return -1;
    }

    //初始化服务器地址
    struct sockaddr_in bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bindaddr.sin_port = htons(3000);

    if(bind(listenfd, (struct sockaddr*)&bindaddr,sizeof(bindaddr))==-1)
    {
        printf("bind listen socker error\n");
        close(listenfd);
        return -1;
    }
    
    //启动监听
    if(listen(listenfd,SOMAXCONN)==-1)
    {
		printf("listen error\n");
        close(listenfd);
        return -1;
    }
     
    struct epoll_event events[MAX_EVENT_NUMBER] = {0};
	
	//创建epollfd
    int epollfd = epoll_create(1);
    if(epollfd == -1)
    {
		printf("create epollfd error\n");
        close(listenfd);
        return -1;
    }

	add_fd(epollfd, listenfd, true);
	
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
		
		//level_trigger(events, ret, epollfd, listenfd);
		edge_trigger(events, ret, epollfd, listenfd);
	}
	
	close(listenfd);

	return 0;
}


