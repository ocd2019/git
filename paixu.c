#include<stdio.h>
#define SIZE  9

/*--------------------------------------------------
����:              ð������
����˵��
    ��һ��:     a[]Ҫ���������
    �ڶ���:     ����ĳ���
����ֵ:         ��
�ر�˵��:   
                        ����Ԫ�ش�С����������
                        �����֮������黹��a[]
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
                /*����a[i] ��a[i+1] ������*/
                a[i] = a[i]^a[i+1];
                a[i+1] = a[i]^a[i+1];
                a[i]= a[i]^a[i+1];
            }
        }
    }
}


/*
�������ܣ������������б�a[low]С�����ַ�����ߣ�
                           �����������б�a[low]������ַ����ұߣ�
                           ��󷵻�a[low]ֵ�������е�λ��
*/
int partions(int a[],int low,int high)
{
    int low_value=a[low];
    while (low<high)
    {
        /*�ȴӸ�λ�ò��ң�ֱ���ҳ�һ����low_valueС�����ֲ��˳�whileѭ��*/
         while (low<high&&a[high]>=low_value)
         --high;

         /*�ҵ�֮�󣬷��ڴ�ʱlow��λ���ϣ����ע�����ʱ��highλ���ϵ�����
         ��û������ģ���Ϊ���Ѿ�����lowλ����ȥ�ˣ�
         ����Ĵ�����highλ���������˻�Ϊһ������������ֵ�*/
         a[low]=a[high];

          /*�ٴӵ�λ�ò��ң�ֱ���ҳ�һ����low_value������ֲ��˳�whileѭ��*/
         while (low<high&&a[low]<=low_value) 
         ++low;

        /*���ҵ������ַŵ�����high��λ���ϣ�
        ���ʱ��highλ���ϵ�������������ģ�
        ��ô���ʱ��lowλ���ϵ�������û������ģ���Ϊ����ֵ������high����*/
         a[high]=a[low];
    }
    /*��ֵ����lowλ���ϣ�ʹ��lowλ���ϵ����������壬
    �Ӷ����������ϵ�ÿ�����ֶ����������*/
    a[low]=low_value;
    return low;
}


void qsort(int a[],int low,int high)
{
	int prvotloc;
	if(low<high)
	{
	     prvotloc=partions(a,low,high); 
	     qsort(a,low,prvotloc-1); //�ݹ�������� ��low ��prvotloc-1
	     qsort(a,prvotloc+1,high); //�ݹ�������� �� prvotloc+1�� high

	}
}

/*--------------------------------------------------
����:              ��������
����˵��
    ��һ��:     a[]Ҫ���������
    �ڶ���:     ����ĳ���
����ֵ:         ��
�ر�˵��:   
                        ����Ԫ�ش�С����������
                        �����֮������黹��a[]
----------------------------------------------------*/
void quicksort(int a[],int n)
{
	qsort(a,0,n-1);
}

/*--------------------------------------------------
����:              ѡ������
����˵��
    ��һ��:     a[]Ҫ���������
    �ڶ���:     ����ĳ���
����ֵ:         ��
�ر�˵��:   
                        ����Ԫ�ش�С����������
                        �����֮������黹��a[]
----------------------------------------------------*/

void select_sort(int a[], int iLength)
{
    int i, j, min; 
    for( i =0; i < iLength -1; i ++) 
    { 
        min = i;    /*���ȼ����һ������Сֵ*/
        for( j = i +1; j < iLength; j ++)
        {
            if( a[min] > a[j]) 
                min = j;
        }
        //����
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
    //ִ��ð������
    bubble_sort(number,SIZE);

    //ִ��ѡ������
    //select_sort(number,SIZE);
    
    //ִ�п�������
    //quicksort(number,SIZE);

    for(i=0;i<SIZE;i++)
    {
        printf("%d ",number[i]);
    }
    printf("\n");
}

