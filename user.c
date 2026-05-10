#include "user.h"

#define USERS_FILE "data/users.txt"

// 检查学号是否存在
static int IDexist(const char *ID) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].ID, ID) == 0) {
            return 1;
        }
    }
    return 0;
}

// 加载用户数据
void loadUsers(void) {
    FILE *fp = fopen(USERS_FILE, "r");
    if (!fp) {
        printf("用户数据文件不存在，将创建新文件\n");
        return;
    }
    
    userCount = 0;
    while (fscanf(fp, "%s %s %s", users[userCount].ID, 
                  users[userCount].name, users[userCount].password) == 3) {
        userCount++;
        if (userCount >= MAX_USERS) break;
    }
    fclose(fp);
    printf("已加载 %d 位用户\n", userCount);
}

// 保存用户数据
void saveUsers(void) {
    FILE *fp = fopen(USERS_FILE, "w");
    if (!fp) {
        printf("无法保存用户数据！\n");
        return;
    }
    
    for (int i = 0; i < userCount; i++) {
        fprintf(fp, "%s %s %s\n", users[i].ID, users[i].name, users[i].password);
    }
    fclose(fp);
}

// 用户注册
void userRegister(void) {
    clear();
    printf("===== 用户注册 =====\n");
    
    user newUser;
    
    while (1) {
        printf("请输入学号(12位): ");
        scanf("%12s", newUser.ID);  // 限制最多读取12个字符
        
        if (strlen(newUser.ID) != 12) {
            printf("学号必须为12位！\n");
            continue;
        }
        
        if (IDexist(newUser.ID)) {
            printf("该学号已被注册！\n");
            continue;
        }
        break;
    }
    
    printf("请输入用户名: ");
    scanf("%s", newUser.name);
    
    printf("请输入密码: ");
    scanf("%s", newUser.password);
    
    users[userCount++] = newUser;
    saveUsers();
    
    printf("注册成功！\n");
    pause();
}

// 用户登录
int userLogin(void) {
    clear();
    printf("===== 用户登录 =====\n");
    
    char inputID[max_ID];
    char inputPassword[max_password];
    
    printf("请输入学号: ");
    scanf("%s", inputID);
    printf("请输入密码: ");
    scanf("%s", inputPassword);
    
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].ID, inputID) == 0 && 
            strcmp(users[i].password, inputPassword) == 0) {
            memcpy(&currentUser, &users[i], sizeof(user));
            printf("登录成功！欢迎 %s\n", currentUser.name);
            pause();
            return 1;
        }
    }
    
    printf("学号或密码错误！\n");
    pause();
    return 0;
}