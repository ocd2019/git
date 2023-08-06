#include <stdio.h>
#include <stdlib.h>
void stack_test1(int a, int b, int c);
void stack_test2(int a, int b, int c);

int main(void)
{
	int a = 1, b = 2, c = 3;
	
	printf("直接调用stack_test1函数：\n");
	stack_test1(a, b, c);
	printf("通过stack_test2函数间接调用stack_test1函数：\n");
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

/* 分配内存空间 */
void *malloc(size_t size);
/* 释放内存空间 */
void free(void *ptr);
/* 分配内存空间 */
void *calloc(size_t num, size_t size);
/* 重新分配内存空间 */
void *realloc(void *ptr, size_t size);





