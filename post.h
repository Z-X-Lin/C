#ifndef POST_H
#define POST_H

#include "common.h"

void postAdd(void);
void postListAll(void);
void postListByType(int type);
int getNextPostId(void);
void testSave(void);//测试保存帖子的函数

char* getTypeName(int type);
void loadPosts(void);
void savePosts(void);
void publishPost(void);//发布帖子函数
void displayPost(void);//显示帖子详情的函数
void sortPosts(void);//排序帖子函数
post* getPostById(int id);//根据ID获取帖子的函数
void updatePost(post *p);//更新帖子的函数
void searchPost(void);//搜索帖子函数
#endif



