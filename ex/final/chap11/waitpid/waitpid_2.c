#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>


int main(){
    pid_t pid;
    int status, ret_val;

    if((pid = fork()) > 0){
        printf("parent : %d\n", getpid());
        sleep(2);
        //wnohang 옵션을 주면 대기하지 않고 바로 return
        //자식 프로세스가 종료될때까지 기다리지 않음 IFEXITED도 error를 return 
        ret_val = waitpid(pid, &status, WNOHANG);
        printf("parent is waiting and terminated %d %d\n", ret_val, WIFEXITED(status));
        exit(0);
    }else if(pid == 0){
        printf("child %d\n", getpid());
        sleep(4);
        printf("child is terminated\n");
        exit(0);
    }
}