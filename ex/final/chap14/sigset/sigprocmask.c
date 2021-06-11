#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

int main(void){
    sigset_t set;
    int count;

    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigprocmask(SIG_BLOCK, &set, NULL);

    for(count = 3; 0<count; count--){
        printf("count %d\n", count);
        sleep(1);
    }

    printf("Ctrl-C에 대한 블록을 해제\n");
    sigprocmask(SIG_UNBLOCK, &set, NULL);
    printf("COUNT 중 interupt걸면 이건 출려안됨 -> mask 풀리자 마자 block 되었던 시그널이 실행되기 때문이다.\n");

    while(1);
    exit(0);
}