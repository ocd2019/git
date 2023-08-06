#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define EPSINON 0.00001
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
	int rank_score;
	float total_score;
	BASIC_INFORMATION basic_info;
}EXCELLENT_STUDENT;


_Bool display_excellent_level_student(const char* file_name)
{
	int i = 0;
	EXCELLENT_STUDENT excellent_student = {0};

	FILE* fp = fopen(file_name, "r");
	if(NULL == fp)
	{
		printf("\tFailed to open %s\n", file_name);
		return 1;
	}

	while(!feof(fp))
	{
		fscanf(fp, "%s %s %s %d %d %d %d %d %d %d %d %f %d\n", 
					excellent_student.basic_info.id,
					excellent_student.basic_info.name,
					excellent_student.basic_info.professional_number,
					&excellent_student.basic_info.general_education_course1,
					&excellent_student.basic_info.general_education_course2,
					&excellent_student.basic_info.general_education_course3,
					&excellent_student.basic_info.public_foundation_course1,
					&excellent_student.basic_info.public_foundation_course2,
					&excellent_student.basic_info.public_foundation_course3,
					&excellent_student.basic_info.major_course1,
					&excellent_student.basic_info.major_course2,
					&excellent_student.total_score,
					&excellent_student.rank_score);

		printf("\t%s %s %s %d %d %d %d %d %d %d %d %f %d\n", 
					excellent_student.basic_info.id,
					excellent_student.basic_info.name,
					excellent_student.basic_info.professional_number,
					excellent_student.basic_info.general_education_course1,
					excellent_student.basic_info.general_education_course2,
					excellent_student.basic_info.general_education_course3,
					excellent_student.basic_info.public_foundation_course1,
					excellent_student.basic_info.public_foundation_course2,
					excellent_student.basic_info.public_foundation_course3,
					excellent_student.basic_info.major_course1,
					excellent_student.basic_info.major_course2,
					excellent_student.total_score,
					excellent_student.rank_score);

		i++;

		memset(&excellent_student, 0, sizeof(EXCELLENT_STUDENT));
	}

	if(0 == strncmp(file_name, "f2.txt", strlen(file_name)))
	{
		printf("\n\t*******************************************************\n");
		printf("\t* 	 school_level_excellent_number:%d            *\n", i);
		printf("\t*******************************************************\n\n");
	}
	else if(0 == strncmp(file_name, "f3.txt", strlen(file_name)))
	{
		printf("\n\t*******************************************************\n");
		printf("\t*      professional_level_excellent_number:%d        *\n", i);
		printf("\t*******************************************************\n\n");
	}

	fclose(fp);

	return 0;
}


_Bool write_excellent_student_data(const char* file_name, int total, EXCELLENT_STUDENT* info)
{
	int i = 0;

	FILE* fp = fopen(file_name, "w");
	if (NULL == fp)
	{
		printf("\tFailed to open %s\n", file_name);
		return 1;
	}

	for (i = 0; i < total; i++)
	{
		fprintf(fp, "%s %s %s %d %d %d %d %d %d %d %d %f %d\n", 
					info[i].basic_info.id,
					info[i].basic_info.name,
					info[i].basic_info.professional_number,
					info[i].basic_info.general_education_course1,
					info[i].basic_info.general_education_course2,
					info[i].basic_info.general_education_course3,
					info[i].basic_info.public_foundation_course1,
					info[i].basic_info.public_foundation_course2,
					info[i].basic_info.public_foundation_course3,
					info[i].basic_info.major_course1,
					info[i].basic_info.major_course2,
					info[i].total_score,
					info[i].rank_score);
	}

	fclose(fp);

	return 0;
}


