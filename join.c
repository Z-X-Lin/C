
/*#include "join.h"
#include "../function/post/post.h"

// 申请记录数组 - 改为在common.c中定义
// #define MAX_APPLICATIONS 100
// static application applications[MAX_APPLICATIONS];
// static int applicationCount = 0;
// static int nextApplicationId = 1;

// 申请数据文件路径
#define APPLICATIONS_FILE "data/applications.txt"

// 加载申请数据
void loadApplication(void) {
    FILE *fp = fopen(APPLICATIONS_FILE, "r");
    if (!fp) {
        printf("申请数据文件不存在，将创建新文件\n");
        return;
    }
    
    applicationCount = 0;
    while (fscanf(fp, "%d %d %s", &applications[applicationCount].postId, 
                  &applications[applicationCount].approved,
                  applications[applicationCount].contact) == 3) {
        // 读取备注（可能包含空格）
        if (fgets(applications[applicationCount].note, sizeof(applications[applicationCount].note), fp)) {
            applications[applicationCount].note[strcspn(applications[applicationCount].note, "\n")] = 0;
        }
        applicationCount++;
        if (applicationCount >= MAX_APPLICATIONS) break;
    }
    fclose(fp);
    printf("已加载 %d 条申请记录\n", applicationCount);
}

// 保存申请数据
void saveApplications(void) {
    FILE *fp = fopen(APPLICATIONS_FILE, "w");
    if (!fp) {
        printf("无法保存申请数据！\n");
        return;
    }
    
    for (int i = 0; i < applicationCount; i++) {
        fprintf(fp, "%d %d %s %s\n", i + 1, applications[i].approved, 
                applications[i].contact, applications[i].note);
    }
    fclose(fp);
    printf("申请数据已保存\n");
}

// 浏览帖子
void browsePosts(int type) {
    printf("\n===== 浏览帖子 =====\n");
    
    int count = 0;
    post* allPosts = getAllPosts(&count);
    
    if (!allPosts || count == 0) {
        printf("暂无帖子\n");
        return;
    }
    
    printf("%-5s %-10s %-15s %-5s %-10s %-10s\n", "ID", "类型", "标题", "人数", "状态", "发布时间");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        if (type == 0 || allPosts[i].type == type || type == 7) { // 7=全部
            if (allPosts[i].status == STATUS_ACTIVE) {
                char *typeName = getTypeNameByValue(allPosts[i].type);
                char statusStr[20];
                switch (allPosts[i].status) {
                    case STATUS_ACTIVE: strcpy(statusStr, "进行中"); break;
                    case STATUS_FULL: strcpy(statusStr, "已满员"); break;
                    case STATUS_EXPIRED: strcpy(statusStr, "已过期"); break;
                    default: strcpy(statusStr, "未知"); break;
                }
                
                struct tm *tm = localtime(&allPosts[i].publishtime);
                char timeStr[20];
                strftime(timeStr, sizeof(timeStr), "%Y-%m-%d", tm);
                
                printf("%-5d %-10s %-15s %d/%d    %-10s %-10s\n", 
                       allPosts[i].postId,
                       typeName ? typeName : "未知",
                       allPosts[i].title,
                       allPosts[i].current_number,
                       allPosts[i].max_number,
                       statusStr,
                       timeStr);
            }
        }
    }
    
    free(allPosts);
}

// 申请参与
int applyToJoin(int postId, char *applicantId) {
    // 检查帖子是否存在
    post *p = getPostById(postId);
    if (!p) {
        printf("帖子不存在！\n");
        return 0;
    }
    
    // 检查帖子状态
    if (p->status != STATUS_ACTIVE) {
        printf("该帖子已不接受申请！\n");
        return 0;
    }
    
    // 检查人数是否已满
    if (p->current_number >= p->max_number) {
        printf("该帖子人数已满！\n");
        return 0;
    }
    
    // 检查是否已申请
    for (int i = 0; i < applicationCount; i++) {
        if (applications[i].postId == postId && 
            strcmp(applications[i].contact, applicantId) == 0) {
            printf("您已申请过该帖子！\n");
            return 0;
        }
    }
    
    // 创建新申请
    if (applicationCount >= MAX_APPLICATIONS) {
        printf("申请已达上限！\n");
        return 0;
    }
    
    printf("请输入联系方式: ");
    scanf("%s", applications[applicationCount].contact);
    
    printf("请输入申请备注: ");
    getchar(); // 清除缓冲区
    fgets(applications[applicationCount].note, sizeof(applications[applicationCount].note), stdin);
    applications[applicationCount].note[strcspn(applications[applicationCount].note, "\n")] = 0;
    
    applications[applicationCount].postId = postId;
    applications[applicationCount].approved = 0; // 未处理
    
    applicationCount++;
    
    // 更新帖子人数
    p->current_number++;
    updatePost(p);
    
    printf("申请成功！等待发布者审核\n");
    return 1;
}

// 查看我的申请（作为申请人）
void viewMyApplications(char *applicantId) {
    printf("\n===== 我的申请 =====\n");
    
    int found = 0;
    printf("%-5s %-10s %-15s %-10s %-10s\n", "帖子ID", "状态", "联系方式", "备注", "审核结果");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < applicationCount; i++) {
        if (strcmp(applications[i].contact, applicantId) == 0) {
            found = 1;
            post *p = getPostById(applications[i].postId);
            char *statusStr = "未处理";
            if (applications[i].approved == 1) statusStr = "已同意";
            else if (applications[i].approved == 2) statusStr = "已拒绝";
            
            printf("%-5d %-15s %-10s %-20s %-10s\n", 
                   applications[i].postId,
                   p ? p->title : "未知",
                   applications[i].contact,
                   applications[i].note,
                   statusStr);
        }
    }
    
    if (!found) {
        printf("您还没有任何申请记录\n");
    }
}

// 查看我的发布收到的申请（作为发布者）
void viewMyApplicationsAsPublisher(char *publisherId) {
    printf("\n===== 收到的申请 =====\n");
    
    int count = 0;
    post* allPosts = getAllPosts(&count);
    
    // 找出该用户发布的所有帖子ID
    int myPostIds[50];
    int myPostCount = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(allPosts[i].publisherId, publisherId) == 0) {
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
}

// 处理申请
void handleApplication(int applyId, int approved) {
    if (applyId < 1 || applyId > applicationCount) {
        printf("申请ID无效！\n");
        return;
    }
    
    applications[applyId - 1].approved = approved;
    
    if (approved == 1) {
        printf("已同意该申请！\n");
    } else if (approved == 2) {
        printf("已拒绝该申请！\n");
    }
    
    saveApplications();
}

// 更新帖子状态
void checkAndUpdateStatus(int postId) {
    post *p = getPostById(postId);
    if (!p) return;
    
    time_t now = time(NULL);
    
    // 检查是否过期
    if (p->end_time < now && p->status == STATUS_ACTIVE) {
        p->status = STATUS_EXPIRED;
        updatePost(p);
        printf("帖子 %d 已过期\n", postId);
    }
    
    // 检查是否满员
    if (p->current_number >= p->max_number && p->status == STATUS_ACTIVE) {
        p->status = STATUS_FULL;
        updatePost(p);
        printf("帖子 %d 已满员\n", postId);
    }
}

// 初始化参与模块
void initJoinModule(void) {
    loadApplication();
}

// 独立测试入口
#ifdef TEST_JOIN_MODULE
int main() {
    printf("===== 单独调试join模块 =====\n");
    
    // 初始化
    initPostModule();
    initJoinModule();
    
    // 测试浏览
    printf("\n--- 测试浏览帖子 ---\n");
    browsePosts(7); // 7=全部
    
    // 测试申请
    printf("\n--- 测试申请参与 ---\n");
    applyToJoin(1001, "2024001");
    
    // 测试查看申请
    printf("\n--- 测试查看我的申请 ---\n");
    viewMyApplications("2024001");
    
    // 测试处理申请
    printf("\n--- 测试处理申请 ---\n");
    handleApplication(1, 1);
    
    // 测试状态更新
    printf("\n--- 测试状态更新 ---\n");
    checkAndUpdateStatus(1001);
    
    // 保存
    saveApplications();
    
    printf("\n===== join模块测试完成 =====\n");
    return 0;
}
#endif*/



//豆包版
#include "join.h"

void joinApply() {
    clear();
    printf("===== 申请搭子 =====\n");
    int pid;
    printf("输入搭子ID：");
    scanf("%d", &pid);

    for (int i = 0; i < partnerCnt; i++) {
        if (partners[i].id == pid) {
            if (partners[i].userId == nowUserId) {
                printf("不能申请自己的搭子！\n");
                pause();
                return;
            }

            Join j;
            j.id = joinCnt + 1;
            j.partnerId = pid;
            j.applyUserId = nowUserId;
            joins[joinCnt++] = j;

            printf("申请成功！\n");
            pause();
            return;
        }
    }

    printf("搭子不存在！\n");
    pause();
}

void joinMyList() {
    clear();
    printf("===== 我的申请 =====\n");
    int flag = 0;

    for (int i = 0; i < joinCnt; i++) {
        if (joins[i].applyUserId == nowUserId) {
            printf("已申请搭子ID：%d\n", joins[i].partnerId);
            flag = 1;
        }
    }

    if (!flag) printf("暂无申请\n");
    pause();
}