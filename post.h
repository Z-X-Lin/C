#ifndef POST_H
#define POST_H

#include "common.h"

void postAdd(void);
void postListAll(void);
void postListByType(int type);
void loadPosts(void);
void savePosts(void);
post* getPostById(int postId);
char* getTypeNameByValue(int type);
int getNextPostId(void);

#endif