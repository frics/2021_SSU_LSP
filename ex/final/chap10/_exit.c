#include<stdio.h>
#include<unistd.h>


int main(void){
    printf("good afternoon?");
    //_exit를 사용하면 stdout 버퍼에 있는게 안비워짐 
    //그냥 프로그램 종료
    _exit(0);
}