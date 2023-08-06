#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>


int main(int argc, char argv[])
{
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	int status1 = 0;
	int status2 = 0;
	int a[6] = {1, 2, 3};
	int b[6] = {0};
	int c[6] = {0};

	//a 给 b 赋值
	while(1)
	{
		sleep(3);

		printf("\nupdate a array:a[0]:%d,a[1]:%d,a[2]:%d,a[3]:%d,a[4]:%d,a[5]:%d\n", a[0], a[1], a[2], a[3], a[4], a[5]);

		for(i = 0; i < 6; i++)
		{
			if(0 != a[i])
			{
				status1 = 0;

				for(j = 0; j < 6; j++)
				{
					if(a[i] == b[j])
					{
						status1 = 1;
						break;
					}
					else if(a[i] != b[j])
					{
						status1 = -1;
						continue;
					}
				}

				//a is exist in b array
				if(1 == status1){
					printf("\n%d is exist in b array\n", a[i]);
				}
				//a is non-exist in b array
				else if(-1 == status1)
				{
					status2 = 0;

					//find a if in b array
					for(k = 0; k < 6; k++)
					{
						if(0 == b[k] && k <= 5)
						{
							status2 = 1;
							break;
						}
						else if(0 != b[k] && 5 == k)
						{
							status2 = -1;
							break;
						}
					}

					//a is non-exist in b array, and b array is not full, add to b array
					if(1 == status2)
					{
						printf("\n%d is non-exist in b array, and b array is not full, add to b array\n", a[i]);
						
						printf("\nold_data:b[0]:%d,b[1]:%d,b[2]:%d,b[3]:%d,b[4]:%d,b[5]:%d\n", b[0], b[1], b[2], b[3], b[4], b[5]);

						b[k] = a[i];
						
						printf("\nnew_data:b[0]:%d,b[1]:%d,b[2]:%d,b[3]:%d,b[4]:%d,b[5]:%d\n", b[0], b[1], b[2], b[3], b[4], b[5]);
					}
					//a is non-exist, but b array is full, loop to add to b array
					else if(-1 == status2)
					{
						printf("\n%d is non-exist in b array, but b array is full, loop to add to b array\n", a[i]);

						printf("\nold_data:b[0]:%d,b[1]:%d,b[2]:%d,b[3]:%d,b[4]:%d,b[5]:%d\n", b[0], b[1], b[2], b[3], b[4], b[5]);

						b[l] = 0;
						b[l] = a[i];
						if(6 == ++l){
							l = 0;
						}
						
						printf("\nnew_data:b[0]:%d,b[1]:%d,b[2]:%d,b[3]:%d,b[4]:%d,b[5]:%d\n", b[0], b[1], b[2], b[3], b[4], b[5]);
					}
				}
			}
		}

		b[2] = 0;
		a[0] += 1;
		a[1] += 2;
		a[2] += 3;
	}

    return 0;
}



