#include "post.h"

int main(){
    int choice;

    loadPosts();

    while(1){
        printf("\n");
        printf("1.Publish Post\n");
        printf("2.Display Post\n");
        printf("3.Search Post\n");
        printf("0.Exit\n");

        printf("Input choice: ");
        scanf("%d", &choice);

        switch(choice){
        case 1:
            publishPost();
            break;
        case 2:
            displayPost();
            break;
        case 3:
            searchPost();
            break;
        case 0:
            return 0;
        default:
            printf("Invalid Input!\n");
        }
    }

}