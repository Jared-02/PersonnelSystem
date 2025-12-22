#include "emp_manager.h"

// --- 内部辅助函数 ---

// 清空缓冲区残留字符
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void inputString(const char *prompt, char *dest, size_t size) {
    char buf[100];
    printf("%s", prompt);
    fgets(buf, sizeof(buf), stdin);

    if (buf[0] == '\n') return;

    buf[strcspn(buf, "\n")] = '\0';
    strncpy(dest, buf, size - 1);
    dest[size - 1] = '\0';
}

void inputInt(const char *prompt, int *value) {
    char buf[32];

    printf("%s", prompt);
    fgets(buf, sizeof(buf), stdin);

    if (buf[0] == '\n') return;

    *value = atoi(buf); // str to int
}

void inputDouble(const char *prompt, double *value) {
    char buf[32];

    printf("%s", prompt);
    fgets(buf, sizeof(buf), stdin);

    if (buf[0] == '\n') return;

    *value = atof(buf); // str to float
}

// 过滤器：ID
int checkID(const Employee *e, void *idStr) {
    return strcmp(e->id, (char*)idStr) == 0;
}
// 过滤器：姓名
int checkName(const Employee *e, void *nameStr) {
    return strcmp(e->name, (char*)nameStr) == 0;
}
// 过滤器：性别
int checkGender(const Employee *e, void *genderStr) {
    return strcmp(e->gender, (char*)genderStr) == 0;
}
// 过滤器：部门
int checkDepartment(const Employee *e, void *deptStr) {
    return strcmp(e->department, (char*)deptStr) == 0;
}
// 过滤器：年龄范围
int checkAgeRange(const Employee *e, void *args) {
    NumberRange *range = (NumberRange*)args;
    return (e->age >= range->min && e->age <= range->max);
}
// 过滤器：工资范围
int checkSalaryRange(const Employee *e, void *args) {
    NumberRange *range = (NumberRange*)args;
    return (e->salary >= range->min && e->salary <= range->max);
}
// 过滤器：空
int checkNone(const Employee *e, void *args) {
    return 1;
}
// 员工过滤函数
void filterEmployees(Employee *head, Predicate predicate, void *args) {
    Employee *curr = head;
    int count = 0;

    printf("\n%-10s %-10s %-8s %-6s %-15s %-10s\n", "工号", "姓名", "性别", "年龄", "部门", "工资");
    printf("----------------------------------------------------------------\n");
    while (curr != NULL) {
        // 回调函数判断
        if (predicate(curr, args)) {
            printf("%-10s %-10s %-8s %-6d %-15s %-10.2f\n", 
                    curr->id, curr->name, curr->gender, curr->age, curr->department, curr->salary);
            count++;
        }
        curr = curr->next;
    }
    if (count == 0) printf("[WARN] 未找到匹配记录。\n");
}

// 统计年龄分布（基于桶）
void ageDistribution(Employee *head, Predicate predicate, void *args) {
    int groups[5] = {0}; // 0:20-29, 1:30-39, 2:40-49, 3:50-59, 4:60+
    int count = 0;
    Employee *curr = head;

    while (curr != NULL) {
        // 子类统计判断
        if (predicate(curr, args)) {
            count++;
            if (curr->age >= 20 && curr->age < 30) groups[0]++;
            else if (curr->age >= 30 && curr->age < 40) groups[1]++;
            else if (curr->age >= 40 && curr->age < 50) groups[2]++;
            else if (curr->age >= 50 && curr->age < 60) groups[3]++;
            else if (curr->age >= 60) groups[4]++;
        }
        curr = curr->next;
    }

    if (count == 0) {
        printf("[WARN] 未找到匹配记录。\n"); return;
    }
    printf("年龄分布统计:\n");
    printf("  20-29岁: %d人\n", groups[0]);
    printf("  30-39岁: %d人\n", groups[1]);
    printf("  40-49岁: %d人\n", groups[2]);
    printf("  50-59岁: %d人\n", groups[3]);
    printf("  60岁以上: %d人\n", groups[4]);
}

// 工资统计函数
void calculateSalary(Employee *head, Predicate predicate, void *args) {
    SalaryStats stats = {0.0, 0, 0.0, 0.0}; // init
    Employee *curr = head;
    int first = 1;

    while (curr != NULL) {
        if (predicate(curr, args)) {
            stats.totalSalary += curr->salary;
            stats.count++;
            
            if (first) {
                stats.maxSalary = curr->salary;
                stats.minSalary = curr->salary;
                first = 0;
            } else {
                if (curr->salary > stats.maxSalary) stats.maxSalary = curr->salary;
                if (curr->salary < stats.minSalary) stats.minSalary = curr->salary;
            }
        }
        curr = curr->next;
    }
    if (stats.count == 0) {
        printf("[WARN] 未找到匹配记录。\n"); return;
    }
    printf("%s工资平均: %.2f 最高: %.2f 最低: %.2f\n", args,
        stats.totalSalary / stats.count, stats.maxSalary, stats.minSalary);
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
        printf("[ERROR] 无法写入文件。\n"); return;
    }
    Employee *curr = head;
    while (curr != NULL) {
        fprintf(fp, "%s %s %s %d %s %.2f\n", 
                curr->id, curr->name, curr->gender, 
                curr->age, curr->department, curr->salary);
        curr = curr->next;
    }
    fclose(fp);
    printf("[INFO] 数据已保存。\n");
}

