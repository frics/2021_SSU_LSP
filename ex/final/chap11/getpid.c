#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

int main(void){
    printf("Process ID = %d\n", getpid());
    printf("Parent Process ID = %d\n", getppid());
    printf("Real User ID = %d\n", getuid());
    printf("Effective User ID = %d\n", geteuid());
    printf("Real Group ID = %d\n", getgid());
    printf("Effective Group ID = %d\n", getegid());

    exit(0);
}