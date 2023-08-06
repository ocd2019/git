//#include <iostream>
#include "unicodeTables.h"
#include "gbkTables.h"
#include <stdio.h>
//using namespace std;

//gbk->unicode
int GbkToUnicode(char *gbk_buf, unsigned short *unicode_buf, int max_unicode_buf_size)
{
    unsigned short word;
    unsigned char *gbk_ptr =(unsigned char *) gbk_buf;
    unsigned short *uni_ptr = unicode_buf;
	unsigned int uni_ind = 0, gbk_ind = 0, uni_num = 0;
	unsigned char ch;
    int word_pos;

    if( !gbk_buf || !unicode_buf )
        return -1;

    while(1)
    {
    	ch = *(gbk_ptr + gbk_ind);

		if(ch == 0x00)
			break;
		
        if( ch > 0x80 )
        {
			//��gbk����������ַ��������ֽ���ϳ�һ��    unsigned short word;
				word = *(gbk_ptr + gbk_ind);
				word <<= 8;
				word += *(gbk_ptr + gbk_ind+1);
				gbk_ind += 2;

            word_pos = word - gbk_first_code;
            if(word >= gbk_first_code && word <= gbk_last_code  && (word_pos < unicode_buf_size))
            {
				*(uni_ptr + uni_ind) = unicodeTables[word_pos];
				uni_ind++;
				uni_num++;
            }
        }
		else
		{
			gbk_ind++;
			*(uni_ptr + uni_ind) = ch;
			uni_ind++;
			uni_num++;
        }
        
        if(uni_num > max_unicode_buf_size - 1)
			break;
    }

    return uni_num;
}

//unicode->gbk
int UnicodeToGbk(unsigned short *unicode_buf, char *gbk_buf, int max_gbk_buf_size)
{
	unsigned short word;
	unsigned short gbk_word;
	unsigned char ch;
    unsigned char *gbk_ptr =(unsigned char *) gbk_buf;
    unsigned short *uni_ptr = unicode_buf;
	unsigned int uni_ind = 0, gbk_ind = 0, gbk_num = 0;
    int word_pos;

    if( !gbk_buf || !unicode_buf )
        return -1;

	while(1)
	{
		word = *(uni_ptr + uni_ind);
		uni_ind++;

		if(word == 0x0000)  //�ַ���������
			break;
		
		if(word < 0x80)  /*ASCII���ò��*/
		{
			*(gbk_ptr + gbk_ind) = (unsigned char)word;
			gbk_ind++;
		}
		else
		{
			word_pos = word - unicode_first_code;
			if(word >= unicode_first_code && word <= unicode_last_code && word_pos < gbk_buf_size)
			{
				gbk_word = gbkTables[word_pos];//gbk_word��gbk���룬����Ϊunsigned short���ͣ���Ҫ��ֳ������ֽ�

				*(gbk_ptr + gbk_ind) = (unsigned char)(gbk_word >> 8);//��ȡ��8λ
				gbk_ind++;
				*(gbk_ptr + gbk_ind) = (unsigned char)(gbk_word >> 0);//��ȡ��8λ
				gbk_ind++;
				gbk_num +=2;//gbk�ַ���2��
			}
		}

		if(gbk_num > max_gbk_buf_size - 1)
			break;
	}

    return gbk_num;
}

int main()
{
#if 1
	//1��gbk->unicode
	char *strGbk ="����ΰҵ";
	//char *strGbk ="a";
	unsigned short strUnicode[128] ={0x00};

	int num = GbkToUnicode(strGbk,strUnicode,128);
   
	for (int i = 0; i < num; i++)  
	{  
		printf("%x\n", strUnicode[i]); 
	}//���1162

#if 0
	unsigned char* p = (unsigned char*)strUnicode;     
	for (int i = 0; i < num*2; i++)  
	{  
		printf("%x\n", *p);
	    p++;  
	}//���1162

	printf("\n");
#endif

 #endif

#if 0
	//2��unicode->gbk
	unsigned short strUnicode[2] = {0x6211,0x00};//����0x00�ַ���������,��ֹ�����ڲ���ѭ��
	char strGbk[5]={0x00};

	int num = UnicodeToGbk(strUnicode,strGbk,5);

    unsigned char* p = (unsigned char*)strGbk;     
    for (int i = 0; i < num; i++)  
    {  
        printf("%0x", *p);  
        p++;  
    }//���ced2
    
    printf("\n");
#endif

    return 0;
}
