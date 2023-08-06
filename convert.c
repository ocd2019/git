/*----------------------------------------------------------------------------------------
	
	Program Explain������ת��
    Create Date��2018.9.27 by lzn
	note:��ȡ������򼰱�̱ʼǿ�����΢�Ź��ںš���zhengnian-2018����������ѧϰ�ʼ�
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

// ʮ����ת��Ϊ��������
int deci_to_other_scale(uint deci_num, int scale, char s[]);
// ������ת��Ϊ��������
int binary_to_other_scale(char *binary_num, int scale, char s[]);
// ʮ������ת��Ϊ��������
int hex_to_other_scale(char *hex_num, int scale, char s[]);
// �˽���ת��Ϊ��������
int oct_to_other_scale(char *oct_num, int scale, char s[]);
// ��������ת��Ϊʮ����
int other_scale_to_deci(char *other_num, int scale);
// ��һ�ֶ�����ת��Ϊʮ���Ƶķ������������ڽϴ�����ݣ�
int binary_to_deci(uint binary_num);

// ��������
//..........

// ���Ժ���
void Test1(void);	// ����ʮ����ת��Ϊ��������
void Test2(void);	// ���Զ�����ת��Ϊ��������
void Test3(void);	// ����ʮ������ת��Ϊ��������
void Test4(void);	// ���԰˽���ת��Ϊ��������
void Test5(void);	// ������������ת��Ϊʮ����

/*********************************************************************************
* Function Name    ��main��������
* Parameter		   ��void
* Return Value     ��0
* Function Explain �� 
* Create Date      ��2018.9.27
**********************************************************************************/
int main(void)
{
	Test2();
	
	return 0;
}

/*********************************************************************************
* Function Name    �� deci_to_other_scale,���޷�������n�����d��2<��scale<��16�����Ʊ�ʾ���ַ���s
* Parameter		   �� num:Ҫת������   scale:������   s:����ת�������
* Return Value     �� 0������  �������ɹ�
* Function Explain �� 
* Create Date      �� 2018.9.27
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
* Function Name    �� binary_to_other_scale,��������ת��Ϊ����������
* Parameter		   �� binary_num����������   scale��������   s������ת�������
* Return Value     �� 0������  �������ɹ�
* Function Explain �� 
* Create Date      �� 2018.9.27
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
* Function Name    �� binary_to_deci,��������ת��Ϊʮ������
* Parameter		   �� binary_num���������� 
* Return Value     �� 0������  �������ɹ�
* Function Explain �� 
* Create Date      �� 2018.9.27
**********************************************************************************/
/* 
	����Ҫ�㣺��¼���������С�1����λ��
	�����������1101
	�򱣴桰1��λ�õ�����Ϊ��buf[3] = {3, 2, 0}  ����λ����������ĩλԪ���У�
	ת����ʮ������Ϊ��deci_num = pow(2,3) + pow(2, 2) + pow(2, 0)
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

	/* ��¼��1����λ�� */
	for ( i = len - 1; i >= 0; i-- )
	{
		if ( binary_str[i] == '1' )
		{
			buf[j++] = len -1 - i;
			printf("%d, ", len -1 - i);
		}
		buf_len = j;
	}

	/* ת��Ϊʮ������ */
	for ( j = 0; j < buf_len; j++ )
	{
		deci_num += pow(2, buf[j]);
	}
	free(buf);
	
	return deci_num;
}

/*********************************************************************************
* Function Name    �� hex_to_other_scale,ʮ������ת��Ϊ��������
* Parameter		   �� hex_num��ʮ��������   scale��������   s������ת�������
* Return Value     �� 0������  �������ɹ�
* Function Explain �� 
* Create Date      �� 2018.9.27
**********************************************************************************/
int hex_to_other_scale(char *hex_num, int scale, char s[])
{
	int deci_sum = 0;

	deci_sum = other_scale_to_deci(hex_num, SIXTEEN_SCALE);
	
	return deci_to_other_scale(deci_sum, scale, s);
}

/*********************************************************************************
* Function Name    �� oct_to_other_scale,�˽���ת��Ϊ��������
* Parameter		   �� oct_num���˽�����   scale��������   s������ת�������
* Return Value     �� 0������  �������ɹ�
* Function Explain �� 
* Create Date      �� 2018.9.27
**********************************************************************************/
int oct_to_other_scale(char *oct_num, int scale, char s[])
{
	int deci_sum = 0;

	deci_sum = other_scale_to_deci(oct_num, EIGHT_SCALE);

	return deci_to_other_scale(deci_sum, scale, s);
}

/*********************************************************************************
* Function Name    �� other_scale_to_deci,��������ת��Ϊʮ����
* Parameter		   �� other_num������������   scale�������� 
* Return Value     �� 0������  deci_sum��ת���ɹ���ʮ������
* Function Explain �� 
* Create Date      �� 2018.9.29
**********************************************************************************/
/*
	��˽���776 = 7*pow(8,2) + 7*pow(8,1) + 6*pow(8,0) = 510
*/
int other_scale_to_deci(char *other_num, int scale)
{
	const char figure[] = "0123456789ABCDEF";
	
	char i, j;
	char other_len = strlen(other_num), figure_len = strlen(figure);
	char deci_num[other_len];
	int deci_sum = 0;
	char flag = 0;

	/* ����scale�������ĸ�λ����˽���776�����7,7,6 */
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
		/* ����scale���������˳����� */
		if(flag == 0)
		{
			printf("Input Error!Please Input Again!\n");
	 		return 0;
		}
		flag = 0;
		
	}

	/* scale����ת��Ϊʮ���ƣ���˽���ת��Ϊʮ��������776 = 7*pow(8,2) + 7*pow(8,1) + 6*pow(8,0) = 510 */
	for ( i = 0; i < other_len; i++ )
	{
		deci_sum += deci_num[other_len-1-i]*pow(scale, i);
	}

	return deci_sum;
}

/*********************************************************************************
 * 							�����ǲ��Ժ���
**********************************************************************************/
/* ����ʮ����ת��Ϊ�������� */
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
			printf("%.2d����ת����%d = %s\n", scale[i], num, buf);
		}
		else
		{
			printf("%.2d����ת��: Error!\n", scale[i]);
		}
	}
}

/* ���Զ�����ת��Ϊ�������� */
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
			printf("%.2d����ת����%s = %s\n", scale[i], binary_str, buf);
		}
		else
		{
			printf("%.2d����ת��: Error!\n", scale[i]);
		}
	}
} 

/* ����ʮ������ת��Ϊ�������� */
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
			printf("%.2d����ת����%s = %s\n", scale[i], hex_str, buf);
		}
		else
		{
			printf("%.2d����ת��: Error!\n", scale[i]);
		}
	}
}

/* ���԰˽���ת��Ϊ�������� */
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
			printf("%.2d����ת����%s = %s\n", scale[i], oct_str, buf);
		}
		else
		{
			printf("%.2d����ת��: Error!\n", scale[i]);
		}
	}
}

/* ��������������ת��Ϊʮ������ */
void Test5(void)
{
	int scale = 0;
	char other_num[32] = {0};
	int deci_num = 0;

	scanf("%d %s",&scale, other_num);
	deci_num = other_scale_to_deci(other_num, scale);
	printf("deci_num = %d\n", deci_num);
}

