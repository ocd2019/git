#include<stdio.h>
#include<string.h>
#include<ctype.h>

int main()
{
	char *test = "\xe4\xb8\xad\xe8\xa7\x86";

	printf("test is %s\n", test);
   
	for(int i = 0; i < strlen(test); i++)
	{
		if(0 != isascii(*(test + i)))
		{
			printf("%c is ascii\n",*(test + i));
		}
		else
		{
			printf("%c is not ascii\n",*(test + i));
		}
	}

   return 0;
}

