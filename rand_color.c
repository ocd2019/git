#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int main(int argc, char *argv[])
{
	unsigned int line_color = 0;
	srand((unsigned int )time(NULL));
	for(int i = 0; i < 3; i++)
	{
		unsigned char tmp = (unsigned char )rand() % 256;
		printf("tmp:%#x\n", tmp);
		
		line_color += tmp << (32 - (i + 2) * 8);
		printf("line_color:%#x\n", line_color);
	}

	return 0;
}

