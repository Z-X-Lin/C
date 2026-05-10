#include "user.h"
#include "post.h"
#include "join.h"

#define USERS_FILE "data/users.txt"

static int IDexist(const char *ID) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].ID, ID) == 0) {
            return 1;
        }
    }
    return 0;
}

static int getUserIndex(const char *ID) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].ID, ID) == 0) {
            return i;
        }
    }
    return -1;
}

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

void userRegister(void) {
    clear();
    printf("===== 用户注册 =====\n");

    user newUser;

    while (1) {
        printf("请输入学号(12位): ");
        scanf("%12s", newUser.ID);

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

void userInfo(void) {
    clear();
    printf("===== 个人信息 =====\n");
    printf("学号: %s\n", currentUser.ID);
    printf("用户名: %s\n", currentUser.name);

    int myPosts = 0;
    for (int i = 0; i < postCount; i++) {
        if (strcmp(posts[i].publisherId, currentUser.ID) == 0) {
            myPosts++;
        }
    }

    int myApplications = 0;
    int approvedApps = 0;
    for (int i = 0; i < applicationCount; i++) {
        if (strcmp(applications[i].applicantId, currentUser.ID) == 0) {
            myApplications++;
            if (applications[i].approved == 1) {
                approvedApps++;
            }
        }
    }

    printf("发布搭子数: %d\n", myPosts);
    printf("申请搭子数: %d\n", myApplications);
    printf("已通过申请: %d\n", approvedApps);
    pause();
}

void userModifyPassword(void) {
    clear();
    printf("===== 修改密码 =====\n");

    char oldPassword[max_password];
    char newPassword[max_password];
    char confirmPassword[max_password];

    printf("请输入原密码: ");
    scanf("%s", oldPassword);

    if (strcmp(oldPassword, currentUser.password) != 0) {
        printf("原密码错误！\n");
        pause();
        return;
    }

    printf("请输入新密码: ");
    scanf("%s", newPassword);

    if (strlen(newPassword) < 6) {
        printf("新密码长度不能少于6位！\n");
        pause();
        return;
    }

    printf("请再次输入新密码: ");
    scanf("%s", confirmPassword);

    if (strcmp(newPassword, confirmPassword) != 0) {
        printf("两次输入的密码不一致！\n");
        pause();
        return;
    }

    int idx = getUserIndex(currentUser.ID);
    if (idx >= 0) {
        strcpy(users[idx].password, newPassword);
        strcpy(currentUser.password, newPassword);
        saveUsers();
        printf("密码修改成功！\n");
    } else {
        printf("修改失败，用户不存在！\n");
    }
    pause();
}

void userDelete(void) {
    clear();
    printf("===== 注销账号 =====\n");

    char confirm[10];

    printf("警告：注销账号将删除所有相关数据！\n");
    printf("确认注销？(yes/no): ");
    scanf("%s", confirm);

    if (strcmp(confirm, "yes") != 0) {
        printf("已取消注销\n");
        pause();
        return;
    }

    int idx = getUserIndex(currentUser.ID);
    if (idx < 0) {
        printf("用户不存在！\n");
        pause();
        return;
    }

    for (int i = idx; i < userCount - 1; i++) {
        users[i] = users[i + 1];
    }
    userCount--;
    saveUsers();

    for (int i = 0; i < applicationCount; i++) {
        if (strcmp(applications[i].applicantId, currentUser.ID) == 0 ||
            strcmp(posts[getPostById(applications[i].postId) ? applications[i].postId : -1].publisherId, currentUser.ID) == 0) {
            applications[i].approved = 3;
        }
    }
    saveApplications();

    printf("账号已注销，感谢您的使用！\n");
    memset(&currentUser, 0, sizeof(user));
    pause();
}