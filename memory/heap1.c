#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//������
int main(void)
{
	heap_test4();
	return 0;
}

const char ro_data[] = "ro_data";
char rw_data[] = "rw_data";
char bss_data[100];

//�Ƿ��ͷž�̬�洢�����ڴ�
void heap_test7(void)
{
	/* ...... */
	/* �����ͷ�ֻ��������ָ�� */
	free(ro_data);
	/* �����ͷ��ѳ�ʼ����д������ָ�� */
	free(rw_data);
	/* �����ͷ�δ��ʼ����д������ָ�� */
	free(bss_data);
	
	/* �����ͷŴ�����ָ�� */
	free(heap_test7);
	/* ...... */
	return;
}

//�Ƿ��ͷ�ջ�ϵ��ڴ�
void heap_test8(void)
{
	char a[20];
	int b;
	
	/* �����ͷ�ջ���ڴ� */
	
	/* ...... */
	free(a);
	free(&b);
	/* ...... */
	return;
}

//�Ƿ��ͷŶ��ϵ��ڴ�---1
void heap_test9(void)
{
	char *pa;
	/* ...... */
	pa = (char*)malloc(sizeof(char)*20);
	free(pa);
	free(pa);  //�����ͷŶ��ڴ�
	/* ...... */
	return;
}

//�Ƿ��ͷŶ��ϵ��ڴ�---2
void heap_test10(void)
{
	char *pa;
	char *pb;
	/* ...... */
	pa = (char*)malloc(sizeof(char)*20);
	pb = pa++;
	free(pb);  //�����ͷŶ��ڴ�
	/* ...... */
	return;
}