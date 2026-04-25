/*
#include "common.h"

// 全局当前登录用户
user currentUser = {0};

// 申请记录数组
#define MAX_APPLICATIONS 100
application applications[MAX_APPLICATIONS];
int applicationCount = 0;

// 用户数据文件路径
#define USERS_FILE "data/users.txt"

// 检查学号是否存在
int IDexist(char *ID) {
    FILE *fp = fopen(USERS_FILE, "r");
    if (!fp) return 0;
    
    user u;
    while (fscanf(fp, "%s %s", u.ID, u.name) == 2) {
        // 读取密码（可能包含空格，使用fgets处理）
        if (fgets(u.password, sizeof(u.password), fp)) {
            // 移除换行符
            u.password[strcspn(u.password, "\n")] = 0;
        }
        if (strcmp(u.ID, ID) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// 用户注册
void registerUser(void) {
    user newUser;
    printf("\n===== 用户注册 =====\n");
    
    while (1) {
        printf("请输入学号(12位): ");
        scanf("%s", newUser.ID);
        
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
    
    FILE *fp = fopen(USERS_FILE, "a");
    if (fp) {
        fprintf(fp, "%s %s %s\n", newUser.ID, newUser.name, newUser.password);
        fclose(fp);
        printf("注册成功！\n");
    } else {
        printf("注册失败：无法打开数据文件！\n");
    }
}

// 用户登录
int userlogin(user *currentUser) {
    char inputID[max_ID];
    char inputPassword[max_password];
    
    printf("\n===== 用户登录 =====\n");
    printf("请输入学号: ");
    scanf("%s", inputID);
    printf("请输入密码: ");
    scanf("%s", inputPassword);
    
    FILE *fp = fopen(USERS_FILE, "r");
    if (!fp) {
        printf("无法打开用户数据文件！\n");
        return 0;
    }
    
    user u;
    while (fscanf(fp, "%s %s", u.ID, u.name) == 2) {
        if (fgets(u.password, sizeof(u.password), fp)) {
            u.password[strcspn(u.password, "\n")] = 0;
        }
        if (strcmp(u.ID, inputID) == 0 && strcmp(u.password, inputPassword) == 0) {
            strcpy(currentUser->ID, u.ID);
            strcpy(currentUser->name, u.name);
            strcpy(currentUser->password, u.password);
            fclose(fp);
            printf("登录成功！欢迎 %s\n", currentUser->name);
            return 1;
        }
    }
    fclose(fp);
    printf("学号或密码错误！\n");
    return 0;
}

// 主菜单
void mainmenu(user *currentuser) {
    int choice;
    
    while (1) {
        printf("\n========== 主菜单 ==========\n");
        printf("当前用户: %s (%s)\n", currentuser->name, currentuser->ID);
        printf("1. 浏览帖子\n");
        printf("2. 发布帖子\n");
        printf("3. 我的参与\n");
        printf("4. 我的发布\n");
        printf("5. 处理申请\n");
        printf("0. 退出登录\n");
        printf("请选择: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                // 浏览帖子
                printf("\n--- 浏览帖子 ---\n");
                printf("1. 拼车 2. 拼单 3. 学习搭子 4. 运动搭子 5. 吃饭搭子 6. 旅游搭子 7. 全部\n");
                printf("请选择类型: ");
                int type;
                scanf("%d", &type);
                // TODO: 调用browsePosts
                printf("浏览功能开发中...\n");
                break;
            case 2:
                // 发布帖子
                printf("\n--- 发布帖子 ---\n");
                // TODO: 调用publishPost
                printf("发布功能开发中...\n");
                break;
            case 3:
                // 我的参与
                printf("\n--- 我的参与 ---\n");
                // TODO: 调用viewMyApplications
                printf("我的参与功能开发中...\n");
                break;
            case 4:
                // 我的发布
                printf("\n--- 我的发布 ---\n");
                // TODO: 显示当前用户发布的帖子
                printf("我的发布功能开发中...\n");
                break;
            case 5:
                // 处理申请
                printf("\n--- 处理申请 ---\n");
                // TODO: 调用handleApplication
                printf("处理申请功能开发中...\n");
                break;
            case 0:
                printf("已退出登录\n");
                return;
            default:
                printf("无效选择！\n");
        }
    }
}*/


//豆包版
#include "common.h"

User users[MAX_USER];
Partner partners[MAX_PARTNER];
Join joins[MAX_JOIN];

int userCnt = 0;
int partnerCnt = 0;
int joinCnt = 0;
int nowUserId = -1;

void clear() {
    system("cls");
}

void pause() {
    printf("\n按回车继续...");
    getchar();
    getchar();
}