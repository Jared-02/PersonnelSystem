#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义最大员工数量
#define MAX_EMP 1000
// 数据文件名
#define DATA_FILE "employees.txt"

// 员工结构体
typedef struct {
    char id[20];        // 员工编号
    char name[50];      // 姓名
    char gender[10];    // 性别
    int age;            // 年龄
    char department[50];// 部门
    double salary;      // 工资
} Employee;

#endif