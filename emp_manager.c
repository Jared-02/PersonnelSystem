#include "emp_manager.h"

// --- 内部辅助函数 (不暴露给 main) ---
void printHeader() {
    printf("\n%-10s %-10s %-8s %-6s %-15s %-10s\n", "编号", "姓名", "性别", "年龄", "部门", "工资");
    printf("----------------------------------------------------------------\n");
}

void printEmployee(Employee e) {
    printf("%-10s %-10s %-8s %-6d %-15s %-10.2f\n", 
           e.id, e.name, e.gender, e.age, e.department, e.salary);
}

// --- 文件操作 ---
void saveData(Employee employees[], int count) {
    FILE *fp = fopen(DATA_FILE, "w");
    if (fp == NULL) {
        printf("错误：无法打开文件进行写入。\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s %s %s %d %s %.2f\n", 
                employees[i].id, employees[i].name, employees[i].gender, 
                employees[i].age, employees[i].department, employees[i].salary);
    }
    fclose(fp);
}

void loadData(Employee employees[], int *count) {
    FILE *fp = fopen(DATA_FILE, "r");
    *count = 0;
    if (fp == NULL) return;

    while (fscanf(fp, "%s %s %s %d %s %lf", 
                  employees[*count].id, employees[*count].name, employees[*count].gender, 
                  &employees[*count].age, employees[*count].department, 
                  &employees[*count].salary) != EOF) {
        (*count)++;
        if (*count >= MAX_EMP) break;
    }
    fclose(fp);
    printf("[系统提示] 已读取 %d 条记录。\n", *count);
}

// --- 录入功能 ---
void addEmployee(Employee employees[], int *count) {
    if (*count >= MAX_EMP) {
        printf("人数已满。\n");
        return;
    }
    Employee e;
    printf("请输入编号: "); scanf("%s", e.id);
    // 简单排重检查
    for(int i=0; i<*count; i++) {
        if(strcmp(employees[i].id, e.id) == 0) {
            printf("编号已存在！\n"); return;
        }
    }
    printf("请输入姓名: "); scanf("%s", e.name);
    printf("请输入性别: "); scanf("%s", e.gender);
    printf("请输入年龄: "); scanf("%d", &e.age);
    printf("请输入部门: "); scanf("%s", e.department);
    printf("请输入工资: "); scanf("%lf", &e.salary);

    employees[*count] = e;
    (*count)++;
    printf("添加成功。\n");
}

// --- 浏览功能 ---
void printAllEmployees(Employee employees[], int count) {
    printHeader();
    for(int i=0; i<count; i++) printEmployee(employees[i]);
}

// --- 【重点优化】通用查询逻辑 ---
// searchType: 1=按部门, 2=按工资范围, 3=按编号(用于内部查找)
void performSearch(Employee employees[], int count, int searchType) {
    char targetStr[50]; // 用于存储目标字符串（部门或ID）
    double minS, maxS;  // 用于存储工资范围
    int found = 0;

    // 1. 根据类型获取用户输入
    if (searchType == 1) {
        printf("请输入部门名称: ");
        scanf("%s", targetStr);
    } else if (searchType == 2) {
        printf("请输入工资范围 (最低 最高): ");
        scanf("%lf %lf", &minS, &maxS);
    }

    printHeader();

    // 2. 统一遍历，根据类型分支判断
    for (int i = 0; i < count; i++) {
        int isMatch = 0;
        
        switch (searchType) {
            case 1: // 按部门 (字符串匹配)
                if (strcmp(employees[i].department, targetStr) == 0) isMatch = 1;
                break;
            case 2: // 按工资 (数值范围)
                if (employees[i].salary >= minS && employees[i].salary <= maxS) isMatch = 1;
                break;
        }

        if (isMatch) {
            printEmployee(employees[i]);
            found = 1;
        }
    }

    if (!found) printf("未找到匹配的记录。\n");
}

void searchMenu(Employee employees[], int count) {
    int choice;
    printf("\n1. 按部门查询\n2. 按工资范围查询\n请选择: ");
    scanf("%d", &choice);
    if (choice == 1 || choice == 2) {
        performSearch(employees, count, choice);
    } else {
        printf("无效选择。\n");
    }
}

// --- 修改与删除 (简化示意，逻辑同前) ---
void modifyEmployee(Employee employees[], int count) {
    char id[20];
    printf("输入要修改的ID: "); scanf("%s", id);
    for(int i=0; i<count; i++) {
        if(strcmp(employees[i].id, id) == 0) {
            printf("找到员工 %s，请输入新工资: ", employees[i].name);
            scanf("%lf", &employees[i].salary);
            printf("修改完成。\n");
            return;
        }
    }
    printf("未找到该ID。\n");
}

void deleteEmployee(Employee employees[], int *count) {
    char id[20];
    printf("输入要删除的ID: "); scanf("%s", id);
    for(int i=0; i<*count; i++) {
        if(strcmp(employees[i].id, id) == 0) {
            employees[i] = employees[(*count)-1]; // 覆盖删除
            (*count)--;
            printf("删除成功。\n");
            return;
        }
    }
    printf("未找到该ID。\n");
}

// --- 统计 (简化) ---
void statsMenu(Employee employees[], int count) {
    if (count == 0) return;
    double sum = 0;
    for(int i=0; i<count; i++) sum += employees[i].salary;
    printf("平均工资: %.2f\n", sum / count);
}