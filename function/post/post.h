#ifndef POST_H
#define POST_H

#include "common.h"

// ========== 帖子模块函数声明 ==========

int publishPost(char *publisherId, int type);//发布新帖子
void loadPosts(void);//从文档中加载所有帖子
void savePosts(void);//保存所有帖子到文档
post* getPostById(int postId);//根据帖子ID查找帖子
void updatePost(post *p);//更新帖子信息
void showPostListByType(int type, char *currentUserId);//按类型显示帖子列表
void sortPostsByTime(post **posts, int count);// 按发布时间排序帖子（最新的在前）
post* getAllPosts(int *outCount);//获取所有帖子
int getNextPostId(void);//获取下一个可用的帖子ID
void initPostModule(void);//初始化子模块
int getTypeByName(char *typeName);//根据类型名称获取类型枚举值
char* getTypeNameByValue(int type);//根据类型枚举值获取类型名称

#endif