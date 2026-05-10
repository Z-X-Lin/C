#include "post.h"

post posts[MAX_POSTS];
int postCount = 0;//帖子数组和计数器
int nextPostId = 1;//下一个帖子ID

void loadPosts(void){
    postCount = 0;

    FILE *fp = fopen("data/posts.txt","r");
    
    if(fp == NULL){
        printf("Failed to open the file post.txt\n");
        return;
    }
    
    while(fscanf(fp,"%d|%[^|\n]\n", //%[^|\n]表示读取直到遇到|或换行的字符串停止
           &posts[postCount].postId,
           posts[postCount].title) == 2)
    {
        postCount++;
        if(posts[postCount - 1].postId >= nextPostId){
            nextPostId = posts[postCount - 1].postId + 1;
        }
    }
    
    for(int i = 0; i < postCount; i++){
        printf("ID:%d\n", posts[i].postId);
        printf("title:%s\n", posts[i].title);
    }

     fclose(fp);
}

void publishPost(void){
    post newPost;

    newPost.postId = nextPostId;//把当前编号给新帖子
    nextPostId++;

    printf("Input title: ");
    scanf("%s", newPost.title);

    printf("Input type(1-7): ");
    scanf("%d", &newPost.type);

    printf("Post ID:%d\n", newPost.postId);
    printf("Title: %s\n", newPost.title);
    printf("Type: %d\n", newPost.type);
    
    posts[postCount] = newPost;//把新帖子放到数组里
    postCount++;

    savePosts();//保存到文件

    printf("Current Post Count: %d\n", postCount);
}

