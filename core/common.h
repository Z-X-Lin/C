#ifndef COMMON_H
#define COMMON_H
//1.2两行是头文件固定模板方案，编译头文件必须使用该定义，用处是防止头文件被重复包含，导致编译错误
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

//定义数组最大范围
#define max_name 20//用户最大名字长度
#define max_password 20//用户可设置的最长密码
#define max_ID 12//用户学号
#define max_title 50//最大标题
#define max_location 100//最大地点长度
#define max_contact 30//最大联系方式
#define max_remark 200//发布者最大备注长度
#define ex_max_remark 200//申请者最大备注长度

//所有结构体定义（我仅举出我所认为的应该拥有的结构体，如有更改或不妥可自行更改添加）
//帖子类型
typedef enum{
    TYPE_CARPOOL=1,//拼车
    TYPE_GROUP_BUY,//拼单
    TYPE_STUDY_BUDDY,//学习搭子
    TYPE_SPORTS_BUDDY,//运动搭子
    TYPE_EAT_BUDDY,//吃饭搭子
    TYPE_TRAVEL_BUDDY,//旅游搭子
    TYPE_OTHER//其他
}posttype;

//帖子状态
typedef enum{
    STATUS_ACTIVE=1,//进行中
    STATUS_FULL,//已满员
    STATUS_EXPIED,//已过期
    STATUS_HIDDEN//隐藏
}poststatus;

//用户结构体
typedef struct{
    char ID[max_ID];
    char name[max_name];
    char password[max_password];
}user;

//帖子结构体
typedef struct{
    int postId;//帖子ID
    char publisherId[max_ID];//发布者学号
    posttype type;//类型
    char title[max_title];//标题
    int max_number;//人数
    int current_number;//当前人数
    char genderlimit;//性别要求
    time_t start_time;//开始时间
    time_t end_time;//截止时间
    char contact[max_contact];//联系方式
    double budget;//预算
    char location[max_location];//地点
    char remark[max_remark];//发布者备注
    poststatus status;
    time_t publishtime;//贴子发布时间
}post;

//申请结构体
typedef struct{
    char contact[max_contact];
    char note[ex_max_remark];//申请者最大备注
    int approved;//是否同意：0.未处理。1.同意。2.拒绝
}application;

//==========函数声明========
//这些函数会在各自的头文件中再次定义并实现，在此处声明是为了更好的让其他部分可以顺利调用这些函数
//A成员
int userlogin(user *currentUser);//用户登录函数，并保存已登录过的用户数据
void registerUser(void);//用户注册函数定义
int IDexist(char *ID);//检查学号是否存在
void mainmenu(user *currentuser);//主菜单函数定义，这个我还没弄懂。。。

//B成员


//C成员


extern user currentUser;//意思：保存【当前已经登录的那个用户】的信息,也就是说这个已经登录过的用户信息在任何模块都可以被调用

#endif
