#include<stdio.h>
#include<stdlib.h>
#include<signal.h>


int main(void){
    sigset_t set;

    sigemptyset(&set);
    sigaddset(&set, SIGINT);

    switch(sigismember(&set, SIGINT)){
        case 1:
            printf("SIGINT is included\n");
            break;
        case 0:
            printf("SININT is not included\n");
            break;
        default:
            printf("failed to call sigismemeber()\n");
    }

    switch(sigismember(&set, SIGSYS)){
        case 1:
            printf("SIGSYS is included\n");
            break;
        case 0:
            printf("SINSYS is not included\n");
            break;
        default:
            printf("failed to call sigismemeber()\n");
    }
}