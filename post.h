#ifndef POST_H
#define POST_H

#include "common.h"

// ========== 帖子模块函数声明 ==========
//数据管理函数
void loadPosts(void);//程序启动时，从posts.txt中读取所有帖子，读取后post[]中就有了数据
void savePosts(void);//把post[]中的数据保存到posts.txt中
post* getPostById(int postId);//根据帖子ID查找帖子，指针是为了后续方便返回原帖修改数据
//业务功能函数
void createPost(user currentUser);//创建一个新帖子
void updatePost(post *p);//更新帖子信息（如参与人数、状态等）
//显示帖子函数
void showPost(post *p);//显示一个帖子
void showPostsByType(posttype type);//显示某种类型的帖子
//工具函数
char* typeToString(posttype type);//根据类型枚举值返回类型名称字符串
void sortPostsByTime(void);//按发布时间排序帖子（最新的在前）

#endif



//豆包版
/*#ifndef POST_H
#define POST_H

#include "common.h"

void postAdd();
void postListAll();

#endif
*/