#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


/*Hex transform to ascii string*/
int hex_to_char(uint8_t *src, char *dest, int len)
{
	int i = 0;

	char low_char = '\0';		//字符串的低位

	char hight_char = '\0';		//字符串的高位
    
    if(NULL == src || NULL == dest)
    	return -1;

    for(i = 0; i < len; i++)
    {
        if('9' < (low_char = '0' + src[i] / 16))
        	low_char += 7;

        if('9' < (hight_char = '0' + src[i] % 16))
        	hight_char += 7;
		
        dest[2 * i] = low_char;

        dest[2 * i + 1] = hight_char;
	}

	dest[2 * i] = '\0';

	return 0;
}


int main(int argc, char argv[])
{
	char result[50] = {0};
	
	char *test = "\xe6\xb5\x8b\xe8\xaf\x95";

	printf("test is %s\n", test);

    //hex_to_char(test, result, strlen(test));

	printf("result is %s\n", result);

	printf("strlen(test) is %ld\n", strlen(test));

	for(int i = 0; i < strlen(test); i++)
	{
		printf("%d is %#x\n", i, *(test + i));

		result[i] = *(test + i);
	}

	printf("result is %s\n", result);

    return 0;
}