void addEmployee(Employee **head) {
    Employee *newNode = createNode();
    printf("\n--- 录入员工 ---\n");
    
    clearInputBuffer();
    while (1) {
        inputString("输入工号: ", newNode->id, sizeof(newNode->id));
        if (findById(*head, newNode->id)) {
            printf("[WARN] 工号已存在，请重输。\n");
        } else {
            break;
        }
    }
    inputString("输入姓名: ", newNode->name, sizeof(newNode->name));
    inputString("输入性别: ", newNode->gender, sizeof(newNode->gender));
    inputInt("输入年龄: ", &newNode->age);
    inputString("输入部门: ", newNode->department, sizeof(newNode->department));
    inputDouble("输入工资: ", &newNode->salary);

    appendNode(head, newNode);
    printf("[INFO] 录入成功。\n");
}

void printAllEmployees(Employee *head) {
    if (head == NULL) {
        printf("[INFO] 暂无数据。\n"); return;
    }
    filterEmployees(head, checkNone, "");
}

void modifyEmployee(Employee *head) {
    char id[20];
    clearInputBuffer();
    inputString("请输入要修改的员工工号: ", id, sizeof(id));
    
    Employee *target = findById(head, id);
    
    if (target == NULL) {
        printf("[INFO] 未找到工号为 %s 的员工。\n", id);
        return;
    }
    // 显示当前信息
    printf("------------------------------------------------\n");
    printf("当前信息 > 姓名:%s | 性别:%s | 年龄:%d | 部门:%s | 工资:%.2f\n", 
           target->name, target->gender, target->age, target->department, target->salary);
    printf("------------------------------------------------\n");
    printf("请依次输入新信息 (注意：工号不可修改，回车跳过该项):\n", target->id);

    inputString("请输入新姓名: ", target->name, sizeof(target->name));
    inputString("请输入新性别: ", target->gender, sizeof(target->gender));
    inputInt("请输入新年龄: ", &target->age);
    inputString("请输入新部门: ", target->department, sizeof(target->department));
    inputDouble("请输入新工资: ", &target->salary);

    printf("[INFO] 修改成功！\n");
}

void deleteEmployee(Employee **head) {
    char id[20];
    clearInputBuffer();
    inputString("输入要删除的工号: ", id, sizeof(id));

    Employee *curr = *head;
    Employee *prev = NULL;

    while (curr != NULL) {
        if (strcmp(curr->id, id) == 0) {
            if (prev == NULL) *head = curr->next;
            else prev->next = curr->next;
            free(curr);
            printf("[INFO] 删除成功。\n");
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("[WARN] 未找到。\n");
}

void searchMenu(Employee *head) {
    int choice;
    clearInputBuffer();
    inputInt("\n1. 工号查询\n2. 姓名查询\n3. 性别查询\n4. 年龄查询\n5. 部门查询\n6. 工资查询\n选择: ", &choice);
    
    char temp[50];
    double min, max;

    if (choice == 1) {
        inputString("输入工号: ", temp, sizeof(temp));
        filterEmployees(head, checkID, temp);
    } else if (choice == 2) {
        inputString("输入姓名: ", temp, sizeof(temp));
        filterEmployees(head, checkName, temp);
    } else if (choice == 3) {
        inputString("输入性别: ", temp, sizeof(temp));
        filterEmployees(head, checkGender, temp);
    } else if (choice == 4) {
        printf("输入年龄范围 (最小 最大): "); scanf("%lf %lf", &min, &max);
        NumberRange ageQuery = {min, max};
        filterEmployees(head, checkAgeRange, &ageQuery);
    } else if (choice == 5) {
        inputString("输入部门: ", temp, sizeof(temp));
        filterEmployees(head, checkDepartment, temp);
    } else if (choice == 6) {
        printf("输入工资范围 (最小 最大): "); scanf("%lf %lf", &min, &max);
        NumberRange salaryQuery = {min, max};
        filterEmployees(head, checkSalaryRange, &salaryQuery);
    }
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
    
    printf("[INFO] 排序完成 (按工资降序)\n");
}

void statsMenu(Employee *head) {
    int parentChoice, childChoice;
    
    char temp[50];
    double min, max;

    clearInputBuffer();
    inputInt("\n1. 工资降序显示\n2. 统计工资详情\n3. 统计年龄分布\n选择: ",
            &parentChoice);
    if (parentChoice == 1) {
        bubbleSort(head);
        printAllEmployees(head);
    } else if (parentChoice == 2) {
        inputInt("\n1. 按性别统计\n2. 按部门统计\n3. 按年龄范围统计\n4. 全统计\n选择: ", &childChoice);

        if (childChoice == 1) {
            inputString("输入性别: ", temp, sizeof(temp));
            calculateSalary(head, checkGender, temp);
        } else if (childChoice == 2) {
            inputString("输入部门: ", temp, sizeof(temp));
            calculateSalary(head, checkDepartment, temp);
        } else if (childChoice == 3) {
            printf("输入年龄范围 (最小 最大): "); scanf("%lf %lf", &min, &max);
            NumberRange ageQuery = {min, max};
            calculateSalary(head, checkAgeRange, &ageQuery);
        } else if (childChoice == 4) {
            calculateSalary(head, checkNone, "");
        }
    } else if (parentChoice == 3) {
        inputInt("\n1. 按性别统计\n2. 按部门统计\n3. 按工资范围统计\n4. 全统计\n选择: ", &childChoice);

        if (childChoice == 1) {
            inputString("输入性别: ", temp, sizeof(temp));
            ageDistribution(head, checkGender, temp);
        } else if (childChoice == 2) {
            inputString("输入部门: ", temp, sizeof(temp));
            ageDistribution(head, checkDepartment, temp);
        } else if (childChoice == 3) {
            printf("输入工资范围 (最小 最大): "); scanf("%lf %lf", &min, &max);
            NumberRange salaryQuery = {min, max};
            ageDistribution(head, checkSalaryRange, &salaryQuery);
        } else if (childChoice == 4) {
            ageDistribution(head, checkNone, "");
        }
    }
}