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
    Employee *head = NULL;
    loadData(&head); // 传地址

    int choice;
    do {
        showMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                addEmployee(&head);
                saveData(head);
                break;
            case 2:
                printAllEmployees(head);
                break;
            case 3:
                searchMenu(head);
                break;
            case 4:
                modifyEmployee(head);
                saveData(head);
                break;
            case 5:
                deleteEmployee(&head);
                saveData(head);
                break;
            case 6:
                statsMenu(head);
                break;
            case 0:
                printf("[INFO] 退出系统，再见！\n");
                break;
            default:
                printf("[WARN] 无效输入，请重新选择。\n");
        }
    } while (choice != 0);

    freeList(&head); // 退出清理
    return 0;
}