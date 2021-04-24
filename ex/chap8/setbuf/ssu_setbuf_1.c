#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#define BUFFER_SIZE 1024

int main(){
    char buf[BUFFER_SIZE];

    //stdout을 버퍼로 설정해서 버퍼가 차야지만 출력이 된다.
    setbuf(stdout, buf);
    printf("Hello ");
    sleep(1);
    printf("!!");
    printf("\n");
    sleep(1);

    setbuf(stdout, NULL);
    printf("how");
    sleep(1);
    printf(" are ");
    sleep(1);
    printf("you?");
    sleep(1);
    printf("\n");
    exit(0);

}
