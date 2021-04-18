#include<stdio.h>
#include<stdlib.h> //exit()
#include<fcntl.h> //creat()
#include<utime.h> //utime()
#include<unistd.h> //getopt()
#include<sys/stat.h> //umask()
#include<sys/types.h> //mode_t

int main(int argc, char *argv[]){

    while(getopt(argc, argv, "acdfhmrt") != -1);
   
    if(utime(argv[optind], NULL) < 0){
        mode_t mode = 0666 - umask(0);
        int fd;
        if((fd = creat(argv[optind], mode)) < 0){
            fprintf(stderr, "creat error for %s\n", argv[optind]);
        }
    }
    exit(0);

}