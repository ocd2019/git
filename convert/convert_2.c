#include <math.h>
#include <stdio.h>
#include <iconv.h>
#include <string.h>


int utf8_convert_utf16(char **fromstr, size_t fromlen, char **tostr, size_t tolen)
{
	iconv_t cd = iconv_open("utf-16le", "utf-8");
	
	iconv(cd, fromstr, &fromlen, tostr, &tolen);

	iconv_close(cd);

	return 0;
}


int main(void)
{
	int i = 0;
	char utf16[128] = {0};
	char utf8[128] = "中视data";
	unsigned int unicode_buf[128] = {0};

	char* utf16_src = utf16;
	char* utf8_src = utf8;
	int utf16_len = 0;

	utf8_convert_utf16(&utf8_src, strlen(utf8), &utf16_src, sizeof(utf16));

	unsigned short *ptr = (unsigned short *)(utf16);
	utf16_len = (int)(utf16_src - utf16) / 2;
	while((unicode_buf[i] = ptr[i])) i++;

	printf("utf16_len:%d\n", utf16_len);

	for(int i = 0; i < utf16_len; i++)
	{
		printf("%x ", unicode_buf[i]);
	}

	printf("\n");

	return 0;
}


