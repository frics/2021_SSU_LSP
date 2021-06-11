#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#define BUFFER_SIZE 1024

void ssu_setbuf(FILE *fp, char *buf);

int main(void){
    char buf[BUFFER_SIZE];
    char *fname = "/dev/pts/2";
    FILE *fp;

    if((fp = fopen(fname, "w")) == NULL){
        fprintf(stderr, "fopen error for %s", fname);
        exit(1);
    }
    ssu_setbuf(fp, buf);
  
    while(1){
        fgets(buf, BUFFER_SIZE, stdin);
        printf("%s", buf);
        if(strcmp(buf, "clear\n") == 0)
            system(buf);
        else
            fputs(buf, fp);
        //fprintf(fp, "%s", buf);
    }
    
/*
    fprintf(fp, "\nHello, ");
    sleep(1);
    fprintf(fp, "UNIX!!");
    sleep(1);
    fprintf(fp, "\n");
    ssu_setbuf(fp, NULL);
    fprintf(fp, "How ");
    sleep(1);
    fprintf(fp, "are ");
    sleep(1);
    fprintf(fp, "you?");
    sleep(1);
    fprintf(fp, "\n");
    sleep(1);

    exit(0);
  */  
}

void ssu_setbuf(FILE *fp, char *buf){
    size_t size;
    int fd;
    int mode;
    
    fd = fileno(fp);

    if(isatty(fd))
        mode =_IOLBF;
    else
        mode=_IOFBF;
    if(buf == NULL){
        mode = _IONBF;
        size = 0;
    }else
        size = BUFFER_SIZE;
    setvbuf(fp, buf, mode, size);
}