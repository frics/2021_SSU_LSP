#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>

#define MODE_EXEC (S_IXUSR | S_IXGRP | S_IXOTH)
int main(int argc, char *argv[]){
    struct stat statbuf;
    int i;

    if(argc < 2){
        fprintf(stderr, "Usage : %s <file1> <file2> ... <fileN>\n", argv[0]);
        exit(1);
    }
    for(i = 1; i< argc; i++){
        if(stat(argv[i], &statbuf) < 0){
            fprintf(stderr, "stat error %s\n", argv[i]);
            continue;
        }
        //user group other에 실행 권한 부여
        statbuf.st_mode |= MODE_EXEC;
        //group, other의 실행권한을 반대로 설정
        statbuf.st_mode ^=(S_IXGRP | S_IXOTH);

        if(chmod(argv[i], statbuf.st_mode) < 0)
            fprintf(stderr, "chmod error %s\n", argv[i]);
        else
            printf("%s : file permission was changed.\n", argv[i]);
    }
    exit(0);
    
}