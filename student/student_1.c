#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENT_NUMBER 6000

typedef struct _BASIC_INFORMATION
{
	char id[6];                       					
	char name[11];                    							
	char professional_number[3];                    		
	int general_education_course1;                           	
	int general_education_course2;
	int general_education_course3;
	int public_foundation_course1;                         
	int public_foundation_course2;
	int public_foundation_course3;
	int major_course1;                  
	int major_course2;
}BASIC_INFORMATION;

typedef struct _EXCELLENT_STUDENT
{
	int total_score;
	int rank_score;
	BASIC_INFORMATION basic_info;
}EXCELLENT_STUDENT;


BASIC_INFORMATION basic_info[MAX_STUDENT_NUMBER] = {0};


_Bool write_excellent_student(char *file_name, int total, EXCELLENT_STUDENT school_level[])
{
	int i = 0;

	FILE *fp = fopen(file_name,"w");

	if(NULL == fp)
	{
		printf("Failed to open %s\n", file_name);
		return 1;
	}

	for(i = 0; i < total; i++)
    {         
       	fprintf(fp,"%s %s %s %d %d %d %d %d %d %d %d %d %d\n", school_level[i].basic_info.id,
													   		   school_level[i].basic_info.name,
															   school_level[i].basic_info.professional_number,
															   school_level[i].basic_info.general_education_course1,
															   school_level[i].basic_info.general_education_course2,
															   school_level[i].basic_info.general_education_course3,
															   school_level[i].basic_info.public_foundation_course1,
															   school_level[i].basic_info.public_foundation_course2,
															   school_level[i].basic_info.public_foundation_course3,
															   school_level[i].basic_info.major_course1,
															   school_level[i].basic_info.major_course2,
															   school_level[i].total_score,
															   school_level[i].rank_score);
    }

	fclose(fp);

	return 0;
}


int swap_excellent_level(EXCELLENT_STUDENT result[], int left, int right)
{
	BASIC_INFORMATION basic_info = {0};
	
	int original_score = result[left].total_score;

	memcpy(&basic_info, &result[left].basic_info, sizeof(BASIC_INFORMATION));

    while(left < right)
    {
		while (left < right && result[right].total_score <= original_score)
        	right--;

        result[left].total_score = result[right].total_score;
		memset(&result[left].basic_info, 0, sizeof(BASIC_INFORMATION));
		memcpy(&result[left].basic_info, &result[right].basic_info, sizeof(BASIC_INFORMATION));

        while (left < right && result[left].total_score >= original_score)
        	left++;

        result[right].total_score = result[left].total_score;
		memset(&result[right].basic_info, 0, sizeof(BASIC_INFORMATION));
		memcpy(&result[right].basic_info, &result[left].basic_info, sizeof(BASIC_INFORMATION));
    }

    result[left].total_score = original_score;
	memset(&result[left].basic_info, 0, sizeof(BASIC_INFORMATION));
	memcpy(&result[left].basic_info, &basic_info, sizeof(BASIC_INFORMATION));

    return left;
}


_Bool sort_excellent_level(EXCELLENT_STUDENT level_result[], int left_node, int right_node)
{
	int i = 0;

	if(left_node < right_node)
	{
	     i = swap_excellent_level(level_result, left_node, right_node);

	     sort_excellent_level(level_result, left_node, i - 1);

	     sort_excellent_level(level_result, i + 1, right_node);
	}

	return 0;
}


_Bool rank_excellent_level(int number, int count, EXCELLENT_STUDENT info[])
{
	int i = 0;
	int j = 0;

	sort_excellent_level(info, 0, count - 1);

	for(i = 0; i < number; i++)
	{
		if(0 == info[i].rank_score){
			info[i].rank_score = ++j;
		}

		if((i + 1) < number && info[i].total_score == info[i + 1].total_score){
			info[i + 1].rank_score = info[i].rank_score;
		}
	}

	return 0;
}