int swap_excellent_level_student(EXCELLENT_STUDENT* result, int left, int right)
{
	BASIC_INFORMATION basic_info = { 0 };

	float original_score = result[left].total_score;

	memcpy(&basic_info, &result[left].basic_info, sizeof(BASIC_INFORMATION));

	while (left < right)
	{
		while (left < right && ((result[right].total_score - original_score) < -EPSINON || fabs(result[right].total_score - original_score) <= EPSINON))
			right--;

		result[left].total_score = result[right].total_score;
		memset(&result[left].basic_info, 0, sizeof(BASIC_INFORMATION));
		memcpy(&result[left].basic_info, &result[right].basic_info, sizeof(BASIC_INFORMATION));

		while (left < right && ((result[left].total_score - original_score) > EPSINON || fabs(result[left].total_score - original_score) <= EPSINON))
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


_Bool sort_excellent_level_student(EXCELLENT_STUDENT* level_result, int left_node, int right_node)
{
	int i = 0;

	if (left_node < right_node)
	{
		i = swap_excellent_level_student(level_result, left_node, right_node);

		sort_excellent_level_student(level_result, left_node, i - 1);

		sort_excellent_level_student(level_result, i + 1, right_node);
	}

	return 0;
}


_Bool rank_excellent_level_student(int number, int count, EXCELLENT_STUDENT* info)
{
	int i = 0;
	int j = 0;
	
	sort_excellent_level_student(info, 0, count - 1);

	for (i = 0; i < number; i++)
	{
		if (0 == info[i].rank_score) {
			info[i].rank_score = ++j;
		}

		if ((i + 1) < number && fabs(info[i].total_score - info[i + 1].total_score) <= EPSINON) {
			info[i + 1].rank_score = info[i].rank_score;
		}
	}

	return 0;
}


int compare_student_id_sort(int total, int count, EXCELLENT_STUDENT* old_data, EXCELLENT_STUDENT* new_data, EXCELLENT_STUDENT* excellent_student)
{
	int i = 0;
	int j = 0;
	int k = 0;

	for (i = 0; i < count; i++)
	{
		for (j = 0; j < total; j++)
		{
			if (0 == strncmp(old_data[i].basic_info.id, new_data[j].basic_info.id, 5))
			{
				excellent_student[k].total_score = new_data[j].total_score;
				excellent_student[k].rank_score = new_data[j].rank_score;
				memcpy(&excellent_student[k].basic_info, &new_data[j].basic_info, sizeof(BASIC_INFORMATION));
				k++;
				break;
			}
		}
	}

	return k;
}


_Bool filter_repeat_level_student(int count, EXCELLENT_STUDENT* info)
{
	int i = 0;
	int j = 0;
	int total = 0;

	FILE* fp = fopen("f2.txt", "r");
	if (NULL == fp)
	{
		printf("\tFailed to open f2.txt\n");
		return 1;
	}

	EXCELLENT_STUDENT* tmp_data = (EXCELLENT_STUDENT*)malloc(MAX_STUDENT_NUMBER * sizeof(EXCELLENT_STUDENT));

	if (NULL == tmp_data) return 1;

	memset(tmp_data, 0, MAX_STUDENT_NUMBER * sizeof(EXCELLENT_STUDENT));

	while (!feof(fp))
	{
		fscanf(fp, "%s %s %s %d %d %d %d %d %d %d %d %f %d\n",
					tmp_data[total].basic_info.id,
					tmp_data[total].basic_info.name,
					tmp_data[total].basic_info.professional_number,
					&tmp_data[total].basic_info.general_education_course1,
					&tmp_data[total].basic_info.general_education_course2,
					&tmp_data[total].basic_info.general_education_course3,
					&tmp_data[total].basic_info.public_foundation_course1,
					&tmp_data[total].basic_info.public_foundation_course2,
					&tmp_data[total].basic_info.public_foundation_course3,
					&tmp_data[total].basic_info.major_course1,
					&tmp_data[total].basic_info.major_course2,
					&tmp_data[total].total_score,
					&tmp_data[total].rank_score);
		total++;
	}

	fclose(fp);

	for (i = 0; i < count; i++)
	{
		for (j = 0; j < total; j++)
		{
			if (0 == strncmp(info[i].basic_info.id, tmp_data[j].basic_info.id, 5)) {
				memset(&info[i], 0, sizeof(EXCELLENT_STUDENT));
			}
		}
	}

	free(tmp_data);

	tmp_data = NULL;

	return 0;
}


_Bool filter_professional_level_student(int count, EXCELLENT_STUDENT* info)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int total = 0;
	int offset = 0;

	EXCELLENT_STUDENT* tmp_data = (EXCELLENT_STUDENT*)malloc(count * sizeof(EXCELLENT_STUDENT));
	EXCELLENT_STUDENT* new_data = (EXCELLENT_STUDENT*)malloc(count * sizeof(EXCELLENT_STUDENT));
	EXCELLENT_STUDENT* old_data = (EXCELLENT_STUDENT*)malloc(count * sizeof(EXCELLENT_STUDENT));

	if (NULL == tmp_data || NULL == new_data || NULL == old_data) return 1;

	memset(tmp_data, 0, count * sizeof(EXCELLENT_STUDENT));
	memset(new_data, 0, count * sizeof(EXCELLENT_STUDENT));
	memset(old_data, 0, count * sizeof(EXCELLENT_STUDENT));

	memcpy(old_data, info, count * sizeof(EXCELLENT_STUDENT));

	for (i = 0; i < count; i++)
	{
		if (0 != strlen(info[i].basic_info.professional_number))
		{
			memcpy(&tmp_data[j], &info[i], sizeof(EXCELLENT_STUDENT));

			memset(&info[i], 0, sizeof(EXCELLENT_STUDENT));

			for (k = 0; k < count; k++)
			{
				if (0 == strncmp(tmp_data[j].basic_info.professional_number, info[k].basic_info.professional_number, 2))
				{
					memcpy(&tmp_data[++j], &info[k], sizeof(EXCELLENT_STUDENT));
					memset(&info[k], 0, sizeof(EXCELLENT_STUDENT));
				}
			}

			offset = (int)(0.1 * j + 0.5);

			rank_excellent_level_student(offset, j, tmp_data);

			memcpy(new_data + total, tmp_data, offset * sizeof(EXCELLENT_STUDENT));

			memset(tmp_data, 0, count * sizeof(EXCELLENT_STUDENT));

			total += offset;

			j = 0;
		}
	}

	filter_repeat_level_student(total, new_data);

	EXCELLENT_STUDENT* excellent_student = (EXCELLENT_STUDENT*)malloc(MAX_STUDENT_NUMBER * sizeof(EXCELLENT_STUDENT));

	if (NULL == excellent_student) return 1;

	memset(excellent_student, 0, MAX_STUDENT_NUMBER * sizeof(EXCELLENT_STUDENT));

	k = compare_student_id_sort(total, count, old_data, new_data, excellent_student);

	write_excellent_student_data("f3.txt", k, excellent_student);

	free(tmp_data);
	free(new_data);
	free(old_data);
	free(excellent_student);

	tmp_data = NULL;
	new_data = NULL;
	old_data = NULL;
	excellent_student = NULL;

	return 0;
}


_Bool filter_school_level_student(int total, int count, EXCELLENT_STUDENT* info)
{
	int k = 0;

	EXCELLENT_STUDENT* old_data = (EXCELLENT_STUDENT*)malloc(count * sizeof(EXCELLENT_STUDENT));

	if (NULL == old_data) return 1;

	memset(old_data, 0, count * sizeof(EXCELLENT_STUDENT));

	memcpy(old_data, info, count * sizeof(EXCELLENT_STUDENT));

	total = (int)(0.05 * total + 0.5);

	rank_excellent_level_student(total, count, info);

	EXCELLENT_STUDENT* excellent_student = (EXCELLENT_STUDENT*)malloc(total * sizeof(EXCELLENT_STUDENT));

	if (NULL == excellent_student) return 1;

	memset(excellent_student, 0, total * sizeof(EXCELLENT_STUDENT));

	k = compare_student_id_sort(total, count, old_data, info, excellent_student);

	write_excellent_student_data("f2.txt", k, excellent_student);

	free(old_data);
	free(excellent_student);

	old_data = NULL;
	excellent_student = NULL;

	return 0;
}


_Bool filter_excellent_level_student(int total, BASIC_INFORMATION info[])
{
	int i = 0;
	int j = 0;

	BASIC_INFORMATION* basic_excellent = (BASIC_INFORMATION *)malloc(total * sizeof(BASIC_INFORMATION));

	for (i = 0; i < total; i++)
	{
		if (info[i].general_education_course1 >= 60
			&& info[i].general_education_course2 >= 60
			&& info[i].general_education_course3 >= 60
			&& info[i].public_foundation_course1 >= 70
			&& info[i].public_foundation_course2 >= 70
			&& info[i].public_foundation_course3 >= 70
			&& info[i].major_course1 >= 80
			&& info[i].major_course2 >= 80)
		{
			memcpy(&basic_excellent[j++], &info[i], sizeof(BASIC_INFORMATION));
		}
	}

	EXCELLENT_STUDENT* excellent_student_1 = (EXCELLENT_STUDENT *)malloc(j * sizeof(EXCELLENT_STUDENT));
	EXCELLENT_STUDENT* excellent_student_2 = (EXCELLENT_STUDENT *)malloc(j * sizeof(EXCELLENT_STUDENT));

	if(NULL == excellent_student_1 || NULL == excellent_student_2) return 1;

	memset(excellent_student_1, 0, j * sizeof(EXCELLENT_STUDENT));
	memset(excellent_student_2, 0, j * sizeof(EXCELLENT_STUDENT));

	for (i = 0; i < j; i++)
	{
		excellent_student_1[i].rank_score = 0;
		memcpy(&excellent_student_1[i].basic_info, &basic_excellent[i], sizeof(BASIC_INFORMATION));
		excellent_student_1[i].total_score = 0.8 * (basic_excellent[i].public_foundation_course1 + \
			basic_excellent[i].public_foundation_course2 + \
			basic_excellent[i].public_foundation_course3) + \
			1.2 * (basic_excellent[i].major_course1 + \
			basic_excellent[i].major_course2);
	}

	memcpy(excellent_student_2, excellent_student_1, j * sizeof(EXCELLENT_STUDENT));

	filter_school_level_student(total, j, excellent_student_1);

	filter_professional_level_student(j, excellent_student_2);

	//printf("\ttotal_number:%d\n", total);
	//printf("\texcellent_number:%d\n", j);

	free(basic_excellent);
	free(excellent_student_1);
	free(excellent_student_2);

	basic_excellent = NULL;
	excellent_student_1 = NULL;
	excellent_student_2 = NULL;

	return 0;
}



_Bool read_original_student_data()
{
	int total = 0;
	BASIC_INFORMATION basic_info[MAX_STUDENT_NUMBER] = {0};

	FILE* fp = fopen("f1.txt", "r");
	if(NULL == fp)
	{
		printf("\tFailed to open f1.txt\n");
		return 1;
	}

	while (!feof(fp))
	{
		fscanf(fp, "%s %s %s %d %d %d %d %d %d %d %d ", 
					basic_info[total].id,
					basic_info[total].name,
					basic_info[total].professional_number,
					&basic_info[total].general_education_course1,
					&basic_info[total].general_education_course2,
					&basic_info[total].general_education_course3,
					&basic_info[total].public_foundation_course1,
					&basic_info[total].public_foundation_course2,
					&basic_info[total].public_foundation_course3,
					&basic_info[total].major_course1,
					&basic_info[total].major_course2);
		total++;
	}

	fclose(fp);

	filter_excellent_level_student(total, basic_info);

	return 0;
}


int main(int argc, char* argv[])
{
	int index = -1;

	do 
	{
		printf("\t*******************************************************\n");
		printf("\t*        1.read data                                  *\n");
		printf("\t*        2.display school level student               *\n");
		printf("\t*        3.display professional level student         *\n");
		printf("\t*        0.exit student management system             *\n");
		printf("\t*******************************************************\n");
	
		printf("\n\tplease input your choice:");

		scanf("%d", &index);

		switch (index)
		{
			case 0:
				break;
			case 1:
				read_original_student_data();
				break;
			case 2:
				display_excellent_level_student("f2.txt");
				break;
			case 3:
				display_excellent_level_student("f3.txt");
				break;
			default:
				printf("\tyour choice is not support!\n");
		}
	} while (0 != index);

	return 0;
}

#if 0
for (i = 0; i < total; i++)
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



