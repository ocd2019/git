#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#ifndef ROUND_UP
#define ROUND_UP(x, n)  ( ((x)+(n)-1u) & ~((n)-1u) )
#define ROUND_DOWN(x, n)  ( (x) & ~((n)-1u) )
//#endif

#define ALIGN_DOWN(x, a) ((x)-(x)%a)
#define ALIGN_UP(x, a) ((x)+(a)-(x)%a)

#define __ALIGN 2
 
size_t RoundUp(size_t num){
  return ((num + (__ALIGN - 1)) & ~(__ALIGN));
}


int main(int argc, char *argv[])
{
	printf("%d\n", ALIGN_UP(65, 2));
	printf("%d\n", ALIGN_DOWN(65, 2));

	printf("####################\n");

	printf("%d\n", ROUND_UP(180 / 200, 2));
	printf("%d\n", ROUND_DOWN(200 / 180, 2));

	printf("%ld\n", RoundUp(3));

	return 0;
}

