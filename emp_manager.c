#include "emp_manager.h"

// --- 内部辅助函数 ---

void printHeader() {
    printf("\n%-10s %-10s %-8s %-6s %-15s %-10s\n", "工号", "姓名", "性别", "年龄", "部门", "工资");
    printf("----------------------------------------------------------------\n");
}

void printEmployee(Employee *e) {
    if (e == NULL) return;
    printf("%-10s %-10s %-8s %-6d %-15s %-10.2f\n", 
           e->id, e->name, e->gender, e->age, e->department, e->salary);
}

// 创建新节点
Employee* createNode() {
    Employee *newNode = (Employee*)malloc(sizeof(Employee));
    if (newNode) newNode->next = NULL;
    return newNode;
}

// 查找节点
Employee* findById(Employee *head, const char *id) {
    Employee *curr = head;
    while (curr != NULL) {
        if (strcmp(curr->id, id) == 0) return curr;
        curr = curr->next;
    }
    return NULL;
}

// 追加节点
void appendNode(Employee **head, Employee *newNode) {
    newNode->next = NULL; 
    if (*head == NULL) {
        *head = newNode;
    } else {
        Employee *temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// --- 外部接口 ---

void freeList(Employee **head) {
    Employee *curr = *head;
    while (curr != NULL) {
        Employee *next = curr->next;
        free(curr);
        curr = next;
    }
    *head = NULL;
}

void loadData(Employee **head) {
    FILE *fp = fopen(DATA_FILE, "r");
    if (fp == NULL) return;

    freeList(head); // 先清空

    while (1) {
        Employee *newNode = createNode();
        if (fscanf(fp, "%s %s %s %d %s %lf", 
                   newNode->id, newNode->name, newNode->gender, 
                   &newNode->age, newNode->department, &newNode->salary) != EOF) {
            appendNode(head, newNode);
        } else {
            free(newNode);
            break;
        }
    }
    fclose(fp);
    printf("[INFO] 数据加载完成。\n");
}

void saveData(Employee *head) {
    FILE *fp = fopen(DATA_FILE, "w");
    if (fp == NULL) {
        printf("无法写入文件。\n"); return;
    }
    Employee *curr = head;
    while (curr != NULL) {
        fprintf(fp, "%s %s %s %d %s %.2f\n", 
                curr->id, curr->name, curr->gender, 
                curr->age, curr->department, curr->salary);
        curr = curr->next;
    }
    fclose(fp);
    printf("[系统] 数据已保存。\n");
}

void addEmployee(Employee **head) {
    Employee *newNode = createNode();
    printf("\n--- 录入员工 ---\n");
    
    while (1) {
        printf("输入工号: "); scanf("%s", newNode->id);
        if (findById(*head, newNode->id)) {
            printf("工号已存在，请重输。\n");
        } else {
            break;
        }
    }
    printf("输入姓名: "); scanf("%s", newNode->name);
    printf("输入性别: "); scanf("%s", newNode->gender);
    printf("输入年龄: "); scanf("%d", &newNode->age);
    printf("输入部门: "); scanf("%s", newNode->department);
    printf("输入工资: "); scanf("%lf", &newNode->salary);

    appendNode(head, newNode);
    printf("录入成功。\n");
}

void printAllEmployees(Employee *head) {
    if (head == NULL) {
        printf("暂无数据。\n"); return;
    }
    printHeader();
    Employee *curr = head;
    while (curr != NULL) {
        printEmployee(curr);
        curr = curr->next;
    }
}

void modifyEmployee(Employee *head) {
    char id[20];
    printf("请输入要修改的员工工号: "); 
    scanf("%s", id);
    
    Employee *target = findById(head, id);
    
    if (target == NULL) {
        printf("未找到工号为 %s 的员工。\n", id);
        return;
    }
    // 显示当前信息
    printf("------------------------------------------------\n");
    printf("当前信息 > 姓名:%s | 性别:%s | 年龄:%d | 部门:%s | 工资:%.2f\n", 
           target->name, target->gender, target->age, target->department, target->salary);
    printf("------------------------------------------------\n");
    printf("请依次输入新信息 (注意：工号不可修改):\n", target->id);

    printf("请输入新姓名: ");
    scanf("%s", target->name);

    printf("请输入新性别: ");
    scanf("%s", target->gender);

    printf("请输入新年龄: ");
    scanf("%d", &target->age);

    printf("请输入新部门: ");
    scanf("%s", target->department);

    printf("请输入新工资: ");
    scanf("%lf", &target->salary);

    printf(">>> 修改成功！\n");
}

void deleteEmployee(Employee **head) {
    char id[20];
    printf("输入要删除的工号: "); scanf("%s", id);

    Employee *curr = *head;
    Employee *prev = NULL;

    while (curr != NULL) {
        if (strcmp(curr->id, id) == 0) {
            if (prev == NULL) *head = curr->next;
            else prev->next = curr->next;
            free(curr);
            printf("删除成功。\n");
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("未找到。\n");
}

void searchMenu(Employee *head) {
    int choice;
    printf("\n1.工号查询 2.部门查询 3.工资查询\n选择: ");
    scanf("%d", &choice);
    
    printHeader();
    int found = 0;
    Employee *curr = head;

    if (choice == 1) {
        char id[20];
        printf("输入工号: ");
        scanf("%s", id);
        while(curr) {
            if (strcmp(curr->id, id) == 0) {
                printEmployee(curr); found=1;
            }
            curr = curr->next;
        }
    } else if (choice == 2) {
        char dept[50];
        printf("输入部门: ");
        scanf("%s", dept);
        while(curr) {
            if (strcmp(curr->department, dept) == 0) {
                printEmployee(curr); found=1;
            }
            curr = curr->next;
        }
    } else if (choice == 3) {
        double min, max;
        printf("输入范围 (最小 最大): ");
        scanf("%lf %lf", &min, &max);
        while(curr) {
            if (curr->salary >= min && curr->salary <= max) {
                printEmployee(curr); found=1;
            }
            curr = curr->next;
        }
    }
    if (!found) printf("未找到匹配记录。\n");
}

// 链表冒泡排序 (只交换数据，不换节点)
void bubbleSort(Employee *head) {
    if (head == NULL || head->next == NULL) return;
    int swapped;
    Employee *ptr1;
    Employee *lptr = NULL; // 标记已排序的部分，每轮必有沉底

    do {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr) {
            // 降序排列 (工资高的在前)
            if (ptr1->salary < ptr1->next->salary) { 
                // 交换数据
                // 1. 交换工资 (浮点数)
                double tempSalary = ptr1->salary;
                ptr1->salary = ptr1->next->salary;
                ptr1->next->salary = tempSalary;
                
                // 2. 交换年龄 (整数)
                int tempAge = ptr1->age;
                ptr1->age = ptr1->next->age;
                ptr1->next->age = tempAge;

                // 3. 交换工号、姓名、性别、部门 (字符串)
                char tempStr[50];
                strcpy(tempStr, ptr1->id);
                strcpy(ptr1->id, ptr1->next->id);
                strcpy(ptr1->next->id, tempStr);

                strcpy(tempStr, ptr1->name);
                strcpy(ptr1->name, ptr1->next->name);
                strcpy(ptr1->next->name, tempStr);

                strcpy(tempStr, ptr1->gender);
                strcpy(ptr1->gender, ptr1->next->gender);
                strcpy(ptr1->next->gender, tempStr);

                strcpy(tempStr, ptr1->department);
                strcpy(ptr1->department, ptr1->next->department);
                strcpy(ptr1->next->department, tempStr);

                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
    
    printf("排序完成 (按工资降序)\n");
}


void statsMenu(Employee *head) {
    int c;
    printf("\n1.按工资降序显示 2.统计平均/最高工资\n选择: ");
    scanf("%d", &c);
    if (c == 1) {
        bubbleSort(head);
        printAllEmployees(head);
    } else if (c == 2) {
        if(!head) return;
        double sum = 0, max = -1;
        int n = 0;
        Employee *p = head;
        while(p) {
            sum += p->salary;
            if(p->salary > max) max = p->salary;
            n++;
            p = p->next;
        }
        printf("平均: %.2f，最高: %.2f\n", sum/n, max);
    }
}