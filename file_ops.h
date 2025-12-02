#ifndef FILE_OPS_H
#define FILE_OPS_H

#include "common.h"

// 将所有员工数据保存到文件
void saveData(Employee employees[], int count) {
    FILE *fp = fopen(DATA_FILE, "w");
    if (fp == NULL) {
        printf("错误：无法打开文件进行写入。\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        // 将信息格式化写入，用空格分隔
        fprintf(fp, "%s %s %s %d %s %.2f\n", 
                employees[i].id, 
                employees[i].name, 
                employees[i].gender, 
                employees[i].age, 
                employees[i].department, 
                employees[i].salary);
    }

    fclose(fp);
    printf("[系统提示] 数据已自动保存。\n");
}

// 从文件加载数据
void loadData(Employee employees[], int *count) {
    FILE *fp = fopen(DATA_FILE, "r");
    if (fp == NULL) {
        // 第一次运行时文件可能不存在，这很正常
        *count = 0;
        return;
    }

    *count = 0;
    // 循环读取，直到文件结束或数组满
    while (fscanf(fp, "%s %s %s %d %s %lf", 
                  employees[*count].id, 
                  employees[*count].name, 
                  employees[*count].gender, 
                  &employees[*count].age, 
                  employees[*count].department, 
                  &employees[*count].salary) != EOF) {
        (*count)++;
        if (*count >= MAX_EMP) break;
    }

    fclose(fp);
    printf("[系统提示] 成功读取了 %d 条员工记录。\n", *count);
}

#endif