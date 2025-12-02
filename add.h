#ifndef ADD_H
#define ADD_H

#include "common.h"

// 检查ID是否已存在，返回1存在，0不存在
int checkIdExists(Employee employees[], int count, char *id) {
    for (int i = 0; i < count; i++) {
        if (strcmp(employees[i].id, id) == 0) {
            return 1; 
        }
    }
    return 0;
}

void addEmployee(Employee employees[], int *count) {
    if (*count >= MAX_EMP) {
        printf("错误：员工人数已满，无法继续添加。\n");
        return;
    }

    Employee newEmp;
    printf("\n--- 录入新员工 ---\n");
    
    while (1) {
        printf("请输入员工编号: ");
        scanf("%s", newEmp.id);
        
        if (checkIdExists(employees, *count, newEmp.id)) {
            printf("错误：该编号已存在，请重新输入。\n");
        } else {
            break;
        }
    }

    printf("请输入姓名: ");
    scanf("%s", newEmp.name);
    
    printf("请输入性别: ");
    scanf("%s", newEmp.gender);
    
    printf("请输入年龄: ");
    scanf("%d", &newEmp.age);
    
    printf("请输入部门: ");
    scanf("%s", newEmp.department);
    
    printf("请输入工资: ");
    scanf("%lf", &newEmp.salary);

    // 添加到数组
    employees[*count] = newEmp;
    (*count)++;
    printf("成功录入员工：%s\n", newEmp.name);
}

#endif