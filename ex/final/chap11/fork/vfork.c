#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

void ssu_local_func(void);

int main(void){
    pid_t pid = getpid();
    printf("Before vfork parent : %d\n", pid);
    ssu_local_func();

    //부모 프로세스가 넘어오면 이미 exit()을 사용하여 시스템 자원을 반환했기 때문에
    //segmentation fault가 뜬다.
    printf("After ssu_local_func, my PID is %d, parent : %d\n", getpid(), getppid());
    // if(pid == getpid())
    //     exit(0);
    // else
    _exit(0);
    return 0;
}

void ssu_local_func(void){

    switch(vfork()){
        case 1:
            fprintf(stderr, "vfork fail\n");
            exit(1);
        case 0:
            printf("I'm child. My PID is %d\n", getpid());
            break;
        default:
            sleep(2);
            break;
    }
}