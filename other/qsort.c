#include <stdio.h>
#include <stdlib.h>

// 定义一个包含两个成员的结构体
struct person {
    char name[20];
    int age;
};

// 比较函数,用于qsort排序时比较结构体的age大小
int cmpfunc(const void *a, const void *b) {
    struct person *personA = (struct person *)a;
    struct person *personB = (struct person *)b;
    return (personA->age - personB->age);
}

int main() {
    // 初始化结构体数组
    struct person people[] = {
        {"Tom", 25},
        {"Bob", 30},
        {"Jenny", 20},
        {"Alice", 28},
        {"Dave", 22}
    };
    int num_people = sizeof(people) / sizeof(struct person);

    // 打印排序前的结果
    printf("排序前：\n");
    for(int i=0; i<num_people; i++){
        printf("%s %d\n", people[i].name, people[i].age);
    }

    // 对数组进行排序
    qsort(people, num_people, sizeof(struct person), cmpfunc);

    // 打印排序后的结果
    printf("\n排序后：\n");
    for(int i=0; i<num_people; i++){
        printf("%s %d\n", people[i].name, people[i].age);
    }

    return 0;
}

