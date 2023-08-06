/*----------------------------------------------------------------------------------------
	
	Program Explain：进制转换
    Create Date：2018.9.27 by lzn
	note:获取更多程序及编程笔记可搜索微信公众号——zhengnian-2018或正念君编程学习笔记
----------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define N sizeof(unsigned int)*8
#define uint unsigned int

#define   TWO_SCALE         2
#define   EIGHT_SCALE       8
#define   SIXTEEN_SCALE     16

// 十进制转换为其他进制
int deci_to_other_scale(uint deci_num, int scale, char s[]);
// 二进制转换为其他进制
int binary_to_other_scale(char *binary_num, int scale, char s[]);
// 十六进制转换为其他进制
int hex_to_other_scale(char *hex_num, int scale, char s[]);
// 八进制转换为其他进制
int oct_to_other_scale(char *oct_num, int scale, char s[]);
// 其他进制转换为十进制
int other_scale_to_deci(char *other_num, int scale);
// 另一种二进制转换为十进制的方法（不适用于较大的数据）
int binary_to_deci(uint binary_num);

// 四则运算
//..........

// 测试函数
void Test1(void);	// 测试十进制转换为其他进制
void Test2(void);	// 测试二进制转换为其他进制
void Test3(void);	// 测试十六进制转换为其他进制
void Test4(void);	// 测试八进制转换为其他进制
void Test5(void);	// 测试其他进制转换为十进制

/*********************************************************************************
* Function Name    ：main，主函数
* Parameter		   ：void
* Return Value     ：0
* Function Explain ： 
* Create Date      ：2018.9.27
**********************************************************************************/
int main(void)
{
	Test2();
	
	return 0;
}

/*********************************************************************************
* Function Name    ： deci_to_other_scale,将无符号整数n翻译成d（2<＝scale<＝16）进制表示的字符串s
* Parameter		   ： num:要转换的数   scale:进制数   s:保存转换后的数
* Return Value     ： 0：错误  其他：成功
* Function Explain ： 
* Create Date      ： 2018.9.27
**********************************************************************************/
int deci_to_other_scale(uint deci_num, int scale, char s[])
{
	static char ch[] = "0123456789ABCDEF";
	char buf[N + 1];
	int i = N, j;
	
	if ( !(scale >= 2 && scale <= 16) )
	{
		s[0] = '\0';
		return 0;
	}

	buf[N] = '\0';
	do
	{
		buf[--i] = ch[deci_num%scale];
		deci_num /= scale;
	}while(deci_num);
	
	for ( j = 0; j < N-1; j++ )//for ( j = 0; (s[j] = buf[i])!='\0'; i++, j++ );
	{
		s[j] = buf[i];
		i++;
	}
	
	return j;
}

/*********************************************************************************
* Function Name    ： binary_to_other_scale,二进制数转换为其他进制数
* Parameter		   ： binary_num：二进制数   scale：进制数   s：保存转换后的数
* Return Value     ： 0：错误  其他：成功
* Function Explain ： 
* Create Date      ： 2018.9.27
**********************************************************************************/
#if 1
int binary_to_other_scale(char *binary_num, int scale, char s[])
{
	int deci_sum = 0;

	deci_sum = other_scale_to_deci(binary_num, TWO_SCALE);

	return deci_to_other_scale(deci_sum, scale, s);
}
#else
uint binary_to_other_scale(uint binary_num, uint scale, char s[])
{
	uint deci_num = 0;
	
	deci_num = binary_to_deci(binary_num);
	
	return deci_to_other_scale(deci_num, scale, s);
} 
#endif


/*********************************************************************************
* Function Name    ： binary_to_deci,二进制数转换为十进制数
* Parameter		   ： binary_num：二进制数 
* Return Value     ： 0：错误  其他：成功
* Function Explain ： 
* Create Date      ： 2018.9.27
**********************************************************************************/
/* 
	程序要点：记录二进制数中“1”的位置
	如二进制数：1101
	则保存“1”位置的数组为：buf[3] = {3, 2, 0}  （低位保存在数组末位元素中）
	转换的十进制数为：deci_num = pow(2,3) + pow(2, 2) + pow(2, 0)
 */
int binary_to_deci_scale(uint binary_num)
{
	char binary_str[64];
	char *buf = NULL;
	char i = 0, j = 0, len = 0;
	int deci_num = 0, buf_len = 0;

	sprintf(binary_str, "%d", binary_num);
	len = strlen(binary_str);
	buf = (char*)malloc(len*sizeof(char));

	/* 记录“1”的位置 */
	for ( i = len - 1; i >= 0; i-- )
	{
		if ( binary_str[i] == '1' )
		{
			buf[j++] = len -1 - i;
			printf("%d, ", len -1 - i);
		}
		buf_len = j;
	}

	/* 转换为十进制数 */
	for ( j = 0; j < buf_len; j++ )
	{
		deci_num += pow(2, buf[j]);
	}
	free(buf);
	
	return deci_num;
}

/*********************************************************************************
* Function Name    ： hex_to_other_scale,十六进制转换为其他进制
* Parameter		   ： hex_num：十六进制数   scale：进制数   s：保存转换后的数
* Return Value     ： 0：错误  其他：成功
* Function Explain ： 
* Create Date      ： 2018.9.27
**********************************************************************************/
int hex_to_other_scale(char *hex_num, int scale, char s[])
{
	int deci_sum = 0;

	deci_sum = other_scale_to_deci(hex_num, SIXTEEN_SCALE);
	
	return deci_to_other_scale(deci_sum, scale, s);
}

