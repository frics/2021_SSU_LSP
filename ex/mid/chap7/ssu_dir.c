#include<stdio.h>
#include<dirent.h>
#include<pwd.h>
#include<sys/stat.h>


int main(){

    DIR *dp;
    char *dir;
    struct stat statbuf;
    struct dirent *d;
    char path[1024];

    dir = ".";
    //디렉터리 오픈
    if((dp = opendir(dir)) == NULL){
        perror(dir);
    }
    //디렉터리 포인터를 하나씩 읽어들인다.
    while((d = readdir(dp)) != NULL){
        sprintf(path, "%s/%s", dir, d->d_name);
        if(lstat(path, &statbuf) < 0)
            perror(path);
        printf("%15s : %0#7o\n", d->d_name, statbuf.st_mode);

    }
}