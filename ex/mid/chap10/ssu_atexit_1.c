#include<stdio.h>
#include<stdlib.h>

void ssu_out(void);

int main(void){
    printf("before if\n");
    if(atexit(ssu_out)){
        fprintf(stderr, "error\n");
        exit(1);
    }
    printf("befor exit\n");
    exit(0);
}

void ssu_out(void){
    printf("atexit suc\n");
}