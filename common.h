#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FILE "employees.txt"

typedef struct Employee {
    char id[20];           // 员工编号
    char name[50];         // 姓名
    char gender[10];       // 性别
    int age;               // 年龄
    char department[50];   // 部门
    double salary;         // 工资
    struct Employee *next; // 链表指针
} Employee;

#endif