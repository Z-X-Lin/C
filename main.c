#include "core/common.h"
#include "function/post/post.h"
#include "function/join/join.h"
#include <time.h>

// 显示欢迎信息
void showWelcome(void) {
    printf("\n");
    printf("========================================\n");
    printf("        校园搭子平台 - 您的校园伙伴       \n");
    printf("               Solo together            \n");
    printf("========================================\n");
    printf("\n");
}

// 显示登录/注册菜单
void showLoginMenu(void) {
    printf("\n===== 登录/注册 =====\n");
    printf("1. 登录\n");
    printf("2. 注册\n");
    printf("0. 退出程序\n");
    printf("请选择: ");
}

// 显示主菜单
void showMainMenu(void) {
    printf("\n========== 主菜单 ==========\n");
    printf("当前用户: %s (%s)\n", currentUser.name, currentUser.ID);
    printf("1. 浏览帖子\n");
    printf("2. 发布帖子\n");
    printf("3. 我的参与\n");
    printf("4. 我的发布\n");
    printf("5. 处理申请\n");
    printf("0. 退出登录\n");
    printf("请选择: ");
}

// 浏览帖子子菜单
void browsePostsMenu(void) {
    int choice;
    while (1) {
        printf("\n===== 浏览帖子 =====\n");
        printf("1. 拼车\n");
        printf("2. 拼单\n");
        printf("3. 学习搭子\n");
        printf("4. 运动搭子\n");
        printf("5. 吃饭搭子\n");
        printf("6. 旅游搭子\n");
        printf("7. 查看全部\n");
        printf("0. 返回上级\n");
        printf("请选择: ");
        scanf("%d", &choice);
        
        if (choice == 0) break;
        
        if (choice >= 1 && choice <= 7) {
            browsePosts(choice);
            
            // 询问是否申请
            if (choice != 7) {
                printf("\n是否申请参与? (1:是 0:否): ");
                int apply;
                scanf("%d", &apply);
                if (apply == 1) {
                    printf("请输入帖子ID: ");
                    int postId;
                    scanf("%d", &postId);
                    applyToJoin(postId, currentUser.ID);
                }
            }
        } else {
            printf("无效选择！\n");
        }
    }
}

// 发布帖子子菜单
void publishPostMenu(void) {
    int choice;
    while (1) {
        printf("\n===== 发布帖子 =====\n");
        printf("1. 拼车\n");
        printf("2. 拼单\n");
        printf("3. 学习搭子\n");
        printf("4. 运动搭子\n");
        printf("5. 吃饭搭子\n");
        printf("6. 旅游搭子\n");
        printf("7. 其他\n");
        printf("0. 返回上级\n");
        printf("请选择: ");
        scanf("%d", &choice);
        
        if (choice == 0) break;
        
        if (choice >= 1 && choice <= 7) {
            publishPost(currentUser.ID, choice);
        } else {
            printf("无效选择！\n");
        }
    }
}

// 查看我的参与
void myApplicationsMenu(void) {
    viewMyApplications(currentUser.ID);
}

// 查看我的发布
void myPostsMenu(void) {
    printf("\n===== 我的发布 =====\n");
    
    int count = 0;
    post* allPosts = getAllPosts(&count);
    
    int found = 0;
    printf("%-5s %-10s %-15s %-5s %-10s\n", "ID", "类型", "标题", "人数", "状态");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        if (strcmp(allPosts[i].publisherId, currentUser.ID) == 0) {
            found = 1;
            char *typeName = getTypeNameByValue(allPosts[i].type);
            char statusStr[20];
            switch (allPosts[i].status) {
                case STATUS_ACTIVE: strcpy(statusStr, "进行中"); break;
                case STATUS_FULL: strcpy(statusStr, "已满员"); break;
                case STATUS_EXPIRED: strcpy(statusStr, "已过期"); break;
                case STATUS_HIDDEN: strcpy(statusStr, "隐藏"); break;
                default: strcpy(statusStr, "未知"); break;
            }
            
            printf("%-5d %-10s %-15s %d/%d    %-10s\n", 
                   allPosts[i].postId,
                   typeName ? typeName : "未知",
                   allPosts[i].title,
                   allPosts[i].current_number,
                   allPosts[i].max_number,
                   statusStr);
        }
    }
    
    if (!found) {
        printf("您还没有发布任何帖子\n");
    }
    
    free(allPosts);
}

// 处理申请子菜单
void handleApplicationsMenu(void) {
    int count = 0;
    post* allPosts = getAllPosts(&count);
    
    // 找出该用户发布的所有帖子ID
    int myPostIds[50];
    int myPostCount = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(allPosts[i].publisherId, currentUser.ID) == 0) {
            myPostIds[myPostCount++] = allPosts[i].postId;
        }
    }
    
    if (myPostCount == 0) {
        printf("您还没有发布任何帖子\n");
        free(allPosts);
        return;
    }
    
    // 显示每个帖子的申请
    for (int j = 0; j < myPostCount; j++) {
        int postId = myPostIds[j];
        post *p = getPostById(postId);
        
        printf("\n--- 帖子【%s】的申请 ---\n", p ? p->title : "未知");
        
        int hasApp = 0;
        for (int i = 0; i < applicationCount; i++) {
            if (applications[i].postId == postId) {
                hasApp = 1;
                char *statusStr = "未处理";
                if (applications[i].approved == 1) statusStr = "已同意";
                else if (applications[i].approved == 2) statusStr = "已拒绝";
                
                printf("  序号: %d\n", i + 1);
                printf("  申请人联系方式: %s\n", applications[i].contact);
                printf("  备注: %s\n", applications[i].note);
                printf("  状态: %s\n", statusStr);
                printf("  --------------------------------\n");
            }
        }
        
        if (!hasApp) {
            printf("  暂无申请\n");
        }
    }
    
    free(allPosts);
    
    // 处理申请
    printf("\n请输入要处理的申请序号 (0返回): ");
    int applyId;
    scanf("%d", &applyId);
    
    if (applyId > 0 && applyId <= applicationCount) {
        printf("选择处理结果 (1:同意 2:拒绝): ");
        int approved;
        scanf("%d", &approved);
        
        if (approved == 1 || approved == 2) {
            handleApplication(applyId, approved);
        } else {
            printf("无效选择！\n");
        }
    }
}

// 主函数
int main(void) {
    showWelcome();
    
    // 初始化模块
    initPostModule();
    initJoinModule();
    
    while (1) {
        showLoginMenu();
        int choice;
        scanf("%d", &choice);
        
        if (choice == 0) {
            printf("感谢使用，再见！\n");
            break;
        }
        
        switch (choice) {
            case 1:
                // 登录
                if (userlogin(&currentUser)) {
                    // 登录成功，进入主菜单
                    while (1) {
                        showMainMenu();
                        int mainChoice;
                        scanf("%d", &mainChoice);
                        
                        if (mainChoice == 0) {
                            printf("已退出登录\n");
                            memset(&currentUser, 0, sizeof(user));
                            break;
                        }
                        
                        switch (mainChoice) {
                            case 1:
                                browsePostsMenu();
                                break;
                            case 2:
                                publishPostMenu();
                                break;
                            case 3:
                                myApplicationsMenu();
                                break;
                            case 4:
                                myPostsMenu();
                                break;
                            case 5:
                                handleApplicationsMenu();
                                break;
                            default:
                                printf("无效选择！\n");
                        }
                    }
                }
                break;
            case 2:
                // 注册
                registerUser();
                break;
            default:
                printf("无效选择！\n");
        }
    }
    
    return 0;
}