#include "common.h"
#include "user.h"
#include "post.h"
#include "join.h"

// 用户菜单
void userMenu(void) {
    int choice;
    while (1) {
        clear();
        printf("===== 校园搭子系统 =====\n");
        printf("当前登录：%s (%s)\n", currentUser.name, currentUser.ID);
        printf("1. 发布搭子\n");
        printf("2. 查看所有搭子\n");
        printf("3. 分类浏览\n");
        printf("4. 申请搭子\n");
        printf("5. 我的申请\n");
        printf("6. 处理申请\n");
        printf("0. 退出登录\n");
        printf("请选择：");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                postAdd();
                break;
            case 2:
                postListAll();
                break;
            case 3:
                // 分类浏览子菜单
                {
                    clear();
                    printf("===== 分类浏览 =====\n");
                    printf("1. 拼车\n");
                    printf("2. 拼单\n");
                    printf("3. 学习搭子\n");
                    printf("4. 运动搭子\n");
                    printf("5. 吃饭搭子\n");
                    printf("6. 旅游搭子\n");
                    printf("7. 其他\n");
                    printf("请选择类型：");
                    int type;
                    scanf("%d", &type);
                    if (type >= 1 && type <= 7) {
                        postListByType(type);
                    } else {
                        printf("无效选择！\n");
                        pause();
                    }
                }
                break;
            case 4:
                joinApply();
                break;
            case 5:
                joinMyList();
                break;
            case 6:
                joinHandleApplications();
                break;
            case 0:
                memset(&currentUser, 0, sizeof(user));
                printf("已退出登录\n");
                return;
            default:
                printf("输入错误！\n");
                pause();
        }
    }
}

// 主函数
int main(void) {
    int choice;
    
    // 初始化数据
    loadUsers();
    loadPosts();
    loadApplications();
    
    while (1) {
        clear();
        printf("===== 校园搭子平台 =====\n");
        printf("1. 注册\n");
        printf("2. 登录\n");
        printf("0. 退出\n");
        printf("请选择：");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                userRegister();
                break;
            case 2:
                if (userLogin()) {
                    userMenu();
                }
                break;
            case 0:
                clear();
                printf("程序已退出\n");
                return 0;
            default:
                printf("输入错误！\n");
                pause();
        }
    }
}