#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>

struct stat statbuf;
int cnt = 0;

void ssu_checkfile(char *fname, time_t *time);

int main(int argc, char *argv[]){
    time_t intertime;

    if(argc!=2){
        fprintf(stderr, "usage : %s <file>\n", argv[0]);
        exit(1);
    }

    if((stat(argv[1], &statbuf)) < 0) {
        fprintf(stderr, "stat error\n");
        exit(1);
    }

    intertime = statbuf.st_mtime;

    while(cnt != 5) {
        ssu_checkfile(argv[1], &intertime);
        sleep(5);
    }

}

void ssu_checkfile(char *fname, time_t *time){
    if(stat(fname, &statbuf) < 0 ){
        fprintf(stderr, "Warning : ssu_checkfile() error\n");
        exit(1);
    }else{
         if(statbuf.st_mtime != *time){
            printf("Warning : %s was modified!\n", fname);
            *time = statbuf.st_mtime;
            cnt++;
        }else{
            printf("Nothing chaged\n");
        }
    }

}