#include <math.h>
#include <stdio.h>
#include <iconv.h>
#include <string.h>


int gbk_convert_unicode(unsigned char *gbk_buf, unsigned int *unicode_buf, int buf_size)
{
	unsigned int *uni_ptr = unicode_buf;
	unsigned char *gbk_ptr = gbk_buf;
	unsigned int uni_number = 0;
	unsigned int uni_index = 0;
	unsigned int gbk_index = 0;
	unsigned short word;
	unsigned char ch;

	while(1)
	{
		ch = *(gbk_ptr + gbk_index);

		if(0x00 == ch || uni_number > buf_size - 1){
			break;
		}
		
		if (0 == (ch & 0x80)) 
		{
			*(unicode_buf + uni_index) = *(gbk_ptr + gbk_index);

			uni_index++;
			uni_number++;
			gbk_index += 1;
		} 
		else if(0xC0 == (ch & 0xE0))  ///< 110x-xxxx 10xx-xxxx
		{
			word = (*(gbk_ptr + gbk_index) & 0x3F) << 6;
			word |= (*(gbk_ptr + gbk_index + 1) & 0x3F);

			*(unicode_buf + uni_index) = word;

			uni_index++;
			uni_number++;
			gbk_index += 2;
		}
		else if(0xE0 == (ch & 0xF0))  ///< 1110-xxxx 10xx-xxxx 10xx-xxxx
		{
			word = (*(gbk_ptr + gbk_index) & 0x1F) << 12;
			word |= (*(gbk_ptr + gbk_index + 1) & 0x3F) << 6;
			word |= (*(gbk_ptr + gbk_index + 2) & 0x3F);

			*(unicode_buf + uni_index) = word;

			uni_index++;
			uni_number++;
			gbk_index += 3;
		} 
		else if(0xF0 == (ch & 0xF8))  ///< 1111-0xxx 10xx-xxxx 10xx-xxxx 10xx-xxxx 
		{
			word = (*(gbk_ptr + gbk_index) & 0x0F) << 18;
			word = (*(gbk_ptr + gbk_index + 1) & 0x3F) << 12;
			word |= (*(gbk_ptr + gbk_index + 2) & 0x3F) << 6;
			word |= (*(gbk_ptr + gbk_index + 3) & 0x3F);

			*(unicode_buf + uni_index) = word;

			uni_index++;
			uni_number++;
			gbk_index += 4;
		} 
		else   ///< 1111-10xx 10xx-xxxx 10xx-xxxx 10xx-xxxx 10xx-xxxx 
		{
			word = (*(gbk_ptr + gbk_index) & 0x07) << 24;
			word = (*(gbk_ptr + gbk_index + 1) & 0x3F) << 18;
			word = (*(gbk_ptr + gbk_index + 2) & 0x3F) << 12;
			word = (*(gbk_ptr + gbk_index + 3) & 0x3F) << 6;
			word = (*(gbk_ptr + gbk_index + 4) & 0x3F);

			*(unicode_buf + uni_index) = word;

			uni_index++;
			uni_number++;
			gbk_index += 5;
		}
	}

	return uni_number;
}


int main(void)
{
	int i = 0;
	int number = 0;
	unsigned int utf16_buf[128] = {0};
	unsigned char gbk_buf[128] = "IPC测试数据";

	number = gbk_convert_unicode(gbk_buf, utf16_buf, sizeof(utf16_buf));

	for(i = 0; i < number; i++){
		printf("%x\n", utf16_buf[i]);
	}

	return 0;
}


