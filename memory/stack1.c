#include <stdio.h>

int main(void)
{
	int a = 1, b = 2, c = 3;
	
	printf("a = %d, &a = %#x \n", a, (unsigned int)&a);
	printf("b = %d, &b = %#x \n", b, (unsigned int)&b);
	printf("c = %d, &c = %#x \n", c, (unsigned int)&c);
	
	return 0;
}
