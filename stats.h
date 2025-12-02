#ifndef STATS_H
#define STATS_H

#include "common.h"
#include "search.h" // 为了使用 printAllEmployees

// 按工资排序 (冒泡排序，降序)
void sortBySalary(Employee employees[], int count) {
    // 为了不打乱原始数据顺序，通常建议复制一份数据进行排序
    // 但作为初学者练习，直接对原数组排序也是可以的
    if (count < 2) {
        printAllEmployees(employees, count);
        return;
    }

    // 简单的冒泡排序
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - 1 - i; j++) {
            if (employees[j].salary < employees[j+1].salary) {
                Employee temp = employees[j];
                employees[j] = employees[j+1];
                employees[j+1] = temp;
            }
        }
    }
    printf("\n[系统提示] 已按工资从高到低排序。\n");
    printAllEmployees(employees, count);
}

// 部门工资统计
void salaryStats(Employee employees[], int count) {
    if (count == 0) return;

    double totalSalary = 0;
    double maxSalary = employees[0].salary;
    char maxName[50];
    strcpy(maxName, employees[0].name);

    for(int i=0; i<count; i++) {
        totalSalary += employees[i].salary;
        if(employees[i].salary > maxSalary) {
            maxSalary = employees[i].salary;
            strcpy(maxName, employees[i].name);
        }
    }

    printf("\n--- 工资统计 ---\n");
    printf("全公司平均工资: %.2f\n", totalSalary / count);
    printf("全公司最高工资: %.2f (归属者: %s)\n", maxSalary, maxName);
}

// 扩展：按性别统计人数
void genderStats(Employee employees[], int count) {
    int male = 0;
    int female = 0;
    for(int i=0; i<count; i++) {
        if(strcmp(employees[i].gender, "男") == 0) male++;
        else if(strcmp(employees[i].gender, "女") == 0) female++;
    }
    printf("\n--- 性别分布统计 ---\n");
    printf("男: %d 人\n", male);
    printf("女: %d 人\n", female);
    printf("其他/未知: %d 人\n", count - male - female);
}

void statsMenu(Employee employees[], int count) {
    int choice;
    printf("\n--- 统计与排序 ---\n");
    printf("1. 按工资降序显示所有员工\n");
    printf("2. 查看工资统计信息 (平均/最高)\n");
    printf("3. 查看性别分布统计\n");
    printf("请选择: ");
    scanf("%d", &choice);

    switch(choice) {
        case 1: sortBySalary(employees, count); break;
        case 2: salaryStats(employees, count); break;
        case 3: genderStats(employees, count); break;
        default: printf("无效选择。\n");
    }
}

#endif