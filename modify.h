#ifndef MODIFY_H
#define MODIFY_H

#include "common.h"

void modifyEmployee(Employee employees[], int count) {
    char targetId[20];
    printf("请输入要修改的员工编号: ");
    scanf("%s", targetId);

    int index = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(employees[i].id, targetId) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("未找到编号为 %s 的员工。\n", targetId);
        return;
    }

    printf("当前信息: %s %s %s %d %s %.2f\n", 
           employees[index].id, employees[index].name, employees[index].gender, 
           employees[index].age, employees[index].department, employees[index].salary);

    printf("请重新输入该员工信息 (编号不可改):\n");
    
    printf("新姓名: ");
    scanf("%s", employees[index].name);
    
    printf("新性别: ");
    scanf("%s", employees[index].gender);
    
    printf("新年龄: ");
    scanf("%d", &employees[index].age);
    
    printf("新部门: ");
    scanf("%s", employees[index].department);
    
    printf("新工资: ");
    scanf("%lf", &employees[index].salary);

    printf("修改成功！\n");
}

#endif