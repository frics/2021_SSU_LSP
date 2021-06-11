#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>


int main(void){
    printf("this tis the original program\n");
    execl("./execl_test1", "execl_test1", "param1", "param2", "param3", (char *)0);
    printf("여기까지 내려오면 안됨");
    exit(0);
}