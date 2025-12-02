#ifndef SEARCH_H
#define SEARCH_H

#include "common.h"

// 辅助函数：打印表头
void printHeader() {
    printf("\n%-10s %-10s %-8s %-6s %-15s %-10s\n", "编号", "姓名", "性别", "年龄", "部门", "工资");
    printf("----------------------------------------------------------------\n");
}

// 辅助函数：打印单行员工信息
void printEmployee(Employee e) {
    printf("%-10s %-10s %-8s %-6d %-15s %-10.2f\n", 
           e.id, e.name, e.gender, e.age, e.department, e.salary);
}

// 功能：浏览所有员工
void printAllEmployees(Employee employees[], int count) {
    if (count == 0) {
        printf("暂无员工信息。\n");
        return;
    }
    printHeader();
    for (int i = 0; i < count; i++) {
        printEmployee(employees[i]);
    }
    printf("----------------------------------------------------------------\n");
    printf("共 %d 名员工。\n", count);
}

// 子功能：按部门查询
void searchByDept(Employee employees[], int count) {
    char dept[50];
    printf("请输入要查询的部门: ");
    scanf("%s", dept);
    
    int found = 0;
    printHeader();
    for(int i=0; i<count; i++) {
        if(strcmp(employees[i].department, dept) == 0) {
            printEmployee(employees[i]);
            found = 1;
        }
    }
    if(!found) printf("未找到该部门的员工。\n");
}

// 子功能：按工资范围查询
void searchBySalary(Employee employees[], int count) {
    double minS, maxS;
    printf("请输入最低工资: ");
    scanf("%lf", &minS);
    printf("请输入最高工资: ");
    scanf("%lf", &maxS);

    int found = 0;
    printHeader();
    for(int i=0; i<count; i++) {
        if(employees[i].salary >= minS && employees[i].salary <= maxS) {
            printEmployee(employees[i]);
            found = 1;
        }
    }
    if(!found) printf("该范围内没有员工。\n");
}

// 查询菜单
void searchMenu(Employee employees[], int count) {
    int choice;
    printf("\n--- 查询菜单 ---\n");
    printf("1. 按部门查询\n");
    printf("2. 按工资范围查询\n");
    printf("请选择: ");
    scanf("%d", &choice);

    switch(choice) {
        case 1: searchByDept(employees, count); break;
        case 2: searchBySalary(employees, count); break;
        default: printf("无效选择。\n");
    }
}

#endif