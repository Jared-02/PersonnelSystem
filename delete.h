#ifndef DELETE_H
#define DELETE_H

#include "common.h"

void deleteEmployee(Employee employees[], int *count) {
    char targetId[20];
    printf("请输入要删除的员工编号: ");
    scanf("%s", targetId);

    int index = -1;
    for (int i = 0; i < *count; i++) {
        if (strcmp(employees[i].id, targetId) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("未找到编号为 %s 的员工。\n", targetId);
        return;
    }

    // 确认删除
    int confirm;
    printf("确认删除员工 %s 吗？(1:是 / 0:否): ", employees[index].name);
    scanf("%d", &confirm);
    if (confirm != 1) {
        printf("已取消删除。\n");
        return;
    }

    // 执行删除：将最后一个元素覆盖到当前位置
    // 如果删除的是最后一个，直接减count即可，这里逻辑通用
    employees[index] = employees[(*count) - 1];
    (*count)--;

    printf("删除成功。\n");
}

#endif