void savePosts(void){
    FILE *fp = fopen("data/posts.txt","w");
    
    if(fp == NULL){
        printf("Failed to open the file post.txt\n");
        return;
    }

    printf("post.txt opened successfully!\n");

    for(int i = 0;i < postCount;i++){
        fprintf(fp,"%d|%s\n",
            posts[i].postId,
            posts[i].title);
    }
    
    fclose(fp);
}
// 根据类型枚举值获取类型名称
/*char* getTypeNameByValue(int type) {
    static char *typeNames[] = {
        "", "拼车", "拼单", "学习搭子", "运动搭子", "吃饭搭子", "旅游搭子", "其他"
    };
    if (type >= 1 && type <= 7) {
        return typeNames[type];
    }
    return "未知";
}

// 获取下一个可用的帖子ID
int getNextPostId(void) {
    return nextPostId++;
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

// 加载帖子数据
void loadPosts(void) {
    FILE *fp = fopen(POSTS_FILE, "r");
    if (!fp) {
        printf("帖子数据文件不存在，将创建新文件\n");
        return;
    }
    
    postCount = 0;
    while (fscanf(fp, "%d %s %d %s %d %d %c %ld %ld %s %lf %s %s %d %ld",
                  &posts[postCount].postId,
                  posts[postCount].publisherId,
                  (int*)&posts[postCount].type,
                  posts[postCount].title,
                  &posts[postCount].max_number,
                  &posts[postCount].current_number,
                  &posts[postCount].genderlimit,
                  &posts[postCount].start_time,
                  &posts[postCount].end_time,
                  posts[postCount].contact,
                  &posts[postCount].budget,
                  posts[postCount].location,
                  posts[postCount].remark,
                  (int*)&posts[postCount].status,
                  &posts[postCount].publishtime) == 15) {
        postCount++;
        if (postCount >= MAX_POSTS) break;
    }
    fclose(fp);
    
    if (postCount > 0) {
        nextPostId = posts[postCount - 1].postId + 1;
    }
    printf("已加载 %d 条帖子\n", postCount);
}

// 保存帖子数据
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
}

// 发布帖子
void postAdd(void) {
    clear();
    printf("===== 发布搭子 =====\n");
    
    if (postCount >= MAX_POSTS) {
        printf("帖子已达上限！\n");
        pause();
        return;
    }
    
    post newPost;
    newPost.postId = getNextPostId();
    strcpy(newPost.publisherId, currentUser.ID);
    newPost.current_number = 0;
    newPost.status = STATUS_ACTIVE;
    newPost.publishtime = time(NULL);
    
    // 选择帖子类型
    printf("选择类型:\n");
    printf("1. 拼车  2. 拼单  3. 学习搭子  4. 运动搭子  5. 吃饭搭子  6. 旅游搭子  7. 其他\n");
    printf("请选择: ");
    scanf("%d", (int*)&newPost.type);
    
    printf("请输入标题: ");
    getchar();
    fgets(newPost.title, sizeof(newPost.title), stdin);
    newPost.title[strcspn(newPost.title, "\n")] = 0;
    
    printf("请输入最大人数: ");
    scanf("%d", &newPost.max_number);
    
    printf("性别要求 (M:男 F:女 N:不限): ");
    scanf(" %c", &newPost.genderlimit);
    
    printf("请输入预算: ");
    scanf("%lf", &newPost.budget);
    
    printf("请输入地点: ");
    getchar();
    fgets(newPost.location, sizeof(newPost.location), stdin);
    newPost.location[strcspn(newPost.location, "\n")] = 0;
    
    printf("请输入联系方式: ");
    scanf("%s", newPost.contact);
    
    printf("请输入备注: ");
    getchar();
    fgets(newPost.remark, sizeof(newPost.remark), stdin);
    newPost.remark[strcspn(newPost.remark, "\n")] = 0;
    
    // 设置默认时间
    newPost.start_time = time(NULL);
    newPost.end_time = newPost.start_time + 7 * 24 * 3600; // 默认7天后结束
    
    posts[postCount++] = newPost;
    savePosts();
    
    printf("发布成功！帖子ID: %d\n", newPost.postId);
    pause();
}

// 显示所有帖子
void postListAll(void) {
    clear();
    printf("===== 所有搭子 =====\n");
    
    if (postCount == 0) {
        printf("暂无搭子\n");
        pause();
        return;
    }
    
    printf("%-5s %-10s %-15s %-8s %-8s %-10s\n", 
           "ID", "类型", "标题", "人数", "状态", "发布者");
    printf("------------------------------------------------------------\n");
    
    for (int i = 0; i < postCount; i++) {
        char *typeName = getTypeNameByValue(posts[i].type);
        char statusStr[20];
        switch (posts[i].status) {
            case STATUS_ACTIVE: strcpy(statusStr, "进行中"); break;
            case STATUS_FULL: strcpy(statusStr, "已满员"); break;
            case STATUS_EXPIRED: strcpy(statusStr, "已过期"); break;
            case STATUS_HIDDEN: strcpy(statusStr, "隐藏"); break;
            default: strcpy(statusStr, "未知"); break;
        }
        
        printf("%-5d %-10s %-15s %d/%d    %-8s %s\n", 
               posts[i].postId,
               typeName,
               posts[i].title,
               posts[i].current_number,
               posts[i].max_number,
               statusStr,
               posts[i].publisherId);
    }
    pause();
}

// 按类型显示帖子
void postListByType(int type) {
    clear();
    printf("===== %s =====\n", getTypeNameByValue(type));
    
    int found = 0;
    printf("%-5s %-15s %-8s %-8s %-10s\n", 
           "ID", "标题", "人数", "状态", "发布者");
    printf("------------------------------------------\n");
    
    for (int i = 0; i < postCount; i++) {
        if (posts[i].type == type && posts[i].status == STATUS_ACTIVE) {
            found = 1;
            char statusStr[20];
            switch (posts[i].status) {
                case STATUS_ACTIVE: strcpy(statusStr, "进行中"); break;
                case STATUS_FULL: strcpy(statusStr, "已满员"); break;
                default: strcpy(statusStr, "未知"); break;
            }
            
            printf("%-5d %-15s %d/%d    %-8s %s\n", 
                   posts[i].postId,
                   posts[i].title,
                   posts[i].current_number,
                   posts[i].max_number,
                   statusStr,
                   posts[i].publisherId);
        }
    }
    
    if (!found) {
        printf("暂无此类搭子\n");
    }
    pause();
}*/