/*********************************************************************************
* Function Name    ： oct_to_other_scale,八进制转换为其他进制
* Parameter		   ： oct_num：八进制数   scale：进制数   s：保存转换后的数
* Return Value     ： 0：错误  其他：成功
* Function Explain ： 
* Create Date      ： 2018.9.27
**********************************************************************************/
int oct_to_other_scale(char *oct_num, int scale, char s[])
{
	int deci_sum = 0;

	deci_sum = other_scale_to_deci(oct_num, EIGHT_SCALE);

	return deci_to_other_scale(deci_sum, scale, s);
}

/*********************************************************************************
* Function Name    ： other_scale_to_deci,其他进制转换为十进制
* Parameter		   ： other_num：其他进制数   scale：进制数 
* Return Value     ： 0：错误  deci_sum：转换成功的十进制数
* Function Explain ： 
* Create Date      ： 2018.9.29
**********************************************************************************/
/*
	如八进制776 = 7*pow(8,2) + 7*pow(8,1) + 6*pow(8,0) = 510
*/
int other_scale_to_deci(char *other_num, int scale)
{
	const char figure[] = "0123456789ABCDEF";
	
	char i, j;
	char other_len = strlen(other_num), figure_len = strlen(figure);
	char deci_num[other_len];
	int deci_sum = 0;
	char flag = 0;

	/* 分离scale进制数的各位，如八进制776分离成7,7,6 */
	for ( i = 0; i < other_len; i++ )
	{
		for ( j = 0; j < scale; j++ )
		{
			if ( other_num[i] == figure[j] )
			{
				deci_num[i] = j;	
				flag = 1;
			}
			
		}
		/* 不是scale进制数则退出函数 */
		if(flag == 0)
		{
			printf("Input Error!Please Input Again!\n");
	 		return 0;
		}
		flag = 0;
		
	}

	/* scale进制转化为十进制，如八进制转换为十进制数：776 = 7*pow(8,2) + 7*pow(8,1) + 6*pow(8,0) = 510 */
	for ( i = 0; i < other_len; i++ )
	{
		deci_sum += deci_num[other_len-1-i]*pow(scale, i);
	}

	return deci_sum;
}

/*********************************************************************************
 * 							以下是测试函数
**********************************************************************************/
/* 测试十进制转化为其他进制 */
void Test1(void)
{
	int num = 0, i = 0;
	int scale[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
	char buf[32];

	printf("Please input a decimalism num : ");
	scanf("%d", &num);
	for ( i = 0; i < sizeof(scale)/sizeof(scale[0]); i++ )
	{
		if ( deci_to_other_scale(num, scale[i], buf) )
		{
			printf("%.2d进制转换：%d = %s\n", scale[i], num, buf);
		}
		else
		{
			printf("%.2d进制转换: Error!\n", scale[i]);
		}
	}
}

/* 测试二进制转化为其他进制 */
void Test2(void)
{
	int num = 0, i = 0;
	int scale[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
	char buf[100], binary_str[100];

	printf("Please input a binary num : ");
	scanf("%s", binary_str);
	for ( i = 0; i < sizeof(scale)/sizeof(scale[0]); i++ )
	{
		if ( binary_to_other_scale(binary_str, scale[i], buf) )
		{
			printf("%.2d进制转换：%s = %s\n", scale[i], binary_str, buf);
		}
		else
		{
			printf("%.2d进制转换: Error!\n", scale[i]);
		}
	}
} 

/* 测试十六进制转化为其他进制 */
void Test3(void)
{
	int i;
	int scale[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
	char buf[32], hex_str[32];

	printf("Please input a hex num : ");
	scanf("%s", hex_str);
	printf("\n%s\n", hex_str);
	for ( i = 0; i < sizeof(scale)/sizeof(scale[0]); i++ )
	{
		if ( hex_to_other_scale(hex_str, scale[i], buf) )
		{
			printf("%.2d进制转换：%s = %s\n", scale[i], hex_str, buf);
		}
		else
		{
			printf("%.2d进制转换: Error!\n", scale[i]);
		}
	}
}

/* 测试八进制转化为其他进制 */
void Test4(void)
{
	int i;
	int scale[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
	char buf[32], oct_str[32];

	printf("Please input a oct num : ");
	scanf("%s", oct_str);
	printf("\n%s\n", oct_str);
	for ( i = 0; i < sizeof(scale)/sizeof(scale[0]); i++ )
	{
		if ( oct_to_other_scale(oct_str, scale[i], buf) )
		{
			printf("%.2d进制转换：%s = %s\n", scale[i], oct_str, buf);
		}
		else
		{
			printf("%.2d进制转换: Error!\n", scale[i]);
		}
	}
}

/* 测试其他进制数转化为十进制数 */
void Test5(void)
{
	int scale = 0;
	char other_num[32] = {0};
	int deci_num = 0;

	scanf("%d %s",&scale, other_num);
	deci_num = other_scale_to_deci(other_num, scale);
	printf("deci_num = %d\n", deci_num);
}

