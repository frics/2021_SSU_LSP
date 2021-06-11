#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

void ssu_addone(void);

//현재 프로세스의 환경 변수가 저장되어 있다.
extern char **environ;

char glob_var[] = "HOBBY=swimming";

int main(void){
    for(int i=0; environ[i] != NULL; i++)
        printf("environ[%d] : %s\n", i, environ[i]);

    putenv(glob_var);
    putenv("HI=sibal");
    ssu_addone();

    printf("My hobby is %s\n", getenv("HOBBY"));
   
    printf("My lover is %s\n", getenv("LOVER"));

    strcpy(glob_var+6, "fising");

    for(int i=0; environ[i] != NULL; i++)
        printf("environ[%d] : %s\n", i, environ[i]);
    exit(0);
}

void ssu_addone(void){
    //환경 변수를 전역 변수가 아닌 지역 변수로 설정하면
    //제대로 저장이 안된다
    char auto_var[10];
    strcpy(auto_var, "LOVER=js");
    putenv(auto_var);
}