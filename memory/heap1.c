#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//主函数
int main(void)
{
	heap_test4();
	return 0;
}

const char ro_data[] = "ro_data";
char rw_data[] = "rw_data";
char bss_data[100];

//非法释放静态存储区的内存
void heap_test7(void)
{
	/* ...... */
	/* 错误释放只读数据区指针 */
	free(ro_data);
	/* 错误释放已初始化读写数据区指针 */
	free(rw_data);
	/* 错误释放未初始化读写数据区指针 */
	free(bss_data);
	
	/* 错误释放代码区指针 */
	free(heap_test7);
	/* ...... */
	return;
}

//非法释放栈上的内存
void heap_test8(void)
{
	char a[20];
	int b;
	
	/* 错误释放栈上内存 */
	
	/* ...... */
	free(a);
	free(&b);
	/* ...... */
	return;
}

//非法释放堆上的内存---1
void heap_test9(void)
{
	char *pa;
	/* ...... */
	pa = (char*)malloc(sizeof(char)*20);
	free(pa);
	free(pa);  //错误释放堆内存
	/* ...... */
	return;
}

//非法释放堆上的内存---2
void heap_test10(void)
{
	char *pa;
	char *pb;
	/* ...... */
	pa = (char*)malloc(sizeof(char)*20);
	pb = pa++;
	free(pb);  //错误释放堆内存
	/* ...... */
	return;
}