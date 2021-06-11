#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(void){
    char *argv[] = {
        "execl_test1", 
        "param1", 
        "param2", 
        (char *)0 
    };
    char *env[] = {
        "NAME=value",
        "nextname=nextvalue",
        "HOME=/home/ubuntu",
        (char *)0
    };

    printf("this is original program\n");
    execve("./execl_test1", argv, env);
    printf("여기 오면 안된다잇\n");
    exit(0);

}