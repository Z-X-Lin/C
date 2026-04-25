#ifndef JOIN_H
#define JOIN_H
//防止被重复包含
#include"common.h"//包含所需头文件

// Mock：测试模式开关 + 依赖替身
#ifdef TEST_JOIN_MODULE  
#define getUserId(id) 1
#define getCurrentTime() "2025-12-25"
#define saveApplicationData(a,b,c) 0
#define loadApplicationData() 0
#endif

//参与模块函数声明
void browsePosts(int type);//浏览
int applyToJoin(int postId,char *applicantId);//申请参与
void viewMyApplications(char *publisherId);//查看申请的参与者
void viewMyApplicationsAsPublisher(char *publisherId);//查看收到的申请（作为发布者）
void handleApplication(int applyId,int approved);//处理申请
void checkAndUpdateStatus(int postId);//更新状态
void loadApplication(void);//加载
void saveApplications(void);//保存
void initJoinModule(void);//初始化参与模块

#endif