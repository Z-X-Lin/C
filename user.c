//豆包版
#include "user.h"

void userRegister() {
    clear();
    printf("===== 用户注册 =====\n");
    User u;
    u.id = userCnt + 1;

    printf("用户名：");
    scanf("%s", u.username);
    printf("密码：");
    scanf("%s", u.pwd);

    users[userCnt++] = u;
    printf("注册成功！\n");
    pause();
}

int userLogin() {
    clear();
    printf("===== 用户登录 =====\n");
    char name[20], pwd[20];

    printf("用户名：");
    scanf("%s", name);
    printf("密码：");
    scanf("%s", pwd);

    for (int i = 0; i < userCnt; i++) {
        if (strcmp(users[i].username, name) == 0 &&
            strcmp(users[i].pwd, pwd) == 0) {
            printf("登录成功！\n");
            return users[i].id;
        }
    }

    printf("账号或密码错误！\n");
    pause();
    return -1;
}