#include<stdio.h>
#define SIZE  9

/*--------------------------------------------------
功能:              冒泡排序
参数说明
    第一个:     a[]要排序的数组
    第二个:     数组的长度
返回值:         无
特别说明:   
                        数组元素大小按升序排列
                        排序好之后的数组还是a[]
----------------------------------------------------*/
void bubble_sort(int a[],int iLength)
{
    int i,j;
    for(j=0;j<iLength-1;j++)
    {
        for(i=0;i<iLength-1-j;i++)
        {
            if(a[i]>a[i+1])
            {
                /*交换a[i] 与a[i+1] 的数据*/
                a[i] = a[i]^a[i+1];
                a[i+1] = a[i]^a[i+1];
                a[i]= a[i]^a[i+1];
            }
        }
    }
}


/*
函数功能：把数组中所有比a[low]小的数字放在左边，
                           把数组中所有比a[low]大的数字放在右边，
                           最后返回a[low]值在数组中的位置
*/
int partions(int a[],int low,int high)
{
    int low_value=a[low];
    while (low<high)
    {
        /*先从高位置查找，直到找出一个比low_value小的数字才退出while循环*/
         while (low<high&&a[high]>=low_value)
         --high;

         /*找到之后，放在此时low的位置上，大家注意这个时候high位置上的数据
         是没有意义的，因为它已经放在low位置上去了，
         下面的代码会把high位置上数据退换为一个有意义的数字的*/
         a[low]=a[high];

          /*再从低位置查找，直到找出一个比low_value大的数字才退出while循环*/
         while (low<high&&a[low]<=low_value) 
         ++low;

        /*把找到的数字放到数组high的位置上，
        这个时候high位置上的数字是有意义的，
        那么这个时候low位置上的数据是没有意义的，因为它的值放在了high上了*/
         a[high]=a[low];
    }
    /*把值放在low位置上，使得low位置上的数据有意义，
    从而整个数组上的每个数字都是有意义的*/
    a[low]=low_value;
    return low;
}


void qsort(int a[],int low,int high)
{
	int prvotloc;
	if(low<high)
	{
	     prvotloc=partions(a,low,high); 
	     qsort(a,low,prvotloc-1); //递归调用排序 由low 到prvotloc-1
	     qsort(a,prvotloc+1,high); //递归调用排序 由 prvotloc+1到 high

	}
}

/*--------------------------------------------------
功能:              快速排序
参数说明
    第一个:     a[]要排序的数组
    第二个:     数组的长度
返回值:         无
特别说明:   
                        数组元素大小按升序排列
                        排序好之后的数组还是a[]
----------------------------------------------------*/
void quicksort(int a[],int n)
{
	qsort(a,0,n-1);
}

/*--------------------------------------------------
功能:              选择排序
参数说明
    第一个:     a[]要排序的数组
    第二个:     数组的长度
返回值:         无
特别说明:   
                        数组元素大小按升序排列
                        排序好之后的数组还是a[]
----------------------------------------------------*/

void select_sort(int a[], int iLength)
{
    int i, j, min; 
    for( i =0; i < iLength -1; i ++) 
    { 
        min = i;    /*首先假设第一个是最小值*/
        for( j = i +1; j < iLength; j ++)
        {
            if( a[min] > a[j]) 
                min = j;
        }
        //交换
        if(min != i) 
        {
            a[min] = a[min]^a[i];
            a[i] = a[min]^a[i];
            a[min] = a[min]^a[i];
        } 
    } 
 }



int main()
{
    int number[SIZE]={50,45,15,78,84,51,24,12,9};
    int i;
    //执行冒泡排序
    bubble_sort(number,SIZE);

    //执行选择排序
    //select_sort(number,SIZE);
    
    //执行快速排序
    //quicksort(number,SIZE);

    for(i=0;i<SIZE;i++)
    {
        printf("%d ",number[i]);
    }
    printf("\n");
}

