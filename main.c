#include <stdio.h>
#include "common.h"
#include "emp_manager.h"

void showMenu() {
    printf("\n=============================\n");
    printf("      人事管理系统\n");
    printf("=============================\n");
    printf("1. 录入员工信息\n");
    printf("2. 浏览所有员工\n");
    printf("3. 查询员工信息\n");
    printf("4. 修改员工信息\n");
    printf("5. 删除员工信息\n");
    printf("6. 工资统计与排序\n");
    printf("0. 退出系统\n");
    printf("=============================\n");
    printf("请输入选项: ");
}

int main() {
    Employee employees[MAX_EMP]; // 员工数组
    int count = 0;               // 当前员工数量

    // 程序启动时从文本文档中读入数据
    loadData(employees, &count);

    int choice;
    do {
        showMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addEmployee(employees, &count);
                // 每次增删改后更新文件
                saveData(employees, count);
                break;
            case 2:
                printAllEmployees(employees, count);
                break;
            case 3:
                searchMenu(employees, count);
                break;
            case 4:
                modifyEmployee(employees, count);
                saveData(employees, count);
                break;
            case 5:
                deleteEmployee(employees, &count);
                saveData(employees, count);
                break;
            case 6:
                statsMenu(employees, count);
                break;
            case 0:
                printf("退出系统，再见！\n");
                break;
            default:
                printf("无效输入，请重新选择。\n");
        }
    } while (choice != 0);

    return 0;
}