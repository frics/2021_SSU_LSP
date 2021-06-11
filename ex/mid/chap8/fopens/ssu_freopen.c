#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>

int main(void){
    char *fname = "../ssu_freopen_test.txt";
    int fd;

    printf("First printf : Hello!\n");

    if((fd = open(fname, O_RDONLY | O_CREAT | O_TRUNC)) < 0){
        fprintf(stderr, "open error %s\n", fname);
        exit(1);
    }
    if(freopen(fname, "w", stdout) != NULL)
        printf("Second printf : Hello!!\n");

    exit(0);
}