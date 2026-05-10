#include "join.h"
#include "post.h"

#define APPLICATIONS_FILE "data/applications.txt"

// 加载申请数据
void loadApplications(void) {
    FILE *fp = fopen(APPLICATIONS_FILE, "r");
    if (!fp) {
        printf("申请数据文件不存在，将创建新文件\n");
        return;
    }
    
    applicationCount = 0;
    while (fscanf(fp, "%d %s %s %s %d %ld",
                  &applications[applicationCount].postId,
                  applications[applicationCount].applicantId,
                  applications[applicationCount].contact,
                  applications[applicationCount].note,
                  &applications[applicationCount].approved,
                  &applications[applicationCount].applyTime) == 6) {
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
        fprintf(fp, "%d %s %s %s %d %ld\n",
                applications[i].postId,
                applications[i].applicantId,
                applications[i].contact,
                applications[i].note,
                applications[i].approved,
                applications[i].applyTime);
    }
    fclose(fp);
}

// 申请搭子
void joinApply(void) {
    clear();
    printf("===== 申请搭子 =====\n");
    
    int postId;
    printf("输入搭子ID：");
    scanf("%d", &postId);
    
    // 检查帖子是否存在
    post *p = getPostById(postId);
    if (!p) {
        printf("搭子不存在！\n");
        pause();
        return;
    }
    
    // 检查是否是自己发布的
    if (strcmp(p->publisherId, currentUser.ID) == 0) {
        printf("不能申请自己发布的搭子！\n");
        pause();
        return;
    }
    
    // 检查帖子状态
    if (p->status != STATUS_ACTIVE) {
        printf("该搭子已不接受申请！\n");
        pause();
        return;
    }
    
    // 检查人数是否已满
    if (p->current_number >= p->max_number) {
        printf("该搭子人数已满！\n");
        pause();
        return;
    }
    
    // 检查是否已申请过
    for (int i = 0; i < applicationCount; i++) {
        if (applications[i].postId == postId && 
            strcmp(applications[i].applicantId, currentUser.ID) == 0) {
            printf("您已申请过该搭子！\n");
            pause();
            return;
        }
    }
    
    // 创建新申请
    if (applicationCount >= MAX_APPLICATIONS) {
        printf("申请已达上限！\n");
        pause();
        return;
    }
    
    application newApp;
    newApp.postId = postId;
    strcpy(newApp.applicantId, currentUser.ID);
    newApp.approved = 0; // 未处理
    newApp.applyTime = time(NULL);
    
    printf("请输入联系方式: ");
    scanf("%s", newApp.contact);
    
    printf("请输入申请备注: ");
    getchar();
    fgets(newApp.note, sizeof(newApp.note), stdin);
    newApp.note[strcspn(newApp.note, "\n")] = 0;
    
    applications[applicationCount++] = newApp;
    saveApplications();
    
    printf("申请成功！等待发布者审核\n");
    pause();
}

// 查看我的申请
void joinMyList(void) {
    clear();
    printf("===== 我的申请 =====\n");
    
    int found = 0;
    printf("%-8s %-15s %-12s %-20s %-10s\n", 
           "帖子ID", "标题", "联系方式", "备注", "状态");
    printf("--------------------------------------------------------\n");
    
    for (int i = 0; i < applicationCount; i++) {
        if (strcmp(applications[i].applicantId, currentUser.ID) == 0) {
            found = 1;
            post *p = getPostById(applications[i].postId);
            char *statusStr = "未处理";
            if (applications[i].approved == 1) statusStr = "已同意";
            else if (applications[i].approved == 2) statusStr = "已拒绝";
            
            printf("%-8d %-15s %-12s %-20s %-10s\n", 
                   applications[i].postId,
                   p ? p->title : "未知",
                   applications[i].contact,
                   applications[i].note,
                   statusStr);
        }
    }
    
    if (!found) {
        printf("暂无申请记录\n");
    }
    pause();
}

// 处理申请（发布者查看并处理收到的申请）
void joinHandleApplications(void) {
    clear();
    printf("===== 处理申请 =====\n");
    
    // 找出该用户发布的所有帖子ID
    int myPostIds[MAX_POSTS];
    int myPostCount = 0;
    for (int i = 0; i < postCount; i++) {
        if (strcmp(posts[i].publisherId, currentUser.ID) == 0) {
            myPostIds[myPostCount++] = posts[i].postId;
        }
    }
    
    if (myPostCount == 0) {
        printf("您还没有发布任何帖子\n");
        pause();
        return;
    }
    
    // 显示所有收到的申请
    int totalApps = 0;
    for (int j = 0; j < myPostCount; j++) {
        int postId = myPostIds[j];
        post *p = getPostById(postId);
        
        printf("\n--- 帖子【%s】(ID:%d)的申请 ---\n", p ? p->title : "未知", postId);
        
        for (int i = 0; i < applicationCount; i++) {
            if (applications[i].postId == postId) {
                totalApps++;
                char *statusStr = "未处理";
                if (applications[i].approved == 1) statusStr = "已同意";
                else if (applications[i].approved == 2) statusStr = "已拒绝";
                
                printf("  序号: %d\n", i + 1);
                printf("  申请人学号: %s\n", applications[i].applicantId);
                printf("  联系方式: %s\n", applications[i].contact);
                printf("  备注: %s\n", applications[i].note);
                printf("  状态: %s\n", statusStr);
                printf("  ------------------------\n");
            }
        }
    }
    
    if (totalApps == 0) {
        printf("暂无申请记录\n");
        pause();
        return;
    }
    
    // 处理申请
    printf("\n请输入要处理的申请序号 (0返回): ");
    int applyIdx;
    scanf("%d", &applyIdx);
    
    if (applyIdx > 0 && applyIdx <= applicationCount) {
        printf("选择处理结果 (1:同意 2:拒绝): ");
        int approved;
        scanf("%d", &approved);
        
        if (approved == 1 || approved == 2) {
            applications[applyIdx - 1].approved = approved;
            
            // 如果同意，更新帖子人数
            if (approved == 1) {
                post *p = getPostById(applications[applyIdx - 1].postId);
                if (p) {
                    p->current_number++;
                    savePosts();
                }
            }
            
            saveApplications();
            printf(approved == 1 ? "已同意该申请！\n" : "已拒绝该申请！\n");
        } else {
            printf("无效选择！\n");
        }
    } else if (applyIdx != 0) {
        printf("无效的申请序号！\n");
    }
    
    pause();
}