_Bool delete_school_level(int count, EXCELLENT_STUDENT info[])
{
	int i = 0;
	int j = 0;
	int total = 0;
	EXCELLENT_STUDENT tmp[MAX_STUDENT_NUMBER] = {0};
	
	FILE *fp = fopen("f2.txt","r");

	if(NULL == fp)
	{
		printf("Failed to open f2.txt\n");
		return 1;
	}

	while(!feof(fp))
	{	  
		fscanf(fp,"%s %s %s %d %d %d %d %d %d %d %d %d %d\n", tmp[total].basic_info.id,
															  tmp[total].basic_info.name,
															  tmp[total].basic_info.professional_number,
															  &tmp[total].basic_info.general_education_course1,
															  &tmp[total].basic_info.general_education_course2,
															  &tmp[total].basic_info.general_education_course3,
															  &tmp[total].basic_info.public_foundation_course1,
															  &tmp[total].basic_info.public_foundation_course2,
															  &tmp[total].basic_info.public_foundation_course3,
															  &tmp[total].basic_info.major_course1,
															  &tmp[total].basic_info.major_course2,
															  &tmp[total].total_score,
															  &tmp[total].rank_score);
		total++;
	}

	fclose(fp);

	for(i = 0; i < count; i++)
	{
		for(j = 0; j < total; j++)
		{
			if(0 == strncmp(info[i].basic_info.id, tmp[j].basic_info.id, 5)){
				memset(&info[i], 0, sizeof(EXCELLENT_STUDENT));
			}
		}
	}

	return 0;
}


_Bool filter_professional_level(int total, int count, EXCELLENT_STUDENT info[])
{
	int i = 0;
	int j = 0;
	int k = 0;
	int offset = 0;
	EXCELLENT_STUDENT tmp[count];
	EXCELLENT_STUDENT new[count];
	EXCELLENT_STUDENT old[count];

	memcpy(old, info, sizeof(old));

	for(i = 0; i < count; i++)
	{
		if(0 != strlen(info[i].basic_info.professional_number))
		{
			memcpy(&tmp[j], &info[i], sizeof(EXCELLENT_STUDENT));

			memset(&info[i], 0, sizeof(EXCELLENT_STUDENT));

			for(k = 0; k < count; k++)
			{
				if(0 == strncmp(tmp[j].basic_info.professional_number, info[k].basic_info.professional_number, 2))
				{
					memcpy(&tmp[++j], &info[k], sizeof(EXCELLENT_STUDENT));
					memset(&info[k], 0, sizeof(EXCELLENT_STUDENT));
				}
			}

			total = (int)(0.1 * j + 0.5);

			rank_excellent_level(total, j, tmp);

			memcpy(new + offset, tmp, total * sizeof(EXCELLENT_STUDENT));

			memset(tmp, 0, sizeof(tmp));

			offset += total;

			j = 0;
		}
	}

	k = 0;

	delete_school_level(offset, new);

	EXCELLENT_STUDENT excellent_student[offset];

	for(i = 0; i < count; i++)
	{
		for(j = 0; j < offset; j++)
		{
			if(0 == strncmp(old[i].basic_info.id, new[j].basic_info.id, 5))
			{
				excellent_student[k].total_score = new[j].total_score;
				excellent_student[k].rank_score = new[j].rank_score;
				memcpy(&excellent_student[k].basic_info, &new[j].basic_info, sizeof(BASIC_INFORMATION));
				k++;
				break;
			}
		}
	}

	write_excellent_student("f3.txt", k, excellent_student);

	printf("professional_level_excellent_number:%d\n", k);

	return 0;
}


_Bool filter_school_level(int total, int count, EXCELLENT_STUDENT info[])
{
	int i = 0;
	int j = 0;
	int k = 0;
	EXCELLENT_STUDENT tmp[count];

	memcpy(tmp, info, sizeof(tmp));

	total = (int)(0.05 * total + 0.5);

	rank_excellent_level(total, count, info);

	EXCELLENT_STUDENT excellent_student[total];

	for(i = 0; i < count; i++)
	{
		for(j = 0; j < total; j++)
		{
			if(0 == strncmp(tmp[i].basic_info.id, info[j].basic_info.id, 5))
			{
				excellent_student[k].total_score = info[j].total_score;
				excellent_student[k].rank_score = info[j].rank_score;
				memcpy(&excellent_student[k].basic_info, &info[j].basic_info, sizeof(BASIC_INFORMATION));
				k++;
				break;
			}
		}
	}

	write_excellent_student("f2.txt", k, excellent_student);

	printf("school_level_excellent_number:%d\n", k);

	return 0;
}


