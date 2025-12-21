#ifndef EMP_MANAGER_H
#define EMP_MANAGER_H

#include "common.h"
// 条件函数指针
typedef int (*Predicate)(const Employee *e, void *args);
// 范围结构体
typedef struct {
    double min;
    double max;
} NumberRange;
typedef struct {
    double totalSalary;
    int count;
    double maxSalary;
    double minSalary;
} SalaryStats;

// 基础链表操作
// 释放链表（二级指针）
void freeList(Employee **head); 

// 文件操作
// 加载数据（二级指针）
void loadData(Employee **head); 
// 保存数据（一级指针）
void saveData(Employee *head);  

// 核心功能
// 添加节点（二级指针）
void addEmployee(Employee **head); 
// 删除节点
void deleteEmployee(Employee **head);

// 浏览功能（一级指针）
void printAllEmployees(Employee *head);
// 修改功能
void modifyEmployee(Employee *head);
// 统计与排序
void statsMenu(Employee *head);
// 查询功能
void searchMenu(Employee *head);

#endif