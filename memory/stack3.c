#include <stdio.h>
#include <stdlib.h>
void stack_test1(int a, int b, int c);
void stack_test2(int a, int b, int c);

int main(void)
{
	int a = 1, b = 2, c = 3;
	
	printf("ֱ�ӵ���stack_test1������\n");
	stack_test1(a, b, c);
	printf("ͨ��stack_test2������ӵ���stack_test1������\n");
	stack_test2(a, b, c);
	
	return 0;
}

void stack_test1(int a, int b, int c)
{
	printf("a = %d, &a = %#x \n", a, (unsigned int)&a);
	printf("b = %d, &b = %#x \n", b, (unsigned int)&b);
	printf("c = %d, &c = %#x \n", c, (unsigned int)&c);
}

void stack_test2(int a, int b, int c)
{
	stack_test1(a, b, c);
}

int *stack_test3(void)
{
	int a;
	/* ...... */
	return (&a);
}

/* �����ڴ�ռ� */
void *malloc(size_t size);
/* �ͷ��ڴ�ռ� */
void free(void *ptr);
/* �����ڴ�ռ� */
void *calloc(size_t num, size_t size);
/* ���·����ڴ�ռ� */
void *realloc(void *ptr, size_t size);





