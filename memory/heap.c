#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void heap_test1(void); // malloc 和free的简单应用
void heap_test2(void); // 后分配内存地址反而更小
void heap_test3(void); // calloc和malloc的区别
void heap_test4(void); // realloc的应用
void heap_test5(void); // 堆内存正确使用方法

//主函数
int main(void)
{
	heap_test4();
	return 0;
}

//malloc和free的简单应用
void heap_test1(void)
{
	int *pa;
	
	pa = (int*)malloc(sizeof(int));
	if ( NULL != pa )
	{
		*pa = 0x1234;
		printf("pa = %#x, *pa = %x\n", (unsigned int)pa, *pa);
		free(pa);
	}
	
	return;
}

//后分配内存地址反而更小
void heap_test2(void)
{
	void *pa;
	void *pb;
	void *pc;
	void *pd;
	pa = (int*)malloc(1024);
	printf("pa = %#x \n", (unsigned int)pa);
	pb = (int*)malloc(1024);
	printf("pb = %#x \n", (unsigned int)pb);
	pc = (int*)malloc(1024);
	printf("pc = %#x \n", (unsigned int)pc);
	free(pb);
	pd = (int*)malloc(1024);
	printf("pd = %#x \n", (unsigned int)pd);

	free(pa);
	free(pc);
	free(pd);
	
	return;
}

//calloc和malloc的主要区别
void heap_test3(void)
{
	unsigned int *pa;
	int i;
	
	pa = (unsigned int*)calloc(sizeof(unsigned int), 5);
	if ( NULL != pa )
	{
		printf("<< colloc pa = %#x >>\n", (unsigned int)pa);
		for ( i = 0; i < 5; i++ )
		{
			printf("pa[%d] = %d \n", i, pa[i]);
		}
		free(pa);
	}
	
	return;
}

//realloc的应用
void heap_test4(void)
{
	int *pa;
	int i;
	
	pa = (int*)malloc(sizeof(int)*6);
	if ( NULL != pa ){
		for ( i = 0; i < 6; i++ ){
			*(pa + i) = i;
		}
		for ( i = 0; i < 6; i++ ){
			printf("pa[%d] = %d \n", i, pa[i]);
		}
	}
	printf("relloc重新分配内存\n");
	pa = (int*)realloc(pa, sizeof(int)*10);
	if ( NULL != pa ){
		for ( i = 0; i < 10; i++ ){
			printf("pa[%d] = %d\n", i, pa[i]);
		}
		free(pa);
	}
	
	return;
}

//堆内存正确使用方法
void heap_test5(void)
{
	char *pa;
	
	pa = (char*)malloc(sizeof(char)*20);
	if ( NULL != pa )
	{
		strcpy(pa, "memory leak");
		printf("pa = %x \n", (unsigned int)pa);
		printf("pa = %s \n", pa);
	}
	free(pa);
	pa = NULL;
	if ( NULL != pa )
	{
		printf("pa = %s \n", (unsigned int)pa);
	}
	printf("pa = %s \n", pa);

	return;
}

//内存泄漏例子
void heap_test6(void)
{
	char *pa;
	pa = (char*)malloc(sizeof(char)*20);
	/* ...... */
	
	return;
}

//野指针例子
void heap_test6(void)
{
	char *pa;
	pa = (char*)malloc(sizeof(char)*20);
	/* ...... */
	free(pa);
	/* ...... */
	printf("pa = %s \n",pa); //野指针被使用
	
	return;
}
