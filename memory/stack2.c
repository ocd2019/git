#include <stdio.h>

void stack_test1(int a, int b, int c);

int main(void)
{
	int a = 1, b = 2, c = 3;
	int a1 = 4, b1 = 5, c1 = 6;
	
	printf("��һ�ε���stack_test1������\n");
	stack_test1(a, b, c);
	printf("�ڶ��ε���stack_test1������\n");
	stack_test1(a1, b1, c1);
	
	return 0;
}

void stack_test1(int a, int b, int c)
{
	printf("a = %d, &a = %#x \n", a, (unsigned int)&a);
	printf("b = %d, &b = %#x \n", b, (unsigned int)&b);
	printf("c = %d, &c = %#x \n", c, (unsigned int)&c);
}