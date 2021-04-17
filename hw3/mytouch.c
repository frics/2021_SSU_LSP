#include<stdio.h>
#include<stdlib.h> //exit()
#include<fcntl.h> //creat()
#include<utime.h> //utime()
#include<unistd.h> //getopt()
#include<sys/stat.h>
#include<sys/types.h>

int main(int argc, char *argv[]){
    //
    mode_t mode = 0666 - umask(0);
    while(getopt(argc, argv, "acdfhmrt") != -1);
   
    printf("%#.4o\n", mode);

    if(utime(argv[optind], NULL) < 0){
        int fd;
        if((fd = creat(argv[optind], mode)) < 0){
            fprintf(stderr, "creat error for %s\n", argv[optind]);
        }
    }
    exit(0);

}