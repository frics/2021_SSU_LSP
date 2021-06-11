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
        ret_val = waitpid(pid, &status, 0);
        printf("parent is waiting and terminated %d %d %d\n", ret_val, WIFEXITED(status), WEXITSTATUS(status));
        exit(0);
    }else if(pid == 0){
        printf("child : %d\n", getpid());
        sleep(4);
        printf("child terminated\n");
        exit(0);
    }
}
