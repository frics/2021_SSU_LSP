#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define BUFFER_SIZE 1024

int main(){
    char buf[BUFFER_SIZE];

    setbuf(stdout, buf);
    printf("HELLO");
    sleep(1);
    printf("!!");
    //printf("\n");
    sleep(1);

    setbuf(stdout, NULL);
    printf("HOW");
    sleep(1);
    printf("are");
    sleep(1);
    
}