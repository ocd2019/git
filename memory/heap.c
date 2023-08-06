#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void heap_test1(void); // malloc ��free�ļ�Ӧ��
void heap_test2(void); // ������ڴ��ַ������С
void heap_test3(void); // calloc��malloc������
void heap_test4(void); // realloc��Ӧ��
void heap_test5(void); // ���ڴ���ȷʹ�÷���

//������
int main(void)
{
	heap_test4();
	return 0;
}

//malloc��free�ļ�Ӧ��
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

//������ڴ��ַ������С
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

//calloc��malloc����Ҫ����
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

//realloc��Ӧ��
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
	printf("relloc���·����ڴ�\n");
	pa = (int*)realloc(pa, sizeof(int)*10);
	if ( NULL != pa ){
		for ( i = 0; i < 10; i++ ){
			printf("pa[%d] = %d\n", i, pa[i]);
		}
		free(pa);
	}
	
	return;
}

//���ڴ���ȷʹ�÷���
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

//�ڴ�й©����
void heap_test6(void)
{
	char *pa;
	pa = (char*)malloc(sizeof(char)*20);
	/* ...... */
	
	return;
}

//Ұָ������
void heap_test6(void)
{
	char *pa;
	pa = (char*)malloc(sizeof(char)*20);
	/* ...... */
	free(pa);
	/* ...... */
	printf("pa = %s \n",pa); //Ұָ�뱻ʹ��
	
	return;
}
