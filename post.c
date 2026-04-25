/*
#include "post.h"

// 帖子数组
#define MAX_POSTS 100
static post posts[MAX_POSTS];
static int postCount = 0;
static int nextPostId = 1;

// 帖子数据文件路径
#define POSTS_FILE "data/posts.txt"

// 初始化帖子模块
void initPostModule(void) {
    loadPosts();
}

// 从文件加载帖子
void loadPosts(void) {
    FILE *fp = fopen(POSTS_FILE, "r");
    if (!fp) {
        printf("帖子数据文件不存在，将创建新文件\n");
        return;
    }
    
    postCount = 0;
    while (fscanf(fp, "%d %s %d", &posts[postCount].postId, 
                  posts[postCount].publisherId,
                  (int*)&posts[postCount].type) == 3) {
        fscanf(fp, " %[^\n]", posts[postCount].title);
        fscanf(fp, "%d %d %c", &posts[postCount].max_number, 
               &posts[postCount].current_number,
               &posts[postCount].genderlimit);
        fscanf(fp, "%ld %ld", &posts[postCount].start_time, &posts[postCount].end_time);
        fscanf(fp, " %[^\n]", posts[postCount].contact);
        fscanf(fp, "%lf", &posts[postCount].budget);
        fscanf(fp, " %[^\n]", posts[postCount].location);
        fscanf(fp, " %[^\n]", posts[postCount].remark);
        fscanf(fp, "%d %ld", (int*)&posts[postCount].status, &posts[postCount].publishtime);
        
        postCount++;
        if (postCount >= MAX_POSTS) break;
    }
    fclose(fp);
    
    if (postCount > 0) {
        nextPostId = posts[postCount - 1].postId + 1;
    }
    
    printf("已加载 %d 条帖子\n", postCount);
}

// 保存帖子到文件
void savePosts(void) {
    FILE *fp = fopen(POSTS_FILE, "w");
    if (!fp) {
        printf("无法保存帖子数据！\n");
        return;
    }
    
    for (int i = 0; i < postCount; i++) {
        fprintf(fp, "%d %s %d %s %d %d %c %ld %ld %s %.2f %s %s %d %ld\n",
                posts[i].postId,
                posts[i].publisherId,
                posts[i].type,
                posts[i].title,
                posts[i].max_number,
                posts[i].current_number,
                posts[i].genderlimit,
                posts[i].start_time,
                posts[i].end_time,
                posts[i].contact,
                posts[i].budget,
                posts[i].location,
                posts[i].remark,
                posts[i].status,
                posts[i].publishtime);
    }
    fclose(fp);
    printf("帖子数据已保存\n");
}

// 获取下一个可用的帖子ID
int getNextPostId(void) {
    return nextPostId++;
}

// 获取所有帖子
post* getAllPosts(int *outCount) {
    *outCount = postCount;
    return posts;
}

// 根据ID获取帖子
post* getPostById(int postId) {
    for (int i = 0; i < postCount; i++) {
        if (posts[i].postId == postId) {
            return &posts[i];
        }
    }
    return NULL;
}

// 更新帖子信息
void updatePost(post *p) {
    for (int i = 0; i < postCount; i++) {
        if (posts[i].postId == p->postId) {
            posts[i] = *p;
            savePosts();
            return;
        }
    }
}

// 根据类型名称获取类型枚举值
int getTypeByName(char *typeName) {
    if (strcmp(typeName, "拼车") == 0) return TYPE_CARPOOL;
    if (strcmp(typeName, "拼单") == 0) return TYPE_GROUP_BUY;
    if (strcmp(typeName, "学习搭子") == 0) return TYPE_STUDY_BUDDY;
    if (strcmp(typeName, "运动搭子") == 0) return TYPE_SPORTS_BUDDY;
    if (strcmp(typeName, "吃饭搭子") == 0) return TYPE_EAT_BUDDY;
    if (strcmp(typeName, "旅游搭子") == 0) return TYPE_TRAVEL_BUDDY;
    if (strcmp(typeName, "其他") == 0) return TYPE_OTHER;
    return TYPE_OTHER;
}

// 根据类型枚举值获取类型名称
char* getTypeNameByValue(int type) {
    static char *typeNames[] = {
        "", "拼车", "拼单", "学习搭子", "运动搭子", "吃饭搭子", "旅游搭子", "其他"
    };
    if (type >= 1 && type <= 7) {
        return typeNames[type];
    }
    return "未知";
}

// 发布新帖子
int publishPost(char *publisherId, int type) {
    if (postCount >= MAX_POSTS) {
        printf("帖子已达上限！\n");
        return 0;
    }
    
    post newPost;
    newPost.postId = getNextPostId();
    strcpy(newPost.publisherId, publisherId);
    newPost.type = type;
    
    printf("请输入标题: ");
    getchar();
    fgets(newPost.title, sizeof(newPost.title), stdin);
    newPost.title[strcspn(newPost.title, "\n")] = 0;
    
    printf("请输入最大人数: ");
    scanf("%d", &newPost.max_number);
    newPost.current_number = 0;
    
    printf("性别要求 (M:男 F:女 N:不限): ");
    scanf(" %c", &newPost.genderlimit);
    
    printf("请输入开始时间 (时间戳): ");
    scanf("%ld", &newPost.start_time);
    
    printf("请输入结束时间 (时间戳): ");
    scanf("%ld", &newPost.end_time);
    
    printf("请输入联系方式: ");
    scanf("%s", newPost.contact);
    
    printf("请输入预算: ");
    scanf("%lf", &newPost.budget);
    
    printf("请输入地点: ");
    getchar();
    fgets(newPost.location, sizeof(newPost.location), stdin);
    newPost.location[strcspn(newPost.location, "\n")] = 0;
    
    printf("请输入备注: ");
    fgets(newPost.remark, sizeof(newPost.remark), stdin);
    newPost.remark[strcspn(newPost.remark, "\n")] = 0;
    
    newPost.status = STATUS_ACTIVE;
    newPost.publishtime = time(NULL);
    
    posts[postCount++] = newPost;
    savePosts();
    
    printf("帖子发布成功！帖子ID: %d\n", newPost.postId);
    return newPost.postId;
}

// 按类型显示帖子列表
void showPostListByType(int type, char *currentUserId) {
    printf("\n===== %s =====\n", getTypeNameByValue(type));
    
    int found = 0;
    for (int i = 0; i < postCount; i++) {
        if (posts[i].type == type && posts[i].status == STATUS_ACTIVE) {
            found = 1;
            printf("ID: %d | 标题: %s | 人数: %d/%d | 地点: %s\n",
                   posts[i].postId, posts[i].title,
                   posts[i].current_number, posts[i].max_number,
                   posts[i].location);
        }
    }
    
    if (!found) {
        printf("暂无此类帖子\n");
    }
}

// 按发布时间排序帖子
void sortPostsByTime(post **posts, int count) {
    post *temp;
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (posts[j]->publishtime < posts[j + 1]->publishtime) {
                temp = posts[j];
                posts[j] = posts[j + 1];
                posts[j + 1] = temp;
            }
        }
    }
}
*/


//豆包版
#include "post.h"

void postAdd() {
    clear();
    printf("===== 发布搭子 =====\n");

    Partner p;
    p.id = partnerCnt + 1;
    p.userId = nowUserId;
    p.status = 0;

    printf("标题：");
    scanf("%s", p.title);
    printf("类型（学习/运动/干饭）：");
    scanf("%s", p.type);
    printf("描述：");
    scanf("%s", p.content);

    partners[partnerCnt++] = p;
    printf("发布成功！\n");
    pause();
}

void postListAll() {
    clear();
    printf("===== 所有搭子 =====\n");
    for (int i = 0; i < partnerCnt; i++) {
        printf("ID:%d | 发布者:%d | %s | %s | %s | %s\n",
               partners[i].id,
               partners[i].userId,
               partners[i].title,
               partners[i].type,
               partners[i].content,
               partners[i].status == 0 ? "可申请" : "已匹配");
    }
    pause();
}