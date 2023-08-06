#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>

#define gettid() syscall(__NR_gettid)

static void SignalHandle(int signal)
{
	printf("\033[31m");
    if (signal == SIGPIPE)
    {
        printf("SignalHandle: SIGPIPE, tid=[%d], signal=[%d]\n", (int)gettid(), signal);
    }
    else if (signal == SIGCHLD)
    {
        int status = 0;
        waitpid(-1, &status, 0);
        printf("children exit status=[%d], tid=[%d], signal=[%d]\n", status, (int)gettid(), signal);
    }
    else
    {
		printf("The exception, tid=[%d], signal=[%d]\n", (int)gettid(), signal);
		exit(EXIT_FAILURE);
    }
	printf("\033[0m"); 
}


static int CaptureSignalRegister()
{
	for (int i = 1; i < 31; i++)
	{
		if (i != 17 && i != 28)
		{
			signal(i, SignalHandle);
		}
	}
	
    return 0;
}


int main(void)
{
	while(1)
	{
		sleep(3);

		CaptureSignalRegister();
	}
	
	return 0;
}

