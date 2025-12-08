#ifndef EMP_MANAGER_H
#define EMP_MANAGER_H

#include "common.h"

// 文件操作
void loadData(Employee employees[], int *count);
void saveData(Employee employees[], int count);

// 核心功能
void addEmployee(Employee employees[], int *count);
void printAllEmployees(Employee employees[], int count);
void modifyEmployee(Employee employees[], int count);
void deleteEmployee(Employee employees[], int *count);

// 统计与排序
void statsMenu(Employee employees[], int count);

// 查询功能
void searchMenu(Employee employees[], int count);

#endif