#include"join.h"
#include"post.h"

//参与模块函数定义
void browsePosts(int type){

}//浏览
int applyToJoin(int postId,char *applicantId){


}//申请参与
void viewMyApplications(char *publisherId){


}//查看申请的参与者
void handleApplication(int applyId,int approved){


}//处理申请
void checkAndUpdateStatus(int postId){


}//更新状态
void loadApplication(void){


}//加载
void saveApplications(void){


}//保存

//独立测试入口
#ifdef TEST_JOIN_MODULE
//测试主函数（单独调试时执行）
int main() {
    // 在这里写测试用例，调用模块内的函数
    printf("===== 单独调试join模块 =====\n");
    
    browsePosts(1);
    applyToJoin(1001, "2024001");
    viewMyApplications("2024001");
    handleApplication(1, 1);
    checkAndUpdateStatus(1001);
    loadApplication();
    saveApplications();
    return 0;
}
#endif