_Bool filter_excellent_student(int total)
{
	int i = 0;
	int j = 0;
	BASIC_INFORMATION basic_excellent[total];

	for(i = 0; i < total; i++)
	{
		if(basic_info[i].general_education_course1 >= 60 
		&& basic_info[i].general_education_course2 >= 60
		&& basic_info[i].general_education_course3 >= 60
		&& basic_info[i].public_foundation_course1 >= 70
		&& basic_info[i].public_foundation_course2 >= 70
		&& basic_info[i].public_foundation_course3 >= 70
		&& basic_info[i].major_course1 >= 80
		&& basic_info[i].major_course2 >= 80)
		{
			memcpy(&basic_excellent[j++], &basic_info[i], sizeof(BASIC_INFORMATION));
		}
	}

	EXCELLENT_STUDENT excellent_student_1[j];
	EXCELLENT_STUDENT excellent_student_2[j];

	for(i = 0; i < j; i++)
	{
		excellent_student_1[i].rank_score = 0;
		memcpy(&excellent_student_1[i].basic_info, &basic_excellent[i], sizeof(BASIC_INFORMATION));
		excellent_student_1[i].total_score = 0.8 * (basic_excellent[i].public_foundation_course1 + \
										   basic_excellent[i].public_foundation_course2 +	\
										   basic_excellent[i].public_foundation_course3) + \
										   1.2 * (basic_excellent[i].major_course1 + \
										   basic_excellent[i].major_course2);
	}

	memcpy(excellent_student_2, excellent_student_1, sizeof(excellent_student_1));

	filter_school_level(total, j, excellent_student_1);

	filter_professional_level(total, j, excellent_student_2);

	printf("total_number:%d\n", total);
	printf("excellent_number:%d\n", j);

	return 0;
}



_Bool read_f1_content(int *total)
{
	*total = 0;

	FILE *fp = fopen("f1.txt","r");

	if(NULL == fp)
	{
		printf("Failed to open f1.txt\n");
		return 1;
	}

	while(!feof(fp))
	{
		fscanf(fp,"%s %s %s %d %d %d %d %d %d %d %d ", basic_info[*total].id,
													   basic_info[*total].name,
													   basic_info[*total].professional_number,
													   &basic_info[*total].general_education_course1,
													   &basic_info[*total].general_education_course2,
													   &basic_info[*total].general_education_course3,
													   &basic_info[*total].public_foundation_course1,
													   &basic_info[*total].public_foundation_course2,
													   &basic_info[*total].public_foundation_course3,
													   &basic_info[*total].major_course1,
													   &basic_info[*total].major_course2);
		(*total)++;
	}

	fclose(fp);

	return 0;
}

int main(int argc, char *argv[])
{
	int index = -1;
	int total_number = 0;

	printf("\t********************************************************\n");
	printf("\t*                   1.读取文件内容                     *\n");
	printf("\t*                   2.显示评优结果                     *\n");
	printf("\t*                   0.退出评价系统                     *\n");
	printf("\t********************************************************\n");

	do{
		printf("\t请输入序号\n");
		scanf("%d", &index);

		switch (index)
		{
			case 1:
				read_f1_content(&total_number);
				break;
			case 2:
				filter_excellent_student(total_number);
				break;
			default:
				printf("\t序号无效哦\n");
		}
	
	}while(0 != index);

	return 0;
}

#if 0
	for(i = 0; i < total; i++)
	{
		printf("%s %s %s %d %d %d %d %d %d %d %d %d %d\n", school_level[i].basic_info.id,
													   	   school_level[i].basic_info.name,
													       school_level[i].basic_info.professional_number,
													       school_level[i].basic_info.general_education_course1,
													       school_level[i].basic_info.general_education_course2,
													       school_level[i].basic_info.general_education_course3,
													       school_level[i].basic_info.public_foundation_course1,
													       school_level[i].basic_info.public_foundation_course2,
													       school_level[i].basic_info.public_foundation_course3,
													       school_level[i].basic_info.major_course1,
													       school_level[i].basic_info.major_course2,
													       school_level[i].total_score,
													       school_level[i].rank_score);


	}
#endif


